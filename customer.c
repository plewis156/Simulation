/***************************************************************
  Paul Lewis
  File Name: customer.c
  Simulation

  Contains functions for creating and freeing a customer node
***************************************************************/

#include "customer.h"

/*
 * A function to create allocate and initialize a new structure
 *
 * @param float time, the absolute time
 * @param int arrbool, boolean to signify if arrival or departure
 *
 * @local struct customer *c, pointer to new customer
 *
 * @return struct customer *, reference to the customer
 */ 
struct customer *newCustomer(float time, int arrbool) {
    struct customer *c = (struct customer *) malloc (sizeof(struct customer));
    if(c == NULL) {
        perror("malloc error. cannot create customer.\n");
        exit(1);
    }

    /* initialize variables
       for the purposes of this simulation, arrbool will always be true */
    if(arrbool) {
        c->arrivalTime = time;
        c->startOfServiceTime = 0.0;
        c->departureTime = -1.0;
        c->pqTime = time;
    } else {
        c->arrivalTime = -1.0;
        c->startOfServiceTime = 0.0;
        c->departureTime = time;
        c->pqTime = time;
    }
    return c;
}
/*
 * A function to free a customer
 *
 * @param struct customer *c, the customer to free
 * 
 * @return struct customer *, reference to the freed customer (NULL) 
 */
struct customer *freeCustomer(struct customer *c) {
    free(c);
    c = NULL;
    return c;
}
