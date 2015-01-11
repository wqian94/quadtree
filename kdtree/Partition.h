/**
Interface for Partition
*/

#ifndef PARTITION_H
#define PARTITION_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./types.h"

#define PARTITION_BLOCK_TYPE uint8_t

/*
 * Partition
 *
 * A simple data structure used for representing the dimension-by-dimension partitions.
 * In particular, this is a bitstring, with bit i representing dimension i, and the value
 * of that bit is 0 if the order for that partition is >=0, and 1 if <0. Thus, by
 * default, a newly-constructed Partition is right-partitioned on every dimension.
 *
 * (This is all-right!)
 *
 * It also keeps track of how many dimensions it represents.
 *
 * The target pointer references the node in the internal representation, created during
 * allocation.
 *
 * Due to the fact that the data is dynamically allocated, shallow copies of a Partition
 * share the same data. Additionally, it needs to be freed by calling
 * Partition_destroy() after use.
 */
typedef struct {
    uint64_t dim;
    void* target;
    PARTITION_BLOCK_TYPE* data;
} Partition;

/*
 * PartitionType
 *
 * PartitionType is to make it easier to interface with the internal representation.
 *
 * LEFT indicates branching down, RIGHT indicates branching up. This will be translated
 * between the actual representation by the functions.
 */
typedef enum {RIGHT, LEFT} PartitionType;

/*
 * Partition_create
 *
 * Allocates enough memory to store data for the given number of dimensions in bitstring
 * form.
 *
 * dimensions - the number of dimensions that this partition needs to represent
 *
 * Returns a Partition struct initialized to 0's for all bits.
 */
Partition Partition_create(uint64_t dimensions);

/*
 * Partition_set
 *
 * Sets the bit at the given position.
 *
 * p - the Partition with the data to modify
 * index - the index of the bit
 * value - the value to set the bit to
 */
void Partition_set(Partition p, uint64_t index, PartitionType value);

/*
 * Partition_get
 * Returns the value of the bit at the given position.
 *
 * p - the Partition with the data to use
 * index - the index of the bit
 */
PartitionType Partition_get(Partition p, uint64_t index);

/*
 * Partition_destroy
 *
 * Frees up the memory used in this partition. This means that all copies of this
 * Partition are now deallocated and should be discarded.
 *
 * P - a Partition with the data to free
 */
void Partition_destroy(Partition p);

/*
 * Partition_purge
 *
 * Frees up all memory used for Partition.
 *
 * NOTE: THIS MEANS THAT ALL PARTITIONS ALLOCATED BEFORE THIS CALL WILL BECOME
 * DEALLOCATED, INVALIDATING THEIR VALUES!
 */
void Partition_purge();

#ifdef KDTREE_TEST
/*
 * Partition_equals
 *
 * Check if two Partition structs represent the same partitioning, including dimensions.
 *
 * a - the first partition
 * b - the second partition
 *
 * Returns true if and only if a and b represent the same partitioning.
 */
bool Partition_equals(Partition a, Partition b);
#endif

#endif
