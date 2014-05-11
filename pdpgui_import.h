#ifndef pdpgui_import_h
#define pdpgui_import_h

#include "pdp_import.h"
#include "pdpgui.h"


// <----------------- GENERAL UTILITY FUNCTIONS

// callback function to read file contents
// gui, model general. wraps pdp_load_from_file_short in pdp_import
gboolean load_from_file_short_cb (GtkWidget *widget, FileData *file_info);

// gui, model general. wraps pdp_load_from_file_long in pdp_import
gboolean load_from_file_long_cb (GtkWidget *widget, FileData *file_info);

// gui, model general. leave in pdpgui_import
void select_file_cb (GtkComboBoxText *widget, FileData * config_file);


// <------------------ MODEL PARAMETER IMPORT FUNCTIONS


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



// <----------------- MODEL TASK IMPORT FUNCTIONS


// temp code - for translating task blocks (ie an array of stroop_trial_data objects) to treestore
// (temporary) utility function, non-gui, model specific
// static void import_task_block_new_to_treestore (GtkTreeStore * store, 
// 						gchar *name,
//						int num_trials, 
//						stroop_trial_data * trial_array);


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
