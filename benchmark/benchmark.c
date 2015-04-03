/**
Benchmarking suite for 2D data structures
*/

#include "benchmark.h"

typedef struct {
    bool insert, query, delete;
    Point *point;
} OperationPacket;

#ifdef READY_TO_RUN
void test_random_n(const uint64_t num_samples) {
    Marsaglia_srand(num_samples % ((1LL << 32) - 1));

    float64_t s1 = 1 << 16;  // size1; chose to use S instead of L
    Point p1 = Point_init(0, 0);
    TYPE *q1 = CONSTRUCTOR(s1, p1);

    test_rand_off();

    Point *points = (Point*)malloc(sizeof(points) * num_samples);
    uint64_t *time_samples = (uint64_t*)malloc(sizeof(uint64_t) * num_samples);
    OperationPacket *packets = (OperationPacket*)malloc(sizeof(OperationPacket) * num_samples);

    Point *insert_ptr = points, *query_ptr = points, *delete_ptr = points;

    uint64_t total_cycles = 0, insert_cycles = 0, query_cycles = 0, delete_cycles = 0;
    uint64_t insert_count = 0, query_count = 0, delete_count = 0;

    register uint64_t i;

    for (i = 0; i < num_samples; i++) {
        OperationPacket pkt = packets[i];
        pkt.insert = false;
        pkt.query = false;
        pkt.delete = false;
        pkt.point = NULL;

        // write
        if (query_ptr >= insert_ptr || rand() < WRATIO) {
            // delete
            if (delete_ptr < insert_ptr && rand() < DRATIO) {
                pkt.delete = true;
                pkt.point = delete_ptr;
                delete_ptr++;
                delete_count++;
                // move the query pointer to something that hasn't been deleted
                if (query_ptr < delete_ptr)
                    query_ptr = delete_ptr;
            }
            // insert
            else {
                double x = (rand() - 0.5) * s1;
                double y = (rand() - 0.5) * s1;
                *insert_ptr = Point_init(x, y);
                pkt.insert = true;
                pkt.point = insert_ptr;
                insert_ptr++;
                insert_count++;
            }
        }
        // read
        else {
            pkt.query = true;
            pkt.point = query_ptr;
            query_ptr++;
            query_count++;
        }
    }

    #ifdef PARALLEL
    #pragma omp parallel for
    for (i = 0; i < num_samples; i++) {
    #else
    for (i = 0; i < num_samples; i++) {
    #endif
        clock_t start, end;
        OperationPacket *pkt = packets + i;
        if (pkt->insert) {
            start = clock();
            INSERT(q1, *pkt->point);
            end = clock();
        }
        else if (pkt->query) {
            start = clock();
            QUERY(q1, *pkt->point);
            end = clock();
        }
        else if (pkt->delete) {
            start = clock();
            DELETE(q1, *pkt->point);
            end = clock();
        }
        else {
            time_samples[i] = (uint64_t)-1;  // that is, a lot of time
            printf("Invalid kind of operation detected at operation %llu/%llu.\n", (unsigned long long)i, COUNT);
            continue;
        }
        time_samples[i] = ((uint64_t)(end - start));
    }

    for (i = 0; i < num_samples; i++) {
        if (packets[i].insert)
            insert_cycles += time_samples[i];
        else if (packets[i].query)
            query_cycles += time_samples[i];
        else if (packets[i].delete)
            delete_cycles += time_samples[i];
    }
    total_cycles = insert_cycles + query_cycles + delete_cycles;

    #ifdef VERBOSE
    printf("Total time for %llu inserts:    %.8lf s\n", (unsigned long long)insert_count, insert_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total time for %llu querys:     %.8lf s\n", (unsigned long long)query_count, query_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total time for %llu deletes:    %.8lf s\n", (unsigned long long)delete_count, delete_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total time for %llu operations: %.8lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%llu, %.8lf", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    printf(", %llu, %.8lf", (unsigned long long)insert_count, insert_cycles / (float64_t)CLOCKS_PER_SEC);
    printf(", %llu, %.8lf", (unsigned long long)query_count, query_cycles / (float64_t)CLOCKS_PER_SEC);
    printf(", %llu, %.8lf", (unsigned long long)delete_count, delete_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("\n");
    #endif

    DESTRUCTOR(q1);

    #ifdef CLEANUP
    CLEANUP();
    #endif

    free(points);
    free(time_samples);
    free(packets);
}
#endif

void test() {
    test_random_n(COUNT);
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
#ifdef READY_TO_RUN
    #ifdef MTRACE
    mtrace();
    #endif
    Marsaglia_srand(0);
    printf("[Beginning tests]\n");
    char testname[128];
    sprintf(testname, "Random %llu test", COUNT);
    start_test(test, testname);
    printf("\n[Ending tests]\n");
    return 0;
#else
    printf("Need to define at compile time:\n");
    printf("-DCOUNT (# operations, must be a LL)\n");
    printf("-DWRATIO (0.0-1.0 write ratio among read/write ops)\n");
    printf("-DDRATIO (0.0-1.0 delete ratio among writes)\n");
    printf("-DHEADER (the header file, in quotes, e.g. \"./DataType.h\")\n");
    printf("-DTYPE (the datatype name)\n");
    printf("-DCONSTRUCTOR (the constructor function)\n");
    printf("-DINSERT (the insertion function)\n");
    printf("-DQUERY (the querying function)\n");
    printf("-DDELETE (the deletion function)\n");
    printf("-DDESTRUCTOR (the datatype destructor)\n");
    printf("\nOptional:\n");
    printf("-DCLEANUP (the cleanup function, takes no argument)\n");
    printf("-DMTRACE (define to enable mtrace)\n");
    printf("-DPARALLEL (use OpenMP to run in parallel; serial otherwise)\n");
    return 11;
#endif
}
