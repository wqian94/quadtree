/**
Naive parallel implementation of compressed skip quadtree with pthreads
*/

#include <assert.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include "../types.h"
#include "../Quadtree.h"
#include "../Point.h"

// rand() functions
#ifdef QUADTREE_TEST
extern uint32_t test_rand();
#define rand() test_rand()
#else
extern uint32_t Marsaglia_rands(uint32_t*);
#define rand() Marsaglia_rands(&mseed)
extern void Marsaglia_srand(uint32_t);
#define srand(x) Marsaglia_srand(x)
#endif

// quadtree counter
#ifdef QUADTREE_TEST
uint64_t QUADTREE_NODE_COUNT = 0;
pthread_mutex_t *QUADTREE_NODE_COUNT_MUTEX = NULL;
#endif

// macros for parallelism
#ifndef trylock
#define trylock(x) pthread_mutex_trylock((pthread_mutex_t*)&x->lock)
#endif

#ifndef lock
//#define lock(x) {while(!pthread_mutex_timedlock((pthread_mutex_t*)&x->lock, &(const struct timespec){ .tv_sec = 1, .tv_nsec = 0}));}
#define lock(x) pthread_mutex_lock((pthread_mutex_t*)&x->lock);
#endif

#ifndef unlock
#define unlock(x) pthread_mutex_unlock((pthread_mutex_t*)&x->lock);
#endif

#define min_node(x, y) ((long)y ^ (((long)x ^ (long)y) & -(x < y)))
#define max_node(x, y) ((long)x ^ (((long)x ^ (long)y) & -(x < y)))

#define sort2(x, y) (y = (Node*)((long)(x = (Node*)(max_node(x, y) ^ (long)(y = (Node*)((long)y ^ (long)x)))) ^ (long)y))

static inline bool Node_valid(Node *node);

void print(Node *n) {
    if (n == NULL)
        printf("NULL\n");
    else
        printf("pointer = %p, is_square = %s, center = (%.6lf, %.6lf), length = %llu, parent = %p, up = %p, down = %p, children = {%p, %p, %p, %p}, dirty = %s, id = %llu\n", n, n->is_square ? "true" : "false", n->center->x, n->center->y, (unsigned long long)n->length,
        !Node_valid(n->parent) ? NULL : n->parent, !Node_valid(n->up) ? NULL : n->up, !Node_valid(n->down) ? NULL : n->down, !Node_valid(n->children[0]) ? NULL : n->children[0], !Node_valid(n->children[1]) ? NULL : n->children[1], !Node_valid(n->children[2]) ? NULL : n->children[2], !Node_valid(n->children[3]) ? NULL : n->children[3],
        n->dirty ? "true" : "false", (unsigned long long)n->id);
}

Node* Node_init(float64_t length, Point center) {
    #ifdef QUADTREE_TEST
    if(QUADTREE_NODE_COUNT_MUTEX == NULL) {
        QUADTREE_NODE_COUNT_MUTEX = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(QUADTREE_NODE_COUNT_MUTEX, NULL);
    }
    #endif

    Node *node = (Node*)malloc(sizeof(*node));
    node->is_square = false;
    node->length = length;
    node->center = (Point*)malloc(sizeof(*node->center)),
    node->parent = NULL;
    node->up = NULL;
    node->down = NULL;
    node->children[0] = NULL;
    node->children[1] = NULL;
    node->children[2] = NULL;
    node->children[3] = NULL;
    pthread_mutex_init((pthread_mutex_t*)&node->lock, pthread_mutex_attr());
    node->dirty = false;
    #ifdef QUADTREE_TEST
    pthread_mutex_lock(QUADTREE_NODE_COUNT_MUTEX);
    node->id = QUADTREE_NODE_COUNT++;
    pthread_mutex_unlock(QUADTREE_NODE_COUNT_MUTEX);
    #endif
    Point_copy(&center, node->center);
    return node;
}

