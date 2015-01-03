/**
Serial Skip KDtree implementation
*/

#include <assert.h>
#include <stdlib.h>

#include "../types.h"
#include "../KDtree.h"
#include "../Point.h"

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

Node* Node_create(float64_t length, Point center) {
    Node* node = (Node*)malloc(sizeof(Node));
    *node = (Node){.is_container = false, .parent = NULL,
        .up = NULL, .down = NULL, //.children is not initialized here
        .length = length, .center = center
#ifdef KDTREE_TEST
, .id = (KDTREE_NODE_COUNT++)
#endif
        };
    register uint64_t i;
    for (i = 0; i < N_PARTITIONS; i++)
        node->children[i] = NULL;
    return node;
}

KDtree* KDtree_create(float64_t length, Point center) {
    KDtree* qtree = Node_create(length, center);
    qtree->is_container = true;
    return qtree;
}

bool KDtree_search(KDtree* node, Point* p) {
    if (node == NULL)  // shouldn't happen, but just in case...
        return false;

    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    // call starts at root of the highest-order tree
    // children[i] is partition i+1, relative to node
    int partition = get_partition(&node->center, p);

    // no such child on this level
    if (node->children[partition] == NULL) {
        // in case we didn't catch this earlier
        if (!node->is_container && Point_equals(node->center, *p))
            return true;
        // move down a level and continue if possible
        else if (node->down != NULL)
            return KDtree_search(node->down, p);
        // otherwise, conclusively not found
        return false;
    }

    // otherwise, recurse on partitions
    return KDtree_search(node->children[partition], p);
}

Node* KDtree_add_helper(Node* node, Point* p) {
    // find where to insert node
    Node* parent = node;
    int partition = get_partition(&parent->center, p);
    while (parent->children[partition] != NULL && parent->children[partition]->is_container) {
        parent = parent->children[partition];
        partition = get_partition(&parent->center, p);
    }

    // recursively create down-nodes
    Node* down_clone = NULL;
    if (parent->down != NULL)
        down_clone = KDtree_add_helper(parent->down, p);

    // create the new node
    Node* new_node = (Node*)Node_create(0.5 * parent->length, *p);

    // update new node's parent and down pointers
    new_node->parent = parent;
    new_node->down = down_clone;

    //update down node's up pointer if down node exists
    if (down_clone != NULL)
        down_clone->up = new_node;

    // if target slot is empty, add it in!
    if (parent->children[partition] == NULL) {
        // find the corresponding container in the parent, which we know has to exist
        Node* parent_down = down_clone;
        while (parent_down != NULL && !Point_equals(parent_down->center, parent->center))
            parent_down = parent_down->parent;

        // update pointers of parent nodes
        parent->down = parent_down;
        if (parent_down != NULL)
            parent_down->up = parent;

        parent->children[partition] = new_node;
    }
    // if target slot isn't empty, we're going to have to create a container for it
    else {
        // create the new container
        Node* container = KDtree_create(new_node->length, get_new_center(parent, partition));
        int container_partition = partition;

        // update the length of the new node
        new_node->length *= 0.5;

        // grab the conflicting node
        Node* sibling = parent->children[partition];
        sibling->length = new_node->length;
        int sibling_partition;

        // find the next container that buckets new_node and sibling in different partitions
        while ((sibling_partition = get_partition(&container->center, &sibling->center)) ==
                (partition = get_partition(&container->center, &new_node->center))) {
            // update center and length of container
            container->center = get_new_center(container, partition);
            container->length = new_node->length;

            // update lengths of new_node and sibling
            new_node->length *= 0.5;
            sibling->length = new_node->length;
        }

        // update container's children
        container->children[partition] = new_node;
        container->children[sibling_partition] = sibling;

        // find container's down by checking up the tree from the bottom child
        Node* container_down = parent->children[container_partition]->down;
        while (container_down != NULL && (!Point_equals(container_down->center, container->center) || !container_down->is_container))
            container_down = container_down->parent;

        container->down = container_down;
        if (container_down != NULL)
            container_down->up = container;

        // update parent's children
        parent->children[container_partition] = container;

        // update parent nodes
        container->parent = parent;
        new_node->parent = container;
        sibling->parent = container;
    }
    // two possibilities: target slot is empty (great), target slot is not empty (darn)
    // target slot is empty: just insert and set pointers
    // target slot is not empty: create new compressed container and swap
    return new_node;
}

