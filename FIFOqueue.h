/***************************************************************
  Paul Lewis
  File Name: FIFOqueue.h
  Simulation

  Contains struct definitions, function prototypes, and #includes for FIFOqueue.c
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "customer.h"

#ifndef _FIFOqueue_h
#define _FIFOqueue_h

/*
 * The FIFO queue structure
 * 
 * @field int size, the size of the queue
 * @field struct customer *head, pointer to element in front of queue
 * @field struct customer *tail, pointer to element in back of queue
 */
struct FIFOqueue {
    int size;
    struct customer *head;
    struct customer *tail;
};

/*
 * A function to allocate and initialize a FIFO queue
 *
 * @return struct FIFOqueue *, reference to new FIFO queue
 */
struct FIFOqueue *newQueue();
/*
 * A function to insert an element into FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 * @param struct customer *c, the element to insert
 */
void enqueue(struct FIFOqueue *q, struct customer *c);
/*
 * A function to remove element from queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return struct customer *, reference to the element which was removed
 */
struct customer *dequeue(struct FIFOqueue *q);
/*
 * A function to get first element without removing
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return struct customer *, reference to element
 */
struct customer *getFirst(struct FIFOqueue *q);
/*
 * A function to check if FIFO queue is empty
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return int, boolean
 */
int isEmptyFIFO(struct FIFOqueue *q);
/*
 * A function to return size of FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * return int, the size
 */
int getSize(struct FIFOqueue *q);
/*
 * A function to free the FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return struct FIFOqueue *, reference to freed FIFO queue (NULL)
 */
struct FIFOqueue *freeFIFOqueue(struct FIFOqueue *q);

#endif
