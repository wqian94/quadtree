/**
Testing suite for correctness of Quadtrees
*/

#include "test.h"

extern bool in_range(Node*, Point*);
extern void Point_string(Point*, char*);

void print_Quadtree(Quadtree* root) {
    printf("Node[id=%llu, center=(%lf, %lf), length=%lf, is_square=%d",
        (unsigned long long)root->id, root->center.x, root->center.y, root->length, root->is_square);

    if (root->parent != NULL)
        printf(", parent=%llu", (unsigned long long)root->parent->id);

    if (root->up != NULL)
        printf(", up=%llu", (unsigned long long)root->up->id);

    if (root->down != NULL)
        printf(", down=%llu", (unsigned long long)root->down->id);

    if (root->children[3] != NULL)
        printf(", children[3]=%llu", (unsigned long long)root->children[3]->id);

    if (root->children[2] != NULL)
        printf(", children[2]=%llu", (unsigned long long)root->children[2]->id);

    if (root->children[0] != NULL)
        printf(", children[0]=%llu", (unsigned long long)root->children[0]->id);

    if (root->children[1] != NULL)
        printf(", children[1]=%llu", (unsigned long long)root->children[1]->id);

    printf("]\n");

    if (root->up != NULL)
        print_Quadtree(root->up);

    if (root->children[2] != NULL)
        print_Quadtree(root->children[2]);

    if (root->children[3] != NULL)
        print_Quadtree(root->children[3]);

    if (root->children[0] != NULL)
        print_Quadtree(root->children[0]);

    if (root->children[1] != NULL)
        print_Quadtree(root->children[1]);

}

void test_sizes() {
    printf("sizeof(Quadtree)  = %lu\n", sizeof(Quadtree));
    printf("sizeof(bool)      = %lu\n", sizeof(bool));
    printf("sizeof(float64_t) = %lu\n", sizeof(float64_t));
    printf("sizeof(Node*)     = %lu\n", sizeof(Node*));
    printf("sizeof(Point)     = %lu\n", sizeof(Point));
    printf("\n===Testing Quadtree size===\n");
    // Quadtree is normally 80 bytes, but we add an id parameter for testing, so it is 88 bytes
    assertLong(88, sizeof(Quadtree), "sizeof(Quadtree)");
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
    int expected_quadrants[8] = {3, 3, 3, 2, 2, 0, 1, 1};
    int i;
    char buffer[100];
    for (i = 0; i < 8; i++) {
        sprintf(buffer, "get_quadrant((%f, %f), (%f, %f))", origin.x, origin.y, points[i].x, points[i].y);
        assertLong(expected_quadrants[i], get_quadrant(&origin, &points[i]), buffer);
    }
}

void test_get_new_center() {
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);
    
    Point new_center = get_new_center(q1, 0);
    assertPoint(Point_create(-4, -4), new_center, "new_center");
    Quadtree_uproot(q1);
}

void test_quadtree_create() {
    printf("\n---Quadtree_create Quadtree Or Square Test---\n");
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);
    assertDouble(s1, q1->length, "q1->length");
    assertPoint(p1, q1->center, "q1->center");
    assertTrue(q1->is_square, "q1->is_square");

    printf("\n---Quadtree_create Node Test---\n");
    Node* q2 = Node_create(s1, p1);
    assertDouble(s1, q2->length, "q2->length");
    assertPoint(p1, q2->center, "q2->center");
    assertFalse(q2->is_square, "q2->is_square");

    Quadtree_uproot(q1);
    Quadtree_uproot(q2);
}

