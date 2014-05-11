#ifndef pdpgui_import_h
#define pdpgui_import_h

#include "pdp_import.h"
#include "pdpgui.h"




// <----------------- GENERAL UTILITY FUNCTIONS

// utility function for clearing all entries from a treeview
gboolean treestore_remove_all (GtkTreeStore * tree_store);

// callback function to read file contents
gboolean load_from_file_cb (GtkWidget *widget, FileData *file_info);

void select_file (GtkComboBoxText *widget, FileData * config_file);



// <------------------ MODEL PARAMETER IMPORT FUNCTIONS

/* HEADERS FOR STATIC FUNCTIONS

// Translator function which specifies how to set model parameters from treestore data
static bool model_parameter_import (gchar* param_name, gchar* param_value, GsStroopParameters *model_params);

// callback which reads parameters from treestore and calls the translator function (model_parameter_import)
static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);

// just proof of concept for now
static void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data);


// specifies how the treeview will look  (renderers etc)
static void setup_model_params_treeview (GtkTreeView * tree);

*/



// create a FileData object containing file pointer, filename, associated treestore, etc.
FileData * create_param_import_objects();

// create the import model params notepage
GtkWidget* create_notepage_import_model_params(PdpGuiObjects * objects);



// <----------------- MODEL TASK IMPORT FUNCTIONS



void model_reset_trial_markers (PdpSimulation *simulation);

/*
static void import_stroop_trial_data_to_treestore (GtkTreeStore * store, stroop_trial_data* data);



static void import_task_block_to_treestore (GtkTreeStore * store, 
					    int num_trials, 
					    stroop_trial_data * trial_array);

static void setup_task_viewer_treeview (GtkTreeView * tree);

*/

FileData * create_task_import_objects();

GtkWidget* create_notepage_view_trials(PdpGuiObjects * objects);

GtkWidget* create_notepage_import_trials (PdpGuiObjects *objects);




#endif
