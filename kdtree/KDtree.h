/**
Interface for KDtree data structure
*/

#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>

#include "./constants.h"
#include "./util.h"
#include "./types.h"
#include "./Point.h"

typedef struct SerialSkipKDtreeNode_t Node;

/*
 * struct SerialSkipKDtreeNode_t
 *
 * Stores information about a node in the kd-tree
 *
 * is_container - true if node is a container, false if is a point
 * center - center of the container, or coordinates of the point
 * length - side length of the container. This means that the boundaries are
 *     length/2 distance from the center. Does not matter for a point
 * parent - the parent node in the same level; NULL if a root node
 * up - the clone of the same node in the next level, if it exists
 * down - the clone of the same node in the previous level; NULL if at lowest level
 * children - the four children of the container; each entry is NULL if there is no child
 *     there. Each index refers to a partition, such that children[0] is Q1, [1] is Q2,
 *     and so on. Should never be all NULL unless node is a point
 */
struct SerialSkipKDtreeNode_t {
    bool is_container;
    Point center;
    float64_t length;
    Node *parent;
    Node *up, *down;
    Node *children[N_PARTITIONS];
#ifdef KDTREE_TEST
    uint64_t id;
#endif
};

/*
typedef struct ParallelSkipKDtreeNode_t Node;
*/

typedef Node KDtree;

#ifdef KDTREE_TEST
/*
 * Node_create
 *
 * Allocates memory for and initializes an empty leaf node in the kd-tree.
 *
 * length - the "length" of the node -- irrelevant for a leaf node, but necessary
 *          for an internal node (container)
 * center - the center of the node
 *
 * Returns a pointer to the created node.
 */
Node* Node_create(float64_t length, Point center);
#endif

/*
 * KDtree_create
 *
 * Allocates memory for and initializes an empty internal node in the kd-tree.
 *
 * In SerialSkipKDtree, this is implemented as a wrapper for Node_create, setting
 * the resultant node's is_container boolean to true before returning it.
 *
 * length - the "length" of the node -- irrelevant for a leaf node, but necessary
 *          for an internal node (container)
 * center - the center of the node
 *
 * Returns a pointer to the created node.
 */
KDtree* KDtree_create(float64_t length, Point center);

/*
 * KDtree_search
 *
 * Searches for p in the kd-tree pointed to by node, within a certain error tolerance.
 *
 * node - the root node to start at
 * p - the point we're searching for
 *
 * Returns whether p is in the kd-tree represented by node.
 */
bool KDtree_search(KDtree* node, Point* p);

/*
 * KDtree_add
 *
 * Adds p to the kd-tree represented by node, the root.
 *
 * node - the root node of the tree to add to
 * p - the point being added
 *
 * Returns whether the add was successful
 */
bool KDtree_add(KDtree* node, Point* p);

/*
 * KDtree_remove
 *
 * Removes p from the kd-tree represented by node, the root.
 *
 * node - the root node of the tree to remove from
 * p - the point being removed
 *
 * Returns whether the remove was successful: false typically indicates that the node
 * wasn't in the tree to begin with.
 */
bool KDtree_remove(KDtree* node, Point* p);

/*
 * KDtree_uproot
 *
 * Uproot removes the entire tree after freeing every node.
 *
 * The freeing order is such that, if a free fails, retrying won't
 * cause memory leaks.
 *
 * This can also be used to uproot subtrees.
 */
bool KDtree_uproot(KDtree* root);

/*
 * in_range
 *
 * Returns true if p is within the boundaries of n, false otherwise.
 *
 * On-boundary counts as being within if on the left or bottom boundaries.
 *
 * n - the container node to check at
 * p - the point to check for
 *
 * Returns whether p is within the boundaries of n.
 */
static inline bool in_range(Node* n, Point* p) {
    register uint64_t i;
    for (i = 0; i < D; i++) {
        if (abs(p->val[i] - n->center.val[i]) > n->length / 2)
            return false;
    }
    return true;
}

/*
 * get_partition
 *
 * Returns the partition [0,2^D) that p is in, relative to the origin point.
 *
 * origin - the point representing the origin of the bounding container
 * p - the point we're trying to find the partition of
 *
 * Returns the partition that p is in, relative to origin.
 */
static inline uint64_t get_partition(Point* origin, Point* p) {
    register uint64_t i, partition = 0;
    for (i = D - 1; i >= 0; i--) {
        partition = (partition << 1) + (p->val[i] >= origin->val[i]);
        if (i == 0)
            break;
    }
    return partition;
}

/*
 * get_new_center
 *
 * Given the current node and a partition, returns the Point representing
 * the center of the container that represents that partition.
 *
 * node - the parent node
 * partition - the partition to search for; must be in range [0,N_PARTITIONS)
 *
 * Returns the center point for the given partition of node.
 */
static inline Point get_new_center(Node* node, uint64_t partition) {
    Point p;

    register uint64_t i;
    for (i = 0; i < D; i++) {
        p.val[i] = node->center.val[i] + (2 * (int)((partition >> i) % 2) - 1) * 0.25 * node->length;
    }

    return p;
}

#ifdef KDTREE_TEST
/*
 * Node_string
 *
 * Writes value of node to the given string buffer.
 *
 * node - the node to write
 * buffer - the buffer to write to
 */
static inline void Node_string(Node* node, char* buffer) {
    sprintf(buffer, "Node{is_container = %s, center = (", (node->is_container ? "YES" : "NO"));
    register uint64_t i;
    for (i = 0; i < D; i++) {
        sprintf(buffer, "%s%f%s", buffer, node->center.val[i], i == D - 1 ? "" : ", ");
    }
    sprintf(buffer, "%s), length = %lf, parent = %s, up = %s, down = %s, children = {",
        buffer, node->length,
        (node->parent == NULL ? "NO" : "YES"), (node->up == NULL ? "NO" : "YES"),
        (node->down == NULL ? "NO" : "YES"));
    for (i = 0; i < N_PARTITIONS; i++) {
        sprintf(buffer, "%s%s%s", buffer, node->children[i] == NULL ? "NO" : "YES", i == D - 1 ? "}}" : ", ");
    }
}
#endif

#endif
