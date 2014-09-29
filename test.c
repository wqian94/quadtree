/**
Testing suite for Quadtrees
*/

#include "./test.h"

extern bool in_range(Node*, Point*);
extern void Point_string(Point*, char*);

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
    Quadtree* node = Quadtree_create(2.0, Point_create(0, 0));
    Point p1 = Point_create(-1, -1);
    Point p2 = Point_create(2, 2);
    char buffer[100], point_buffer[100], node_buffer[256];

    Node_string(node, node_buffer);

    Point_string(&p1, point_buffer);
    sprintf(buffer, "in_range(node, %s)", point_buffer);
    assertTrue(in_range(node, &p1), buffer);

    Point_string(&p2, point_buffer);
    sprintf(buffer, "in_range(node, %s)", point_buffer);
    assertFalse(in_range(node, &p2), buffer);

    Quadtree_uproot(node);
}

void test_get_quadrant() {
    Point origin = Point_create(0, 0);
    Point points[8] = {
        Point_create(1, 0), Point_create(1, 1), Point_create(0, 1), Point_create(-1, 1),
        Point_create(-1, 0), Point_create(-1, -1), Point_create(0, -1), Point_create(1, -1)
    };
    int expected_quadrants[8] = {0, 0, 0, 1, 1, 2, 3, 3};
    int i;
    char buffer[100];
    for (i = 0; i < 8; i++) {
        sprintf(buffer, "get_quadrant((%f, %f), (%f, %f))", origin.x, origin.y, points[i].x, points[i].y);
        assertLong(expected_quadrants[i], get_quadrant(&origin, &points[i]), buffer);
    }
}

void test_get_new_center() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);
    
    Point new_center = get_new_center(q1, 0);
    Point_string(&new_center, buffer);
    assertPoint(Point_create(4, 4), new_center, buffer);
    Quadtree_uproot(q1);
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
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(7, 7);
    Point p4 = Point_create(3, 3);

    printf("\n---Quadtree_add One Node Test---\n");
    Quadtree_add(q1, &p2);
    Node* q2 = q1->children[0];

    int count_q1_levels = 0;
    Node* node;
    for (node = q1; node->up != NULL; node = node->up)
        count_q1_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q1->center.x, q1->center.y);
    assertLong(4, count_q1_levels, buffer);

    int count_q2_levels = 0;
    for (node = q2; node->up != NULL; node = node->up)
        count_q2_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q2->center.x, q2->center.y);
    assertLong(4, count_q2_levels, buffer);

    Quadtree_add(q1, &p3);
    Node* square1 = q1->children[get_quadrant(&q1->center, &p2)];
    Point_string(&square1->center, buffer);
    assertPoint(Point_create(4, 4), square1->center, buffer);

    assertTrue(square1->is_square, "q1->children[0].is_square");

    Node* q3 = square1->children[0];
    assertFalse(q3 == NULL, "(q1->children[0]->children[0] == NULL)");

    if (q3 != NULL) {
        Point_string(&q3->center, buffer);
        assertPoint(p3, q3->center, buffer);

        sprintf(buffer, "get_quadrant(Point(%f, %f), Point(%f, %f))",
            square1->center.x, square1->center.y, q3->center.x, q3->center.y);
        assertLong(0, get_quadrant(&square1->center, &q3->center), buffer);
    }
    else {  // alert to problems
        assertError("square1->children[0] is not NULL");
        assertError("square1->children[0]->center is not NULL");
    }

    assertFalse(square1->children[2] == NULL, "(q1->children[0]->children[2] == NULL)");

    sprintf(buffer, "get_quadrant(Point(%f, %f), Point(%f, %f))",
        square1->center.x, square1->center.y, q2->center.x, q2->center.y);
    assertLong(2, get_quadrant(&square1->center, &q2->center), buffer);

    Quadtree_add(q1, &p4);
    if (square1->children[2] != NULL) {
        Node* square2 = square1->children[2];
        Point_string(&square2->center, buffer);
        assertPoint(Point_create(2, 2), square2->center, buffer);

        Point_string(&square2->children[0]->center, buffer);
        assertPoint(p4, square2->children[0]->center, buffer);
    }
    else {  // alert to problems
        assertError("square1->children[0]->children[2] is not NULL");
        assertError("square1->children[0]->children[2]->center is not NULL");
    }

    Quadtree_uproot(q1);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    mtrace();
    printf("[Beginning tests]\n");
    
    start_test(test_sizes, "Struct sizes");
    start_test(test_in_range, "in_range");
    start_test(test_get_quadrant, "get_quadrant");
    start_test(test_get_new_center, "get_new_center");
    start_test(test_quadtree_create, "Quadtree_create");
    start_test(test_quadtree_add, "Quadtree_add");

    printf("\n[Ending tests]\n");
    printf("=============================================\n");
    printf("         TESTS AND ASSERTIONS REPORT         \n");
    printf("================== | ========================\n");
    printf("TOTAL  TESTS: %4lld | TOTAL  ASSERTIONS: %5lld\n", total_tests(), total_assertions());
    printf("PASSED TESTS: %4lld | PASSED ASSERTIONS: %5lld\n", passed_tests(), passed_assertions());
    printf("FAILED TESTS: %4lld | FAILED ASSERTIONS: %5lld\n", total_tests() - passed_tests(), total_assertions() - passed_assertions());
    printf("================== | ========================\n");
    return 0;
}
