// definitions for structs used to communicate between callback functions
#ifndef gs_stroop_gui_h
#define gs_stroop_gui_h

#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>

#include "pdp_objects.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"

// dimensions of main gui widgets
#define MAIN_WINDOW_WIDTH_DEFAULT 800
#define MAIN_WINDOW_HEIGHT_DEFAULT 800
#define TASK_VIEW_WIDTH 750
#define TASK_VIEW_HEIGHT 500


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


// <------------------------ IMPORT FUNCTIONS (transferred from old pdpgui_import)
// callback which reads parameters from treestore and calls the translator function (model_parameter_import)
// gui, model specific. move to gs_stroop_gui or similar. 
// Wraps gs_stroop_parameters_import_commit
// static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);

/*
// just proof of concept for now
// gui, model specific. not sure what this function actually does??
static void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data);


// specifies how the treeview will look  (renderers etc)
// gui, model specific.
static void setup_model_params_treeview (GtkTreeView * tree);

*/


// create the import model params notepage
// gui, specialised to interface of a specific model. should be in gs_stroop_gui.
GtkWidget* create_notepage_import_model_params(PdpGuiObjects * objects);


// gui, model general. move to pdpgui?
void model_reset_trial_markers (PdpSimulation *simulation);


// gui, model specific
// static void setup_task_viewer_treeview (GtkTreeView * tree);


// gui, model specific
GtkWidget* create_notepage_view_trials(PdpGuiObjects * objects);


// non-gui, model specific. wraps gs_stroop_model_task_import_commit
// static void model_task_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);



// gui, model specific
GtkWidget* create_notepage_import_trials (PdpGuiObjects *objects);



#endif
