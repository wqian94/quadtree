/**
Serial Skip Quadtree implementation
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
uint64_t QUADTREE_NODE_COUNT = 0;
#else
extern uint32_t Marsaglia_rand();
#define rand() Marsaglia_rand()
extern void Marsaglia_srand(uint32_t);
#define srand(x) Marsaglia_srand(x)
#endif

Node* Node_create(float64_t length, Point center) {
    Node* node = (Node*)malloc(sizeof(Node));
    *node = (Node){.is_square = false, .parent = NULL,
        .up = NULL, .down = NULL, .children = {NULL, NULL, NULL, NULL},
        .length = length, .center = (Point*)malloc(sizeof(Point))
#ifdef QUADTREE_TEST
, .id = (QUADTREE_NODE_COUNT++)
#endif
        };
    Point_copy(&center, node->center);
    return node;
}

Quadtree* Quadtree_create(float64_t length, Point center) {
    Quadtree* qtree = Node_create(length, center);
    qtree->is_square = true;
    return qtree;
}

bool Quadtree_search(Quadtree* node, Point* p) {
    if (node == NULL)  // shouldn't happen, but just in case...
        return false;

    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    // call starts at root of the highest-order tree
    // children[i] is quadrant i+1, relative to node
    int quadrant = get_quadrant(node->center, p);

    // no such child on this level
    if (node->children[quadrant] == NULL) {
        // in case we didn't catch this earlier
        if (!node->is_square && Point_equals(*node->center, *p))
            return true;
        // move down a level and continue if possible
        else if (node->down != NULL)
            return Quadtree_search(node->down, p);
        // otherwise, conclusively not found
        return false;
    }

    // otherwise, recurse on quadrants
    return Quadtree_search(node->children[quadrant], p);
}

Node* Quadtree_add_helper(Node* node, Point* p, int depth) {
    // find where to insert node
    Node* parent = node;
    int quadrant = get_quadrant(parent->center, p);
    while (parent->children[quadrant] != NULL && parent->children[quadrant]->is_square
            && in_range(parent->children[quadrant], p)) {
        parent = parent->children[quadrant];
        quadrant = get_quadrant(parent->center, p);
    }

    // ignore out-of-bounds
    if (!in_range(parent, p))
        return NULL;

    // ignore repeats
    if (parent->children[quadrant] != NULL && Point_equals(*parent->children[quadrant]->center, *p))
        return NULL;

    // recursively create down-nodes
    Node* down_clone = NULL;
    if (parent->down != NULL)
        if ((down_clone = Quadtree_add_helper(parent->down, p, depth + 1)) == NULL)
            return NULL;

    // create the new node
    Node* new_node = (Node*)Node_create(0.5 * parent->length, *p);

    // update new node's parent and down pointers
    new_node->parent = parent;
    new_node->down = down_clone;

    //update down node's up pointer if down node exists
    if (down_clone != NULL)
        down_clone->up = new_node;

    // if target slot is empty, add it in!
    if (parent->children[quadrant] == NULL) {
        // find the corresponding square in the parent, which we know has to exist
        Node* parent_down = down_clone;
        while (parent_down != NULL && !Point_equals(*parent_down->center, *parent->center))
            parent_down = parent_down->parent;

        // update pointers of parent nodes
        parent->down = parent_down;
        if (parent_down != NULL)
            parent_down->up = parent;

        parent->children[quadrant] = new_node;
    }
    // if target slot isn't empty, we're going to have to create a square for it
    else {
        // create the new square
        int square_quadrant = quadrant;
        Node* square = Quadtree_create(new_node->length, get_new_center(parent, square_quadrant));

        // update the length of the new node
        new_node->length *= 0.5;

        // grab the conflicting node
        Node* sibling = parent->children[quadrant];
        sibling->length = new_node->length;
        int sibling_quadrant;

        // find the next square that buckets new_node and sibling in different quadrants
        while ((sibling_quadrant = get_quadrant(square->center, sibling->center)) ==
                (quadrant = get_quadrant(square->center, new_node->center))) {
            // update center and length of square
            Point new_center = get_new_center(square, quadrant);
            Point_copy(&new_center, square->center);
            square->length = new_node->length;

            // update lengths of new_node and sibling
            new_node->length *= 0.5;
            sibling->length = new_node->length;
        }

        // update square's children
        square->children[quadrant] = new_node;
        square->children[sibling_quadrant] = sibling;

        // find square's down by checking up the tree from the bottom child
        Node* square_down = parent->children[square_quadrant]->down;
        while (square_down != NULL && (!Point_equals(*square_down->center, *square->center) || !square_down->is_square))
            square_down = square_down->parent;

        square->down = square_down;
        if (square_down != NULL)
            square_down->up = square;

        // update parent's children
        parent->children[square_quadrant] = square;

        // update parent nodes
        square->parent = parent;
        new_node->parent = square;
        sibling->parent = square;
    }
    // two possibilities: target slot is empty (great), target slot is not empty (darn)
    // target slot is empty: just insert and set pointers
    // target slot is not empty: create new compressed square and swap
    return new_node;
}

bool Quadtree_add(Quadtree* node, Point* p) {
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    /*while (rand() % 100 < 50) {  // branch to highest level in tree, creating levels as needed
        if (node->up == NULL) {  // create new level
            Quadtree* up = Quadtree_create(node->length, *node->center);
            up->is_square = true;
            up->down = node;
            node->up = up;
        }
        node = node->up;
    }*/
    return Quadtree_add_helper(node, p, 0) != NULL;
}

