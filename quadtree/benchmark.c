/**
Benchmarking suite for insertion/deletion/querying data structures
*/

#include "test.h"

void print_Quadtree(Quadtree *root) {
    printf("Node[pointer=%p, ", root);
#ifdef QUADTREE_TEST
    printf("id=%llu, ", (unsigned long long)root->id);
#endif
    printf("center=(%lf, %lf), length=%lf, is_square=%d",
        root->center->x, root->center->y, root->length, root->is_square);

    if (root->parent != NULL)
#ifdef QUADTREE_TEST
        printf(", parent=%llu", (unsigned long long)root->parent->id);
#else
        printf(", parent=%p", root->parent);
#endif

    if (root->up != NULL)
#ifdef QUADTREE_TEST
        printf(", up=%llu", (unsigned long long)root->up->id);
#else
        printf(", up=%p", root->up);
#endif

    if (root->down != NULL)
#ifdef QUADTREE_TEST
        printf(", down=%llu", (unsigned long long)root->down->id);
#else
        printf(", down=%p", root->down);
#endif

    if (root->children[0] != NULL)
#ifdef QUADTREE_TEST
        printf(", children[0]=%llu", (unsigned long long)root->children[0]->id);
#else
        printf(", children[0]=%p", root->children[0]);
#endif

    if (root->children[1] != NULL)
#ifdef QUADTREE_TEST
        printf(", children[1]=%llu", (unsigned long long)root->children[1]->id);
#else
        printf(", children[1]=%p", root->children[1]);
#endif

    if (root->children[2] != NULL)
#ifdef QUADTREE_TEST
        printf(", children[2]=%llu", (unsigned long long)root->children[2]->id);
#else
        printf(", children[2]=%p", root->children[2]);
#endif

    if (root->children[3] != NULL)
#ifdef QUADTREE_TEST
        printf(", children[3]=%llu", (unsigned long long)root->children[3]->id);
#else
        printf(", children[3]=%p", root->children[3]);
#endif

    printf("]\n");

    if (root->up != NULL)
        print_Quadtree(root->up);

    if (root->children[0] != NULL)
        print_Quadtree(root->children[0]);

    if (root->children[1] != NULL)
        print_Quadtree(root->children[1]);

    if (root->children[2] != NULL)
        print_Quadtree(root->children[2]);

    if (root->children[3] != NULL)
        print_Quadtree(root->children[3]);
}

void test_random_n(const uint64_t num_samples) {
    Marsaglia_srand(num_samples % ((1LL << 32) - 1));
    char buffer[1000];
    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_init(0, 0);
    Quadtree* q1 = Quadtree_init(s1, p1);
    Point* points = (Point*)malloc(sizeof(points) * num_samples);

    test_rand_off();

    float64_t time_samples[num_samples];
    uint64_t total_cycles = 0;

    // inserting everything

    uint64_t i;
    for (i = 0; i < num_samples; i++) {
        double x = (Marsaglia_random() - 0.5) * s1;
        double y = (Marsaglia_random() - 0.5) * s1;
        points[i] = Point_init(x, y);
        clock_t start = clock();
        bool result = Quadtree_add(q1, points[i]);
        clock_t end = clock();
        if (result) {
            time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
            total_cycles += (end - start);
        }
        else {
            i--;
        }
    }

    #ifdef VERBOSE
    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%llu, %.8lf, ", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    // then to try searching for everything
    total_cycles = 0;

    for (i = 0; i < num_samples; i++) {
        clock_t start = clock();
        bool result = Quadtree_search(q1, points[i]);
        clock_t end = clock();
        time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
        total_cycles += (end - start);
    }

    #ifdef VERBOSE
    printf("Total time for %llu queries: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%.8lf, ", total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    // now to remove everything, in order
    total_cycles = 0;

    uint64_t count = 0;
    for (i = 0; i < num_samples; i++) {
        clock_t start = clock();
        bool result = Quadtree_remove(q1, points[i]);
        clock_t end = clock();
        count += Quadtree_search(q1, points[i]);
        time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
        total_cycles += (end - start);
    }

    #ifdef VERBOSE
    printf("Total time for %llu deletes: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%.8lf, ", total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    #ifdef VERBOSE
    printf("Search count: %llu\n", (unsigned long long)count);

    QuadtreeFreeResult result = Quadtree_free(q1);
    printf("Levels: %llu\nTotal number of leftover nodes (should be 1): %llu\nNumber of leftover leaf nodes (should be 0): %llu\n",
        (unsigned long long)result.levels, (unsigned long long)result.total, (unsigned long long)result.leaf);
    #else
    Quadtree_free(q1);
    #endif
}

void test_random_2_10() {test_random_n(1LL << 10);}
void test_random_2_11() {test_random_n(1LL << 11);}
void test_random_2_12() {test_random_n(1LL << 12);}
void test_random_2_13() {test_random_n(1LL << 13);}
void test_random_2_14() {test_random_n(1LL << 14);}
void test_random_2_15() {test_random_n(1LL << 15);}
void test_random_2_16() {test_random_n(1LL << 16);}
void test_random_2_17() {test_random_n(1LL << 17);}
void test_random_2_18() {test_random_n(1LL << 18);}
void test_random_2_19() {test_random_n(1LL << 19);}
void test_random_2_20() {test_random_n(1LL << 20);}
void test_random_2_21() {test_random_n(1LL << 21);}
void test_random_2_22() {test_random_n(1LL << 22);}
void test_random_2_23() {test_random_n(1LL << 23);}
void test_random_2_24() {test_random_n(1LL << 24);}
void test_random_2_25() {test_random_n(1LL << 25);}
void test_random_2_26() {test_random_n(1LL << 26);}
void test_random_2_27() {test_random_n(1LL << 27);}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    Marsaglia_srand(0);
    #ifdef VERBOSE
    printf("[Beginning tests]\n");
    
    start_test(test_random_2_10, "Random 2^10 test");
    start_test(test_random_2_15, "Random 2^15 test");
    start_test(test_random_2_20, "Random 2^20 test");
    //start_test(test_random_2_25, "Random 2^25 test");
    //start_test(test_random_2_26, "Random 2^26 test");
    //start_test(test_random_2_27, "Random 2^27 test");

    printf("\n[Ending tests]\n");
    /*printf("=============================================\n");
    printf("         TESTS AND ASSERTIONS REPORT         \n");
    printf("================== | ========================\n");
    printf("TOTAL  TESTS: %4lld | TOTAL  ASSERTIONS: %5lld\n", total_tests(), total_assertions());
    printf("PASSED TESTS: %4lld | PASSED ASSERTIONS: %5lld\n", passed_tests(), passed_assertions());
    printf("FAILED TESTS: %4lld | FAILED ASSERTIONS: %5lld\n", total_tests() - passed_tests(), total_assertions() - passed_assertions());
    printf("================== | ========================\n");*/
    #else
    test_random_2_10();
    test_random_2_11();
    test_random_2_12();
    test_random_2_13();
    test_random_2_14();
    test_random_2_15();
    test_random_2_16();
    test_random_2_17();
    test_random_2_18();
    test_random_2_19();
    test_random_2_20();
    test_random_2_21();
    test_random_2_22();
    test_random_2_23();
    test_random_2_24();
    test_random_2_25();
    test_random_2_26();
    test_random_2_27();
    printf("\n");
    #endif
    return 0;
}
