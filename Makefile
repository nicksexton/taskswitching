CC=gcc
CFLAGS= `pkg-config --cflags glib-2.0` -Wall -Werror -g
LIBS = `pkg-config --libs glib-2.0` 

OBJECTS = gs_stroop.o pdp_objects.o activation_funcs.o random_generator_functions.o simulated_subjects.o gs_stroop_subjects.o

all:  gs_stroop

# gs_stroop: gs_stroop.o pdp_objects.o activation_functions.o
gs_stroop: $(OBJECTS) 
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) -lgsl -lgslcblas -lm $(LIBS) 

gs_stroop_subjects.o: simulated_subjects.o 
	$(CC) -c gs_stroop_subjects.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

pdp_objects: pdp_objects.o 

pdp_objects.o: pdp_objects.c activation_funcs.o
# activation_funcs.o: activation_funcs.c

random_generator_functions.o: 
	$(CC) -c random_generator_functions.c $(CFLAGS) -lgsl -lgslcblas -lm 

simulated_subjects.o:
	$(CC) -c simulated_subjects.c $(CFLAGS) $(LIBS)

clean: 
	rm -f gs_stroop *.o *.[ch]~
