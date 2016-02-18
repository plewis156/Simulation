CFLAGS = -Wall 
CC = gcc

objects = simulation.o heap.o FIFOqueue.o customer.o 

test: $(objects)
	$(CC) $(CFLAGS) -o simulation $(objects) -lm

simulation.o: simulation.c 
heap.o: heap.c
FIFOqueue.o: FIFOqueue.c
customer.o: customer.c

.PHONY : clean
clean: 
	rm simulation $(objects)
