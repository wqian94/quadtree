/**
Point implementation
*/

#include <stdio.h>
#include <string.h>

#include "../Point.h"

/*
 * struct point_list_t
 *
 * This is the internal representation of the Point data as a linked list.
 */
typedef struct point_list_t {
    uint64_t dimension;
    float64_t* data;
    struct point_list_t *prev, *next;
} Point_list;

static Point_list* point_list = NULL;

Point Point_create(uint64_t dimension, float64_t* data) {
    // create node for keeping track of points
    Point_list* pl = (Point_list*)malloc(sizeof(Point_list));
    pl->dimension = dimension;
    pl->prev = pl;
    pl->next = pl;

    // allocate and copy point data
    pl->data = (float64_t*)malloc(sizeof(float64_t) * dimension);
    memcpy((void*)pl->data, (void*)data, sizeof(float64_t) * dimension);

    // point_list == NULL means no elements in list yet
    if (point_list == NULL)
        point_list = pl;
    else {
        // otherwise, rearrange pointers
        pl->prev = point_list->prev;
        pl->next = point_list;
        pl->prev->next = pl;
        pl->next->prev = pl;
    }

    // Point is just a container now
    return (Point){.target = (void*)pl, .val = pl->data};
}

void Point_destroy(Point* p) {
    Point_list* pl = (Point_list*)p->target;
    if (pl->prev == pl)
        point_list = NULL;
    else {
        pl->prev->next = pl->next;
        pl->next->prev = pl->prev;
    }

    free(pl->data);
    free(pl);
}

void Point_purge() {
    if (point_list == NULL)
        return;

    point_list->prev->next = NULL;
    while (point_list != NULL) {
        Point_list* pl = point_list;
        point_list = point_list->next;

        free(pl->data);
        free(pl);
    }
}

bool Point_equals(Point a, Point b) {
    register uint64_t i, D[2] = {((Point_list*)a.target)->dimension, ((Point_list*)b.target)->dimension};

    // dimension checking
    if (D[0] != D[1])
        return false;

    // dimensions match here
    for (i = 0; i < D[0]; i++)
        if (abs(a.val[i] - b.val[i]) > PRECISION)
            return false;
    return true;
}

#ifdef KDTREE_TEST
void Point_string(Point* p, char* buffer) {
    sprintf(buffer, "Point(");
    register uint64_t i, D = ((Point_list*)p->target)->dimension;
    for (i = 0; i < D; i++)
        sprintf(buffer, "%s%lf%s", buffer, p->val[i], i == D - 1 ? ")" : ", ");
}
#endif
