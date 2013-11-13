CC=gcc
CFLAGS=-Wall -W -g -pg -I/usr/include -L/usr/lib/ 

all:  gs_stroop

# gs_stroop: gs_stroop.o pdp_objects.o activation_functions.o
gs_stroop: gs_stroop.o pdp_objects.o activation_funcs.o random_generator_functions.o
	$(CC) $(CFLAGS) gs_stroop.o pdp_objects.o activation_funcs.o random_generator_functions.o -o gs_stroop -lgsl -lgslcblas -lm

pdp_objects: pdp_objects.o 
pdp_objects.o: pdp_objects.c activation_funcs.o
activation_funcs.o: activation_funcs.c
random_generator_functions.o: random_generator_functions.c
	$(CC) $(CFLAGS) -lgsl -lgslcblas -lm -c random_generator_functions.c 

clean: rm -f gs_stroop gs_stroop.o pdp_objects.o activation_funcs.o