/**
BinaryTreeNode library, for binary trees
*/

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "./Partition.h"

typedef struct SerialBinaryTreeNode_t BinaryTreeNode;
typedef BinaryTreeNode BinaryTree;

/*
 * struct BinaryTreeNode_t
 *
 * Data structure for storing children of a KDtree.
 *
 * Implemented as a binary search tree, the depth is guaranteed to be at most D, the
 * number of dimensions. Partition and leaf should only be present in leaf nodes, where
 * left and right are both NULL.
 *
 * removed - the logical remove flag
 * partition - the partition that this binary tree node represents
 * leaf - a leaf of the tree, for use at the very bottom of the tree, to retrieve
 *      the appropriate KDnode struct
 * left - the left branch of the binary tree
 * right - the right branch of the binary tree
 */
struct SerialBinaryTreeNode_t{
    bool removed;
    Partition* partition;
    KDnode* leaf;
    BinaryTreeNode *left, *right;
};

/*
 * BinaryTree_create
 *
 * Creates an empty BinaryTreeNode, with the removed flag as false, and left and right
 * pointers to NULL.
 *
 * Returns a reference to a BinaryTree as described.
 */
BinaryTree* BinaryTree_create() {
    BinaryTree* node = (BinaryTree*)malloc(sizeof(BinaryTree));
    node->removed = false;
    node->partition = NULL;
    node->leaf = NULL;
    node->left = node->right = NULL;
    return node;
}

/*
 * BinaryTree_add
 *
 * Adds a new KDnode to this tree, with the given partition.
 *
 * tree - the BinaryTree to insert into
 * partition - the Partition to use
 * node - the KDnode to insert
 *
 * Returns the BinaryTreeNode that contains node.
 */
BinaryTreeNode* BinaryTree_add(BinaryTree* tree, Partition* partition, KDnode* node) {
    register int i;
    for (i = 0; i < partition->dim; i++) {
        if (tree->left == NULL && tree->right == NULL)
            break;

        if (Partition_get(partition, i) == LEFT) {
            if (tree->left == NULL)
                tree->left = BinaryTree_create();
            tree = tree->left;
        }
        else {
            if (tree->right == NULL)
                tree->right = BinaryTree_create();
            tree = tree->right;
        }
    }

    if (tree->leaf != NULL) {
        if (Partition_get(partition, i) == LEFT) {
            // move current data to a child
            tree->right = BinaryTree_create();
            tree->right->partition = tree->partition;
            tree->right->leaf = tree->leaf;
            tree->partition = NULL;
            tree->leaf = NULL;

            tree->left = BinaryTree_create();
            tree = tree->left;
        }
        else {
            // move current data to a child
            tree->left = BinaryTree_create();
            tree->left->partition = tree->partition;
            tree->left->leaf = tree->leaf;
            tree->partition = NULL;
            tree->leaf = NULL;

            tree->right = BinaryTree_create();
            tree = tree->right;
        }
    }

    tree->partition = partition;
    tree->leaf = node;

    return tree;
}

/*
 * BinaryTree_remove_subtree
 *
 * Removes the binary tree node representing the given partition from the subtree, and
 * frees its memory, where the partition is accessed using the provided dimension.
 *
 * tree - the BinaryTree subtree, expected to not be the node to be removed
 * partition - the partition to remove
 * dimension - the dimension to examine
 *
 * Returns true if successful (partition found), false otherwise.
 */
bool BinaryTree_remove_subtree(BinaryTree* tree, Partition* partition, uint64_t dimension) {
    if (Partition_get(partition, dimension) == LEFT) {
        if (tree->left->leaf != NULL) {  // parent of leaf node we might be interested in
            if (Partition_equals(tree->left->partition, partition)) {
                // collapse right and free children
                tree->left->removed = true;
                tree->partition = tree->right->partition;
                tree->leaf = tree->right->leaf;
                tree->left = tree->right->left;
                tree->right = tree->right->right;

                free(tree->left);
                free(tree->right);
                return true;
            }
            return false;
        }
        return BinaryTree_remove_subtree(tree->left, partition, dimension + 1);
    }
    else {
        if (tree->right->leaf != NULL) {  // parent of leaf we might be interested in
            if (Partition_equals(tree->right->partition, partition)) {
                // collapse left and free children
                tree->right->removed = true;
                tree->partition = tree->left->partition;
                tree->leaf = tree->left->leaf;
                tree->left = tree->left->left;
                tree->right = tree->left->right;

                free(tree->left);
                free(tree->right);
                return true;
            }
            return false;
        }
        return BinaryTree_remove_subtree(tree->right, partition, dimension + 1);
    }

    return removed;
}

/*
 * BinaryTree_remove
 *
 * Removes the binary tree node representing the given partition, and frees its memory.
 *
 * tree - the BinaryTree to remove from
 * partition - the partition to remove
 *
 * Returns true if successful (partition found), false otherwise.
 */
bool BinaryTree_remove(BinaryTree* tree, Partition* partition) {
    if (tree->left == NULL && tree->right == NULL) {
        if (Partition_equals(tree->partition, partition)) {
            tree->removed = true;
            free(tree);
            return true;
        }
        return false;
    }

    return BinaryTree_remove_subtree(tree, partition, 0);
}

/*
 * BinaryTree_search
 *
 * Returns the KDnode in a given partition, or NULL if no partition is found.
 *
 * tree - the BinaryTree to search in
 * partition - the partition to search for
 *
 * Returns a reference to the KDnode that is in the given partition, or NULL if that
 * partition is not found in the tree.
 */
KDnode* BinaryTree_search(BinaryTree* tree, Partition* partition) {
    register uint64_t i;
    for (i = 0; i < partition->dim; i++) {
        if (tree->leaf != NULL)
            break;

        if (Partition_get(i) == LEFT)
            tree = tree->left;
        else
            tree = tree->right;
    }

    return !tree->removed && Partition_equals(tree->partition, partition) ? tree->leaf : NULL;
}

/*
 * BinaryTree_uproot
 *
 * Frees all memory related to this binary tree.
 *
 * tree - the BinaryTree to uproot
 */
void BinaryTree_uproot(BinaryTree* tree) {
    if (tree->left != NULL)
        BinaryTree_uproot(tree->left);
    if (tree->right != NULL)
        BinaryTree_uproot(tree->right);

    tree->removed = true;
    free(tree);
}
#endif
