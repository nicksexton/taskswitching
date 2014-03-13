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

#endif
