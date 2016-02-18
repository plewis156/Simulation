/***************************************************************
  Paul Lewis
  File Name: simulation.c
  Simulation

  A program for running a simulation of arrivals and departures
  with a given number of server nodes for processing various statistics
***************************************************************/

#include "simulation.h"
#include "FIFOqueue.h"
#include "heap.h"

/*
 * Global variables for keeping track of statistics
 */
float totalTime;
float totalServiceTime;
float totalWaitTime;
float idleTime;
int serviceAvailable;
int numberOfCustomers;
int numInQueue;

/* 
 * A program to run a simulation of arrivals and departures
 * with a given number of service nodes
 *
 * @local int i, a counter
 * @local int lambda, the average number of arrivals per time unit
 * @local int mu, the average number of customers to service per time unit
 * @local int m, the number of servers
 * @local int n, total number of arrivals to service
 * @local char line[], the buffer for getting values from file
 * @local int ar[], an array for holding the integer values from the file
 * @local FILE *fp, the file pointer to the file holding the values for lambda, mu, M, and N
 *
 * @return 0 
 */
int main(void) {
    int i, lambda, mu, m, n;
    char line[BUFFER_SIZE];
    int ar[STATS];    
    FILE *fp;
    fp = fopen("simulation.txt", "r");
    if(fp == NULL) {
        perror("Unable to open file\n");
        exit(0);
    }

    for(i=0;fgets(line,sizeof(line),fp)!=NULL;i++)      // get values from file to test 
        ar[i] = atoi(line);

    lambda = ar[0];
    mu = ar[1];
    m = ar[2];
    n = ar[3];
    /* initialize global variables */
    numberOfCustomers = 0;
    totalTime = 0.0;
    totalServiceTime = 0.0;
    totalWaitTime = 0.0;
    numInQueue = 0;
    /* seed random number generator */
    srand(time(0));    
    
    printPreCalc(lambda, mu, m, n);

    runSimulation(lambda, mu, m, n);

    return 0;
}
/*
 * A function for generating a random time interval.
 * 
 * @param float avg, either lambda or mu
 *
 * @local float f, a random float (0..1] 
 * @local float intervalTime, the time average time interval
 *
 * @return float, the average time interval 
 */
float getNextRandomInterval(float avg) {
    float f = rand()/((float)RAND_MAX);
    float intervalTime = -1 * (1.0/avg) * log(f);
    return intervalTime;
}
/*
 * A function for generating a given number of arrivals
 * into a priority queue
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int n, the total number of arrivals
 * @param struct heap *h, the priority queue
 *
 * @local int i, a counter
 * @local float temp, a random interval
 */
void generateArrivals(int lambda, int n, struct heap *h) {
    int i = h->theSize+1;
    float temp;
    while(numberOfCustomers < n && i<HEAPSIZE) {
        temp = getNextRandomInterval((float)lambda);
        totalTime += temp;      // keep track of absolute time
        h->array[i] = newCustomer(totalTime, 1);
        h->theSize++;           // increment size of priority queue
        numberOfCustomers++;    // keep track of number of customers   
        i++;
    }
    buildHeap(h);               // heapify, may be necessary if a departure event currently in priority queue has a lower priority pqTime than one of the added arrivals
}
/* 
 * A function for processing the next event in the priority queue
 * May be an arrival or a departure. May need to put an arrival
 * in a FIFO queue
 *
 * @param struct heap *h, the priority queue
 * @param struct FIFOqueue *q, the FIFO queue
 * @param int mu, the average number of customers to service per time unit
 *
 * @local float temp, a random interval
 * @local float temp2, the difference between start of service time and arrival time
 * @local float idle, used to keep track of amount of idle time
 * @local struct customer *event, the event to process
 * @local struct customer *cust, a customer to process from FIFO queue
 * @local struct customer *check, used to check arrival time of next event in priority queue to keep track of idle time
 */
