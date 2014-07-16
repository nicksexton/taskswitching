#define MAX_FIELDS 10
#define FIELD_SIZE 51
// MAX_LINE_LENGTH should be MAX_FIELDS * FIELD_SIZE
#define MAX_LINE_LENGTH 512

#define FILENAME_MAX_LENGTH 40

#include <stdbool.h>


// tree model for storing parameter names/values
enum {
  COL_PARAMETER_NAME,
  COL_PARAMETER_VALUE,
  N_COLUMNS
};


// tree model for task viewer/importer
enum {
  COL_BLOCK_NAME,
  COL_TASK_ID,
  COL_TASK_PATTERN_1,
  COL_TASK_PATTERN_2,
  COL_TASK_PATTERN_3,
  COL_TASK_PARAM_1,
  COL_TASK_PARAM_2,
  N_TASK_COLUMNS
};

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
