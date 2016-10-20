/*! \file
 * Implementation of a simple memory allocator.  The allocator manages a small
 * pool of memory, provides memory chunks on request, and reintegrates freed
 * memory back into the pool.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2010.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#define INT_SIZE sizeof(int)    // size of header and footer

#include "myalloc.h"


/*!
 * These variables are used to specify the size and address of the memory pool
 * that the simple allocator works against.  The memory pool is allocated within
 * init_myalloc(), and then myalloc() and free() work against this pool of
 * memory that mem points to.
 */
int MEMORY_SIZE;
unsigned char *mem;
/*!
 * This function initializes both the allocator state, and the memory pool.  It
 * must be called before myalloc() or myfree() will work at all.
 *
 * Note that we allocate the entire memory pool using malloc().  This is so we
 * can create different memory-pool sizes for testing.  Obviously, in a real
 * allocator, this memory pool would either be a fixed memory region, or the
 * allocator would request a memory region from the operating system (see the
 * C standard function sbrk(), for example).
 */
void init_myalloc() {

    /*
     * Allocate the entire memory pool, from which our simple allocator will
     * serve allocation requests.
     */
    mem = (unsigned char *) malloc(MEMORY_SIZE);
    if (mem == 0) {
        fprintf(stderr,
                "init_myalloc: could not get %d bytes from the system\n",
                MEMORY_SIZE);
        abort();
    }
    /* Initialize the initial state of your memory pool here. 
    * Set header and footer and take into account the size of both
    * positive numbers will indicate that the memory is free
    * negative numbers indicate that the memory is allocated */

    // set header and footer for the total memory pool
    *((int *) mem) = MEMORY_SIZE - 2 * INT_SIZE;
    *((int *) (mem + MEMORY_SIZE - INT_SIZE)) = MEMORY_SIZE - 2 * INT_SIZE;
}


/*
 * Attempt to allocate a chunk of memory of "size" bytes.  Return 0 if
 * allocation fails. Allocation uses the best-fit method. This approach
 * is more efficient for memory usage because it looks for the smallest 
 * memory block in which the allocator can place a process. 
 * The downside is that this could leave a tiny segment of memory unallocated
 * just beyond the new process, which is fragmentation. 
 * 
 * In heap implementation, n is the chunks of memory being allocated, so
 * the time complexity of allocation is O(n) because it searches through
 * the memory pool for the minimum fitting memory block. 
 */

unsigned char *myalloc(int size) {
    unsigned char * temp = mem;
    unsigned char * minptr = NULL;
    int min = MEMORY_SIZE; // find the best fit
    int next = 0;
    // check each header until you reach the end of the memory pool
    while (temp < mem + MEMORY_SIZE) {
        // is our current block big enough and
        // is the block smaller than the current minimum sized block
        if (*((int *)temp) >= size && *((int *)temp) < min) { 
            // set new minimum    
            minptr = temp;
            min = *((int *) minptr); 
        }
        // iterate forward to next free block
        next = abs(*((int *) temp)); 
        temp += next + 2 * INT_SIZE; 
    }

    // no free block of memory found
    if (minptr == NULL) {
        // error called
        fprintf(stderr, "myalloc: cannot allocate block of size %d bytes \n", size);
        return (unsigned char *) 0;
    }

    // minimum block of sufficient size has been found -
    int original_size = *((int *)minptr);

    // if our block is big enough to split
    /* A buffer of 70 is placed to deal with fragmentation. The amount of
    * memory being unused is small, so it is not likely any new processes
    * can be loaded here. Widening the condition will make it more likely
    * that free memory blocks can later coaelsce. The memory utilization in
    * the test program increases from .68 to .701 */
    if (*((int *)minptr) > size + 2 * INT_SIZE + 70)
    {
        *((int *)minptr) = -size; // sets head
        // success: set mem to the allocated size
        *((int *) (minptr + size + INT_SIZE)) = -size; // sets tail
        // return a pointer to the payload, which is after the header

        int newSize = original_size - size - 2 * INT_SIZE;
        // set next head
        *((int *) (minptr + size + 2 * INT_SIZE)) = newSize;
        // set new tail
        *((int *) (minptr + original_size + INT_SIZE)) = newSize;

    // not big enough to split, negate header/footer
    } else { 
        *((int *) minptr) = -original_size; // negate head
        *((int *) (minptr + original_size + INT_SIZE)) = -original_size;   
    }
    // pointer to the payload
    return minptr + INT_SIZE; 
}


/*
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc(). When a block is freed, it is coalesced with adjacent free blocks
 * in constant time (Constant-time deallocation). The states of the adjacent 
 * blocks are checked in the footer or header, and the block sizes are 
 * updated if any of the adjacent blocks are free. The time complexity for 
 * coalescing free memory blocks is O(1).
 * The time complexity for deallocation is O(1) because the status of the
 * memory block is updated and then coalescing is checked, both of which are in
 * constant time.
 */
void myfree(unsigned char *oldptr) {
    // the address returned in myalloc() was at the payload 
    // so shift to the header and free the memory
    oldptr -= INT_SIZE; 
    int oldptr_size = *((int *) oldptr);
    *((int *) oldptr) = abs(oldptr_size); 
    // free the memory (tail)
    *((int *)( oldptr + abs(oldptr_size) + INT_SIZE)) = abs(oldptr_size); 
    // update old pointer size
    oldptr_size = *((int *) oldptr); 

    /* Coalesce Memory -
    * check previous allocated chunk of memory by finding the size
    * in the previous footer 
    * check if tail
    */
    if (oldptr + oldptr_size + INT_SIZE < (mem + MEMORY_SIZE - INT_SIZE)) 
    {
        /* check the next allocated chunk of memory by moving forward by
        * the newly freed memory size */
        unsigned char * nextHead = oldptr + oldptr_size + 2 * INT_SIZE;
        int next_size = *((int *) nextHead);
        // if the memory is free, coalesce
        if (next_size > 0) { 
            oldptr_size += next_size + 2 * INT_SIZE;
            // update head and tail
            *((int *) oldptr) = oldptr_size; 
            *((int *) (oldptr + oldptr_size + INT_SIZE)) = oldptr_size; 
        }
    }

    // there is no previous memory block if current ptr is at head
    if (oldptr > mem) 
    {
        // get size in prev footer
        int prev_size = *((int *) (oldptr - INT_SIZE)); 
        unsigned char * prevhead = NULL;
        // is free memory chunk
        if (prev_size > 0) { 
            prevhead = oldptr - 2 * INT_SIZE - prev_size;
            // update the size in the previous pointer
            prev_size += oldptr_size + 2 * INT_SIZE;
            // update head and tail
            (*(int *) prevhead) = prev_size; 
            (*(int *) (prevhead + prev_size + INT_SIZE)) = prev_size; 
        }
    }
}