Quadtree* Quadtree_init(float64_t length, Point center) {
    Quadtree *qtree = Node_init(length, center);
    qtree->is_square = true;
    return qtree;
}

/*
 * Node_free
 *
 * Frees the memory used to represent this node.
 *
 * node - the node to be freed
 */
static inline void Node_free(Node *node) {
    #ifdef PARALLEL
    pthread_mutex_destroy((pthread_mutex_t*)&node->lock);
    #endif
    free((void*)node->center);
    free((void*)node);
}

/*
 * Node_valid
 *
 * Returns false if node is either NULL or is dirty, and true otherwise. A value of true
 * indicates that the node is both non-NULL and not logically deleted.
 *
 * node - the node to check
 *
 * Returns whether the node is valid for use.
 */
static inline bool Node_valid(Node *node) {
    return node != NULL && !node->dirty;
}

/*
 * struct LockSet_t
 *
 * Linked list struct for locking chains.
 *
 * node - the node whose lock we've acquired
 * next - the next LockSet in the list
 * last - the last LockSet in the list
 */
typedef volatile struct LockSet_t {
    Node *node;
    bool locked;
    volatile struct LockSet_t *next, *last;
} LockSet;

/*
 * LockSet_init
 *
 * Creates a new LockSet stack, with the first element being node.
 *
 * node - the first element
 *
 * Returns a pointer to the created list.
 */
static inline LockSet* LockSet_init(Node *node) {
    LockSet *ls = (LockSet*)malloc(sizeof(LockSet));
    ls->node = node;
    ls->locked = false;
    ls->last = ls;
    ls->next = NULL;
}

/*
 * LockSet_empty
 *
 * Returns an empty LockSet stack. The first insertion will insert the node in-place.
 */
static inline LockSet* LockSet_empty() {
    LockSet *ls = LockSet_init(NULL);
    ls->last = NULL;
    return ls;
}

/*
 * LockSet_lock
 *
 * Acquires a lock on node, and appends a LockSet element to the stack, containing the
 * node. If the node is invalid, it will not be locked.
 *
 * lockset - the LockSet to append to
 * node - the node to lock on
 */
static inline void LockSet_lock(LockSet *lockset, Node *node) {
    bool locked = true;
    if (Node_valid(node)) {
        // do a deadlock prevention check
        LockSet *ls;
        for (ls = lockset; ls != NULL; ls = ls->next)
            if (ls->node == node)
                break;
        if (ls == NULL) {
            lock(node);
        }
        else
            locked = false;
    }
    if (lockset->last == NULL) {  // empty lockset, insert in-place
        lockset->node = node;
        lockset->next = NULL;
        lockset->last = lockset;
    }
    else {  // to maintain LIFO, need to move data to next node to insert at head
        LockSet *next = LockSet_init(lockset->node);
        next->locked = lockset->locked;
        next->next = lockset->next;
        next->last = lockset->last;
        lockset->next = next;
        lockset->node = node;
    }
    lockset->locked = locked;
}

/*
 * LockSet_unlock
 *
 * Unlocks every node in lockset, and frees all memory. The input parameter will also be
 * deallocated.
 *
 * lockset - the LockSet stack to fully unlock and deallocate
 */
static inline void LockSet_unlock(LockSet *lockset) {
    LockSet *curr;
    for (curr = lockset; lockset != NULL; curr = lockset) {
        lockset = lockset->next;
        if (Node_valid(curr->node) && curr->locked)
            unlock((curr->node));
        free((void*)curr);
    }
}

void print_LockSet(LockSet *lockset) {
    uint64_t count = 0;
    while (lockset != NULL) {
        printf("locked=%s ", lockset->locked ? "true" : "false");
        print(lockset->node);
        lockset = lockset->next;
        count++;
    }
    printf("Total length: %llu\n", (unsigned long long)count);
}

/* lock2
 *
 * Locks x, y in increasing id order.
 *
 * x, y - the nodes to lock
 */
