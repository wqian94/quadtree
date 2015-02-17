/**
Serial Skip KDtree implementation
*/

#ifndef SKIP_KDTREE_C
#define SKIP_KDTREE_C
#define SERIAL_SKIP_KDTREE_C

#include <assert.h>
#include <stdlib.h>

#include "../types.h"
#include "../KDtree.h"
#include "../Point.h"

typedef KDtree Tree;
typedef KDnode Node;
typedef LinkedList List;
typedef BinaryTree BTree;

// rand() functions
#ifdef KDTREE_TEST
extern uint32_t test_rand();
#define rand() test_rand()
static uint64_t KDTREE_NODE_COUNT = 0;
#else
extern uint32_t Marsaglia_rand();
#define rand() Marsaglia_rand()
extern void Marsaglia_srand(uint32_t);
#define srand(x) Marsaglia_srand(x)
#endif

static inline bool in_range(Node* n, Point p, uint64_t D) {
    register uint64_t i;
    for (i = 0; i < D; i++) {
        if (    p.val[i] <  n->center.val[i] - n->length / 2 ||
                p.val[i] >= n->center.val[i] + n->length / 2)
            return false;
    }
    return true;
}

static inline Partition get_partition(Point* origin, Point* p, uint64_t D) {
    Partition part = Partition_create(D);
    register uint64_t i;
    for (i = 0; i < D; i++)
        Partition_set(part, i, (p->val[i] < origin->val[i] ? LEFT : RIGHT));

    return partition;
}

static inline Point get_new_center(Node* node, Partition partition, uint64_t D) {
    Point p;

    float64_t data[D];
    register uint64_t i;
    for (i = 0; i < D; i++)
        p.val[i] = node->center.val[i] + (2 * (Partition_get(partition, i) == RIGHT) - 1) * 0.25 * node->length;

    return p;
}

Tree KDtree_create(float64_t length, Point center, uint64_t D) {
    return (Tree){.tree = NULL, .list = NULL, .D = D};
}

Node* Node_create(float64_t length, Point center, NodeType type, uint64_t D) {
    Node* node = (Node*)malloc(sizeof(Node));
    *node = (Node){.type = type, .parent = NULL,
        .up = NULL, .down = NULL, //.children is not initialized here
        .length = length, .center = center,
        .children = BinaryTree_create()
#ifdef KDTREE_TEST
, .id = (KDTREE_NODE_COUNT++)
#endif
        };
    return node;
}

bool KDtree_search_helper(Tree* tree, Node* node, Point* p) {
    if (node == NULL)  // shouldn't happen, but just in case...
        return false;

    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    // call starts at root of the highest-order tree
    Partition partition = get_partition(&node->center, p, tree->D);

    Node* child = BinaryTree_search(node->children, &partition);

    // no such child on this level
    if (child == NULL) {
        // in case we didn't catch this earlier
        if (node->type == REGION && Point_equals(node->center, *p))
            return true;
        // move down a level and continue if possible
        else if (node->down != NULL)
            return KDtree_search_helper(tree, node->down, p);
        // otherwise, conclusively not found
        return false;
    }

    // otherwise, recurse on partitions
    return KDtree_search(tree, child, p);
}

bool KDtree_search(KDtree* tree, Point* p) {
    if (tree->tree == NULL)
        return false;
    return KDtree_search_helper(tree, (Node*)tree->tree, p);
}

Node* KDtree_add_helper(Tree* tree, Node* node, Point* p) {
    // find where to insert node
    Node *parent = node, *child;
    Partition partition = get_partition(&parent->center, p);
    //Node* child = BinaryTree_search(parent
    while ((child = BinaryTree_search(parent->children, &partition)) != NULL && child->type == REGION) {
        parent = child;
        partition = get_partition(&parent->center, p);
    }

    // recursively create down-nodes
    Node* down_clone = NULL;
    if (parent->down != NULL)
        down_clone = KDtree_add_helper(tree, parent->down, p);

    // create the new node
    Node* new_node = Node_create(0.5 * parent->length, *p, NodeType.POINT, tree->D);

    // update new node's parent and down pointers
    new_node->parent = parent;
    new_node->down = down_clone;

    //update down node's up pointer if down node exists
    if (down_clone != NULL)
        down_clone->up = new_node;

    // if target slot is empty, add it in!
    if (child == NULL) {
        // find the corresponding container in the parent, which we know has to exist
        Node* parent_down = down_clone;
        while (parent_down != NULL && !Point_equals(parent_down->center, parent->center))
            parent_down = parent_down->parent;

        // update pointers of parent nodes
        parent->down = parent_down;
        if (parent_down != NULL)
            parent_down->up = parent;

        BinaryTree_add(parent->children, &partition, new_node);
    }
    // if target slot isn't empty, we're going to have to create a container for it
    else {
        // create the new region
        Node* region = Node_create(new_node->length, get_new_center(parent, partition), NodeType.REGION);
        Partition region_partition = partition;

        // update the length of the new node
        new_node->length *= 0.5;

        // grab the conflicting node
        Node* sibling = BinaryTree_search(parent->children, &partition);
        sibling->length = new_node->length;
        Partition sibling_partition;

        // find the next region that buckets new_node and sibling in different partitions
        while (Partition_equals(sibling_partition = get_partition(&region->center, &sibling->center), 
                partition = get_partition(&region->center, &new_node->center))) {
            // update center and length of region
            region->center = get_new_center(container, partition);
            region->length = new_node->length;

            // update lengths of new_node and sibling
            new_node->length *= 0.5;
            sibling->length = new_node->length;
        }

        // update region's children
        BinaryTree_add(region->children, &partition, new_node);
        BinaryTree_add(region->children, &sibling_partition, sibling);

        // find region's down by checking up the tree from the bottom child
        Node* region_down = child->down;
        while (region_down != NULL && (!Point_equals(region_down->center, region->center) || !region_down->is_region))
            region_down = region_down->parent;

        region->down = region_down;
        if (region_down != NULL)
            region_down->up = region;

        // update parent's children
        BinaryTree_add(parent->children, &region_partition, region);

        // update parent nodes
        region->parent = parent;
        new_node->parent = region;
        sibling->parent = region;
    }
    // two possibilities: target slot is empty (great), target slot is not empty (darn)
    // target slot is empty: just insert and set pointers
    // target slot is not empty: create new compressed region and swap
    return new_node;
}

