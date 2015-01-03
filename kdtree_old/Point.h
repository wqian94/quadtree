/**
Point
*/

#ifndef POINT_H
#define POINT_H

#ifdef KDTREE_TEST_H
#include <stdio.h>
#endif

#include <stdarg.h>
#include <stdlib.h>

#include "./constants.h"
#include "./types.h"

#define abs(x) ((1 - 2 * ((x) < 0)) * (x))
#define PRECISION 1e-6

typedef struct Point_t{
    float64_t val[D];
} Point;

//#define Point_create(x) (Point){x}
bool Point_equals(Point a, Point b);

#ifdef KDTREE_TEST_H
void Point_string(Point* p, char* buffer) {
    sprintf(buffer, "Point(");
    register uint64_t i;
    for (i = 0; i < D; i++)
        sprintf(buffer, "%s%lf%s", buffer, p->val[i], i == D - 1 ? ")" : ", ");
}
#endif

#endif