/*static inline void lock2(Node *x, Node *y) {
    sort2(x, y);
    lock(x);
    lock(y);
}*/

/* lock3
 *
 * Locks x, y, z in increasing id order.
 *
 * x, y, z - the nodes to lock
 */
/*static inline void lock3(Node *x, Node *y, Node *z) {
    sort2(x, y);
    sort2(y, z);
    sort2(x, y);
    lock(x);
    lock(y);
    lock(z);
}*/

/* lock4
 *
 * Locks w, x, y, z in increasing id order.
 *
 * w, x, y, z - the nodes to lock
 */
/*static inline void lock4(Node *w, Node *x, Node *y, Node *z) {
    sort2(w, x);
    sort2(x, y);
    sort2(y, z);
    sort2(w, x);
    sort2(x, y);
    sort2(w, x);
    lock(w);
    lock(x);
    lock(y);
    lock(z);
}*/

/*
 * Quadtree_search_helper
 *
 * Recursive helper function to traverse the tree horizontally before dropping levels.
 *
 * Invariant: node is always a square.
 *
 * node - the square to look in
 * p - the point to search for
 *
 * Returns whether p is in node.
 */
bool Quadtree_search_helper(Node *node, Point *p) {
    if (!in_range(node, p))
        return false;

    register uint8_t quadrant = get_quadrant(node->center, p);

    // if the target child is NULL, we try to drop down a level
    if (!Node_valid(node->children[quadrant])) {
        if (Node_valid(node->down))
            return Quadtree_search_helper(node->down, p);
        // otherwise, we're on the bottom-most level and just can't find the point
        return false;
    }

    // here, the child exists

    // if is a square, move to it and recurse
    if (node->children[quadrant]->is_square)
        return Quadtree_search_helper(node->children[quadrant], p);

    // otherwise, we check if the child point matches, since it's a point node
    if (Point_equals(node->children[quadrant]->center, p))
        return true;

    // if we're here, then we need to branch down a level
    if (Node_valid(node->down))
        return Quadtree_search_helper(node->down, p);

    // here, we have nowhere else to search for, so we give up
    return false;
}

bool Quadtree_search(Quadtree *node, Point p) {
    if (!Node_valid(node))
        return false;

    while (Node_valid(node->up))
        node = node->up;

    return Quadtree_search_helper(node, &p);
}

/*
 * Quadtree_add_find_root
 *
 * Returns the node at which insertion should begin, based on the gap depth.
 *
 * node - the node to start at, should be the root
 * p - the point that we intend on inserting
 * gap_depth - the gap depth
 *
 * Returns the node that insertion should begin at. The subtree represented by this point
 * should sufficiently cover all locations we could want to insert p.
 */
Node* Quadtree_add_find_root(Node *node, Point *p, const uint64_t gap_depth) {
    Node *parent = NULL, *child = node, *prevparent;
    register uint64_t i;
    for (i = 0; i <= gap_depth; i++) {
        do {
            prevparent = parent;
            parent = child;
            child = parent->children[get_quadrant(parent->center, p)];
        } while (Node_valid(child) && child->is_square && in_range(child, p));

        child = parent->down;
    }

    return parent;
}

/*
 * Quadtree_add_lock
 *
 * Routine to lock nodes that need to be locked before insertion begins.
 *
 * node - the node to start at
 * p - the points to insert
 * lockset - the lockset to append to
 *
 * Returns true if all locks successful, false if aborted.
 */
