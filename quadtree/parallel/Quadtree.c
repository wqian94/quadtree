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
extern uint32_t Marsaglia_rand();
#define rand() Marsaglia_rand()
extern void Marsaglia_srand(uint32_t);
#define srand(x) Marsaglia_srand(x)
#endif

// quadtree counter
#ifdef QUADTREE_TEST
uint64_t QUADTREE_NODE_COUNT = 0;
#endif

Node* Node_init(float64_t length, Point center) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->is_square = false;
    node->length = length;
    node->center = (Point*)malloc(sizeof(node->center)),
    node->parent = NULL;
    node->up = NULL;
    node->down = NULL;
    node->children[0] = NULL;
    node->children[1] = NULL;
    node->children[2] = NULL;
    node->children[3] = NULL;
    #ifdef PARALLEL
    pthread_rwlock_init(&node->lock, NULL);
    #endif
    #ifdef QUADTREE_TEST
    node->id = QUADTREE_NODE_COUNT++;
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
    pthread_rwlock_destroy(&node->lock);
    #endif
    free(node->center);
    free(node);
}

/*
 * struct Quadtree_pthread_meta_t
 *
 * Stores metadata for pthread executions.
 *
 * node - a reference to the Quadtree node
 * p - the Point of interest
 * ret - a reference to where the return value should be written
 * func - a reference to the function to execute
 */
typedef struct Quadtree_pthread_meta_t {
    Quadtree *node;
    Point p;
    bool *ret;
    bool (*func)(Quadtree*, Point*, const bool);
} Quadtree_pthread_meta;

/*
 * Quadtree_pthread_execute
 *
 * Executes the given function on a new pthread.
 *
 * data - the metadata reference
 */
void* Quadtree_pthread_execute(void *data) {
    Quadtree_pthread_meta *metadata = (Quadtree_pthread_meta*)data;
    bool ret_val = metadata->func(metadata->node, &metadata->p, true);
    if (metadata->ret != NULL)
        *metadata->ret = ret_val;
    free(metadata);
    return (void*)ret_val;
}

#ifdef PARALLEL
/*
 * Quadtree_pthread_create
 *
 * Creates a pthread which will call the target function, func.
 *
 * pthread - a pthread_t reference
 * pthread_attr - a pthread_attr_t reference
 * node - the root of the tree to start searching at
 * p - the point to search for
 * ret - the result buffer to store the search result
 * func - the target function
 *
 * Returns the return value of pthread_create.
 */
int Quadtree_pthread_create(pthread_t *pthread, pthread_attr_t *pthread_attr,
        Quadtree *node, Point p, bool *ret, bool (*func)(Quadtree*, Point*, const bool)) {
    Quadtree_pthread_meta *data = (Quadtree_pthread_meta*)malloc(sizeof(*data));
    data->node = node;
    data->p = p;
    data->ret = ret;
    data->func = func;
    return pthread_create(pthread, pthread_attr, Quadtree_pthread_execute, (void*)data);
}
#endif

/*
 * Quadtree_search_helper
 *
 * Recursive helper function to traverse the tree horizontally before dropping levels.
 *
 * Invariant: node is always a square.
 *
 * node - the square to look in
 * p - the point to search for
 * parallel - whether to enable parallel constructs
 *
 * Returns whether p is in node.
 */
