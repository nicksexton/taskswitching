CC=gcc
CFLAGS= `pkg-config --cflags glib-2.0` -Wall -Werror -g
LIBS = `pkg-config --libs glib-2.0` 

OBJECTS = gs_stroop.o pdp_objects.o activation_funcs.o random_generator_functions.o simulated_subjects.o

all:  gs_stroop

# gs_stroop: gs_stroop.o pdp_objects.o activation_functions.o
gs_stroop: $(OBJECTS) 
<<<<<<< HEAD
=======
#	$(CC) -o $@ $(CFLAGS) -lgsl -lgslcblas -lm $(OBJECTS) $(LIBS) 
>>>>>>> a24a4c2952a6773ceb11192f0f663718081fbbb8
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) -lgsl -lgslcblas -lm $(LIBS) 

pdp_objects: pdp_objects.o 

pdp_objects.o: pdp_objects.c activation_funcs.o
# activation_funcs.o: activation_funcs.c

random_generator_functions.o: 
<<<<<<< HEAD
	$(CC) -c random_generator_functions.c $(CFLAGS) -lgsl -lgslcblas -lm 
=======
	$(CC) -o $@ -lgsl -lgslcblas -lm 
>>>>>>> a24a4c2952a6773ceb11192f0f663718081fbbb8

simulated_subjects.o:
	$(CC) -c simulated_subjects.c $(CFLAGS) $(LIBS)

clean: 
	rm -f gs_stroop *.o *.[ch]~