void processNextEvent(struct heap *h, struct FIFOqueue *q, int mu, int m) {
    float temp, temp2, idle;
    struct customer *event;  
    struct customer *cust;      
    struct customer *check;
    event = deleteMin(h);               // get next event from priority queue
    if(event->departureTime < 0) {      // if arrival
        if(serviceAvailable > 0) {
            serviceAvailable--;
            event->startOfServiceTime = event->arrivalTime;
            temp = getNextRandomInterval((float)mu);
            totalServiceTime += temp;   // keep track of total service time
            event->departureTime = event->arrivalTime + temp;
            event->pqTime = event->departureTime;
            percolateUp(h,event);       // add event back to priority queue as departure event
        } else {
            enqueue(q,event);           // place in FIFO queue
            numInQueue++;               // keep track of number of customers going into FIFO queue
        }
    } else {
        serviceAvailable++;
        if(serviceAvailable == m && getSize(q) == 0) {      // if all servers are available and FIFO is empty
            check = getMin(h);                              // record idle time
            idle = check->arrivalTime - event->departureTime;
            idleTime += idle;
        } 
        if(getSize(q) > 0) {            // check if customer in FIFO queue
            cust = dequeue(q);          // get next customer in FIFO queue
            cust->startOfServiceTime = event->departureTime;
            temp = getNextRandomInterval((float)mu);
            totalServiceTime += temp;   // keep track of total service time
            temp2 = cust->startOfServiceTime - cust->arrivalTime;
            totalWaitTime += temp2;     // keep track of total wait time
            cust->departureTime = cust->startOfServiceTime + temp;
            cust->pqTime = cust->departureTime;
            percolateUp(h,cust);        // add event back to priority queue as departure event
            serviceAvailable--;
        }
        freeCustomer(event);        // free memory of event
    }
}
/*
 * A function to call other functions to run the simulation
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int mu, the average number of customers to service per time unit
 * @param int m, the number of servers
 * @param int n, total number of arrivals to service
 *
 * @local int i, a counter
 * @local struct heap *h, the priority queue
 * @local struct FIFOqueue *q, the FIFO queue
 */
void runSimulation(int lambda, int mu, int m, int n) {
    int i;
    struct heap *h = constructHeap(0, NULL);    // create priority queue
    struct FIFOqueue *q = newQueue();           // create FIFO queue
    generateArrivals(lambda, n, h);             // generate first arrivals
    serviceAvailable = m;
    for(i=0;i<m;i++)                            // process first m events
        processNextEvent(h, q, mu, m);
    while(h->theSize > 0) {
        processNextEvent(h, q, mu, m);          // process events
        if((numberOfCustomers < n) && (h->theSize <= m+1))
            generateArrivals(lambda, n, h);     // add more events to priority queue when necessary
    }
    printPostCalc();        // print a posteriori statistics
    freeHeap(h);            // free memory of priority queue
    freeFIFOqueue(q);       // free memory of FIFO queue
}
/* 
 * A function to calculate Po
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * 
 * @ local int i, j; counters
 * @ local float temp, temp, factorial, mfact, a, b, c, d, e, f;
 *   variables for compartmentalizing the equation for simplification
 * @ local float answer, the value to return
 *
 * @return float, the value for Po
 */
float calculatePo(float lambda, float mu, float m) {
    int i, j;
    float temp, answer, factorial = 1, mfact =1;
    for(i=0;i<=(m-1);i++) {     // calculate value for series (sigma i=M-1, i=0)(1/i!)(lambda/mu)^i
        for(j=1;j<=i;j++) 
            factorial *= (float)j;
        temp += ((pow((lambda/mu),i))/factorial);                         
    }
    
    for(j=1;j<=m;j++)           // calculate value for M!
        mfact *= (float)j;   
    
    float a = 1.0/mfact;        // 1/M!
    float b = lambda/mu;        // lambda/mu
    float c = pow(b,m);         // (lamda/mu)^M
    float d = m*mu;             // M*mu
    float e = d-lambda;         // M*mu-lambda
    float f = d/e;              // (M*mu)/(M*mu-lambda)

    answer = (1.0/(temp+a*c*f));    //(1/((sigma i=M-1, i=0)(1/i!)(lambda/mu)^i)+1/M!(lamda/mu)^M(M*mu/M*mu-lambda)
   
    return answer;
}
/*
 * A function to calculate L
 * 
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * @param float po, the value of Po
 *
 * @local int i, a counter
 * @local float fact, a, b, c, d, e;
 *  variables for compartmentalizing the equation for simplification
 * @local float answer, the value to return
 *
 * @return float, the value for L
 */
