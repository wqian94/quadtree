/**
Point
*/

#ifndef POINT_H
#define POINT_H

#include <stdio.h>

#include "./types.h"

#define abs(x) ((1 - 2 * ((x) < 0)) * (x))
#define PRECISION 1e-6

#ifdef DIMENSIONS
#define D DIMENSIONS
#else
#define D 3
#endif

/**
 * struct Point_t
 *
 * Represents D-dimensional data. Contains D members.
 */
typedef struct Point_t{
    float64_t data[D];
} Point;

/**
 * Point_init
 *
 * Returns a Point that represents (x, y, ...).
 *
 * x - the first coordinate of the point
 * y - the second coordinate of the point
 * ... - other coordinates of the point
 *
 * Returns a Point representing (x, y, ...).
 */
#if D == 2
Point Point_init(float64_t x, float64_t y);
#elif D == 3
Point Point_init(float64_t x, float64_t y, float64_t z);
#endif

/**
 * Point_compare
 *
 * Returns a value indicating whether a is <, =, or > b.
 *
 * A positive value indicates that b < a, 0 indicates equivalence, and a negative
 * value indicates that b > a. The x coordinates are compared first, and if they
 * are within the precision range, than the y coordinates are compared.
 *
 * a - the point to compare against
 * b - the point to compare
 *
 * Returns a value <0 if a < b, =0 if a == b, and >0 if a > b.
 */
int8_t Point_compare(Point a, Point b);

/**
 * Point_equals
 *
 * Returns true if the two points are within precision error of each other in both
 * coordinates.
 *
 * a - the first point to compare
 * b - the second point to compare
 *
 * Returns true if the two points are equal, up to precision error, and false otherwise.
 */
safe bool Point_equals(Point *a, Point *b);

/**
 * Point_copy
 *
 * Copies the coordinate data from the first point to the second.
 *
 * from - the point to copy from
 * to - the point to copy to
 */
safe void Point_copy(Point *from, Point *to);

static void Point_string(Point *p, char *buffer) {
    sprintf(buffer, "Point(%lf", p->data[0]);
    register uint64_t i;
    for (i = 1; i < D; i++)
        sprintf(buffer, "%s, %lf", buffer, p->data[i]);
    sprintf(buffer, "%s)", buffer);
}

#endif
