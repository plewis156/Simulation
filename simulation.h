/***************************************************************
  Paul Lewis
  File Name: simulation.h
  Simulation

  Contains function prototypes, #defines, #includes for simulation.c
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include "customer.h"
#include "heap.h"
#include "FIFOqueue.h"

#ifndef _simulation_h
#define _simulation_h

/*
 * The number of statistics to use in calculation
 */
#define STATS 4
/*
 * The size of buffer to hold values of stats from value
 */
#define BUFFER_SIZE 10

/*
 * A function for generating a random time interval.
 * 
 * @param float avg, either lambda or mu
 *
 * @return float, the average time interval 
 */
float getNextRandomInterval(float avg);
/*
 * A function for generating a given number of arrivals
 * into a priority queue
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int n, the total number of arrivals
 * @param struct heap *h, the priority queue
 */
void generateArrivals(int lambda, int n, struct heap *h);
/* 
 * A function for processing the next event in the priority queue
 * May be an arrival or a departure. May need to put an arrival
 * in a FIFO queue
 *
 * @param struct heap *h, the priority queue
 * @param struct FIFOqueue *q, the FIFO queue
 * @param int mu, the average number of customers to service per time unit
 * @param int m, the number of servers
 */
void processNextEvent(struct heap *h, struct FIFOqueue *q, int mu, int m);
/*
 * A function to call other functions to run the simulation
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int mu, the average number of customers to service per time unit
 * @param int m, the number of servers
 * @param int n, total number of arrivals to service
 */
void runSimulation(int lambda, int mu, int m, int n);
/* 
 * A function to calculate Po
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * 
 * @return float, the value for Po
 */
float calculatePo(float lambda, float mu, float m);
/*
 * A function to calculate L
 * 
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * @param float po, the value of Po
 *
 * @return float, the value for L
 */
float calculateL(float lambda, float mu, float m, float po);
/*
 * A function to calculate W
 * 
 * @param float lambda, the average number of arrivals per time unit
 * @param float l, the value for L
 *
 * @return float, the value for W
 */
float calculateW(float lambda, float l);
/*
 * A function to calculate Lq
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customer to service per time unit
 * @param float l, the value for L
 *
 * @return float, the value or Lq
 */
float calculateLq(float lambda, float mu, float l);
/* 
 * A function to calculate Wq
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float lq, the value for Lq
 *
 * @return float, the value for Wq
 */
float calculateWq(float lambda, float lq);
/*
 * A function to calculate Rho
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * 
 * @return float, the value for Rho
 */
float calculateRho(float lambda, float mu, float m);
/*
 * A function to calculate and print a priori statistics for the simulation
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int mu, the average number of customers to service per time unit
 * @param int m, the number of servers
 * @param int n, total number of arrivals to service
 */
void printPreCalc(int lambda, int mu, int m, int n);
/*
 * A function to calculate and print a posteriori statistics for the simulation
 */
void printPostCalc();

#endif
