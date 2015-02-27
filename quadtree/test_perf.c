/**
Testing suite for performance of Quadtrees
*/

#include "test.h"

void test_powers_of_two() {
    Marsaglia_srand(2);
    char buffer[1000];
    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    test_rand_off();

    uint64_t num_samples = 128;

    float64_t time_samples[num_samples];
    uint64_t total_cycles = 0;

    uint64_t i;
    for (i = 0; i < num_samples; i++) {
        unsigned long long denom = 1L << i;
        if (denom == 0)
            break;
        double x = 0.25 * s1 / denom;
        double y = 0.25 * s1 / denom;
        if (abs(x) <= PRECISION && abs(y) <= PRECISION)
            break;
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
    num_samples = i;

    #ifdef VERBOSE
    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%llu, %.8lf, ", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    Quadtree_uproot(q1);
}

void test_random_n(const uint64_t num_samples) {
    Marsaglia_srand(num_samples % ((1L << 32) - 1));
    char buffer[1000];
    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);
    Point* points = (Point*)malloc(sizeof(points) * num_samples);

    test_rand_off();

    float64_t time_samples[num_samples];
    uint64_t total_cycles = 0;

    // inserting everything

    uint64_t i;
    for (i = 0; i < num_samples; i++) {
        double x = (Marsaglia_random() - 0.5) * s1;
        double y = (Marsaglia_random() - 0.5) * s1;
        points[i] = Point_create(x, y);
        clock_t start = clock();
        bool result = Quadtree_add(q1, points + i);
        clock_t end = clock();
        if (result) {
            time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
            total_cycles += (end - start);
        }
        else
            i--;
    }

    #ifdef VERBOSE
    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%llu, %.8lf, ", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    // now to remove everything, in order

    total_cycles = 0;

    for (i = 0; i < num_samples; i++) {
        clock_t start = clock();
        bool result = Quadtree_remove(q1, points + i);
        clock_t end = clock();
        time_samples[i] = ((float64_t)(end - start)); // / CLOCKS_PER_SEC;
        total_cycles += (end - start);
    }

    #ifdef VERBOSE
    printf("Total time for %llu deletes: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%.8lf, ", total_cycles / (float64_t)CLOCKS_PER_SEC);
    #endif

    Quadtree_uproot(q1);
}

void test_random_2_10() {
    test_random_n(1L << 10);
}

void test_random_2_15() {
    test_random_n(1L << 15);
}

void test_random_2_20() {
    test_random_n(1L << 20);
}

void test_random_2_25() {
    test_random_n(1L << 25);
}

void test_random_2_26() {
    test_random_n(1L << 26);
}

void test_random_2_27() {
    test_random_n(1L << 27);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    Marsaglia_srand(0);
    #ifdef VERBOSE
    printf("[Beginning tests]\n");
    
    //start_test(test_powers_of_two, "Powers of two test");
    start_test(test_random_2_10, "Random 2^10 test");
    start_test(test_random_2_15, "Random 2^15 test");
    start_test(test_random_2_20, "Random 2^20 test");
    start_test(test_random_2_25, "Random 2^25 test");
    start_test(test_random_2_26, "Random 2^26 test");
    start_test(test_random_2_27, "Random 2^27 test");

    printf("\n[Ending tests]\n");
    printf("=============================================\n");
    printf("         TESTS AND ASSERTIONS REPORT         \n");
    printf("================== | ========================\n");
    printf("TOTAL  TESTS: %4lld | TOTAL  ASSERTIONS: %5lld\n", total_tests(), total_assertions());
    printf("PASSED TESTS: %4lld | PASSED ASSERTIONS: %5lld\n", passed_tests(), passed_assertions());
    printf("FAILED TESTS: %4lld | FAILED ASSERTIONS: %5lld\n", total_tests() - passed_tests(), total_assertions() - passed_assertions());
    printf("================== | ========================\n");
    #else
    //test_powers_of_two();
    test_random_2_10();
    test_random_2_15();
    test_random_2_20();
    test_random_2_25();
    test_random_2_26();
    test_random_2_27();
    printf("\n");
    #endif
    return 0;
}
