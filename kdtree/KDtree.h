/**
Interface for KDtree data structure
*/

#ifndef KDTREE_H
#define KDTREE_H

#include <stdio.h>

#include "./util.h"
#include "./types.h"
#include "./Point.h"
#include "./Partition.h"

#include "serial/SerialSkipKDtree.h"

/*
 * KDtree_create
 *
 * Allocates memory for a the kd-tree.
 *
 * The return value of this constructor is the object that will serve as the interface
 * between the client and the internal representation, which is slightly complicated.
 *
 * length - the side length of the REGION that we are working in. The valid REGION is
 *          thus length/2 from the origin in each dimension, including the lower bound,
 *          excluding the upper bound. Thus, if length = 2, and the center is (0, 0), the
 *          point (-1, -1) is included, but (1, 1) is not.
 * origin - the origin of the REGION that we are working in.
 * D      - the number of dimensions
 *
 * Returns the created struct.
 */
KDtree KDtree_create(float64_t length, Point origin, uint64_t D);

/*
 * KDtree_search
 *
 * Searches for p in the kd-tree represented by tree, within a certain error tolerance.
 *
 * tree - the tree to search in
 * p - the point to search for
 *
 * Returns whether p is in the kd-tree represented by tree.
 */
bool KDtree_search(KDtree* tree, Point* p);

/*
 * KDtree_add
 *
 * Adds p to the kd-tree represented by tree.
 *
 * tree - the tree to add to
 * p - the point being added
 *
 * Returns whether the add was successful
 */
bool KDtree_add(KDtree* tree, Point* p);

/*
 * KDtree_remove
 *
 * Removes p from the kd-tree represented by tree.
 *
 * tree - the tree to remove from
 * p - the point being removed
 *
 * Returns whether the remove was successful: false typically indicates that the point
 * wasn't in the tree to begin with.
 */
bool KDtree_remove(KDtree* tree, Point* p);

/*
 * KDtree_uproot
 *
 * Uproot removes the entire tree after freeing everything used to represent the tree.
 *
 * The freeing order is such that, if a free fails, retrying won't
 * cause memory leaks.
 *
 * Note that, after this operation succeeds, the input parameter no longer points to
 * allocated memory.
 */
bool KDtree_uproot(KDtree* tree);

#ifdef SKIP_QUADTREE_C
/*
 * in_range
 *
 * Returns true if p is on or within the boundaries of n, false otherwise.
 *
 * On-boundary counts as being within if on the left or bottom boundaries.
 * Points and nodes will be assumed to have values in the dimensions specified.
 *
 * n - the REGION node to check at
 * p - the point to check for
 * D - the number of dimensions to check
 *
 * Returns whether p is within the boundaries of n.
 */
static inline bool in_range(KDnode* n, Point p, uint64_t D);

/*
 * get_partition
 *
 * Returns the partition that p is in, relative to the origin point, as a Partition
 * object, for the given number of dimensions. Both points are assumed to have values
 * in the dimensions specified.
 *
 * origin - the point representing the origin of the bounding REGION
 * p - the point we're trying to find the partition of
 * D - the number of dimensions
 *
 * Returns the partition that p is in, relative to origin.
 */
static inline Partition get_partition(Point* origin, Point* p, uint64_t D);

/*
 * get_new_center
 *
 * Given the current node and a partition, returns the Point representing the center of
 * the REGION that represents that partition, with the given number of dimensions.
 *
 * node - the parent node
 * partition - the Partition to determine the new center for
 * D - the number of dimensions
 *
 * Returns the center point for the given partition of node.
 */
static inline Point get_new_center(Node* node, Partition partition, uint64_t D);

#ifdef KDTREE_TEST
/*
 * Node_create
 *
 * Allocates memory for and initializes an empty node in the kd-tree.
 *
 * length - the "length" of the node -- irrelevant for a leaf node, but necessary
 *          for an internal node (region)
 * center - the center of the node
 * type - a NodeType type
 * D - the number of dimensions
 *
 * Returns a pointer to the created node.
 */
Node* Node_create(float64_t length, Point center, NodeType type, uint64_t D);

/*
 * Node_string
 *
 * Writes value of node to the given string buffer.
 *
 * node - the node to write
 * buffer - the buffer to write to
 */
static inline void Node_string(Node* node, char* buffer) {
    /*sprintf(buffer, "Node{is_REGION = %s, center = (", (node->is_REGION ? "YES" : "NO"));
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
    }*/
}
#endif

#endif

#endif