bool Quadtree_search_helper(Node *node, Point *p, const bool parallel) {
    if (!in_range(node, p))
        return false;

    register uint8_t quadrant = get_quadrant(node->center, p);

    // if the target child is NULL, we try to drop down a level
    if (node->children[quadrant] == NULL) {
        if (node->down != NULL) {
            #ifdef PARALLEL
            if (parallel) {
                pthread_rwlock_rdlock(&node->down->lock);
                pthread_rwlock_unlock(&node->lock);
            }
            #endif
            return Quadtree_search_helper(node->down, p, parallel);
        }
        // otherwise, we're on the bottom-most level and just can't find the point
        else {
            #ifdef PARALLEL
            if (parallel) {
                pthread_rwlock_unlock(&node->lock);
            }
            #endif
            return false;
        }
    }

    // here, the child exists

    // if is a square, move to it and recurse
    if (node->children[quadrant]->is_square) {
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_rdlock(&node->children[quadrant]->lock);
            pthread_rwlock_unlock(&node->lock);
        }
        #endif
        return Quadtree_search_helper(node->children[quadrant], p, parallel);
    }

    // otherwise, we check if the child point matches, since it's a point node
    if (Point_equals(node->children[quadrant]->center, p)) {
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_unlock(&node->children[quadrant]->lock);
        }
        #endif
        return true;
    }

    // if we're here, then we need to branch down a level
    if (node->down != NULL) {
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_rdlock(&node->down->lock);
            pthread_rwlock_unlock(&node->lock);
        }
        #endif
        return Quadtree_search_helper(node->down, p, parallel);
    }

    // here, we have nowhere else to search for, so we give up
    #ifdef PARALLEL
    if (parallel) {
        pthread_rwlock_unlock(&node->children[quadrant]->lock);
    }
    #endif
    return false;
}

/*
 * Quadtree_search_driver
 *
 * What actually runs the search function. This abstraction allows for both serial and
 * parallel call methods.
 *
 * node - the root of the tree to start searching at
 * p - the point of interest
 * parallel - whether to enable parallel constructs
 *
 * Returns whether the point is in the tree.
 */
bool Quadtree_search_driver(Quadtree *node, Point *p, const bool parallel) {
    if (node == NULL)
        return false;

    #ifdef PARALLEL
    if (parallel) {
        pthread_rwlock_rdlock(&node->lock);
    }
    #endif

    while (node->up != NULL) {
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_rdlock(&node->up->lock);
            pthread_rwlock_unlock(&node->lock);
        }
        #endif
        node = node->up;
    }

    return Quadtree_search_helper(node, p, parallel);
}

bool Quadtree_search(Quadtree *node, Point p) {
    return Quadtree_search_driver(node, &p, false);
}

#ifdef PARALLEL
int Quadtree_parallel_search(pthread_t *pthread, pthread_attr_t *pthread_attr,
        Quadtree *node, Point p, bool *ret) {
    return Quadtree_pthread_create(pthread, pthread_attr, node, p, ret, Quadtree_search_driver);
}
#endif

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
 * parallel - whether to enable parallel constructs
 *
 * Returns the corresponding node, one level lower, or NULL if the action failed.
 */
Node* Quadtree_add_helper(Node *node, Point *p, const uint64_t gap_depth, const bool parallel) {
    if (!in_range(node, p))
        return NULL;

    // horizontal traversal
    Node *parent;
    do {
        parent = node;
        node = parent->children[get_quadrant(parent->center, p)];
    } while(node != NULL && node->is_square && in_range(node, p));

    // check for duplication
    if (!gap_depth && node != NULL && !node->is_square && Point_equals(node->center, p))
        return NULL;

    // branch down a level if possible
    Node *down_node = NULL;
    if (parent->down != NULL)
        if ((down_node = Quadtree_add_helper(parent->down, p,
                (gap_depth > 0) && (gap_depth - 1), parallel)) == NULL)
            return NULL;

    // if gap_depth is not zero, we shouldn't actually add anything
    if (gap_depth)
        return down_node;

    #ifdef PARALLEL
    if (parallel) {
        pthread_rwlock_wrlock(&parent->lock);
        pthread_rwlock_wrlock(&down_node->lock);
    }
    #endif
    Node *new_node = Node_init(0.5 * parent->length, *p);
    new_node->parent = parent;

    if (down_node != NULL) {
        new_node->down = down_node;
        down_node->up = new_node;
    }

    // time to try inserting onto this level
    register uint8_t quadrant = get_quadrant(parent->center, p);

    // if the slot is empty, it's trivial
    if (parent->children[quadrant] == NULL) {
        parent->children[quadrant] = new_node;
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_unlock(&parent->lock);
            pthread_rwlock_unlock(&down_node->lock);
        }
        #endif
        return new_node;
    }
    // if it's not empty, that means there's already a node there...
    else {
        // grab the sibling-to-be
        Node *sibling = parent->children[quadrant];
        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_wrlock(&sibling->lock);
        }
        #endif

        // create a new square to contain the sibling and the new node
        uint8_t square_quadrant = quadrant;
        Node *square = Quadtree_init(0.5 * parent->length, get_new_center(parent, quadrant));
        square->parent = parent;

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
        if (parent->down != NULL) {
            Node *down_square = parent->down;
            while (!Point_equals(down_square->center, square->center) ||
                    abs(down_square->length - square->length) > PRECISION)
                down_square = down_square->children[get_quadrant(down_square->center, square->center)];
            #ifdef PARALLEL
            if (parallel) {
                pthread_rwlock_wrlock(&down_square->lock);
            }
            #endif
            square->down = down_square;
            down_square->up = square;
            #ifdef PARALLEL
            if (parallel) {
                pthread_rwlock_unlock(&down_square->lock);
            }
            #endif
        }

        square->parent->children[square_quadrant] = square;
        new_node->parent = square;
        sibling->parent = square;

        #ifdef PARALLEL
        if (parallel) {
            pthread_rwlock_unlock(&parent->lock);
            pthread_rwlock_unlock(&down_node->lock);
            pthread_rwlock_unlock(&sibling->lock);
        }
        #endif
    }

    return new_node;
}

