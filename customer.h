/***************************************************************
  Paul Lewis
  File Name: customer.h
  Simulation

  Contains struct definitions, function prototypes, and #includes for customer.c
***************************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifndef _customer_h
#define _customer_h

/*
 * A structure for a customer
 *
 * @field float arrivalTime, the time of arrival
 * @field float startOfServiceTime, the time the customer is served
 * @field float departureTime, the time of departure
 * @field float pqTime, equal to either arrival or departure time
 *  used for comparison in functions
 * @field struct customer *nextCust, pointer to next customer
 *  used for FIFO queue
 */
struct customer {
    float arrivalTime;
    float startOfServiceTime;
    float departureTime;
    float pqTime;
    struct customer *nextCust;
};

/*
 * A function to create allocate and initialize a new structure
 *
 * @param float time, the absolute time
 * @param int arrbool, boolean to signify if arrival or departure
 *
 * @return struct customer *, reference to the customer
 */ 
struct customer *newCustomer(float time, int arrbool);
/*
 * A function to free a customer
 *
 * @param struct customer *c, the customer to free
 * 
 * @return struct customer *, reference to the freed customer (NULL) 
 */
struct customer *freeCustomer(struct customer *c);

#endif
