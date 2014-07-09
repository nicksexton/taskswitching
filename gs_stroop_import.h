#include "pdp_import.h"

// Imports model parameters - specific 'translator' function
bool model_parameter_import (gchar* param_name, gchar* param_value, GsStroopParameters *model_params);



// non-gui, model-specific function. move to gs_stroop_import
void gs_stroop_parameters_import_commit (FileData *config_file, 
					 GHashTable *model_params);

// non-gui, model specific.  Imports to a buffer
void import_stroop_trial_data_to_treestore (GtkTreeStore * store, 
					   GtkTreeIter *iter, 
					   stroop_trial_data* data);


void gs_stroop_model_task_import_commit (FileData *task_config_file, 
					 GtkTreeStore *task_store);


// non-gui, model specific
// use task_store as a buffer for importing tasks (task blocks)
FileData * create_task_import_objects();
