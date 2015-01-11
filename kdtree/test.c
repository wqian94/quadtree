/**
Testing suite for KDtrees
*/

#include "./test.h"

//extern bool in_range(Node*, Point*);

/*void print_KDtree(KDtree* root) {
    printf("Node[id=%llu, center=(%lf, %lf), length=%lf, is_container=%d",
        (unsigned long long)root->id, root->center.val[0], root->center.val[1], root->length, root->is_container);

    if (root->parent != NULL)
        printf(", parent=%llu", (unsigned long long)root->parent->id);

    if (root->up != NULL)
        printf(", up=%llu", (unsigned long long)root->up->id);

    if (root->down != NULL)
        printf(", down=%llu", (unsigned long long)root->down->id);

    register uint64_t i;
    for (i = 0; i < N_PARTITIONS; i++) {
        if (root->children[i] != NULL)
            printf(", children[%llu]=%llu", (unsigned long long)i, (unsigned long long)root->children[3]->id);
    }
    printf("]\n");

    if (root->up != NULL)
        print_KDtree(root->up);

    for (i = 0; i < N_PARTITIONS; i++) {
        if (root->children[i] != NULL)
            print_KDtree(root->children[i]);
    }
}

void test_sizes() {
    printf("sizeof(KDtree)    = %lu\n", sizeof(KDtree));
    printf("sizeof(bool)      = %lu\n", sizeof(bool));
    printf("sizeof(float64_t) = %lu\n", sizeof(float64_t));
    printf("sizeof(Node*)     = %lu\n", sizeof(Node*));
    printf("sizeof(Point)     = %lu\n", sizeof(Point));
    printf("\n===Testing KDtree size===\n");
    // KDtree is normally 72 + 8D bytes, but we add an id parameter for testing, so it is +8 bytes
    assertLong(72 + 8 * D + 8, sizeof(KDtree), "sizeof(KDtree)");
}*/

void test_point() {
    register int32_t i;

    printf("\n---1-D Point_create test---\n");
    Point p1 = Point_create(1, (float64_t[]){1});
    assertLong(1, p1.val[0], "p1.val[0]");

    printf("\n---2-D Point_create test---\n");
    Point p2 = Point_create(2, (float64_t[]){0, 1});
    assertLong(0, p2.val[0], "p2.val[0]");
    assertLong(1, p2.val[1], "p2.val[1]");

    printf("\n---3-D Point_create test---\n");
    Point p3 = Point_create(3, (float64_t[]){0, 1, 2});
    assertLong(0, p3.val[0], "p3.val[0]");
    assertLong(1, p3.val[1], "p3.val[1]");
    assertLong(2, p3.val[2], "p3.val[2]");

    printf("\n---10-D Point_create test---\n");
    float64_t point_coords_10D[10];
    for (i = 0; i < 10; i++)
        point_coords_10D[i] = i * i;
    Point p10 = Point_create(10, point_coords_10D);
    for (i = 0; i < 10; i++) {
        char buffer[20];
        sprintf(buffer, "p10.val[%d]", i);
        assertLong(i * i, p10.val[i], buffer);
    }

    Point_purge();
}

void test_partition() {
    Partition p1 = Partition_create(2);
    assertLong(2, p1.dim, "p1.dim");
    assertLong(RIGHT, Partition_get(p1, 0), "Partition_get(p1, 0)");
    assertLong(RIGHT, Partition_get(p1, 1), "Partition_get(p1, 1)");
    assertTrue(Partition_equals(p1, p1), "Partition_equals(p1, p1)");

    Partition p2 = Partition_create(2);
    assertLong(2, p2.dim, "p2.dim");
    assertLong(RIGHT, Partition_get(p2, 0), "Partition_get(p2, 0)");
    assertLong(RIGHT, Partition_get(p2, 1), "Partition_get(p2, 1)");
    assertTrue(Partition_equals(p1, p2), "Partition_equals(p1, p2)");
    Partition_set(p2, 1, LEFT);
    assertLong(RIGHT, Partition_get(p2, 0), "Partition_get(p2, 0)");
    assertLong(LEFT, Partition_get(p2, 1), "Partition_get(p2, 1)");
    assertFalse(Partition_equals(p1, p2), "Partition_equals(p1, p2)");

    Partition p3 = Partition_create(3);
    assertLong(3, p3.dim, "p3.dim");
    assertFalse(Partition_equals(p1, p3), "Partition_equals(p1, p3)");

    Partition_purge();
}