bool Quadtree_add_lock(Node *node, Point *p, LockSet *lockset) {
    Node *parent = NULL, *child = node;
    do {
        parent = child;
        child = child->children[get_quadrant(child->center, p)];
    } while (Node_valid(child) && child->is_square && in_range(child, p));

    // add parent to lockset
    LockSet_lock(lockset, parent);

    // add sibling to lockset, or nothing if there is no sibling
    LockSet_lock(lockset, child);

    // validate parent and child
    if (!Node_valid(parent))  // parent must be valid
        return false;
    // child can be valid or invalid, but if valid, must have correct references
    if (Node_valid(child) && (
            parent->children[get_quadrant(parent->center, child->center)] != child ||
            child->parent != parent) &&
            !in_range(child, p))
        return false;

    // acquire appropriate down node
    if (Node_valid(parent->down) && Node_valid(child)) {
        Node *down = NULL, *next_down = parent->down;
        do {
            down = next_down;
            next_down = down->children[get_quadrant(down->center, p)];
        } while(Node_valid(next_down) &&
                next_down->is_square &&
                in_range(next_down, child->center) &&
                in_range(next_down, p));
        LockSet_lock(lockset, down);

        // validate down; if fails, aborts and retries
        if (down->length - child->length < -PRECISION ||
                !in_range(down, child->center) || !in_range(down, p) ||

                // this checks to see if down is the tightest square possible
                Node_valid(down->parent) &&  // exclude the root
                Node_valid(next_down) &&
                next_down->is_square &&
                in_range(next_down, child->center) &&
                in_range(next_down, p) &&
                get_quadrant(down->center, child->center) ==
                get_quadrant(down->center, p)) {
            /*printf("Balls\n");
            print(down);
            print(next_down);
            printf("(%lf, %lf) (%lf, %lf)\n", child->center->x, child->center->y, p->x, p->y);
            printf("\n");*/
            /*lockset->node = NULL;
            unlock(down);
            down = NULL;*/
            return false;
        }
    }
    else
        LockSet_lock(lockset, NULL);  // placeholder

    // now, recurse on parent's down if available
    if (Node_valid(parent->down))
        return Quadtree_add_lock(parent->down, p, lockset);
    return true;
}

/* Quadtree_add_validate
 *
 * Validates whether the nodes locked in a LockSet are still valid for insertion.
 *
 * Key invariants:
 *     - there should be 3*N nodes in the LockSet, 3 for each level
 *     - every triple should be (parent, child, down square)
 *     - parent must not be a NULL node; child and down can be NULL
 *     - parent and child point to each other if child is not NULL
 *     - if child is NULL, so should down
 *     - if parent's down and child are not NULL, neither should down
 *     - parent must be capable of containing p
 *     - child must be in the same quadrant as the node to be inserted
 *     - if child is a square, it cannot be capable of containing p
 *     - if down is not NULL, it must be capable of containing child and p
 *     - if down is not NULL, its children cannot contain both child and p
 *
 * lockset - the lockset to validate
 * p - the point to insert
 *
 * Returns whether the lockset is validated, given that we want to insert p.
 */
bool Quadtree_add_validate(LockSet *lockset, Point *p) {
    while(lockset != NULL) {
        Node *parent, *child, *down;

        // set parent, child, down
        down = lockset->node;
        if (lockset->next == NULL)
            return false;

        child = lockset->next->node;
        if (lockset->next->next == NULL)
            return false;

        parent = lockset->next->next->node;

        // iterating variable incremented
        lockset = lockset->next->next->next;

        // validate parent must be valid
        if (!Node_valid(parent))
            return false;

        // validate parent and child point to each other
        if (parent->children[get_quadrant(parent->center, p)] != child ||
                Node_valid(child) && child->parent != parent)
            return false;

        // validate down is NULL if child is NULL
        if (!Node_valid(child) && Node_valid(down))
            return false;

        // validate down if parent's down and child exist
        if (Node_valid(parent->down) && Node_valid(child) && !Node_valid(down))
            return false;

        // validate p is in range of parent
        if (!in_range(parent, p))
            return false;

        // validate child and p are in same quadrant of parent
        if (Node_valid(child) && get_quadrant(parent->center, child->center) != get_quadrant(parent->center, p))
            return false;

        // validate p is not in range of child
        if (Node_valid(child) && child->is_square && in_range(child, p))
            return false;

        // validate down can contain both child and p
        if (Node_valid(down) && (!in_range(down, child->center) || !in_range(down, p)))
            return false;

        // validate that none of down's children can contain both child and p
        if (Node_valid(down)) {
            int i;
            for (i = 0; i < 4; i++)
                if (Node_valid(down->children[i]) &&
                        in_range(down->children[i], child->center) &&
                        in_range(down->children[i], p))
                    return false;
        }
    }

    return true;
}

