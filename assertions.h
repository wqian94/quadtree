/**
Auxilliary functions to help facilitate testing
*/

#ifndef ASSERTIONS_H
#define ASSERTIONS_H

static inline void assertLong(long long expected, long long actual, char* text) {
    printf("assert(%s == %lld)...", text, expected);
    fflush(stdout);
    printf("%s\n", actual == expected ? "OK" : "FAILED");
}

static inline void assertDouble(double expected, double actual, char* text) {
    printf("assert(%s == %lf)...", text, expected);
    fflush(stdout);
    printf("%s\n", actual == expected ? "OK" : "FAILED");
}

static inline void assertPoint(Point expected, Point actual, char* text) {
    printf("assert(%s == Point(%lf, %lf))...", text, expected.x, expected.y);
    fflush(stdout);
    printf("%s\n", Point_equals(actual, expected) ? "OK" : "FAILED");
}

#endif