void test_quadtree_add() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 0, 0, 0, 0, 99, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(7, 7);
    Point p4 = Point_create(3, 3);
    Point p5 = Point_create(-2, -2);
    Point p6 = Point_create(0.5, 0.5);

    printf("\n---Quadtree_add One Node Test---\n");
    Quadtree_add(q1, &p2);
    Node* q2 = q1->children[3];

    int count_q1_levels = 0;
    Node* node;
    for (node = q1; node != NULL; node = node->up)
        count_q1_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q1->center.x, q1->center.y);
    assertLong(5, count_q1_levels, buffer);

    int count_q2_levels = 0;
    for (node = q2; node != NULL; node = node->up)
        count_q2_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q2->center.x, q2->center.y);
    assertLong(5, count_q2_levels, buffer);

    printf("\n---Quadtree_add Conflicting Node Test---\n");
    Quadtree_add(q1, &p3);
    Node* square1 = q1->children[get_quadrant(&q1->center, &p2)];
    assertPoint(Point_create(4, 4), square1->center, "square1->center");

    assertTrue(square1->is_square, "q1->children[3].is_square");

    Node* q3 = square1->children[3];
    assertFalse(q3 == NULL, "(q1->children[3]->children[3] == NULL)");

    if (q3 != NULL) {
        assertPoint(p3, q3->center, "q3->center");

        sprintf(buffer, "get_quadrant(Point(%f, %f), Point(%f, %f))",
            square1->center.x, square1->center.y, q3->center.x, q3->center.y);
        assertLong(3, get_quadrant(&square1->center, &q3->center), buffer);
    }
    else {  // alert to problems
        assertError("square1->children[3] is not NULL");
        assertError("square1->children[3]->center is not NULL");
    }

    assertFalse(square1->children[0] == NULL, "(q1->children[3]->children[0] == NULL)");

    sprintf(buffer, "get_quadrant(Point(%f, %f), Point(%f, %f))",
        square1->center.x, square1->center.y, q2->center.x, q2->center.y);
    assertLong(0, get_quadrant(&square1->center, &q2->center), buffer);

    printf("\n---Quadtree_add Inner Square Generation Test---\n");
    Quadtree_add(q1, &p4);
    Node* square2 = NULL;
    if (square1->children[0] != NULL) {
        square2 = square1->children[0];
        assertPoint(Point_create(2, 2), square2->center, "square2->center");

        if (square2->children[3] != NULL) {
            assertPoint(p4, square2->children[3]->center, "square2->children[3]->center");
        }
        else {
            assertError("square2->children[3]->center is not NULL");
        }
    }
    else {  // alert to problems
        assertError("square1->children[3]->children[0] is not NULL");
        assertError("square1->children[3]->children[0]->center is not NULL");
    }

    printf("\n---Quadtree_add Greater Depth Test---\n");
    Quadtree_add(q1, &p5);
    assertTrue(q1->children[0] != NULL, "(q1->children[0] != NULL)");
    if (q1->children[0] != NULL) {
        assertFalse(q1->children[0]->is_square, "q1->children[0]->is_square");
        assertPoint(Point_create(-2, -2), q1->children[0]->center, "q1->children[0]->center");
    }
    else {
        assertError("q1->children[0]->is_square is not NULL");
        assertError("q1->children[0]->center is not NULL");
    }

    printf("\n---Quadtree_add Alternating Quadrant Test---\n");
    Quadtree_add(q1, &p6);
    Node* square3 = NULL;
    if (square2 != NULL && square2->children[0] != NULL) {
        square3 = square2->children[0];
        assertPoint(Point_create(1, 1), square3->center, "square3->center");
    }
    else
        assertError("square3->center is not NULL");

    if (square3 != NULL && square3->children[3] != NULL)
        assertPoint(p2, square3->children[3]->center, "square3->children[3]->center");
    else
        assertError("square3->children[3]->center is not NULL");

    if (square3 != NULL && square3->children[0] != NULL)
        assertPoint(p6, square3->children[0]->center, "square3->children[0]->center");
    else
        assertError("square3->children[0]->center is not NULL");

    if (square3 != NULL && square3->children[0] != NULL && square3->children[0]->up != NULL)
        assertPoint(p6, square3->children[0]->up->center, "square3->children[0]->up->center");
    else
        assertError("square3->children[0]->up->center is not NULL");

    if (square3 != NULL && square3->up != NULL) {
        if (square3->up->children[0] != NULL)
            assertPoint(p6, square3->up->children[0]->center, "square3->up->children[0]->center");
        else
            assertError("square3->up->children[0]->center is not NULL");
    }
    else {  // if we get here, it means that our parent-finding algorithm is wrong
        assertError("square3->up->children[0]->center is not NULL [invalid parent]");
    }

    Quadtree_uproot(q1);
}

void test_quadtree_search() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(3, 3);
    Point p4 = Point_create(2.5, 2.5);
    Point p5 = Point_create(-2, -2);
    Point p6 = Point_create(-2.1, -2.1);
    Point p7 = Point_create(3.25, 1.25);

    Quadtree_add(q1, &p2);
    Quadtree_add(q1, &p3);
    Quadtree_add(q1, &p4);
    Quadtree_add(q1, &p5);
    Quadtree_add(q1, &p6);
    Quadtree_add(q1, &p7);

    assertFalse(Quadtree_search(q1, &p1), "Quadtree_search(q1, p1)");  // because p1 isn't actually a data point
    assertTrue(Quadtree_search(q1, &p2), "Quadtree_search(q1, p2)");
    assertTrue(Quadtree_search(q1, &p3), "Quadtree_search(q1, p3)");
    assertTrue(Quadtree_search(q1, &p4), "Quadtree_search(q1, p4)");
    assertTrue(Quadtree_search(q1, &p5), "Quadtree_search(q1, p5)");
    assertTrue(Quadtree_search(q1, &p6), "Quadtree_search(q1, p6)");
    assertTrue(Quadtree_search(q1, &p7), "Quadtree_search(q1, p7)");

    Quadtree_uproot(q1);
}

