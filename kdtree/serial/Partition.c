/**
Partition implementation
*/

#include <stdio.h>
#include <string.h>

#include "../Partition.h"

typedef struct partition_list_t {
    PARTITION_BLOCK_TYPE* data;
    struct partition_list_t *prev, *next;
} Partition_list;

static Partition_list* partition_list = NULL;

Partition Partition_create(uint64_t dimensions) {
    register uint64_t size = (dimensions + sizeof(PARTITION_BLOCK_TYPE) - 1) / sizeof(PARTITION_BLOCK_TYPE) * sizeof(PARTITION_BLOCK_TYPE);

    Partition_list* pl = (Partition_list*)malloc(sizeof(Partition_list));
    pl->data = (PARTITION_BLOCK_TYPE*)malloc(size);
    pl->prev = pl;
    pl->next = pl;

    if (partition_list == NULL)
        partition_list = pl;
    else {
        pl->prev = partition_list->prev;
        pl->next = partition_list;
        pl->prev->next = pl;
        pl->next->prev = pl;
    }

    Partition p = {.target = (void*)pl, .dim = dimensions, .data = pl->data};
    memset(p.data, 0, size);

    return p;
}

void Partition_set(Partition p, uint64_t index, PartitionType value) {
    char val = (value == LEFT) ? 1 : 0; // 1 if left, 0 if right
    uint64_t byte_off = index / sizeof(PARTITION_BLOCK_TYPE), bit_off = index % sizeof(PARTITION_BLOCK_TYPE);
    if (val)
        p.data[byte_off] ^= 1 << bit_off;
    else
        p.data[byte_off] &= ~(1 << bit_off);
}

PartitionType Partition_get(Partition p, uint64_t index) {
    uint64_t byte_off = index / sizeof(PARTITION_BLOCK_TYPE), bit_off = index % sizeof(PARTITION_BLOCK_TYPE);
    return ((p.data[byte_off] >> bit_off) & 1) ? LEFT : RIGHT;
}

void Partition_destroy(Partition p) {
    Partition_list* pl = (Partition_list*)p.target;
    free(pl->data);
    free(pl);
}

void Partition_purge() {
    if (partition_list == NULL)
        return;

    partition_list->prev->next = NULL;
    while (partition_list != NULL) {
        Partition_list* pl = partition_list;
        partition_list = partition_list->next;
        free(pl->data);
        free(pl);
    }
}

#ifdef KDTREE_TEST
bool Partition_equals(Partition a, Partition b) {
    if (a.dim != b.dim)
        return false;

    register uint64_t i, size = (a.dim + sizeof(PARTITION_BLOCK_TYPE) - 1) / sizeof(PARTITION_BLOCK_TYPE);
    for (i = 0; i < size; i++) {
        if (a.data[i] != b.data[i])
            return false;
    }

    return true;
}
#endif