bool KDtree_add(KDtree* tree, Point* p) {
    Node* node = (Node*)tree->tree;
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    while (rand() % 100 < 50) {  // branch to highest level in tree, creating levels as needed
        if (node->up == NULL) {  // create new level
            KDtree* up = Node_create(node->length, node->center);
            up->type = NodeType.REGION;
            up->down = node;
            node->up = up;
        }
        node = node->up;
    }
    KDtree_add_helper(tree, node, p);
    return true;
}

// updates the pointers relating to the container if needed
/*
 * update_region
 *
 * Updates the provided node to ensure valid representation, but only if node is a
 * region.
 *
 * In particular:
 * - If node has only 1 child, we will replace the container with that one child.
 * - If node has no children, we will delete the container, detroying a level if that
 *   region was a root node, unless we're at the lowest-level region.
 * - Otherwise, do nothing.
 *
 * node - the region that we're attempting to update
 */
static inline void update_region(Node* node) {
    if (node == NULL || node->type != NodeType.REGION)
        return;

    int i, count_children = BinaryTree_leaf_count(node->children);

    // delete container if no children left: should only really happen at the root
    if (count_children == 0) {
        // if we're at the root of the lowest level, don't do anything
        if (node->parent != NULL || node->down != NULL) {
            // unset pointer from parent
            if (node->parent != NULL)
                BinaryTree_remove(node->parent->children,
                    &get_partition(&node->parent->center, &node->center));

            // reset pointer from down
            if (node->down != NULL)
                node->down->up = node->up;

            // reset pointer from up
            if (node->up != NULL)
                node->up->down = node->down;

            free(node);
        }
    }
    // if only 1 child left, replace region with that child
    else if (count_children == 1) {
        // only_child can't be NULL because we already checked
        Node* only_child = BinaryTree_find_leaf(node->children);

        // if not at root, do the replacement; if at root, do nothing
        if (node->parent != NULL) {
            BinaryTree_add(node->parent->children, &get_partition(&node->parent->center, &node->center), only_child);
            only_child->parent = node->parent;

            // reset pointer from down
            if (node->down != NULL)
                node->down->up = node->up;

            // reset pointer from up
            if (node->up != NULL)
                node->up->down = node->down;

            free(node);
        }
    }
}

bool KDtree_remove_helper(Node* node, Point* p) {
    if (node == NULL)  // one base case
        return false;

    if (!in_range(node, p))  // boundary check
        return false;

    if (node->is_container) {
        bool removed = KDtree_remove(node->children, &get_partition(&node->center, p), p);

        // if we made a change, check the rep to see if this container is needed anymore
        if (removed) {
            update_region(node);
        }
        // otherwise, the node might not be on this level, so drop a level
        else {
            removed = KDtree_remove(node->down, p);
        }

        return removed;
    }
    // otherwise, we're at a leaf!
    else {
        // if the point doesn't match, then we haven't found it
        if (!Point_equals(node->center, *p))
            return false;

        // otherwise, this is the point!

        // cache down point for later
        Node* down = node->down;

        // leaf nodes are guaranteed parent nodes
        Partition partition = get_partition(&node->parent->center, p);

        // reset pointers
        BinaryTree_remove(node->parent->children, &partition);

        if (node->down != NULL)
            node->down->up = node->up;

        if (node->up != NULL)
            node->up->down = node->down;

        // reset parent's pointers
        update_region(node->parent);

        // reset pointer to parent
        node->parent = NULL;

        free(node);

        KDtree_remove_helper(down, p);

        return true;
    }
}

bool KDtree_remove(KDtree* tree, Point* p) {
    return KDtree_remove_helper((Node*)tree->tree, p);
}

void KDnode_purge(Node* node);
void KDnode_purge_helper(BinaryTree* bt) {
    bt->removed = true;

    if (bt->left != NULL)
        KDnode_purge_helper(bt->left);

    if (bt->right != NULL)
        KDnode_purge_helper(bt->right);

    if (bt->leaf != NULL)
        KDnode_purge(bt->leaf);

    Partition_destroy(bt->partition);
    free(bt);
}

bool KDnode_purge(Node* node) {
    KDnode_purge_helper(node->children);
    free(node);
}

bool KDtree_uproot(KDtree* tree) {
    return KDnode_purge((Node*)tree->tree) &&
           LinkedList_purge((LinkedList*)tree->list);
}

#endif
