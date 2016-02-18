/***************************************************************
  Paul Lewis
  File Name: FIFOqueue.c
  Simulation

  Contains functions for creating, using, and freeing a FIFO queue
***************************************************************/

#include "FIFOqueue.h"

/*
 * A function to allocate and initialize a FIFO queue
 * 
 * @local struct FIFOqueue *q, the new FIFO queue
 *
 * @return struct FIFOqueue *, reference to new FIFO queue
 */
struct FIFOqueue *newQueue() {
    struct FIFOqueue *q = (struct FIFOqueue *) malloc(sizeof(struct FIFOqueue));
    if(q == NULL) {
        perror("malloc error. cannot create queue.\n");
        exit(1);
    }    

    q->size = 0; // intialize size

    return q;
}
/*
 * A function to insert an element into FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 * @param struct customer *c, the element to insert
 */
void enqueue(struct FIFOqueue *q, struct customer *c) {
    if(q->size == 0) {      // if queue is empty, head and tail point to new element
        q->head = c;
        q->tail =  c;
        q->size++;
    } else {                // if queue is not empty, new element points to tail, then tail points to new element
        c->nextCust = q->tail;
        q->tail = c;
        q->size++;
    }
}
/*
 * A function to remove element from queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @local struct customer *tmp, pointer to tail
 * @local struct customer *toServe, the element to return 
 *
 * @return struct customer *, reference to the element which was removed
 */
struct customer *dequeue(struct FIFOqueue *q) {
    struct customer *tmp = q->tail;         // tmp points to tail
    struct customer *toServe = q->head;     // toServe points to head
    if(q->size == 0) {
        printf("Nothing to dequeue.\n");    // if queue is empty return NULL
        return NULL;
    } else if(q->size == 1) {               // if size is 1, decrement size, queue is now empty
        q->size--;
    } else {                                // if size is greater than 1
        while(tmp->nextCust != q->head)     // tmp points to element next to head
            tmp = tmp->nextCust;
        q->head = tmp;                      // head points to tmp
        q->size--;                          // decrement size
    }

    return toServe;         // return element
}
/*
 * A function to get first element without removing
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return struct customer *, reference to element
 */
struct customer *getFirst(struct FIFOqueue *q) {
    return q->head;
}
/*
 * A function to check if FIFO queue is empty
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @return int, boolean
 */
int isEmptyFIFO(struct FIFOqueue *q) {
    if(q->size == 0)
        return 1;
    else
        return 0;    
}
/*
 * A function to return size of FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * return int, the size
 */
int getSize(struct FIFOqueue *q) {
    return q->size;
}
/*
 * A function to free the FIFO queue
 *
 * @param struct FIFOqueue *q, the FIFO queue
 *
 * @local struct customer *tmp, pointer to next element after 3
 *
 * @return struct FIFOqueue *, reference to freed FIFO queue (NULL)
 */
struct FIFOqueue *freeFIFOqueue(struct FIFOqueue *q) {
    if(q->size > 2) {   // if size > 2, move through queue and free all elements
        struct customer *tmp;
        tmp = q->tail->nextCust;
        while(tmp->nextCust != q->head) {        
            free(q->tail);
            q->tail = tmp;
            tmp = tmp->nextCust;
        }
        free(q->head);
        free(q->tail);
        free(tmp);
    } else if(q->size == 2) {   // if size == 2, just free head and tail
        free(q->head);
        free(q->tail);
    } else if(q->size == 1) {   // if size == 1, just free head
        free(q->head);
    }
    free(q);    // free FIFO queue structure
    q = NULL;
    return q;
}