/*
 * Quadtree_add_driver
 *
 * What actually runs the add function. This abstraction allows for both serial and
 * parallel call methods.
 *
 * node - the root of the tree to add to
 * p - the point to add
 * parallel - whether to enable parallel constructs
 *
 * Returns whether the add is successful.
 */
bool Quadtree_add_driver(Quadtree *node, Point *p, const bool parallel) {
    while (rand() % 100 < 50) {
        if (node->up == NULL) {
            #ifdef PARALLEL
            if (parallel) {
                pthread_rwlock_wrlock(&node->lock);
            }
            if (node->up == NULL) {
            #endif
                node->up = Quadtree_init(node->length, *node->center);
                node->up->down = node;
            #ifdef PARALLEL
            }
            if (parallel) {
                pthread_rwlock_unlock(&node->lock);
            }
            #endif
        }
        node = node->up;
    }
    
    register uint64_t gap_depth = 0;  // number of layers to ignore when inserting

    while (node->up != NULL) {
        gap_depth++;
        node = node->up;
    }

    return Quadtree_add_helper(node, p, gap_depth, parallel) != NULL;
}

bool Quadtree_add(Quadtree *node, Point p) {
    return Quadtree_add_driver(node, &p, false);
}

#ifdef PARALLEL
int Quadtree_parallel_add(pthread_t *pthread, pthread_attr_t *pthread_attr,
        Quadtree *node, Point p, bool *ret) {
    return Quadtree_pthread_create(pthread, pthread_attr, node, p, ret, Quadtree_add_driver);
}
#endif

/*
 * Quadtree_remove_node
 *
 * Helper function to remove all instances of the given node and properly relink pointers
 * to it.
 *
 * Invariant: node must have either a parent or a down.
 *
 * node - the node to remove
 * parallel - whether to enable parallel constructs
 *
 * Returns true if removal is successful, false otherwise.
 */
