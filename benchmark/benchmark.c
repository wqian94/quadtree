/**
Benchmarking suite for 2D data structures
*/


#include "benchmark.h"

#ifndef max
#define max(x, y) (y ^ ((x ^ y) & -(x > y)))
#endif

typedef struct {
    bool insert, query, delete;
    Point *point;
} OperationPacket;

#ifdef READY_TO_RUN
uint64_t execute(TYPE *root, OperationPacket *pkt, uint64_t i) {
    clock_t start, end;
    if (pkt->insert) {
        start = clock();
        INSERT(root, *pkt->point);
        end = clock();
    }
    else if (pkt->query) {
        start = clock();
        QUERY(root, *pkt->point);
        end = clock();
    }
    else if (pkt->delete) {
        start = clock();
        DELETE(root, *pkt->point);
        end = clock();
    }
    else {
        printf("Invalid kind of operation detected at operation %llu/%llu.\n", (unsigned long long)i, COUNT);
        return (uint64_t)-1;  // that is, a lot of time
    } 

    return (uint64_t)(end - start);
}

void test_random_n(const uint64_t num_samples) {
    Marsaglia_srand(num_samples % ((1LL << 32) - 1));
    pthread_mutex_attr_init();

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
        OperationPacket *pkt= packets + i ;
        pkt->insert = false;
        pkt->query = false;
        pkt->delete = false;
        pkt->point = NULL;

        // write
        if (query_ptr >= insert_ptr || random() < WRATIO) {
            // delete
            if (delete_ptr < insert_ptr && random() < DRATIO) {
                pkt->delete = true;
                pkt->point = delete_ptr;
                delete_ptr++;
                delete_count++;
            }
            // insert
            else {
                double x, y;
                x = (random() - 0.5) * s1;
                y = (random() - 0.5) * s1;
                *insert_ptr = Point_init(x, y);
                pkt->insert = true;
                pkt->point = insert_ptr;
                insert_ptr++;
                insert_count++;
            }
        }
        // read
        else {
            query_ptr = delete_ptr;
            while (query_ptr + 1 < insert_ptr)
                if (random() < 0.1)
                    break;
                else
                    query_ptr++;
            pkt->query = true;
            pkt->point = query_ptr;
            query_count++;
        }
    }
    #ifdef VERBOSE
    printf("Prepared:\n    %10llu inserts\n    %10llu queries\n    %10llu deletes\n",
        (unsigned long long)insert_count, (unsigned long long)query_count, (unsigned long long)delete_count);
    #endif

    uint64_t start_time = clock();
    #ifdef PARALLEL
        #ifdef VERBOSE
        printf("Parallel\n");
        #endif
    #ifdef OMP_NTHREADS
    omp_set_num_threads(OMP_NTHREADS);
        #ifdef VERBOSE
        printf("Parallel %llu threads\n", (unsigned long long)OMP_NTHREADS);
        #endif
    #endif
    #pragma omp parallel for
    for (i = 0; i < num_samples; i++) {
    #else
        #ifdef VERBOSE
        printf("Serial\n");
        #endif
    for (i = 0; i < num_samples; i++) {
        //printf("%llu\n", (unsigned long long)i);
    #endif
        clock_t start, end;
        OperationPacket *pkt = packets + i;
        time_samples[i] = execute(q1, pkt, i);
    }
    uint64_t end_time = clock();

    uint64_t insert_slowest = 0, query_slowest = 0, delete_slowest = 0, slowest = 0;
    for (i = 0; i < num_samples; i++) {
        if (packets[i].insert) {
            insert_cycles += time_samples[i];
            insert_slowest = max(insert_slowest, time_samples[i]);
        }
        else if (packets[i].query) {
            query_cycles += time_samples[i];
            query_slowest = max(query_slowest, time_samples[i]);
        }
        else if (packets[i].delete) {
            delete_cycles += time_samples[i];
            delete_slowest = max(delete_slowest, time_samples[i]);
        }
        slowest = max(slowest, time_samples[i]);
    }
    total_cycles = insert_cycles + query_cycles + delete_cycles;

    uint64_t overall_cycles = end_time - start_time;

    #ifdef VERBOSE
    printf("Real time for %10llu operations:   %12.4lf s\n", (unsigned long long)num_samples, overall_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total work for %10llu inserts:     %12.4lf s\n", (unsigned long long)insert_count, insert_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total work for %10llu queries:     %12.4lf s\n", (unsigned long long)query_count, query_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total work for %10llu deletes:     %12.4lf s\n", (unsigned long long)delete_count, delete_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Total work for %10llu operations:  %12.4lf s\n", (unsigned long long)num_samples, total_cycles / (float64_t)CLOCKS_PER_SEC);
    printf("Span of %17llu inserts:     %12.4lf s\n", (unsigned long long)insert_count, insert_slowest / (float64_t)CLOCKS_PER_SEC);
    printf("Span of %17llu queries:     %12.4lf s\n", (unsigned long long)query_count, query_slowest / (float64_t)CLOCKS_PER_SEC);
    printf("Span of %17llu deletes:     %12.4lf s\n", (unsigned long long)delete_count, delete_slowest / (float64_t)CLOCKS_PER_SEC);
    printf("Span of %17llu operations:  %12.4lf s\n", (unsigned long long)num_samples, slowest / (float64_t)CLOCKS_PER_SEC);
    #else
    printf("%llu, %llu, %llu, %llu", (unsigned long long)num_samples, (unsigned long long)CLOCKS_PER_SEC,
        (unsigned long long)overall_cycles, (unsigned long long)total_cycles);
    printf(", %llu, %llu", (unsigned long long)insert_count, (unsigned long long)insert_cycles);
    printf(", %llu, %llu", (unsigned long long)query_count, (unsigned long long)query_cycles);
    printf(", %llu, %llu", (unsigned long long)delete_count, (unsigned long long)delete_cycles);
    printf(", %llu, %llu", (unsigned long long)num_samples, (unsigned long long)slowest);
    printf(", %llu, %llu", (unsigned long long)insert_count, (unsigned long long)insert_slowest);
    printf(", %llu, %llu", (unsigned long long)query_count, (unsigned long long)query_slowest);
    printf(", %llu, %llu", (unsigned long long)delete_count, (unsigned long long)delete_slowest);
    printf("\n");
    #endif

    DESTRUCTOR(q1);

    #ifdef CLEANUP
    CLEANUP();
    #endif

    free(points);
    free(time_samples);
    free(packets);
    pthread_mutex_attr_destroy();
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

    #ifdef VERBOSE
    printf("[Beginning tests]\n");
    char testname[128];
    sprintf(testname, "Random %llu test", COUNT);
    start_test(test, testname);
    printf("\n[Ending tests]\n");
    #else
    test();
    #endif

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
