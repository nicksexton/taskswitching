// definitions for structs used to communicate between callback functions
#ifndef pdpgui_h
#define pdpgui_h

typedef struct pdp_simulation {

  pdp_model *model; // head of a possible list of models

  gsl_rng * random_generator;

  // should make params and subject(s) generic
  

  subject_popn *subjects;

  // current state of the simulation
  int current_subject;
  int current_trial;

} PdpSimulation;



// wrapper struct for simulation data and pointer to other objects on notepage
// eg to issue redraw events
typedef struct pdpgui_objects {

  PdpSimulation * simulation;
  GtkWidget * model_sub_notepage;

} PdpGuiObjects;

#endif
