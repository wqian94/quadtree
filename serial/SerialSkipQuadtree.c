/**
Serial Skip Quadtree implementation
*/

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../Quadtree.h"
#include "../Point.h"
#include "../types.h"

/*
 * in_range
 *
 * Returns true if p is within the boundaries of n, false otherwise.
 *
 * On-boundary counts as being within if on the left or bottom boundaries
 *
 * n - the square node to check at
 * p - the point to check for
 */
static inline bool in_range(Node* n, Point* p) {
    return n->center.x - n->length / 2 <= p->x &&
        n->center.x + n->length / 2 > p->x &&
        n->center.y - n->length / 2 <= p->y &&
        n->center.y + n->length / 2 > p->y;
}

/*
 * get_quadrant
 *
 * Returns the quadrant [0,4) that p is in, relative to the origin point
 *
 * origin - the point representing the origin of the bounding square
 * p - the point we're trying to find the quadrant of
 */
static inline int get_quadrant(Point* origin, Point* p) {
    return ((p->x < origin->x) != (p->y < origin->y)) + 2 * (p->y < origin->y);
}

/*
 * get_new_center
 *
 * Given the current node and a quadrant, returns the Point representing
 * the center of the square that represents that quadrant
 *
 * node - the parent node
 * quadrant - the quadrant to search for; must be in range [0,4)
 */
static inline Point get_new_center(Node* node, int quadrant) {
    return (Point){
        .x = node->center.x + 0.5 - (((quadrant + 1) % 4) / 2),  // taking advantage of integer division
        .y = node->center.y + 0.5 - ((quadrant % 4) / 2)
        };
}

Quadtree* Quadtree_create(float64_t length, Point center) {
    Quadtree* qtree = (Quadtree*)malloc(sizeof(Quadtree));
    *qtree = (Quadtree){.is_square = false, .parent = NULL,
        .up = NULL, .down = NULL, .children = {NULL, NULL, NULL, NULL},
        .length = length, .center = center};
    return qtree;
}

bool Quadtree_search(Quadtree* node, Point* p) {
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    // call starts at root of the highest-order tree
    // children[i] is quadrant i+1, relative to node
    int quadrant = get_quadrant(&node->center, p);
    if (node->children[quadrant]->is_square) {
        return Quadtree_search(node->children[quadrant], p);
    }
    else if (Point_equals(node->children[quadrant]->center, *p)) {
        return true;
    }
    else if (node->children[quadrant]->down != NULL) {  // not lowest-level tree
        return Quadtree_search(node->children[quadrant]->down, p);
    }
    return false;
}

Node* Quadtree_add_helper(Node* node, Point* p) {
    // call starts at the root of the highest-order tree
    Node* down_clone = NULL;
    if (node->down != NULL)
        down_clone = Quadtree_add_helper(node->down, p);

    // children[i] is quadrant i+1, relative to node
    int quadrant = get_quadrant(&node->center, p);
    Node* new_node = (Node*)Quadtree_create(0.5 * node-> length, *p);
    new_node->down = down_clone;
    new_node->parent = node;

    if (node->children[quadrant] == NULL) {
        node->children[quadrant] = new_node;
        return new_node;
    }
    else if (node->children[quadrant]->is_square) {
        return Quadtree_add_helper(node->children[quadrant], p);
    }
    else {  // node.children[quadrant] is preexisting point
        // square is the smallest square containing both node.children[quadrant] and newNode
        Point square_center = get_new_center(node, quadrant);
        Node* square = Quadtree_create(new_node->length, square_center);
        square->is_square = true;

        Node* sibling = node->children[quadrant];
        int sibling_quadrant;

        node->children[quadrant] = square;
        square->parent = node;
        new_node->parent = square;
        sibling->parent = square;

        while (1) {
            new_node->length *= 0.5;
            sibling->length *= 0.5;
            quadrant = get_quadrant(&square_center, &new_node->center);
            sibling_quadrant = get_quadrant(&square_center, &sibling->center);

            // exit the loop once we find a square that puts new_node and sibling in separate quadrants
            if (quadrant != sibling_quadrant)
                break;

            // if nodes still collide in the same quadrant...
            square_center = get_new_center(square, quadrant);
            square->length = new_node->length;
        }

        // once we've found the perfect square...
        square->children[quadrant] = new_node;
        square->children[sibling_quadrant] = sibling;

        return new_node;
    }
    return NULL;
}

