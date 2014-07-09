// Functions and objects for setting up and running a pdp simulation
// ie. presenting a series of trials to a pdp model
// All functions should be Non-gui, model general

#ifndef pdp_procedure_h
#define pdp_procedure_h

#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>

#include "pdp_objects.h"


// <---------------------------- TEMPORARY -------------------------
#include "gs_stroop_subjects.h"

// -----------------------------------------------------------------

typedef struct pdp_simulation {

  pdp_model *model; // head of a possible list of models
  gsl_rng * random_generator;

  // should make params and subject(s) generic
  // GsStroopParameters *model_params; // defined in gs_stroop.h. OLD, deprecated

  GHashTable *model_params_htable; // NEW way to store global params

  // current state of the simulation
  int current_subject;
  int current_trial;

  subject_popn *subjects; // defined in gs_stroop_subjects.h


  // task store
  // In new version, move toward using the task_store as a repository for trial info
  // subjects becomes a repo for data only (and can maybe be stored in text file?)
  GtkTreeStore * task_store;
  GtkTreePath * current_trial_path;
  GtkTreeIter * current_trial_iter;

  stroop_trial_data * current_trial_data;  // defined in gs_stroop_subjects



} PdpSimulation;


// non-gui, model-general, needs modification
gint model_current_trial_get (PdpSimulation *simulation);

// non-gui, model-general, needs modification
gboolean model_current_trial_is_last (PdpSimulation *simulation);

// allocates memory for simulation, inits subject population, runs constructors
// non-gui, model specific
PdpSimulation * create_simulation ();

// frees memory for simulation
// non-gui, model specific
void free_simulation (PdpSimulation * simulation);


#endif