void test_quadtree_remove() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(3, 3);
    Point p4 = Point_create(2.5, 2.5);
    Point p5 = Point_create(-2, -2);
    Point p6 = Point_create(-2.1, -2.1);
    Point p7 = Point_create(3.25, 1.25);

    Quadtree_add(q1, &p2);
    Quadtree_add(q1, &p3);
    Quadtree_add(q1, &p4);
    Quadtree_add(q1, &p5);
    Quadtree_add(q1, &p6);
    Quadtree_add(q1, &p7);

    assertTrue(Quadtree_search(q1, &p2), "Quadtree_search(q1, p2)");
    Quadtree_remove(q1, &p2);
    assertFalse(Quadtree_search(q1, &p2), "Quadtree_search(q1, p2)");

    assertTrue(Quadtree_search(q1, &p3), "Quadtree_search(q1, p3)");
    Quadtree_remove(q1, &p3);
    assertFalse(Quadtree_search(q1, &p3), "Quadtree_search(q1, p3)");

    assertTrue(Quadtree_search(q1, &p4), "Quadtree_search(q1, p4)");
    Quadtree_remove(q1, &p4);
    assertFalse(Quadtree_search(q1, &p4), "Quadtree_search(q1, p4)");

    assertTrue(Quadtree_search(q1, &p5), "Quadtree_search(q1, p5)");
    Quadtree_remove(q1, &p5);
    assertFalse(Quadtree_search(q1, &p5), "Quadtree_search(q1, p5)");

    assertTrue(Quadtree_search(q1, &p6), "Quadtree_search(q1, p6)");
    Quadtree_remove(q1, &p6);
    assertFalse(Quadtree_search(q1, &p6), "Quadtree_search(q1, p6)");

    assertTrue(Quadtree_search(q1, &p7), "Quadtree_search(q1, p7)");
    Quadtree_remove(q1, &p7);
    assertFalse(Quadtree_search(q1, &p7), "Quadtree_search(q1, p7)");

    Quadtree_uproot(q1);
}

void test_randomized() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    test_rand_off();

    Point p2 = Point_create(1, 1);
    Point p3 = Point_create(3, 3);
    Point p4 = Point_create(2.5, 2.5);
    Point p5 = Point_create(-2, -2);
    Point p6 = Point_create(-2.1, -2.1);
    Point p7 = Point_create(3.25, 1.25);

    Quadtree_add(q1, &p2);
    Quadtree_add(q1, &p3);
    Quadtree_add(q1, &p4);
    Quadtree_add(q1, &p5);
    Quadtree_add(q1, &p6);
    Quadtree_add(q1, &p7);

    assertTrue(Quadtree_search(q1, &p2), "Quadtree_search(q1, p2)");
    Quadtree_remove(q1, &p2);
    assertFalse(Quadtree_search(q1, &p2), "Quadtree_search(q1, p2)");

    assertTrue(Quadtree_search(q1, &p3), "Quadtree_search(q1, p3)");
    Quadtree_remove(q1, &p3);
    assertFalse(Quadtree_search(q1, &p3), "Quadtree_search(q1, p3)");

    assertTrue(Quadtree_search(q1, &p4), "Quadtree_search(q1, p4)");
    Quadtree_remove(q1, &p4);
    assertFalse(Quadtree_search(q1, &p4), "Quadtree_search(q1, p4)");

    assertTrue(Quadtree_search(q1, &p5), "Quadtree_search(q1, p5)");
    Quadtree_remove(q1, &p5);
    assertFalse(Quadtree_search(q1, &p5), "Quadtree_search(q1, p5)");

    assertTrue(Quadtree_search(q1, &p6), "Quadtree_search(q1, p6)");
    Quadtree_remove(q1, &p6);
    assertFalse(Quadtree_search(q1, &p6), "Quadtree_search(q1, p6)");

    assertTrue(Quadtree_search(q1, &p7), "Quadtree_search(q1, p7)");
    Quadtree_remove(q1, &p7);
    assertFalse(Quadtree_search(q1, &p7), "Quadtree_search(q1, p7)");

    Quadtree_uproot(q1);
}

void test_performance() {
    char buffer[1000];
    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    test_rand_off();

    const uint64_t num_samples = 1L << 17;

    float64_t time_samples[num_samples];
    uint64_t total_cycles = 0;

    uint64_t i;
    for (i = 0; i < num_samples; i++) {
        double x = (Marsaglia_random() - 0.5) * s1;
        double y = (Marsaglia_random() - 0.5) * s1;
        Point p = Point_create(x, y);
        clock_t start = clock();
        bool result = Quadtree_add(q1, &p);
        clock_t end = clock();
        if (result) {
            time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
            total_cycles += (end - start);
        }
        else
            i--;
    }

    double time_samples_d1[num_samples - 1];  // first derivatives
    for (i = 0; i < num_samples - 1; i++) {
        time_samples_d1[i] = time_samples[i + 1] - time_samples[i];
    }

    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);

    Quadtree_uproot(q1);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    mtrace();
    Marsaglia_srand(time(NULL));
    printf("[Beginning tests]\n");
    
    start_test(test_sizes, "Struct sizes");
    start_test(test_in_range, "in_range");
    start_test(test_get_quadrant, "get_quadrant");
    start_test(test_get_new_center, "get_new_center");
    start_test(test_quadtree_create, "Quadtree_create");
    start_test(test_quadtree_add, "Quadtree_add");
    start_test(test_quadtree_search, "Quadtree_search");
    start_test(test_quadtree_remove, "Quadtree_remove");
    start_test(test_randomized, "Randomized (in-environment)");
    start_test(test_performance, "Performance tests");

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
