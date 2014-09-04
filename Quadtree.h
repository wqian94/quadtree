/**
Interface for Quadtree data structure
*/

#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdbool.h>

#include "./types.h"
#include "./Point.h"

typedef struct SerialSkipQuadtreeNode_t Node;

/*
 * struct SerialSkipQuadtreeNode_t
 *
 * Stores information about a node in the quadtree
 *
 * is_square - true if node is a square, false if is a point
 * center - center of the square, or coordinates of the point
 * length - side length of the square. This means that the boundaries are
 *     length/2 distance from the center. Does not matter for a point
 * parent - the parent node in the same level; NULL if a root node
 * up - the clone of the same node in the next level, if it exists
 * down - the clone of the same node in the previous level; NULL if at lowest level
 * children - the four children of the square; each entry is NULL if there is no child
 *     there. Each index refers to a quadrant, such that children[0] is Q1, [1] is Q2,
 *     and so on. Should never be all NULL unless node is a point
 */
struct SerialSkipQuadtreeNode_t {
    bool is_square;
    Point center;
    float64_t length;
    Node *parent;
    Node *up, *down;
    Node *children[4];
};

/*
typedef struct ParallelSkipQuadtreeNode_t Node;
*/

typedef Node Quadtree;

Quadtree* Quadtree_create(float64_t length, Point center);
bool Quadtree_search(Quadtree* node, Point* p);
bool Quadtree_add(Quadtree* node, Point* p);
bool Quadtree_remove(Quadtree* node, Point* p);

/*
 * Uproot removes the entire tree after freeing every node
 *
 * The freeing order is such that, if a free fails, retrying won't
 * cause memory leaks.
 *
 * This can also be used to uproot subtrees.
 */
bool Quadtree_uproot(Quadtree* root);

// For testing, ensure all functions can be tested
#ifdef QUADTREE_TEST_H
static inline bool in_range(Node* n, Point* p);
static inline int get_quadrant(Point* origin, Point* p);
static inline Point get_new_center(Node* node, int quadrant);
Node* Quadtree_add_helper(Node* node, Point* p);
#endif

#endif
