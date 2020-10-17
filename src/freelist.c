/* freelist.c: Free List Implementation
 *
 * The FreeList is an unordered doubly-linked circular list containing all the
 * available memory allocations (memory that has been previous allocated and
 * can be re-used).
 **/

#include "malloc/counters.h"
#include "malloc/freelist.h"

/* Global Variables */

Block FreeList = {-1, -1, &FreeList, &FreeList};

/* Functions */

/**
 * Search for an existing block in free list with at least the specified size
 * using the first fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_ff(size_t size) {
    // TODO: Implement first fit algorithm

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {

        if (curr->capacity >= size) {
            curr->size = size;
            return  curr;
        }
    }

    return NULL;

}

/**
 * Search for an existing block in free list with at least the specified size
 * using the best fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_bf(size_t size) {
    // TODO: Implement best fit algorithm

    Block *smallest = NULL;

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        
        if (curr->capacity >=  size && !smallest) {
            smallest = curr;
        }

        else if (curr->capacity >= size && curr->capacity < smallest->capacity) {
            smallest = curr;
        }
    }
    
    if(smallest)
        smallest->size = size;

    return  smallest;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the worst fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search_wf(size_t size) {
    // TODO: Implement worst fit algorithm
    Block *largest = NULL;

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {

        if (curr->capacity >= size && !largest) {
            largest = curr;
        }

        else if (curr->capacity >= size && curr->capacity > largest->capacity) {
            largest = curr;
        }
    }

    if(largest)
        largest->size = size;

    return  largest;
}

/**
 * Search for an existing block in free list with at least the specified size.
 *
 * Note, this is a wrapper function that calls one of the three algorithms
 * above based on the compile-time setting.
 *
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block * free_list_search(size_t size) {
    Block * block = NULL;
#if     defined FIT && FIT == 0
    block = free_list_search_ff(size);
#elif   defined FIT && FIT == 1
    block = free_list_search_wf(size);
#elif   defined FIT && FIT == 2
    block = free_list_search_bf(size);
#endif

    if (block) {
        Counters[REUSES]++;
    }
    return block;
}

/**
 * Insert specified block into free list.
 *
 * Scan the free list and attempt to merge specified block into an existing
 * block (or a current block into the specified block, updating the pointers
 * appropriately).
 *
 * If a merge is not possible, then simply add the block to the end of the free
 * list.
 * @param   block   Pointer to block to insert into free list.
 **/
void    free_list_insert(Block *block) {
    // TODO: Implement free list insertion

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        if (block_merge(block, curr)) {

            block->prev = curr->prev;
            block->next = curr->next;

            curr->prev->next = block;
            curr->next->prev = block;

            return;
        }

        if (block_merge(curr, block)) return;
    }

    // Add block to the end of the free list
    Block *tail = FreeList.prev;

    tail->next = block;
    FreeList.prev = block;

    block->next = &FreeList;
    block->prev = tail;
}

/**
 * Return length of free list.
 * @return  Length of the free list.
 **/
size_t  free_list_length() {
    // TODO: Implement free list length

    size_t counter = 0;

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        counter++;
    }

    return counter;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