/*
 * Quadtree_add_helper
 *
 * Helper function to add new points to the tree.
 *
 * This function uses the LockSet stack generated during the locking stage to aid in the
 * insertion process. This assumes that Quadtree_add_lock() and Quadtree_add_validate()
 * have both been called and were both successful.
 *
 * We then take each triple of stacked items in the LockSet queue and use that to insert
 * new nodes in the tree such that the optimistic search function works properly.
 *
 * lockset - the lockset generated by Quadtree_add_lock() and validated by
 *           Quadtree_add_validate()
 * p - the point to insert
 *
 * Returns the topmost corresponding node, or NULL if the point already exists.
 */
Node* Quadtree_add_helper(LockSet *lockset, Point *p) {
    Node *down = NULL, *node = NULL, *new_down_square = NULL;
    while (lockset != NULL) {
        Node *parent = lockset->next->next->node,
             *child = lockset->next->node,
             *down_square = lockset->node;

        // iterate our iterator along
        lockset = lockset->next->next->next;

        // set up the new node
        node = Node_init(0, *p);
        node->parent = parent;
        node->down = down;

        // needs to draw new square
        if (Node_valid(child)) {
            Node *square = NULL;

            // start at the shortcut if we have it
            if (Node_valid(down_square))
                square = Quadtree_init(down_square->length, *down_square->center);
            else if (Node_valid(new_down_square))
                square = Quadtree_init(new_down_square->length, *new_down_square->center);
            else if (!Node_valid(parent->down))
                square = Quadtree_init(parent->length, *parent->center);

            while (get_quadrant(square->center, child->center) ==
                    get_quadrant(square->center, p)) {
                *square->center = get_new_center(square, get_quadrant(square->center, p));
                square->length *= 0.5;
            }

            // first check new_down_square
            if (Node_valid(new_down_square) && abs(new_down_square->length - square->length) <= PRECISION) {
                square->down = new_down_square;
            }
            // otherwise, it should be the previously-calculated down_square
            else if (Node_valid(down_square) && abs(down_square->length - square->length) <= PRECISION) {
                square->down = down_square;
            }
            else if(Node_valid(parent->down)) {
                int i = 1;
            }

            // now link children
            square->children[get_quadrant(square->center, node->center)] = node;
            square->children[get_quadrant(square->center, child->center)] = child;
            square->parent = parent;

            // now link parents
            node->parent = square;
            child->parent = square;
            parent->children[get_quadrant(parent->center, p)] = square;

            // link down square if needed
            //if (Node_valid(square->down))
            //    square->down->up = square;

            new_down_square = square;
        }
        // just insert directly
        else {
            parent->children[get_quadrant(parent->center, p)] = node;
            new_down_square = NULL;
        }

        // forego setting the down node's up pointer; seg fault if we actually ever use
        // this in an internal node
        //if (Node_valid(down))
        //    down->up = node;
        down = node;
    }

    return node;
}

