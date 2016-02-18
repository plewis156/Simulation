# Simulation
A project to explore simulations, modeling, event-driven programming, time intervals, a priori and a posteriori statistics, and the usefulness of priority queues and the heap data structure.

This is a project meant to test the throughput for a hypothetical service center with a varying number of service channels. The simulation uses a priority queue to simulate arrivals and departures and a FIFO queue to simulate wait times for service. It uses time intervals to simulate arrivals in a continuous probability function.

It requires initial parameters: lambda, the number of arrivals per time interval (e.g. 2); mu, the number of arrivals which can be serviced per time interval (e.g. 3); M, the number of service channels (e.g. 4); and N, the number of total arrivals for the simulation to process (e.g. 5000).

The program seeks to find statistics based on these parameters. These statistics are: Po, the percentage of idle time (the amount of time that no customer is being served and no customer is waiting in the FIFO queue); L, the average number of customers in system (the number being served and the number of customers in FIFO queue); W, the average time a customer spends in the system (time spent in FIFO queue and time spent being served); Lq, the number of customers waiting in FIFO queue; Wq, the average time a customer spends waiting in the FIFO queue; rho, the utilization factor of the system (the proportion of the system's resources that is being used by arrivals).

The program first prints a priori statistics based on lambda, mu, and M. It then runs the simulation and prints a posteriori statistics based on data gained from the simulation.

An example run:

lambda = 2 mu = 3 M = 2 N = 5000

Printing a priori calculations...

Po = 0.5000 L = 0.7500 W = 0.3750 Lq = 0.0833 Wq = 0.0417 Rho = 0.3333

Printing a posteriori calculations...

Percentage of idle time (Po) = 0.4916 Average time spent in system (W) = 0.3737 Average time spent waiting in queue (Wq) = 0.0420 Probability of having to wait for service = 0.1698 Probability of not having to wait for service = 0.8302
