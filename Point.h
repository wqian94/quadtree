/**
Point
*/

#ifndef POINT_H
#define POINT_H

#include <stdbool.h>

#include "./types.h"

#define abs(x) ((1 - 2 * (x < 0)) * x)
#define PRECISION 1e-6

typedef struct Point_t{
    float32_t x, y;
} Point;

Point Point_create(float32_t x, float32_t y);
bool Point_equals(Point a, Point b);

#endif
