/**
Naive parallel implementation of compressed skip quadtree with pthreads
*/

#include <assert.h>
#include <stdlib.h>

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
//#define lock(x) {if (x == NULL){printf("NULLlock\n");getchar();}int code;printf("%llx wants %p\n",(unsigned long long)pthread_self(),&x->lock);do{code=pthread_mutex_trylock(&x->lock);}while(code);printf("%llx obtained %p\n",(unsigned long long)pthread_self(),&x->lock);}
#define lock(x) pthread_mutex_lock(&x->lock)
#define trylock(x) pthread_mutex_trylock(&x->lock)
#define unlock(x) pthread_mutex_unlock(&x->lock)
//#define unlock(x) {pthread_mutex_unlock(&x->lock);printf("%llx released %p\n",(unsigned long long)pthread_self(),&x->lock);}

#define min_node(x, y) ((long)y ^ (((long)x ^ (long)y) & -(x < y)))
#define max_node(x, y) ((long)x ^ (((long)x ^ (long)y) & -(x < y)))

#define sort2(x, y) (y = (Node*)((long)(x = (Node*)(max_node(x, y) ^ (long)(y = (Node*)((long)y ^ (long)x)))) ^ (long)y))

void print(Node *n) {
    if (n == NULL)
        printf("NULL\n");
    else
        printf("pointer = %p, is_square = %s, center = (%.6lf, %.6lf), length = %llu, parent = %p, up = %p, down = %p, children = {%p, %p, %p, %p}, dirty = %s, id = %llu\n", n, n->is_square ? "true" : "false", n->center->x, n->center->y, (unsigned long long)n->length, n->parent, n->up, n->down, n->children[0], n->children[1], n->children[2], n->children[3], n->dirty ? "true" : "false", (unsigned long long)n->id);
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
    pthread_mutex_init(&node->lock, NULL);
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
    pthread_mutex_destroy(&node->lock);
    #endif
    free(node->center);
    free(node);
}

/* lock2
 *
 * Locks x, y in increasing id order.
 *
 * x, y - the nodes to lock
 */
static inline void lock2(Node *x, Node *y) {
    sort2(x, y);
    lock(x);
    lock(y);
}

/* lock3
 *
 * Locks x, y, z in increasing id order.
 *
 * x, y, z - the nodes to lock
 */
static inline void lock3(Node *x, Node *y, Node *z) {
    sort2(x, y);
    sort2(y, z);
    sort2(x, y);
    lock(x);
    lock(y);
    lock(z);
}

/* lock4
 *
 * Locks w, x, y, z in increasing id order.
 *
 * w, x, y, z - the nodes to lock
 */