/*
void test_in_range() {
    KDtree* node = KDtree_create(2.0, ((Point){0, 0}));
    Point p1 = ((Point){-1, -1});
    Point p2 = ((Point){2, 2});
    char buffer[100], point_buffer[100], node_buffer[256];

    Node_string(node, node_buffer);

    Point_string(&p1, point_buffer);
    sprintf(buffer, "in_range(node, %s)", point_buffer);
    assertTrue(in_range(node, &p1), buffer);

    Point_string(&p2, point_buffer);
    sprintf(buffer, "in_range(node, %s)", point_buffer);
    assertFalse(in_range(node, &p2), buffer);

    KDtree_uproot(node);
}

void test_get_partition() {
    Point origin = ((Point){0, 0});
    Point points[8] = {
        ((Point){1, 0}), ((Point){1, 1}), ((Point){0, 1}), ((Point){-1, 1}),
        ((Point){-1, 0}), ((Point){-1, -1}), ((Point){0, -1}), ((Point){1, -1})
    };
    int expected_partitions[8] = {3, 3, 3, 2, 2, 0, 1, 1};
    int i;
    char buffer[100];
    for (i = 0; i < 8; i++) {
        sprintf(buffer, "get_partition((%f, %f), (%f, %f))", origin.val[0], origin.val[1], points[i].val[0], points[i].val[1]);
        assertLong(expected_partitions[i], get_partition(&origin, &points[i]), buffer);
    }
}

void test_get_new_center() {
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);
    
    Point new_center = get_new_center(q1, 3);
    assertPoint(((Point){4, 4}), new_center, "new_center");
    KDtree_uproot(q1);
}

void test_kdtree_create() {
    printf("\n---KDtree_create KDtree Or Square Test---\n");
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);
    assertDouble(s1, q1->length, "q1->length");
    assertPoint(p1, q1->center, "q1->center");
    assertTrue(q1->is_container, "q1->is_container");

    printf("\n---KDtree_create Node Test---\n");
    Node* q2 = Node_create(s1, p1);
    assertDouble(s1, q2->length, "q2->length");
    assertPoint(p1, q2->center, "q2->center");
    assertFalse(q2->is_container, "q2->is_container");

    KDtree_uproot(q1);
    KDtree_uproot(q2);
}

void test_kdtree_add() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 0, 0, 0, 0, 99, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = ((Point){1, 1});
    Point p3 = ((Point){7, 7});
    Point p4 = ((Point){3, 3});
    Point p5 = ((Point){-2, -2});
    Point p6 = ((Point){0.5, 0.5});

    printf("\n---KDtree_add One Node Test---\n");
    KDtree_add(q1, &p2);
    Node* q2 = q1->children[3];

    int count_q1_levels = 0;
    Node* node;
    for (node = q1; node != NULL; node = node->up)
        count_q1_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q1->center.val[0], q1->center.val[1]);
    assertLong(5, count_q1_levels, buffer);

    int count_q2_levels = 0;
    for (node = q2; node != NULL; node = node->up)
        count_q2_levels++;
    sprintf(buffer, "Levels of Node(%lf, %lf)", q2->center.val[0], q2->center.val[1]);
    assertLong(5, count_q2_levels, buffer);

    printf("\n---KDtree_add Conflicting Node Test---\n");
    KDtree_add(q1, &p3);
    Node* container1 = q1->children[get_partition(&q1->center, &p2)];
    assertPoint(((Point){4, 4}), container1->center, "container1->center");

    assertTrue(container1->is_container, "q1->children[3].is_container");

    Node* q3 = container1->children[3];
    assertFalse(q3 == NULL, "(q1->children[3]->children[3] == NULL)");

    if (q3 != NULL) {
        assertPoint(p3, q3->center, "q3->center");

        sprintf(buffer, "get_partition(Point(%f, %f), Point(%f, %f))",
            container1->center.val[0], container1->center.val[1], q3->center.val[0], q3->center.val[1]);
        assertLong(3, get_partition(&container1->center, &q3->center), buffer);
    }
    else {  // alert to problems
        assertError("container1->children[3] is not NULL");
        assertError("container1->children[3]->center is not NULL");
    }

    assertFalse(container1->children[0] == NULL, "(q1->children[3]->children[0] == NULL)");

    sprintf(buffer, "get_partition(Point(%f, %f), Point(%f, %f))",
        container1->center.val[0], container1->center.val[1], q2->center.val[0], q2->center.val[1]);
    assertLong(0, get_partition(&container1->center, &q2->center), buffer);

    printf("\n---KDtree_add Inner Square Generation Test---\n");
    KDtree_add(q1, &p4);
    Node* container2 = NULL;
    if (container1->children[0] != NULL) {
        container2 = container1->children[0];
        assertPoint(((Point){2, 2}), container2->center, "container2->center");

        if (container2->children[3] != NULL) {
            assertPoint(p4, container2->children[3]->center, "container2->children[3]->center");
        }
        else {
            assertError("container2->children[3]->center is not NULL");
        }
    }
    else {  // alert to problems
        assertError("container1->children[3]->children[0] is not NULL");
        assertError("container1->children[3]->children[0]->center is not NULL");
    }

    printf("\n---KDtree_add Greater Depth Test---\n");
    KDtree_add(q1, &p5);
    assertTrue(q1->children[0] != NULL, "(q1->children[0] != NULL)");
    if (q1->children[0] != NULL) {
        assertFalse(q1->children[0]->is_container, "q1->children[0]->is_container");
        assertPoint(((Point){-2, -2}), q1->children[0]->center, "q1->children[0]->center");
    }
    else {
        assertError("q1->children[0]->is_container is not NULL");
        assertError("q1->children[0]->center is not NULL");
    }

    printf("\n---KDtree_add Alternating Partition Test---\n");
    KDtree_add(q1, &p6);
    Node* container3 = NULL;
    if (container2 != NULL && container2->children[0] != NULL) {
        container3 = container2->children[0];
        assertPoint(((Point){1, 1}), container3->center, "container3->center");
    }
    else
        assertError("container3->center is not NULL");

    if (container3 != NULL && container3->children[3] != NULL)
        assertPoint(p2, container3->children[3]->center, "container3->children[3]->center");
    else
        assertError("container3->children[3]->center is not NULL");

    if (container3 != NULL && container3->children[0] != NULL)
        assertPoint(p6, container3->children[0]->center, "container3->children[0]->center");
    else
        assertError("container3->children[0]->center is not NULL");

    if (container3 != NULL && container3->children[0] != NULL && container3->children[0]->up != NULL)
        assertPoint(p6, container3->children[0]->up->center, "container3->children[0]->up->center");
    else
        assertError("container3->children[0]->up->center is not NULL");

    if (container3 != NULL && container3->up != NULL) {
        if (container3->up->children[0] != NULL)
            assertPoint(p6, container3->up->children[0]->center, "container3->up->children[0]->center");
        else
            assertError("container3->up->children[0]->center is not NULL");
    }
    else {  // if we get here, it means that our parent-finding algorithm is wrong
        assertError("container3->up->children[0]->center is not NULL [invalid parent]");
    }

    KDtree_uproot(q1);
}

void test_kdtree_search() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = ((Point){1, 1});
    Point p3 = ((Point){3, 3});
    Point p4 = ((Point){2.5, 2.5});
    Point p5 = ((Point){-2, -2});
    Point p6 = ((Point){-2.1, -2.1});
    Point p7 = ((Point){3.25, 1.25});

    KDtree_add(q1, &p2);
    KDtree_add(q1, &p3);
    KDtree_add(q1, &p4);
    KDtree_add(q1, &p5);
    KDtree_add(q1, &p6);
    KDtree_add(q1, &p7);

    assertFalse(KDtree_search(q1, &p1), "KDtree_search(q1, p1)");  // because p1 isn't actually a data point
    assertTrue(KDtree_search(q1, &p2), "KDtree_search(q1, p2)");
    assertTrue(KDtree_search(q1, &p3), "KDtree_search(q1, p3)");
    assertTrue(KDtree_search(q1, &p4), "KDtree_search(q1, p4)");
    assertTrue(KDtree_search(q1, &p5), "KDtree_search(q1, p5)");
    assertTrue(KDtree_search(q1, &p6), "KDtree_search(q1, p6)");
    assertTrue(KDtree_search(q1, &p7), "KDtree_search(q1, p7)");

    KDtree_uproot(q1);
}

void test_kdtree_remove() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);

    // engineer our own "random" values
    uint32_t rand_food[32] = {0, 0, 0, 0, 99, 0, 0, 99, 0, 0, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
    test_rand_feed(rand_food, 32);

    Point p2 = ((Point){1, 1});
    Point p3 = ((Point){3, 3});
    Point p4 = ((Point){2.5, 2.5});
    Point p5 = ((Point){-2, -2});
    Point p6 = ((Point){-2.1, -2.1});
    Point p7 = ((Point){3.25, 1.25});

    KDtree_add(q1, &p2);
    KDtree_add(q1, &p3);
    KDtree_add(q1, &p4);
    KDtree_add(q1, &p5);
    KDtree_add(q1, &p6);
    KDtree_add(q1, &p7);

    assertTrue(KDtree_search(q1, &p2), "KDtree_search(q1, p2)");
    KDtree_remove(q1, &p2);
    assertFalse(KDtree_search(q1, &p2), "KDtree_search(q1, p2)");

    assertTrue(KDtree_search(q1, &p3), "KDtree_search(q1, p3)");
    KDtree_remove(q1, &p3);
    assertFalse(KDtree_search(q1, &p3), "KDtree_search(q1, p3)");

    assertTrue(KDtree_search(q1, &p4), "KDtree_search(q1, p4)");
    KDtree_remove(q1, &p4);
    assertFalse(KDtree_search(q1, &p4), "KDtree_search(q1, p4)");

    assertTrue(KDtree_search(q1, &p5), "KDtree_search(q1, p5)");
    KDtree_remove(q1, &p5);
    assertFalse(KDtree_search(q1, &p5), "KDtree_search(q1, p5)");

    assertTrue(KDtree_search(q1, &p6), "KDtree_search(q1, p6)");
    KDtree_remove(q1, &p6);
    assertFalse(KDtree_search(q1, &p6), "KDtree_search(q1, p6)");

    assertTrue(KDtree_search(q1, &p7), "KDtree_search(q1, p7)");
    KDtree_remove(q1, &p7);
    assertFalse(KDtree_search(q1, &p7), "KDtree_search(q1, p7)");

    KDtree_uproot(q1);
}

void test_randomized() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);

    test_rand_off();

    Point p2 = ((Point){1, 1});
    Point p3 = ((Point){3, 3});
    Point p4 = ((Point){2.5, 2.5});
    Point p5 = ((Point){-2, -2});
    Point p6 = ((Point){-2.1, -2.1});
    Point p7 = ((Point){3.25, 1.25});

    KDtree_add(q1, &p2);
    KDtree_add(q1, &p3);
    KDtree_add(q1, &p4);
    KDtree_add(q1, &p5);
    KDtree_add(q1, &p6);
    KDtree_add(q1, &p7);

    assertTrue(KDtree_search(q1, &p2), "KDtree_search(q1, p2)");
    KDtree_remove(q1, &p2);
    assertFalse(KDtree_search(q1, &p2), "KDtree_search(q1, p2)");

    assertTrue(KDtree_search(q1, &p3), "KDtree_search(q1, p3)");
    KDtree_remove(q1, &p3);
    assertFalse(KDtree_search(q1, &p3), "KDtree_search(q1, p3)");

    assertTrue(KDtree_search(q1, &p4), "KDtree_search(q1, p4)");
    KDtree_remove(q1, &p4);
    assertFalse(KDtree_search(q1, &p4), "KDtree_search(q1, p4)");

    assertTrue(KDtree_search(q1, &p5), "KDtree_search(q1, p5)");
    KDtree_remove(q1, &p5);
    assertFalse(KDtree_search(q1, &p5), "KDtree_search(q1, p5)");

    assertTrue(KDtree_search(q1, &p6), "KDtree_search(q1, p6)");
    KDtree_remove(q1, &p6);
    assertFalse(KDtree_search(q1, &p6), "KDtree_search(q1, p6)");

    assertTrue(KDtree_search(q1, &p7), "KDtree_search(q1, p7)");
    KDtree_remove(q1, &p7);
    assertFalse(KDtree_search(q1, &p7), "KDtree_search(q1, p7)");

    KDtree_uproot(q1);
}

void test_performance() {
    char buffer[1000];
    float64_t s1 = 16.0;  // size1; chose to use S instead of L
    Point p1 = ((Point){0, 0});
    KDtree* q1 = KDtree_create(s1, p1);

    test_rand_off();

    const uint64_t num_samples = 32;

    double time_samples[num_samples];

    uint64_t i;
    for (i = 0; i < num_samples; i++) {
        double x = 0.25 * s1 / (1 << i);  //(Marsaglia_random() - 0.5) * s1;
        double y = 0.25 * s1 / (1 << i);  //(Marsaglia_random() - 0.5) * s1;
        Point p = ((Point){x, y});
        clock_t start = clock();
        KDtree_add(q1, &p);
        clock_t end = clock();
        time_samples[i] = ((double)(end - start)); // / CLOCKS_PER_SEC;
        if (i)
            time_samples[i] += time_samples[i - 1];
    }

    double time_samples_d1[num_samples - 1];  // first derivatives
    for (i = 0; i < num_samples - 1; i++) {
        time_samples_d1[i] = time_samples[i + 1] - time_samples[i];
    }

    double time_samples_d2[num_samples - 2];  // second derivatives
    for (i = 0; i < num_samples - 2; i++) {
        time_samples_d2[i] = time_samples_d1[i + 1] - time_samples_d1[i];
    }

    // expecting (1/(N+1))/(1/N) = N/(N+1) ratio
    double ratios[num_samples - 3];  // ratios
    for (i = 0; i < num_samples - 3; i++) {
        ratios[i] = time_samples_d2[i + 1] / (time_samples_d2[i] ? time_samples_d2[i] : 1);
    }

    // N/(N+1) * (N+1)/(N+2) * ... telescopes to N/(N+M)
    double ratio_product = 1;
    for (i = 0; i < num_samples - 3; i++) {
        ratio_product *= ratios[i] ? ratios[i] : 1;
    }

    printf("ratio_product: %lf\n%lu/%u = %lf, %lu/%u = %lf, %lu/%u = %lf\n",
        ratio_product, num_samples - 3, 1, (num_samples - 3)/1.0,
        num_samples - 2, 1, (num_samples - 2)/1.0, num_samples - 1, 1, (num_samples - 1)/1.0);

    KDtree_uproot(q1);
}*/

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    mtrace();
    printf("[Beginning tests]\n");
    
    /*start_test(test_sizes, "Struct sizes");
    */
    start_test(test_point, "Point tests");
    start_test(test_partition, "Partition tests");
    /*
    start_test(test_in_range, "in_range");
    start_test(test_get_partition, "get_partition");
    start_test(test_get_new_center, "get_new_center");
    start_test(test_kdtree_create, "KDtree_create");
    start_test(test_kdtree_add, "KDtree_add");
    start_test(test_kdtree_search, "KDtree_search");
    start_test(test_kdtree_remove, "KDtree_remove");
    start_test(test_randomized, "Randomized (in-environment)");
    start_test(test_performance, "Performance tests");*/

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
