/**
Point implementation
*/

#include "./Point.h"

Point Point_init(float64_t x, float64_t y) {
    return (Point){.x = x, .y = y};
}

int8_t Point_compare(Point a, Point b) {
    register bool xdiff = abs(a.x - b.x) > PRECISION;
    register bool ydiff = abs(a.y - b.y) > PRECISION;
    return xdiff * (2 * (a.x > b.x) - 1) + !xdiff * ydiff * (2 * (a.y > b.y) - 1);
}

bool Point_equals(Point *a, Point *b) {
    return abs(a->x - b->x) <= PRECISION && abs(a->y - b->y) <= PRECISION;
}

void Point_copy(Point* from, Point* to) {
    to->x = from->x;
    to->y = from->y;
}