// updates the pointers relating to the square if needed
/*
 * update_square
 *
 * Updates the provided node to ensure valid representation, but only if node is a square.
 *
 * In particular:
 * - If node has only 1 child, we will replace the square with that one child.
 * - If node has no children, we will delete the square, detroying a level if that square
 *   was a root node, unless we're at the lowest-level square.
 * - Otherwise, do nothing.
 *
 * node - the square that we're attempting to update
 */
static inline void update_square(Quadtree* node) {
    if (node == NULL || !node->is_square)
        return;

    int i, count_children = 0;
    for (i = 0; i < 4; i++)
        count_children += node->children[i] != NULL;

    // delete square if no children left: should only really happen at the root
    if (count_children == 0) {
        // if we're at the root of the lowest level, don't do anything
        if (node->parent != NULL || node->down != NULL) {
            // unset pointer from parent
            if (node->parent != NULL)
                node->parent->children[get_quadrant(node->parent->center, node->center)] = NULL;

            // reset pointer from down
            if (node->down != NULL)
                node->down->up = node->up;

            // reset pointer from up
            if (node->up != NULL)
                node->up->down = node->down;

            free(node->center);
            free(node);
        }
    }
    // if only 1 child left, replace square with that child
    else if (count_children == 1) {
        Node* only_child = NULL;
        for (i = 0; i < 4; i++)
            if ((only_child = node->children[i]) != NULL)
                break;

        // only_child can't be NULL because we already checked

        // if not at root, do the replacement; if at root, do nothing
        if (node->parent != NULL) {
            node->parent->children[get_quadrant(node->parent->center, node->center)] = only_child;
            only_child->parent = node->parent;

            // reset pointer from down
            if (node->down != NULL)
                node->down->up = node->up;

            // reset pointer from up
            if (node->up != NULL)
                node->up->down = node->down;

            free(node->center);
            free(node);
        }
    }
}

/*
 * Quadtree_remove_node
 *
 * Removes all copies of node from the tree if it matches p.
 */