float calculateL(float lambda, float mu, float m, float po) {
    int i;
    float fact = 1, answer;
    for(i=1;i<=(m-1);i++)           // calculate value for (M-1)!
        fact *= (float)i;
                           
    float a = lambda*mu;            // lambda*mu
    float b = lambda/mu;            // lambda/mu
    float c = (m*mu)-lambda;        // M*mu-lambda
    float d = pow(b,m);             // (lambda/mu)^M
    float e = pow(c,2);             // (M*mu-lambda)^2
    
    answer = ((a*d)/(fact*e))*po+b; // ((lambda*mu)(lambda/mu)^M)/(M-1)!(M*mu-lambda)^2)*Po+lambda/mu

    return answer;
}
/*
 * A function to calculate W
 * 
 * @param float lambda, the average number of arrivals per time unit
 * @param float l, the value for L
 *
 * @local float answer, the value to return
 *
 * @return float, the value for W
 */
float calculateW(float lambda, float l) {
    float answer;

    answer = l/lambda;  // L/lambda
    
    return answer;
}
/*
 * A function to calculate Lq
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customer to service per time unit
 * @param float l, the value for L
 *
 * @local float answer, the value to return
 *
 * @return float, the value or Lq
 */
float calculateLq(float lambda, float mu, float l) {
    float answer;

    answer = l - (lambda/mu);   // L-(lambda/mu

    return answer;
}
/* 
 * A function to calculate Wq
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float lq, the value for Lq
 *
 * @local float answer, the value to return
 *
 * @return float, the value for Wq
 */
float calculateWq(float lambda, float lq) {
    float answer;

    answer = lq/lambda;     // Lq/lambda

    return answer;
}
/*
 * A function to calculate Rho
 *
 * @param float lambda, the average number of arrivals per time unit
 * @param float mu, the average number of customers to service per time unit
 * @param float m, the number of servers
 * 
 * @local float answer, the value to return
 *
 * @return float, the value for Rho
 */
float calculateRho(float lambda, float mu, float m) {
    float answer;

    answer = lambda/(m*mu);     // lambda/(M*mu)

    return answer;
}
/*
 * A function to calculate and print a priori statistics for the simulation
 *
 * @param int lambda, the average number of arrivals per time unit
 * @param int mu, the average number of customers to service per time unit
 * @param int m, the number of servers
 * @param int n, total number of arrivals to service
 *
 * @local float po, the value for Po
 * @local float l, the value for L
 * @local float w, the value for W
 * @local float lq, the value for Lq
 * @local float wq, the value for Wq
 * @local floar rho, the value for Rho
 */
void printPreCalc(int lambda, int mu, int m, int n) {
    printf("\n");
    printf("lambda = %d\n", lambda);
    printf("mu = %d\n", mu);
    printf("M = %d\n", m);
    printf("\nPrinting a priori calculations...\n\n");

    float po, l, w, lq, wq, rho;
    po = calculatePo((float)lambda,(float)mu,(float)m);
    l = calculateL((float)lambda, (float)mu, (float)m, po);
    w = calculateW((float)lambda, l);
    lq = calculateLq((float)lambda, (float)mu, l);
    wq = calculateWq((float) lambda, lq);
    rho = calculateRho((float)lambda, (float)mu, (float)m);
    printf("Po =  %5.4f\n", po);
    printf("L = %5.4f\n", l);
    printf("W = %5.4f\n", w);
    printf("Lq = %5.4f\n", lq);
    printf("Wq = %5.4f\n", wq);
    printf("Rho = %5.4f\n", rho);

}
/*
 * A function to calculate and print a posteriori statistics for the simulation
 *
 * @local float f1, the new value for Po
 * @local float f2, the new value for W
 * @local float f3, the new value for Wq
 * @local float f4, the probability for having to wait for service
 * @local float f5, the probability for not having to wait for service
 */
void printPostCalc() {
    printf("\nPrinting a posteriori calculations...\n\n");
    float f1, f2, f3, f4, f5;    
    f1 = idleTime/totalTime;
    f2 = (totalWaitTime + totalServiceTime)/(float)numberOfCustomers;
    f3 = totalWaitTime/(float)numberOfCustomers;
    f4 = numInQueue/(float)numberOfCustomers;
    f5 = 1.0 - f4;

    printf("Percentage of idle time (Po) = %5.4f\n", f1);
    printf("Average time spent in system (W) = %5.4f\n", f2);
    printf("Average time spent waiting in queue (Wq) = %5.4f\n", f3);
    printf("Probability of having to wait for service = %5.4f\n", f4);
    printf("Probability of not having to wait for service = %5.4f\n\n", f5);
}