bool Quadtree_add(Quadtree *node, Point p) {
    Quadtree *param_node = node;
    if (!Node_valid(node))
        return false;

    #ifndef QUADTREE_TEST
    uint32_t mseed = Marsaglia_seed + (uint32_t)clock() + (uint32_t)pthread_self();
    mseed = rand();
    uint32_t try = mseed & 0xF;
    uint32_t mask = ~0 << try;
    mseed = (mseed >> try) | ((mseed & ~mask) << try);
    #endif

    while (rand() % 100 < 50) {
        if (!Node_valid(node->up)) {
            #ifdef PARALLEL
            lock(node);
            Node *new_up;
            if (!Node_valid(node->up)) {
            #endif
                new_up = Quadtree_init(node->length, *node->center);
                new_up->down = node;
                node->up = new_up;
            #ifdef PARALLEL
            }
            else
                new_up = node->up;
            unlock(node);
            #endif
            node = new_up;
        }
        else
            node = node->up;
    }
    
    register uint64_t gap_depth = 0;  // number of layers to ignore when inserting

    while (Node_valid(node->up)) {
        gap_depth++;
        node = node->up;
    }

    Node *root;
    LockSet *lockset;
    register uint64_t count = 10;
    while (count) {
        root = Quadtree_add_find_root(node, &p, gap_depth);

        lockset = LockSet_empty();  // initialize as empty

        bool locked = false, validated = false;
        if (locked = Quadtree_add_lock(root, &p, lockset)) {
            if (validated = Quadtree_add_validate(lockset, &p))
                break;  // validated!
        }

        LockSet_unlock(lockset);  // retry

        count--;
        if (!count) {  // passed threshold
            printf("Failed to insert (%lf, %lf); locked = %s, validated = %s\n", p.x, p.y, locked ? "true" : "false", validated ? "true" : "false");
            return false;
        }
    }

    Node *new_node = Quadtree_add_helper(lockset, &p);
    LockSet_unlock(lockset);
    return new_node != NULL;
}

/*
 * Quadtree_remove_node
 *
 * Helper function to remove all instances of the given node and properly relink pointers
 * to it.
 *
 * Invariant: node must have either a parent or a down.
 *
 * node - the node to remove
 *
 * Returns true if removal is successful, false otherwise.
 */
bool Quadtree_remove_node(Node *node) {
    if (!Node_valid(node) || !Node_valid(node->down) && !Node_valid(node->parent))
        return false;

    if (Node_valid(node->parent))
        lock(node->parent);
    lock(node);

    // if is square, determine whether need to remove, and if so, which node to move up
    if (node->is_square) {
        register uint8_t num_children = 0, i;
        Node *child = NULL;
        for (i = 0; i < 4; i++)
            if (Node_valid(node->children[i])) {
                num_children++;
                child = node->children[i];
            }

        // cannot remove square if more than 1 child
        if (num_children > 1) {
            if (Node_valid(node->parent))
                unlock(node->parent);
            unlock(node);
            return false;
        }

        // if we have a child, then we relink parent to point to this child, and unlink
        // ourself from the parent
        if (num_children == 1) {
            // however this cannot happen at the root. If we're going to remove a copy of
            // the root, we want no children on that node
            if (!Node_valid(node->parent)) {
                unlock(node);
                return false;
            }

            // if all goes well, we can relink
            lock(child);
            child->parent = node->parent;
            node->parent->children[get_quadrant(node->parent->center, node->center)] = child;
            unlock(child);
        }

        // otherwise, 0 children, and no problem
    }

    // now, we can get rid of our node
    //Node_free(node);
    node->dirty = true;
    unlock(node);

    // then, recurse up the parent as necessary
    if (Node_valid(node->parent)) {
        register uint8_t num_children = 0, i;
        for (i = 0; i < 4; i++)
            num_children += Node_valid(node->parent->children[i]);
        unlock(node->parent);
        if (num_children < 2)
            Quadtree_remove_node(node->parent);
    }

    // finally, recurse on down
    if (Node_valid(node->down))
        Quadtree_remove_node(node->down);

    return true;
}

/*
 * Quadtree_remove_helper
 *
 * Recursive helper function to remove nodes. Removal starts at highest-level occurance
 * and progresses downward.
 *
 * node - the node to start at
 * p - the point to remove
 *
 * Returns true if the node was successfully removed, false if not.
 */
