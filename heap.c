/***************************************************************
  Paul Lewis
  File Name: heap.c
  Simulation

  Contains functions for creating, using, and freeing a priority queue in the form of a heap
***************************************************************/

#include "heap.h"

/*
 * Function to add an element to a priority queue
 *
 * @param struct heap *h, the priority queue
 * @param struct customer *cust, the element to add
 *
 * @local int slot, the slot to check where to insert
 */
void percolateUp(struct heap *h, struct customer *cust) {
    h->array[0] = cust;     // sentinel
    int slot = ++h->theSize;    // increment size
    while(cust->pqTime < h->array[slot/2]->pqTime) {    // search for slot to place customer
        h->array[slot] = h->array[slot/2];
        slot /= 2;
    }   
    h->array[slot] = cust; // place customer
}
/*
 * Function to swap an element with its children by priority order
 *
 * @param struct heap *h, the priority queue
 * @param int slot, the slot to percolate
 *
 * @local int child, the child of the current slot
 * @local customer *tmp, the element to swap in the heap
 */
void percolateDown(struct heap *h, int slot) {
    int child;
    struct customer *tmp = h->array[slot];  // element at slot to percolate down

    while(slot * 2 <= h->theSize) {         // loop to rearrange parent with its children in heap order
        child = slot * 2;                   // smaller items further down the heap are copied up until place for insertion is found
        if(child != h->theSize && h->array[child+1]->pqTime < h->array[child]->pqTime) {
            child++;
        }
        if(h->array[child]->pqTime < tmp->pqTime) {
            h->array[slot] = h->array[child];
        } else {
            break;
        }
        slot = child;   
    }    
    h->array[slot] = tmp;       // place element in correct slot
}
/*
 * A function to reorder a priority queue by priority order
 *
 * @param struct heap *h, the priority queue
 *
 * @local int i, a counter
 */
void buildHeap(struct heap *h) {
    int i;
    for(i=h->theSize/2; i>0; i--) {
        percolateDown(h, i);
    }
}
/*
 * Function to initialize a heap and build a heap if given an array
 * Params should be 0 and NULL if initializing an empty heap
 *
 * @param int initialSize, the size of the array passed to the function
 * @param struct customer *a[], an array of elements to place into new heap
 *
 * @local int i, a counter
 * @local struct heap *h, reference to new heap
 *
 * @return struct heap *, reference to the heap
 */
struct heap *constructHeap(int initialSize, struct customer **a) {
    int i;
    struct heap *h = (struct heap *) malloc (sizeof(struct heap));
    if(h == NULL) {
        perror("malloc failed. cannot construct heap.\n");
        exit(1);
    }
    h->array = malloc(sizeof(struct customer *)*(HEAPSIZE+1));
    if(h->array == NULL) {
        perror("malloc failed. cannot construct heap array.\n");
        exit(1);
    }
    
    /* initialize values 
       for the purposes of this simulation, a[] will be NULL */
    if(a != NULL) {         
        for(i=0;i<initialSize;i++)
            h->array[i+1] = a[i];
        buildHeap(h);
        h->totalSize = HEAPSIZE+1;
        h->theSize = initialSize;
        h->empty = 0;
    } else {
        h->totalSize = HEAPSIZE+1;
        h->theSize = initialSize;
        h->empty = 1;
    }
    
    return h;
}
/*
 * A function to remove element from heap
 *
 * @param struct heap *h, the heap
 *
 * @local struct customer *tmp, the element to remove
 *
 * @return struct customer *, reference to removed element
 */
struct customer *deleteMin(struct heap *h) {
    struct customer *tmp = h->array[1];     // copy the item at top of heap to tmp
    if(h->theSize == 0) {
        printf("Nothing in priority queue.\n");
        return NULL;
    } else {
        h->array[1] = h->array[h->theSize--];   // copy item at end of heap to top of heap and decrement size
        if(h->theSize == 0)
            h->empty = 1;
        percolateDown(h,1); 
    }
    return tmp;
}
/*
 * A function to check if heap is 
 *
 * @param struct heap *h, the heap
 *
 * @return int, boolean
 */
int isEmptyHeap(struct heap *h) {
    return h->empty;
}
/*
 * A function to return a reference to the first element in priority queue
 * without removing it
 *
 * @param struct heap *h, the priority queue
 *
 * @return struct customer *, a reference to the element
 */
struct customer *getMin(struct heap *h) {
    return h->array[1];
}
/*
 * A function free a heap
 * 
 * @param struct heap *h, the heap
 *
 * @local int i, a counter
 *
 * @return struct heap *, the reference to the freed heap (NULL)
 */
struct heap *freeHeap(struct heap *h) {
    int i;
    if(h->theSize != 0) {   // if array has elements, free those elements
        if(h->theSize > 1) {
            for(i=1;i<h->theSize;i++) 
                free(h->array[i]);
        } else {
            free(h->array[1]);
        }
    }
    free(h->array);         // free the array
    free(h);                // free the heap structure
    h=NULL;
    return h;   
}
