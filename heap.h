/***************************************************************
  Paul Lewis
  File Name: heap.h
  Simulation

  Contains struct definitions, function prototypes, #defines, and #includes for heap.c
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "customer.h"

#ifndef _heap_h
#define _heap_h

/*
 * The size of the priority queue
 */
#define HEAPSIZE 100

/*
 * The priority queue structure
 * 
 * @field int theSize, the current size of the heap
 * @field int totalSize, the size of the array
 * @field int empty, boolean to show if heap is empty
 * @field struct customer **array, an array of customers
 */
struct heap {
    int theSize;
    int totalSize;
    int empty;
    struct customer **array;
};
/*
 * Function to add an element to a priority queue
 *
 * @param struct heap *h, the priority queue
 * @param struct customer *cust, the element to add
 */
void percolateUp(struct heap *h, struct customer *cust);
/*
 * Function swap an element with its children by priority order
 *
 * @param struct heap *h, the priority queue
 * @param int slot, the slot to percolate
 */
void percolateDown(struct heap *h, int slot);
/*
 * A function to reorder a priority queue by priority order
 *
 * @param struct heap *h, the priority queue
 */
void buildHeap(struct heap *h);
/*
 * Function to initialize a heap and build a heap if given an array
 * Params should be 0 and NULL if initializing an empty heap
 *
 * @param int initialSize, the size of the array passed to the function
 * @param struct customer *a[], an array of elements to place into new heap
 *
 * @return struct heap *, reference to the heap
 */
struct heap *constructHeap(int initialSize, struct customer **a);
/*
 * A function to remove element from heap
 *
 * @param struct heap *h, the heap
 *
 * @return struct customer *, reference to removed element
 */
struct customer *deleteMin(struct heap *h);
/*
 * A function to return a reference to the first element in priority queue
 * without removing it
 *
 * @param struct heap *h, the priority queue
 *
 * @return struct customer *, a reference to the element
 */
struct customer *getMin(struct heap *h);
/*
 * A function to check if heap is 
 *
 * @param struct heap *h, the heap
 *
 * @return int, boolean
 */
int isEmptyHeap(struct heap *h);
/*
 * A function free a heap
 * 
 * @param struct heap *h, the heap
 *
 * @return struct heap *, the reference to the freed heap (NULL)
 */
struct heap *freeHeap(struct heap *h);

#endif