/*bool Quadtree_remove_helper(Node *node, Point *p) {
    if (!in_range(node, p))
        return false;

    register uint8_t quadrant = get_quadrant(node->center, p);

    // if the target child is NULL, we try to drop down a level
    if (!Node_valid(node->children[quadrant])) {
        if (Node_valid(node->down))
            return Quadtree_remove_helper(node->down, p);
        // otherwise, we're on the bottom-most level and just can't find the point
        else
            return false;
    }

    // here, the child exists

    // if is a square, move to it and recurse if in range
    if (node->children[quadrant]->is_square && in_range(node->children[quadrant], p))
        return Quadtree_remove_helper(node->children[quadrant], p);

    // otherwise, we check if the child point matches, since it's a point node
    if (Point_equals(node->children[quadrant]->center, p))
        return Quadtree_remove_node(node->children[quadrant]);

    // if we're here, then we need to branch down a level
    if (Node_valid(node->down))
        return Quadtree_remove_helper(node->down, p);

    // here, we have nowhere else to search for, so we give up
    return false;
 
}*/

bool Quadtree_remove_lock(Node *node, Point *p, LockSet *lockset) {
    Node *parent = NULL;
    while (Node_valid(node)) {
        parent = node;

        register uint8_t quadrant = get_quadrant(node->center, p);
        node = parent->children[quadrant];

        if (Node_valid(node)) {
            if (node->is_square) {
                // if not in range, need to branch down; otherwise, keep going
                if (!in_range(node, p))
                    node = parent->down;
            }
            else {
                // point found
                if (Point_equals(node->center, p)) {
                    LockSet_lock(lockset, parent->parent);
                    LockSet_lock(lockset, parent);
                    LockSet_lock(lockset, node);
                    /*print(parent->parent);
                    print(parent);
                    print(node);
                    puts("");*/
                    
                    Node *child = NULL;
                    // only look for child if parent is not root
                    if (Node_valid(parent->parent)) {
                        register uint8_t i;
                        for (i = 0; i < 4; i++)
                            if (Node_valid(parent->children[i]) && parent->children[i] != node) {
                                if (Node_valid(child)) {
                                    child = NULL;
                                    break;
                                }
                                else
                                    child = parent->children[i];
                            }
                    }

                    LockSet_lock(lockset, child);

                    // and now a shortcut
                    if (Node_valid(node->down))
                        node = node->down->parent;
                    else
                        node = NULL;
                }
                else {
                    node = parent->down;
                }
            }
        }
        // dead-end on this level, branch down
        else {
            node = parent->down;
        }
    }

    return true;
}

/*
 * Quadtree_remove_validate
 *
 * Validates lockset for these properties:
 *     - is (parent-parent, parent, node, child) tuple
 *     - all three pairs parent<->child pointers for each level
 *     - node's center is actually p
 *     - each level connects to the approriate down nodes (downmost points to NULL)
 *     - if pparent is NULL, parent must be a root node
 *     - unless parent is root, if child is NULL, parent must have != 2 children, including node
 *     - unless parent is root, if parent has exactly 2 children, child must not be NULL
 *     - if parent is root, child must be NULL
 *
 * lockset - the lockset to validate
 * p - the point of interest to remove
 *
 * Returns whether the lockset passes validation.
 */
bool Quadtree_remove_validate(LockSet *lockset, Point *p) {
    // empty lockset -> node not found
    if (lockset->last == NULL)
        return true;

    // starts off at first level, should be NULL
    Node *down_pp = NULL, *down_p = NULL, *down_n = NULL, *down_c = NULL;
    while (lockset != NULL) {
        Node *pparent = lockset->next->next->next->node,
             *parent = lockset->next->next->node,
             *node = lockset->next->node,
             *child = lockset->node;

        // iterate
        lockset = lockset->next->next->next->next;

        // check tuple order and pointers
        if (Node_valid(pparent) && (pparent->children[get_quadrant(pparent->center, parent->center)] != parent ||
                parent->parent != pparent))
            return false;
        if (parent->children[get_quadrant(parent->center, node->center)] != node || node->parent != parent)
            return false;
        if (Node_valid(child) && (parent->children[get_quadrant(parent->center, child->center)] != child ||
                child->parent != parent))
            return false;

        // check that we have the right node
        if (!Point_equals(node->center, p))
            return false;

        // check for down pointers
        if (parent->down != down_p || node->down != down_n)
            return false;

        // check for pparent being NULL -> node is root
        if (Node_valid(pparent) != Node_valid(parent->parent))
            return false;

        // now check for only-other-child cases
        if (Node_valid(pparent)) {
            register uint8_t num_children = 0, i;
            for (i = 0; i < 4; i++)
                num_children += Node_valid(parent->children[i]);

            // exactly 2 children <-> child is not NULL
            if ((num_children == 2) != Node_valid(child))
                return false;
        }
        // if parent is root, child must be NULL
        else if (Node_valid(child))
            return false;
    }

    return true;
}

