/**
Point implementation
*/

#include <stdio.h>

#include "../Point.h"

bool Point_equals(Point a, Point b) {
    register uint64_t i;
    for (i = 0; i < D; i++)
        if (abs(a.val[i] - b.val[i]) > PRECISION)
            return false;
    return true;
}
