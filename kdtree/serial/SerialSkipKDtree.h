/**
Data structures for the serial implementation of SkipKDtree
*/

#ifndef SERIAL_SKIP_KDTREE_H
#define SERIAL_SKIP_KDTREE_H

#include <stdbool.h>

typedef struct SerialSkipKDtree_t KDtree;

/*
 * struct SerialSkipKDtree
 *
 * The overarching data structure that the client interacts with. In and of itself, it is
 * a container for a bunch of pointers related to the data structure itself.
 *
 * tree - a reference to the tree structure
 * list - a reference to the linked list of points that are related to this tree
 * D - the number of dimensions in this tree
 */
struct SerialSkipKDtree_t {
    void *tree, *list;
    uint64_t D;
};

// isolate the internal representations from use by the client
#ifdef SERIAL_SKIP_KDTREE_C
typedef struct SerialSkipKDtreeNode_t KDnode;
typedef enum {POINT, REGION} NodeType;

/*
 * struct SerialSkipKDtreeNode_t
 *
 * Stores information about a node in the kd-tree. In particular, this is how we
 * represent the tree structure, with references down to the information structure.
 *
 * type - takes on values of either POINT or REGION, as an enum
 * center - center of the REGION, or coordinates of the POINT, in a ListNode
 * length - side length of the REGION. This means that the boundaries are
 *     length/2 distance from the center. Does not matter for a POINT
 * parent - the parent node in the same level; NULL if a root node
 * up - the clone of the same node in the next level, if it exists
 * down - the clone of the same node in the previous level; NULL if at lowest level
 * children - the children of the container; this is represented in a binary tree
 *     structure, using the ListNodeTree struct. Children are accessed using Partition
 *     structs to denote which branches of the tree to take.
 */
struct SerialSkipKDtreeNode_t {
    NodeType type;
    ListNode* center;
    float64_t length;
    Node *parent;
    Node *up, *down;
    ListNodeTree* children;
#ifdef KDTREE_TEST
    uint64_t id;
#endif
};

#include "./SerialList.h"
#include "./SerialBinaryTree.h"
#endif

#endif