bool Quadtree_add(Quadtree* node, Point* p) {
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    while (rand() % 100 < 50) {  // branch to highest level in tree, creating levels as needed
        if (node->up == NULL) {  // create new level
            Node* up = Quadtree_create(node->length, node->center);
            up->down = node;
            node->up = up;
        }
        node = node->up;
    }
    Quadtree_add_helper(node, p);
    return true;
}

bool Quadtree_remove(Quadtree* node, Point* p) {
    if (!in_range(node, p))  // check to make sure p is within the boundaries
        return false;
    if (node->is_square) {
        int quadrant = get_quadrant(&node->center, p);
        return Quadtree_remove(node->children[quadrant], p);
    }
    else if (Point_equals(node->center, *p)) {
        int quadrant = get_quadrant(&node->parent->center, p);
        int i;

        if (node->parent != NULL) {  // parent shouldn't be null, but...
            // determine how many siblings are null
            int count_null_siblings = -1;  // exclude current node from count
            for (i = 0; i < 4; i++)
                count_null_siblings += (node->parent->children[i] == NULL);

            // no other sibling means something went wrong, unless the parent is a root node
            if (count_null_siblings == 0) {
                if (node->parent != NULL && node->parent->parent == NULL) {
                    // if we're here, then this must be the top-most level
                    // because being here means this level has only one node: the
                    // node we're about to remove, so any higher levels must be
                    // empty already because we're deleting from highest levels first
                    node->parent->parent->down->up = NULL;
                    free(node->parent->parent);  // remove level
                }
                else {
                    // hopefully we never get here
                    // TODO: throw a legitimate error when this happens
                    assert(node->parent != NULL && node->parent->parent == NULL);
                    return false;
                }
            }
            // only 1 other sibling, replace parent square with sibling
            else if (count_null_siblings == 1) {
                Node* sibling = NULL;
                for (i = 0; i < 4; i++)
                    if (i == quadrant)  // skip the current node
                        continue;
                    else if (node->parent->children[i] != NULL) {
                        sibling = node->parent->children[i];
                        break;
                    }

                // replacement
                if (node->parent->parent == NULL) {  // is null if parent is at root-level node
                    // clearing current node since we don't want to delete root node
                    node->parent->children[quadrant] = NULL;
                }
                else {  // otherwise, normal execution
                    int parent_quadrant = get_quadrant(&node->parent->parent->center, &node->parent->center);
                    node->parent->parent->children[parent_quadrant] = sibling;
                    if (node->parent->down != NULL)  // unlink levels
                        node->parent->down->up = NULL;
                    free(node->parent);  // free the parent square
                }
            }
            else {  // if 2 or 3 siblings, then we simply remove the child
                node->parent->children[quadrant] = NULL;
            }
        }

        // free and recurse
        Node* down = node->down;  // free the current node first, so cache the down node address
        free(node);
        if (down != NULL)  // drop a level
            return Quadtree_remove(node->down, p);
        return true;  // if we're at a lowest-level leaf
    }
    else {  // we're at a leaf node with no results...
        if (node->down != NULL)  // drop a level
            return Quadtree_remove(node->down, p);
        return false;  // if we're at a lowest-level leaf
    }
}

bool Quadtree_uproot(Quadtree* root) {
    // uprooting from the topmost tree down
    if (root->up != NULL)
        if (!Quadtree_uproot(root->up))  // if something returned an error...
            return false;

    int i;
    for(i = 0; i < 4; i++)
        if (root->children[i] != NULL)
            Quadtree_uproot(root->children[i]);

    // unset child from parent, in case parent isn't being uprooted
    if (root->parent != NULL) {
        int quadrant = get_quadrant(&root->parent->center, &root->center);
        root->parent->children[quadrant] = NULL;
    }

    // unset top from bottom, so that the tree is uprooted layer by layer
    if (root->down != NULL)
        root->down->up = NULL;

    // free the current node
    free(root);

    return true;
}
