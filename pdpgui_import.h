#ifndef pdpgui_import_h
#define pdpgui_import_h

#include "pdp_import.h"
#include "pdpgui.h"




// <----------------- GENERAL UTILITY FUNCTIONS

// callback function to read file contents
// gui, model general. re-write as wrapper function around a non-gui version?
gboolean load_from_file_short_cb (GtkWidget *widget, FileData *file_info);

// non-gui model-general function which parses a file into a treestore. 
// wrapped by load_from_file_cb 
gboolean pdp_load_from_file_short (FileData *file_info);

// gui, model general. re-write as wrapper function around a non-gui version?
gboolean load_from_file_long_cb (GtkWidget *widget, FileData *file_info);



// gui, model general. leave in pdpgui_import
void select_file (GtkComboBoxText *widget, FileData * config_file);



// <------------------ MODEL PARAMETER IMPORT FUNCTIONS

/* HEADERS FOR STATIC FUNCTIONS

// Translator function which specifies how to set model parameters from treestore data
// non-gui, model specific. Move to gs_stroop_import or similar
static bool model_parameter_import (gchar* param_name, gchar* param_value, GsStroopParameters *model_params);

// callback which reads parameters from treestore and calls the translator function (model_parameter_import)
// gui, model general. re-write as wrapper function around non-gui version.
static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);

// just proof of concept for now
// gui, model general. not sure what this function actually does??
static void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data);



// specifies how the treeview will look  (renderers etc)
// gui, model general.
static void setup_model_params_treeview (GtkTreeView * tree);

*/




// create the import model params notepage
// gui, specialised to interface of a specific model. should be in gs_stroop_gui.
GtkWidget* create_notepage_import_model_params(PdpGuiObjects * objects);



// <----------------- MODEL TASK IMPORT FUNCTIONS


// non-gui, model specific. 
// static void import_stroop_trial_data_to_treestore (GtkTreeStore * store, 
//						   GtkTreeIter *iter, 
//						   stroop_trial_data* data);


// temp code - for translating task blocks (ie an array of stroop_trial_data objects) to treestore
// (temporary) utility function, non-gui, model specific
// static void import_task_block_new_to_treestore (GtkTreeStore * store, 
// 						gchar *name,
//						int num_trials, 
//						stroop_trial_data * trial_array);


// gui, model general
void model_reset_trial_markers (PdpSimulation *simulation);



// gui, model specific
// static void setup_task_viewer_treeview (GtkTreeView * tree);


// non-gui, model specific
FileData * create_task_import_objects();

// gui, model specific
GtkWidget* create_notepage_view_trials(PdpGuiObjects * objects);


// non-gui, model specific. re-write as wrapper function.
// static void model_task_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);


// gui, model specific
GtkWidget* create_notepage_import_trials (PdpGuiObjects *objects);


#endif
