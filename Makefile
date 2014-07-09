CC=gcc
#CFLAGS= -fopenmp `pkg-config --cflags glib-2.0` -Wall -Werror -g
#LIBS = `pkg-config --libs glib-2.0` 

CFLAGS= -fopenmp `pkg-config --cflags gtk+-3.0` -Wall -g
LIBS = `pkg-config --libs gtk+-3.0` -lm

OBJECTS = gs_stroop_model.o pdp_objects.o pdp_activation_funcs.o pdp_procedure.o pdp_import.o random_generator_functions.o gs_stroop_subjects.o gs_stroop_analyse.o gs_stroop_import.o 
GUI_OBJECTS = pdpgui_plot.o pdpgui_import.o lib_cairox.o 

BINARIES = gs_stroop_sim_indiffs
GUI_BINARIES = gs_stroop_gui

all: $(BINARIES) $(GUI_BINARIES)
gui: $(GUI_BINARIES)
console: $(BINARIES)


# core objects

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


# pdp_objects: pdp_objects.o 

pdp_objects.o: pdp_activation_funcs.o
	$(CC) -c pdp_objects.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

pdp_procedure.o: pdp_objects.o
	$(CC) -c pdp_procedure.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)

random_generator_functions.o: 
	$(CC) -c random_generator_functions.c $(CFLAGS) -lgsl -lgslcblas -lm 


# gui

gs_stroop_gui: gs_stroop_gui.o $(OBJECTS) $(GUI_OBJECTS) 
	$(CC) -o $@ $(CFLAGS) gs_stroop_gui.o $(OBJECTS) $(GUI_OBJECTS)  -lgsl -lgslcblas -lm $(LIBS) 	
gs_stroop_gui.o:
	$(CC) -c gs_stroop_gui.c $(CFLAGS) -lgsl -lgslcblas -lm $(LIBS)



clean: 
	rm -f gs_stroop *.o *.[ch]~ $(BINARIES) $(GUI_BINARIES)