static inline void lock4(Node *w, Node *x, Node *y, Node *z) {
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
}

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

    if (node->dirty && node->down != NULL)
        return Quadtree_search_helper(node->down, p);

    register uint8_t quadrant = get_quadrant(node->center, p);

    // if the target child is NULL, we try to drop down a level
    if (!Node_valid(node->children[quadrant])) {
        if (node->down != NULL)
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
 * Quadtree_add_helper
 *
 * Recursive helper function to add new points to the tree.
 *
 * The process is three-part:
 * 1. We traverse node on the topmost level to where p should be added.
 * 2. We then branch down to create lower-level nodes first.
 * 3. We then take the lower-level node and use it as our down for this level.
 *
 * node - the node to start inserting at; should be a square
 * p - the point to add
 * gap_depth - the number of levels we need to go through before actually inserting nodes
 *
 * Returns the corresponding node, one level lower, or NULL if the action failed.
 */
Node* Quadtree_add_helper(Node *node, Point *p, const uint64_t gap_depth) {
    if (!in_range(node, p))
        return NULL;

    // horizontal traversal
    volatile Node *parent = NULL, *child = node;
    do {
        lock(child);
        if (parent != NULL)
            unlock(parent);
        parent = child;
        child = parent->children[get_quadrant(parent->center, p)];
    } while(Node_valid(child) && child->is_square && in_range(child, p));

    // check for duplication
    if (!gap_depth && Node_valid(child) && !child->is_square && Point_equals(node->center, p)) {
        unlock(parent);
        return NULL;
    }

    // branch down a level if possible
    volatile Node *down_node = NULL;
    if (Node_valid(parent->down)) {
        unlock(parent);
        Node *parent_down = parent->down;
        if (!Node_valid(down_node = Quadtree_add_helper(parent_down, p,
                (gap_depth > 0) * (gap_depth - 1)))) {
            return NULL;
        }
        lock(parent);
    }

    // if gap_depth is not zero, we shouldn't actually add anything
    if (gap_depth) {
        unlock(parent);
        return down_node;
    }

    child = parent;
    unlock(child);
    do {
        lock(child);
        if (parent != NULL)
            unlock(parent);
        parent = child;
        child = parent->children[get_quadrant(parent->center, p)];
    } while(Node_valid(child) && child->is_square && in_range(child, p));

    volatile Node *new_node = Node_init(0, *p);
    new_node->parent = parent;

    // time to try inserting onto this level
    register uint8_t quadrant = get_quadrant(parent->center, p);

    // if the slot is empty, it's trivial
    if (!Node_valid(parent->children[quadrant])) {
        lock(new_node);

        if (Node_valid(down_node))
            lock(down_node);

        new_node->down = down_node;
        if (Node_valid(down_node))
            down_node->up = new_node;

        parent->children[quadrant] = new_node;

        unlock(parent);
        unlock(new_node);
        if (Node_valid(down_node))
            unlock(down_node);
    }
    // if it's not empty, that means there's already a node there...
    else {
        // create a new square to contain the sibling and the new node
        uint8_t square_quadrant = quadrant;
        volatile Node *square = Quadtree_init(0.5 * parent->length, get_new_center(parent, quadrant));
        lock(square);
        lock(new_node);
        square->parent = parent;

        // grab the sibling-to-be
        volatile Node *sibling = parent->children[quadrant];
        lock(sibling);

        // now, we keep splitting until the new node and the sibling are in different quadrants
        register uint8_t sibling_quadrant;
        while ( (sibling_quadrant = get_quadrant(square->center, sibling->center)) ==
                (quadrant = get_quadrant(square->center, new_node->center))) {
            Point new_square_center = get_new_center(square, quadrant);
            Point_copy(&new_square_center, square->center);
            square->length *= 0.5;
        }

        // okay, now we have separate quadrants to use
        square->children[quadrant] = new_node;
        square->children[sibling_quadrant] = sibling;

        // now, we need to find the down square to this square, if we're not on the last level
        volatile Node *down_square = NULL;
        if (Node_valid(parent->down)) {
            down_square = parent->down;
            lock(down_square);
            while (!Point_equals(down_square->center, square->center) ||
                    abs(down_square->length - square->length) > PRECISION) {
                Node *prev_down = down_square;
                down_square = down_square->children[get_quadrant(down_square->center, square->center)];
                lock(down_square);
                unlock(prev_down);
            }
            square->down = down_square;
        }

        if (Node_valid(down_node)) {
            lock(down_node);
        }

        bool try = !parent->dirty && !square->dirty && !new_node->dirty && !sibling->dirty &&
            (Node_valid(down_square) && Node_valid(down_node) || !Node_valid(parent->down));
        if (try) {
            if (Node_valid(down_node))
                down_node->up = new_node;
            new_node->down = down_node;

            parent->children[square_quadrant] = square;
            new_node->parent = square;
            sibling->parent = square;

            if (Node_valid(down_square))
                down_square->up = square;
        }

        unlock(parent);
        unlock(square);
        unlock(new_node);
        unlock(sibling);
        if (Node_valid(down_square))
            unlock(down_square);
        if (Node_valid(down_node))
            unlock(down_node);

        if (!try) {
            Node_free(square);
            Node_free(new_node);
            return Quadtree_add_helper(node, p, gap_depth);
        }
    }

    return new_node;
}

bool Quadtree_add(Quadtree *node, Point p) {
    if (!Node_valid(node))
        return false;

    uint32_t mseed = Marsaglia_seed + (uint32_t)clock() + (uint32_t)pthread_self();
    mseed = rand();
    uint32_t try = mseed & 0xF;
    uint32_t mask = ~0 << try;
    mseed = (mseed >> try) | ((mseed & ~mask) << try);

    while (rand() % 100 < 50) {
        if (!Node_valid(node->up)) {
            #ifdef PARALLEL
            lock(node);
            if (!Node_valid(node->up)) {
            #endif
                Node *new_up = Quadtree_init(node->length, *node->center);
                new_up->down = node;
                node->up = new_up;
            #ifdef PARALLEL
            }
            unlock(node);
            #endif
        }
        node = node->up;
    }
    
    register uint64_t gap_depth = 0;  // number of layers to ignore when inserting

    while (Node_valid(node->up)) {
        gap_depth++;
        node = node->up;
    }

    return Quadtree_add_helper(node, &p, gap_depth) != NULL;
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

    if (Node_valid(node->up))
        lock(node->up);
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
            if (Node_valid(node->up))
                unlock(node->up);
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
                if (Node_valid(node->up))
                    unlock(node->up);
                if (Node_valid(node->parent))
                    unlock(node->parent);
                unlock(node);
                return false;
            }

            // if all goes well, we can relink
            node->parent->children[get_quadrant(node->parent->center, node->center)] = child;
            child->parent = node->parent;
            //node->parent = NULL;
        }

        // otherwise, 0 children, and no problem
    }
    if (Node_valid(node->down))
        lock(node->down);

    // now, get rid of pointers from the parent
    Node *parent = node->parent, *up = node->up, *down = node->down;
    if (Node_valid(parent) && parent->children[get_quadrant(parent->center, node->center)] == node)
        parent->children[get_quadrant(parent->center, node->center)] = NULL;

    // next, unlink pointers from up and down
    if (Node_valid(node->up)) {
        node->up->down = NULL;
        //node->up = NULL;
    }
    if (Node_valid(node->down)) {
        node->down->up = NULL;
        //node->down = NULL;
    }

    // now, we can get rid of our node
    //Node_free(node);
    node->dirty = true;
    unlock(node);

    if (Node_valid(up))
        unlock(up);

    if (Node_valid(down))
        unlock(down);

    // then, recurse up the parent as necessary
    if (Node_valid(parent)) {
        register uint8_t num_children = 0, i;
        for (i = 0; i < 4; i++)
            num_children += Node_valid(parent->children[i]);
        unlock(parent);
        if (num_children < 2)
            Quadtree_remove_node(parent);
    }

    // finally, recurse on up and down
    if (Node_valid(up))
        Quadtree_remove_node(up);

    if (Node_valid(down))
        Quadtree_remove_node(down);

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
bool Quadtree_remove_helper(Node *node, Point *p) {
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
 
}

bool Quadtree_remove(Quadtree *node, Point p) {
    while (Node_valid(node->up))
        node = node->up;

    return Quadtree_remove_helper(node, &p);
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
