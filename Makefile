CC=gcc
CFLAGS= -fopenmp `pkg-config --cflags glib-2.0` -Wall -Werror -g
LIBS = `pkg-config --libs glib-2.0` 

OBJECTS = gs_stroop_model.o pdp_objects.o pdp_activation_funcs.o random_generator_functions.o gs_stroop_subjects.o gs_stroop_analyse.o

all:  gs_stroop_sim_indiffs


gs_stroop_sim_indiffs: gs_stroop_sim_indiffs.o $(OBJECTS) 
	$(CC) -o $@ -D_THREADED_ $(CFLAGS) $(OBJECTS) gs_stroop_sim_indiffs.o -lgsl -lgslcblas -lm $(LIBS) 

gs_stroop_sim_indiffs.o:
	$(CC) -c gs_stroop_sim_indiffs.c -D_THREADED_ $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

gs_stroop_model.o:
	$(CC) -c gs_stroop_model.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

gs_stroop_subjects.o: 
	$(CC) -c gs_stroop_subjects.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

gs_stroop_analyse.o:
	$(CC) -c gs_stroop_analyse.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)


pdp_objects: pdp_objects.o 

pdp_objects.o: pdp_objects.c pdp_activation_funcs.o


random_generator_functions.o: 
	$(CC) -c random_generator_functions.c $(CFLAGS) -lgsl -lgslcblas -lm 


clean: 
	rm -f gs_stroop *.o *.[ch]~
