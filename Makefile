CC=gcc
CFLAGS=-Wall -g

all:  pdp_objects

pdp_objects: pdp_objects.o activation_functions.o 
pdp_objects.o: pdp_objects.c  
activation_functions.o: activation_functions.c

clean: rm -f pdp_objects pdp_objects.o activation_functions.o