bool Quadtree_remove_node(Node *node, const bool parallel) {
    if (node->down == NULL && node->parent == NULL)
        return false;

    // if is square, determine whether need to remove, and if so, which node to move up
    if (node->is_square) {
        register uint8_t num_children = 0, i;
        Node *child = NULL;
        for (i = 0; i < 4; i++)
            if (node->children[i] != NULL) {
                num_children++;
                child = node->children[i];
            }

        // cannot remove square if more than 1 child
        if (num_children > 1)
            return false;

        // if we have a child, then we relink parent to point to this child, and unlink
        // ourself from the parent
        if (num_children == 1) {
            // however this cannot happen at the root. If we're going to remove a copy of
            // the root, we want no children on that node
            if (node->parent == NULL)
                return false;

            // if all goes well, we can relink
            node->parent->children[get_quadrant(node->parent->center, node->center)] = child;
            child->parent = node->parent;
            node->parent = NULL;
        }

        // otherwise, 0 children, and no problem
    }

    // now, get rid of pointers from the parent
    Node *parent = node->parent, *up = node->up, *down = node->down;
    if (parent != NULL)
        parent->children[get_quadrant(parent->center, node->center)] = NULL;

    // next, unlink pointers from up and down
    if (node->up != NULL) {
        node->up->down = NULL;
        node->up = NULL;
    }
    if (node->down != NULL) {
        node->down->up = NULL;
        node->down = NULL;
    }

    // now, we can get rid of our node
    Node_free(node);

    // then, recurse up the parent as necessary
    if (parent != NULL) {
        register uint8_t num_children = 0, i;
        for (i = 0; i < 4; i++)
            num_children += parent->children[i] != NULL;
        if (num_children < 2)
            Quadtree_remove_node(parent, parallel);
    }

    // finally, recurse on up and down
    if (up != NULL)
        Quadtree_remove_node(up, parallel);
    if (down != NULL)
        Quadtree_remove_node(down, parallel);

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
 * parallel - whether to enable parallel constructs
 *
 * Returns true if the node was successfully removed, false if not.
 */
bool Quadtree_remove_helper(Node *node, Point *p, const bool parallel) {
    if (!in_range(node, p))
        return false;

    register uint8_t quadrant = get_quadrant(node->center, p);

    // if the target child is NULL, we try to drop down a level
    if (node->children[quadrant] == NULL) {
        if (node->down != NULL)
            return Quadtree_remove_helper(node->down, p, parallel);
        // otherwise, we're on the bottom-most level and just can't find the point
        else
            return false;
    }

    // here, the child exists

    // if is a square, move to it and recurse if in range
    if (node->children[quadrant]->is_square && in_range(node->children[quadrant], p))
        return Quadtree_remove_helper(node->children[quadrant], p, parallel);

    // otherwise, we check if the child point matches, since it's a point node
    if (Point_equals(node->children[quadrant]->center, p))
        return Quadtree_remove_node(node->children[quadrant], parallel);

    // if we're here, then we need to branch down a level
    if (node->down != NULL)
        return Quadtree_remove_helper(node->down, p, parallel);

    // here, we have nowhere else to search for, so we give up
    return false;
 
}

/*
 * Quadtree_remove_driver
 *
 * What actually runs the remove function. This abstraction allows for both serial and
 * parallel call methods.
 *
 * node - the root of the tree to remove from
 * p - the point to remove
 * parallel - whether to enable parallel constructs
 *
 * Returns whether the remove is successful.
 */
bool Quadtree_remove_driver(Quadtree *node, Point *p, const bool parallel) {
    while (node->up != NULL)
        node = node->up;

    return Quadtree_remove_helper(node, p, parallel);
}

bool Quadtree_remove(Quadtree *node, Point p) {
    return Quadtree_remove_driver(node, &p, false);
}

#ifdef PARALLEL
int Quadtree_parallel_remove(pthread_t *pthread, pthread_attr_t *pthread_attr,
        Quadtree *node, Point p, bool *ret) {
    return Quadtree_pthread_create(pthread, pthread_attr, node, p, ret, Quadtree_remove_driver);
}
#endif

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
    result->leaf += !node->is_square;

    // should no longer have references
    Node_free(node);

    return success;
}

QuadtreeFreeResult Quadtree_free(Quadtree *root) {
    QuadtreeFreeResult result = (QuadtreeFreeResult){ .total = 0, .leaf = 0, .levels = 0 };

    while (root->up != NULL)
        root = root->up;

    while (root != NULL) {
        Node *next_root = root->down;
        result.levels += Quadtree_free_helper(root, &result);
        root = next_root;
    }

    return result;
}
