/**
Auxilliary functions to help facilitate testing
*/

#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include "./types.h"
#include "./Point.h"

static uint64_t TOTAL_ASSERTIONS = 0, PASSED_ASSERTIONS = 0;

#define assertLong(expected, actual, text) assert_long(__FILE__, __LINE__, expected, actual, text)
#define assertDouble(expected, actual, text) assert_double(__FILE__, __LINE__, expected, actual, text)
#define assertTrue(actual, text) assert_true(__FILE__, __LINE__, actual, text)
#define assertFalse(actual, text) assert_false(__FILE__, __LINE__, actual, text)
#define assertPoint(expected, actual, text) assert_point(__FILE__, __LINE__, expected, actual, text)
#define assertError(text) assert_error(__FILE__, __LINE__, text)

static inline void assert_long(char *file, int line, long long expected, long long actual, char *text) {
    printf("%s: line %4d: assert(%s == %lld)...", file, line, text, expected);
    fflush(stdout);
    if (actual != expected)
        printf("was actually %lld...", actual);
    printf("%s\n", actual == expected ? "OK" : " FAILED");

    TOTAL_ASSERTIONS++;
    PASSED_ASSERTIONS += actual == expected;
}

static inline void assert_double(char *file, int line, float64_t expected, float64_t actual, char *text) {
    printf("%s: line %4d: assert(%s == %lf)...", file, line, text, expected);
    fflush(stdout);
    if (actual != expected)
        printf("was actually %lf...", actual);
    printf("%s\n", actual == expected ? "OK" : " FAILED");

    TOTAL_ASSERTIONS++;
    PASSED_ASSERTIONS += actual == expected;
}

static inline void assert_true(char *file, int line, bool actual, char *text) {
    printf("%s: line %4d: assert(%s == true)...", file, line, text);
    fflush(stdout);
    printf("%s\n", actual == true ? "OK" : " FAILED");

    TOTAL_ASSERTIONS++;
    PASSED_ASSERTIONS += actual == true;
}

static inline void assert_false(char *file, int line, bool actual, char *text) {
    printf("%s: line %4d: assert(%s == false)...", file, line, text);
    fflush(stdout);
    printf("%s\n", actual == false ? "OK" : " FAILED");

    TOTAL_ASSERTIONS++;
    PASSED_ASSERTIONS += actual == false;
}

static inline void assert_point(char *file, int line, Point expected, Point actual, char *text) {
    printf("%s: line %4d: assert(%s == Point(%lf, %lf))...", file, line, text, expected.x, expected.y);
    fflush(stdout);
    bool equals = Point_equals(&actual, &expected);
    if (!equals)
        printf("was actually Point(%lf, %lf)...", actual.x, actual.y);
    printf("%s\n", equals ? "OK" : " FAILED");

    TOTAL_ASSERTIONS++;
    PASSED_ASSERTIONS += equals;
}

static inline void assert_error(char *file, int line, char *text) {
    printf("%s: line %4d: assert(%s)... FAILED...\n", file, line, text);

    TOTAL_ASSERTIONS++;
}

static long long passed_assertions() {
    return PASSED_ASSERTIONS;
}

static long long total_assertions() {
    return TOTAL_ASSERTIONS;
}

#endif