bool Quadtree_remove_node(Quadtree* node, Point* p) {
    if (!node->is_square && Point_equals(*node->center, *p)) {
        do {
            Node* deleted_node = node;
            Node* parent = node->parent;
            node = node->down;

            // leaf nodes are guaranteed parent nodes
            int quadrant = get_quadrant(deleted_node->parent->center, p);

            // reset pointers
            deleted_node->parent->children[quadrant] = NULL;

            if (deleted_node->down != NULL)
                deleted_node->down->up = deleted_node->up;

            if (deleted_node->up != NULL)
                deleted_node->up->down = deleted_node->down;

            // reset pointer to parent
            deleted_node->parent = NULL;

            free(deleted_node->center);
            free(deleted_node);

            // reset parent's pointers
            update_square(parent);
        } while(node != NULL);

        return true;
    }

    char buf1[1000], buf2[1000];
    Point_string(node->center, buf1);
    Point_string(p, buf2);
    printf("%llu\n", (unsigned long long)node->id);
    puts(buf1);
    puts(buf2);

    //print_Quadtree(node);

    /*while (node->parent != NULL)
        node = node->parent;

    print_Quadtree(node);

    Quadtree_uproot(node);
    exit(0);*/

    return false;
}

/*
 * Quadtree_remove_helper
 *
 * Invariant: node is always a square.
 */
bool Quadtree_remove_helper(Quadtree* node, Point* p) {
/*    if (node == NULL)
        return false;

    if (!in_range(node, p))
        return false;

    // recurses from topmost root node
    int quadrant = get_quadrant(node->center, p);
    Node* child = node->children[quadrant];

    if (child
/*    if (node == NULL)  // shouldn't happen, but just in case...
        return false;

    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;

    // call starts at root of the highest-order tree
    // children[i] is quadrant i+1, relative to node
    int quadrant = get_quadrant(node->center, p);

    if (node->children[quadrant] != NULL) {
        if (node->children[quadrant]->is_square)
            return Quadtree_remove_helper(node->children[quadrant], p);

        if (Quadtree_remove_node(node->children[quadrant], p))
            return true;
    }

    if (node->down != NULL)
        return Quadtree_remove_helper(node->down, p);

    return false;*/
    if (node == NULL)  // shouldn't happen, but just in case...
        return false;

    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    // call starts at root of the highest-order tree
    // children[i] is quadrant i+1, relative to node
    int quadrant = get_quadrant(node->center, p);

    // no such child on this level
    if (node->children[quadrant] == NULL) {
        // in case we didn't catch this earlier
        if (Quadtree_remove_node(node, p))
            return true;
        // move down a level and continue if possible
        else if (node->down != NULL)
            return Quadtree_remove_helper(node->down, p);
        // otherwise, conclusively not found
        return false;
    }

    // otherwise, recurse on quadrants
    return Quadtree_remove_helper(node->children[quadrant], p);
}

bool Quadtree_remove(Quadtree* node, Point* p) {
    while (node->up != NULL)
        node = node->up;

    return Quadtree_remove_helper(node, p);
}

uint64_t Quadtree_uproot_helper(Quadtree* root) {
    uint64_t count = !root->is_square;

    if (root->is_square) {
        int i;
        for(i = 0; i < 4; i++)
            if (root->children[i] != NULL)
                count += Quadtree_uproot_helper(root->children[i]);
    }

    // unset child from parent, in case parent isn't being uprooted
    if (root->parent != NULL) {
        int quadrant = get_quadrant(root->parent->center, root->center);
        root->parent->children[quadrant] = NULL;
    }

    // unset top from bottom, so that the tree is uprooted layer by layer
    if (root->down != NULL)
        root->down->up = NULL;

    // free the current node
    free(root->center);
    free(root);

    return count;
}

uint64_t Quadtree_uproot(Quadtree* root) {
    uint64_t count = 0;

    // uprooting from the topmost tree down
    if (root->up != NULL)
        if (!(count = Quadtree_uproot(root->up)))  // if something returned an error...
            return 0;

    return count + Quadtree_uproot_helper(root);
}
