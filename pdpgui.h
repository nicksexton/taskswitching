// definitions for structs used to communicate between callback functions
#ifndef pdpgui_h
#define pdpgui_h

#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>

#include "pdp_objects.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"


typedef struct pdp_simulation {

  pdp_model *model; // head of a possible list of models
  gsl_rng * random_generator;

  // should make params and subject(s) generic
  GsStroopParameters *model_params;


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

  // pointers to files & tree stores for parameter import
  // current implementation - single file treestore for all parameter imports
  FileData * config_file;


  // pointers to label widgets - for updating dashboard information
  GtkWidget * model_headerbar_label_subject;
  GtkWidget * model_headerbar_label_trial;
  GtkWidget * model_headerbar_label_trial_data;


} PdpGuiObjects;

#endif
