/**
Interface for Quadtree data structure
*/

#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>

#include "./util.h"
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
 *     length/2 distance from the center-> Does not matter for a point
 * parent - the parent node in the same level; NULL if a root node
 * up - the clone of the same node in the next level, if it exists
 * down - the clone of the same node in the previous level; NULL if at lowest level
 * children - the four children of the square; each entry is NULL if there is no child
 *     there. Each index refers to a quadrant, such that children[0] is Q1, [1] is Q2,
 *     and so on. Should never be all NULL unless node is a point
 */
struct SerialSkipQuadtreeNode_t {
    bool is_square;
    Point* center;
    float64_t length;
    Node *parent;
    Node *up, *down;
    Node *children[4];
#ifdef QUADTREE_TEST
    uint64_t id;
#endif
};

/*
typedef struct ParallelSkipQuadtreeNode_t Node;
*/

typedef Node Quadtree;

#ifdef QUADTREE_TEST
/*
 * Node_create
 *
 * Allocates memory for and initializes an empty leaf node in the quadtree.
 *
 * length - the "length" of the node -- irrelevant for a leaf node, but necessary
 *          for an internal node (square)
 * center - the center of the node
 *
 * Returns a pointer to the created node.
 */
Node* Node_create(float64_t length, Point center);
#endif

/*
 * Quadtree_create
 *
 * Allocates memory for and initializes an empty internal node in the quadtree.
 *
 * In SerialSkipQuadtree, this is implemented as a wrapper for Node_create, setting
 * the resultant node's is_square boolean to true before returning it.
 *
 * length - the "length" of the node -- irrelevant for a leaf node, but necessary
 *          for an internal node (square)
 * center - the center of the node
 *
 * Returns a pointer to the created node.
 */
Quadtree* Quadtree_create(float64_t length, Point center);

/*
 * Quadtree_search
 *
 * Searches for p in the quadtree pointed to by node, within a certain error tolerance.
 *
 * node - the root node to start at
 * p - the point we're searching for
 *
 * Returns whether p is in the quadtree represented by node.
 */
bool Quadtree_search(Quadtree* node, Point* p);

/*
 * Quadtree_add
 *
 * Adds p to the quadtree represented by node, the root.
 *
 * node - the root node of the tree to add to
 * p - the point being added
 *
 * Returns whether the add was successful
 */
bool Quadtree_add(Quadtree* node, Point* p);

/*
 * Quadtree_remove
 *
 * Removes p from the quadtree represented by node, the root.
 *
 * node - the root node of the tree to remove from
 * p - the point being removed
 *
 * Returns whether the remove was successful: false typically indicates that the node
 * wasn't in the tree to begin with.
 */
bool Quadtree_remove(Quadtree* node, Point* p);

/*
 * Quadtree_uproot
 *
 * Uproot removes the entire tree after freeing every node.
 *
 * The freeing order is such that, if a free fails, retrying won't
 * cause memory leaks.
 *
 * This can also be used to uproot subtrees.
 */
bool Quadtree_uproot(Quadtree* root);

/*
 * in_range
 *
 * Returns true if p is within the boundaries of n, false otherwise.
 *
 * On-boundary counts as being within if on the left or bottom boundaries.
 *
 * n - the square node to check at
 * p - the point to check for
 *
 * Returns whether p is within the boundaries of n.
 */
static inline bool in_range(Node* n, Point* p) {
    return n->center->x - n->length / 2 <= p->x &&
        n->center->x + n->length / 2 > p->x &&
        n->center->y - n->length / 2 <= p->y &&
        n->center->y + n->length / 2 > p->y;
}

/*
 * get_quadrant
 *
 * Returns the quadrant [0,4) that p is in, relative to the origin point.
 *
 * origin - the point representing the origin of the bounding square
 * p - the point we're trying to find the quadrant of
 *
 * Returns the quadrant that p is in, relative to origin.
 */
static inline int get_quadrant(Point* origin, Point* p) {
    return (p->x >= origin->x) + 2 * (p->y >= origin->y);
}

/*
 * get_new_center
 *
 * Given the current node and a quadrant, returns the Point representing
 * the center of the square that represents that quadrant.
 *
 * node - the parent node
 * quadrant - the quadrant to search for; must be in range [0,4)
 *
 * Returns the center point for the given quadrant of node.
 */
static inline Point get_new_center(Node* node, int quadrant) {
    return (Point){
        .x = node->center->x + ((quadrant % 2) - 0.5) * 0.5 * node->length,
        .y = node->center->y + ((quadrant / 2) - 0.5) * 0.5 * node->length
        };
}

#ifdef QUADTREE_TEST
/*
 * Node_string
 *
 * Writes value of node to the given string buffer.
 *
 * node - the node to write
 * buffer - the buffer to write to
 */
static inline void Node_string(Node* node, char* buffer) {
    sprintf(buffer, "Node{is_square = %s, center = (%f, %f), length = %lf, parent = %s, up = %s, down = %s, children = {%s, %s, %s, %s}}",
        (node->is_square ? "YES" : "NO"), node->center->x, node->center->y, node->length,
        (node->parent == NULL ? "NO" : "YES"), (node->up == NULL ? "NO" : "YES"),
        (node->down == NULL ? "NO" : "YES"),
        (node->children[0] == NULL ? "NO" : "YES"), (node->children[1] == NULL ? "NO" : "YES"),
        (node->children[2] == NULL ? "NO" : "YES"), (node->children[3] == NULL ? "NO" : "YES"));
    /*sprintf(buffer, "Node{is_square = %s, center = (%f, %f), length = %lf, parent = %p, up = %p, down = %p, children = {%p, %p, %p, %p}}",
        (node->is_square ? "YES" : "NO"), node->center->x, node->center->y, node->length,
        node->parent, node->up, node->down,
        node->children[0], node->children[1], node->children[2], node->children[3]);*/
}
#endif

#endif
