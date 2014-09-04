/**
Testing suite for Quadtrees
*/

#ifndef QUADTREE_TEST_H
#define QUADTREE_TEST_H

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <mcheck.h>

#include "./Quadtree.h"
#include "./assertions.h"

void start_test(void (*func)(), const char* suite_name) {
    printf("\n===Testing %s===\n", suite_name);
    func();
    printf("\n");
}

void test_sizes() {
    printf("sizeof(Quadtree)  = %lu\n", sizeof(Quadtree));
    printf("sizeof(bool)      = %lu\n", sizeof(bool));
    printf("sizeof(float64_t) = %lu\n", sizeof(float64_t));
    printf("sizeof(Node*)     = %lu\n", sizeof(Node*));
    printf("sizeof(Point)     = %lu\n", sizeof(Point));
    printf("\n===Testing Quadtree size===\n");
    assertLong(80, sizeof(Quadtree), "sizeof(Quadtree)");
}

void test_in_range() {
}

void test_quadtree_create() {
    printf("\n---Quadtree_create One Test---\n");
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);
    assertDouble(s1, q1->length, "q1->length");
    assertPoint(p1, q1->center, "q1->center");
    Quadtree_uproot(q1);
}

void test_quadtree_add() {
    printf("\n---Quadtree_add One Node Test---\n");
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    // use srand to seed for what we want, so that testing is deterministic
    srand(2014);  // next 32 rand()'s: 63,84,99,80,8,69,57,84,79,17,56,99,54,34,10,3,16,47,10,1,51,12,12,14,75,98,22,45,65,79,14,80
    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(7, 7);
    Point p4 = Point_create(3, 3);

    Quadtree_uproot(q1);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    mtrace();
    printf("[Beginning tests]\n");
    
    start_test(test_sizes, "Struct sizes");
    start_test(test_in_range, "in_range");
    start_test(test_quadtree_create, "Quadtree_create");
    start_test(test_quadtree_add, "Quadtree_add");
    return 0;
}

#endif
