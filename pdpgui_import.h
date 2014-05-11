#ifndef pdpgui_import_h
#define pdpgui_import_h

#include "pdp_import.h"
#include "gs_stroop_gui.h" // TEMPORARY!!


// <----------------- GENERAL UTILITY FUNCTIONS

// callback function to read file contents
// gui, model general. wraps pdp_load_from_file_short in pdp_import
gboolean load_from_file_short_cb (GtkWidget *widget, FileData *file_info);

// gui, model general. wraps pdp_load_from_file_long in pdp_import
gboolean load_from_file_long_cb (GtkWidget *widget, FileData *file_info);

// gui, model general. leave in pdpgui_import
void select_file_cb (GtkComboBoxText *widget, FileData * config_file);




// <----------------- MODEL TASK IMPORT FUNCTIONS


// temp code - for translating task blocks (ie an array of stroop_trial_data objects) to treestore
// (temporary) utility function, non-gui, model specific
// static void import_task_block_new_to_treestore (GtkTreeStore * store, 
// 						gchar *name,
//						int num_trials, 
//						stroop_trial_data * trial_array);







#endif