bool Quadtree_remove_helper(LockSet *lockset) {
    // empty lockset
    if (lockset->last == NULL)
        return false;

    Node *pparent = lockset->next->next->next->node,
         *parent = lockset->next->next->node,
         *node = lockset->next->node,
         *child = lockset->node;

    // recurse to top level first
    LockSet *next_set = lockset->next->next->next->next;
    if (next_set != NULL && !Quadtree_remove_helper(next_set))
        return false;

    node->dirty = true;

    // if need to delete parent, also do that now
    if (Node_valid(child)) {
        pparent->children[get_quadrant(pparent->center, parent->center)] = child;
        child->parent = pparent;
        parent->dirty = true;
    }

    return true;
}

bool Quadtree_remove(Quadtree *node, Point p) {
    while (Node_valid(node->up))
        node = node->up;

    LockSet *lockset;
    register uint64_t count = 10;
    while (count) {
        Node *root = node;

        lockset = LockSet_empty();

        bool locked = false, validated = false;
        if (locked = Quadtree_remove_lock(root, &p, lockset)) {
            if (validated = Quadtree_remove_validate(lockset, &p)) {
                break;
            }
        }

        LockSet_unlock(lockset);

        count--;

        if (!count) {  // passed threshold
            //printf("Failed to delete (%lf, %lf); locked = %s, validated = %s\n", p.x, p.y, locked ? "true" : "false", validated ? "true" : "false");
            return false;
        }
    }

    //bool result = Quadtree_remove_helper(node, &p);
    bool result = Quadtree_remove_helper(lockset);
    LockSet_unlock(lockset);
    return result;
}

/*
 * Quadtree_free_helper
 *
 * Recursively frees the direct children of this node. Does not deal with other levels of
 * the tree.
 *
 * Stores result information in the referenced QuadtreeFreeResult struct.
 *
 * node - the node to free
 * result - the result to write to
 *
 * Returns whether freedom of the (sub)tree start at this node was successful.
 */
bool Quadtree_free_helper(Node *node, QuadtreeFreeResult *result) {
    bool success = true;
    if (node->is_square) {
        register uint8_t i;
        for (i = 0; i <4; i++)
            if (node->children[i] != NULL) {
                success &= Quadtree_free_helper(node->children[i], result);
                node->children[i] = NULL;
            }
    }

    // up and down
    if (node->up != NULL)
        node->up->down = NULL;
    if (node->down != NULL)
        node->down->up = NULL;

    result->total++;
    if (!node->dirty) {
        result->clean++;
        result->leaf += !node->is_square;
    }

    // should no longer have references
    Node_free(node);

    return success;
}

QuadtreeFreeResult Quadtree_free(Quadtree *root) {
    QuadtreeFreeResult result = (QuadtreeFreeResult){ .total = 0, .clean = 0, .leaf = 0, .levels = 0 };

    while (root->up != NULL)
        root = root->up;

    while (root != NULL) {
        Node *next_root = root->down;
        result.levels += Node_valid(root) * Quadtree_free_helper(root, &result);
        root = next_root;
    }

    return result;
}
