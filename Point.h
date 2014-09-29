/**
Point
*/

#ifndef POINT_H
#define POINT_H

#include "./types.h"

#define abs(x) ((1 - 2 * ((x) < 0)) * (x))
#define PRECISION 1e-6

typedef struct Point_t{
    float32_t x, y;
} Point;

Point Point_create(float32_t x, float32_t y);
bool Point_equals(Point a, Point b);

#ifdef QUADTREE_TEST_H
void Point_string(Point* p, char* buffer) {
    sprintf(buffer, "Point(%lf, %lf)", p->x, p->y);
}
#endif

#endif
