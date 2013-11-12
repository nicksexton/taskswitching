CC=gcc
CFLAGS=-Wall -W -g

all:  gs_stroop

# gs_stroop: gs_stroop.o pdp_objects.o activation_functions.o
gs_stroop: gs_stroop.o pdp_objects.o activation_funcs.o

pdp_objects: pdp_objects.o 
pdp_objects.o: pdp_objects.c activation_funcs.o
activation_funcs.o: activation_funcs.c

clean: rm -f gs_stroop gs_stroop.o pdp_objects.o activation_funcs.o