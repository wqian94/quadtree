/**
Testing suite for performance of Quadtrees
*/

#include "test.h"

void test_powers_of_two() {
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

    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);

    Quadtree_uproot(q1);
}

void test_random_n(const uint64_t num_samples) {
    char buffer[1000];
    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_create(0, 0);
    Quadtree* q1 = Quadtree_create(s1, p1);

    test_rand_off();

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

    printf("Total time for %llu inserts: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);

    Quadtree_uproot(q1);
}

void test_random_1024() {
    test_random_n(1L << 10);
}

void test_random_65536() {
    test_random_n(1L << 16);
}

void test_random_131072() {
    test_random_n(1L << 17);
}

void test_random_262144() {
    test_random_n(1L << 18);
}

void test_random_524288() {
    test_random_n(1L << 19);
}

void test_random_1048576() {
    test_random_n(1L << 20);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    Marsaglia_srand(time(NULL));
    printf("[Beginning tests]\n");
    
    start_test(test_powers_of_two, "Powers of two test");
    start_test(test_random_1024, "Random 1024 test");
    start_test(test_random_65536, "Random 65536 test");
    start_test(test_random_131072, "Random 131072 test");
    start_test(test_random_262144, "Random 262144 test");
    start_test(test_random_524288, "Random 524288 test");
    start_test(test_random_1048576, "Random 1048576 test");

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
