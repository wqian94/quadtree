/**
Point implementation
*/

#include "./Point.h"

#if D == 2
Point Point_init(float64_t x, float64_t y) {
#elif D == 3
Point Point_init(float64_t x, float64_t y, float64_t z) {
#endif
    return (Point){.data = {
        x
        ,y
#if D == 3
        ,z
#endif
        }};
}

int8_t Point_compare(Point a, Point b) {
    register uint64_t i;
    for (i = 0; i < D; i++)
        if (abs(a.data[i] - b.data[i]) > PRECISION)
            return (2 * (a.data[i] > b.data[i]) - 1);
}

bool Point_equals(Point *a, Point *b) {
    register uint64_t i;
    for (i = 0; i < D; i++)
        if (abs(a->data[i] - b->data[i]) > PRECISION)
            return false;
    return true;
}

void Point_copy(Point* from, Point* to) {
    register uint64_t i;
    for (i = 0; i < D; i++)
        to->data[i] = from->data[i];
}
