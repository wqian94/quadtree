/**
Point implementation
*/

#include "../Point.h"

Point Point_create(float32_t x, float32_t y) {
    return (Point){.x = x, .y = y};
}

bool Point_equals(Point a, Point b) {
    return abs(a.x - b.x) <= PRECISION && abs(a.y - b.y) <= PRECISION;
}

void Point_copy(Point* from, Point* to) {
    to->x = from->x;
    to->y = from->y;
}
