// definitions for structs used to communicate between callback functions
#ifndef pdpgui_h
#define pdpgui_h

#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>

#include "pdp_objects.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"

// dimensions of main gui widgets
#define MAIN_WINDOW_WIDTH_DEFAULT 800
#define MAIN_WINDOW_HEIGHT_DEFAULT 800
#define TASK_VIEW_WIDTH 750
#define TASK_VIEW_HEIGHT 750


typedef struct pdp_simulation {

  pdp_model *model; // head of a possible list of models
  gsl_rng * random_generator;

  // should make params and subject(s) generic
  GsStroopParameters *model_params;


  subject_popn *subjects;

  // current state of the simulation
  int current_subject;
  int current_trial;


  // task store
  // In new version, move toward using the task_store as a repository for trial info
  // subjects becomes a repo for data only (and can maybe be stored in text file?)
  GtkTreeStore * task_store;
  GtkTreePath * current_trial_path;
  GtkTreeIter * current_trial_iter;

  stroop_trial_data * current_trial_data; 


} PdpSimulation;



// wrapper struct for simulation data and pointer to other objects on notepage
// eg to issue redraw events
typedef struct pdpgui_objects {


  // GUI objects that we want to refer to 
  PdpSimulation * simulation;
  GtkWidget * model_sub_notepage;


  // pointers to files & tree stores for parameter import
  // current implementation - single file treestore for all parameter imports
  FileData * config_file;
  FileData * task_config_file;



  // pointers to label widgets - for updating dashboard information
  GtkWidget * model_headerbar_label_subject;
  GtkWidget * model_headerbar_label_trial;
  GtkWidget * model_headerbar_label_trial_data;
  GtkWidget * model_headerbar_spin_trial;

} PdpGuiObjects;

#endif