bool KDtree_add(KDtree* node, Point* p) {
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    while (rand() % 100 < 50) {  // branch to highest level in tree, creating levels as needed
        if (node->up == NULL) {  // create new level
            KDtree* up = KDtree_create(node->length, node->center);
            up->is_container = true;
            up->down = node;
            node->up = up;
        }
        node = node->up;
    }
    KDtree_add_helper(node, p);
    return true;
}

// updates the pointers relating to the container if needed
/*
 * update_container
 *
 * Updates the provided node to ensure valid representation, but only if node is a container.
 *
 * In particular:
 * - If node has only 1 child, we will replace the container with that one child.
 * - If node has no children, we will delete the container, detroying a level if that container
 *   was a root node, unless we're at the lowest-level container.
 * - Otherwise, do nothing.
 *
 * node - the container that we're attempting to update
 */
static inline void update_container(KDtree* node) {
    if (node == NULL || !node->is_container)
        return;

    int i, count_children = 0;
    for (i = 0; i < N_PARTITIONS; i++)
        count_children += node->children[i] != NULL;

    // delete container if no children left: should only really happen at the root
    if (count_children == 0) {
        // if we're at the root of the lowest level, don't do anything
        if (node->parent != NULL || node->down != NULL) {
            // unset pointer from parent
            if (node->parent != NULL)
                node->parent->children[get_partition(&node->parent->center, &node->center)] = NULL;

            // reset pointer from down
            if (node->down != NULL)
                node->down->up = node->up;

            // reset pointer from up
            if (node->up != NULL)
                node->up->down = node->down;

            free(node);
        }
    }
    // if only 1 child left, replace container with that child
    else if (count_children == 1) {
        Node* only_child = NULL;
        for (i = 0; i < N_PARTITIONS; i++)
            if ((only_child = node->children[i]) != NULL)
                break;

        // only_child can't be NULL because we already checked

        // if not at root, do the replacement; if at root, do nothing
        if (node->parent != NULL) {
            node->parent->children[get_partition(&node->parent->center, &node->center)] = only_child;
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

bool KDtree_remove(KDtree* node, Point* p) {
    if (node == NULL)  // one base case
        return false;

    if (!in_range(node, p))  // boundary check
        return false;

    if (node->is_container) {
        int partition = get_partition(&node->center, p);
        bool removed = KDtree_remove(node->children[partition], p);

        // if we made a change, check the rep to see if this container is needed anymore
        if (removed) {
            update_container(node);
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
        int partition = get_partition(&node->parent->center, p);

        // reset pointers
        node->parent->children[partition] = NULL;

        if (node->down != NULL)
            node->down->up = node->up;

        if (node->up != NULL)
            node->up->down = node->down;

        // reset parent's pointers
        update_container(node->parent);

        // reset pointer to parent
        node->parent = NULL;

        free(node);

        KDtree_remove(down, p);

        return true;
    }
}

bool KDtree_uproot(KDtree* root) {
    // uprooting from the topmost tree down
    if (root->up != NULL)
        if (!KDtree_uproot(root->up))  // if something returned an error...
            return false;

    int i;
    for(i = 0; i < N_PARTITIONS; i++)
        if (root->children[i] != NULL)
            KDtree_uproot(root->children[i]);

    // unset child from parent, in case parent isn't being uprooted
    if (root->parent != NULL) {
        int partition = get_partition(&root->parent->center, &root->center);
        root->parent->children[partition] = NULL;
    }

    // unset top from bottom, so that the tree is uprooted layer by layer
    if (root->down != NULL)
        root->down->up = NULL;

    // free the current node
    free(root);

    return true;
}
