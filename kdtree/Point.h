/**
Point
*/

#ifndef POINT_H
#define POINT_H

#ifdef KDTREE_TEST
#include <stdio.h>
#endif

#include <stdarg.h>
#include <stdlib.h>

#include "./types.h"

#define abs(x) ((1 - 2 * ((x) < 0)) * (x))
#define PRECISION 1e-6

/*
 * Point
 *
 * The data container for multidimensional data, which is pointed to by val. The pointer
 * called target points to the internal representation used by the library (the node in
 * the linked list).
 */
typedef struct {
    void* target;
    float64_t* val;
} Point;

/*
 * Point_create
 *
 * Allocates memory for and creates a Point container. Data is actually stored in an
 * internal linked list in the library, and the returned object is just a struct of
 * references to the node and the data.
 *
 * Used according to the following syntax:
 *     Point_create(dimension, (float64_t[]){coordinates})
 *
 * For example, to create a point representing (1, 1, 1):
 *     Point_create(3, (float64_t[]){1, 1, 1})
 *
 * dimension - the number of elements to allocate in the data array
 * data - the data to be copied
 *
 * Returns a copy of the struct containing the two references.
 */
Point Point_create(uint64_t dimension, float64_t* data);

/*
 * Point_destroy
 *
 * Frees the memory used to store the data relating to this Point, as well as the node
 * that contained the data in the internal representation.
 *
 * p - the reference to the Point that should be destroyed
 */
void Point_destroy(Point* p);

/*
 * Point_purge
 *
 * Frees up all memory used to represent Point data. Should be called at the end of
 * program execution to free up all remaining data.
 *
 * Note: ALL POINT DATA WILL BE FREED. This means that any Point previously allocated
 * will no longer be allocated.
 */
void Point_purge();

/*
 * Point_equals
 *
 * Returns whether two points have the same dimension and coordinate values, up to a
 * fixed precision defined by the macro PRECISION.
 *
 * a - the first Point
 * b - the secont Point
 *
 * Returns whether these two Point objects refer to the same data, up to a precision
 * factor.
 */
bool Point_equals(Point a, Point b);

#ifdef KDTREE_TEST
/*
 * Point_string
 *
 * Writes the coordinates of the Point into a string buffer.
 *
 * p - the Point of interest
 * buffer - the string buffer
 */
void Point_string(Point* p, char* buffer);
#endif

#endif
