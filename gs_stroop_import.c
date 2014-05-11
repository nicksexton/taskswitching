/* Non-gui functions for importing parameters and tasks to gs stroop model */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "pdp_import.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"


bool model_parameter_import (gchar* param_name, gchar* param_value, GsStroopParameters *model_params) {

  bool return_value = true; // returns true if import succeeds

  if (!strcmp (param_name, "ACTIVATION_MAX")) { 
    model_params->activation_max = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->activation_max);
  }
  else if (!strcmp (param_name, "ACTIVATION_MIN")) {
    model_params->activation_min = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->activation_min);
  }
  else if (!strcmp (param_name, "RESPONSE_THRESHOLD")) {
    model_params->response_threshold = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->response_threshold);
  }
  else if (!strcmp (param_name, "STEP_SIZE")) {
    model_params->step_size = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.4f\n", param_name, model_params->step_size);
  }
  else if (!strcmp (param_name, "SQUASHING_PARAM")) {
    model_params->squashing_param = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->squashing_param);
  }
  else if (!strcmp (param_name, "NOISE")) {
    model_params->noise = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->noise);
  }
  else if (!strcmp (param_name, "BIAS_OUTPUTUNIT")) {
    model_params->bias_outputunit = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_outputunit);
  }
  else if (!strcmp (param_name, "BIAS_TASKDEMAND")) {
    model_params->bias_taskdemand = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_taskdemand);
  }  
  else if (!strcmp (param_name, "BIAS_NONE")) {
    model_params->bias_none = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_none);
  }
  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_WORD")) {
    model_params->stimulus_input_strength_word = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->stimulus_input_strength_word);
  }
  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_COLOUR")) {
    model_params->stimulus_input_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->stimulus_input_strength_colour);
  }
  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_INHIBITORY_WT")) {
    model_params->taskdemand_output_inhibitory_wt = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->taskdemand_output_inhibitory_wt);
  }
  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_EXCITATORY_WT")) {
    model_params->taskdemand_output_excitatory_wt = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->taskdemand_output_excitatory_wt);
  }
  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_WORD")) {
    model_params->topdown_control_strength_word = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->topdown_control_strength_word);
  }
  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_COLOUR")) {
    model_params->topdown_control_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->topdown_control_strength_colour);
  }
  else if (!strcmp (param_name, "LEARNING_RATE")) {
    model_params->learning_rate = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->learning_rate);
  }
  else if (!strcmp (param_name, "MAX_CYCLES")) {
    model_params->max_cycles = (double) g_ascii_strtoll (param_value, NULL, 10);
    printf ("parameter %s now %d\n", param_name, model_params->max_cycles);
  }
  else if (!strcmp (param_name, "HEBBIAN_LEARNING_PERSISTENCE")) {
    model_params->hebb_persist = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %d\n", param_name, model_params->hebb_persist);    
  }
  else if (!strcmp (param_name, "RSI_SCALE_PARAM")) {
    model_params->rsi_scale_param = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->rsi_scale_param);    
  }



  else {
    printf ("warning! parameter %s not recognised\n", param_name);
    return_value = false;
  }

  return return_value;
}


void gs_stroop_parameters_import_commit (FileData *config_file, 
					 GsStroopParameters *model_params) {

  GtkTreeIter iter; 
  gboolean more;

  more = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(config_file->tree_store), &iter);

  while (more) {

    // tree is not empty, process 
    gchar *param_name;
    gchar *param_value;

   
    gtk_tree_model_get (GTK_TREE_MODEL(config_file->tree_store), 
			&iter, COL_PARAMETER_NAME, &param_name, -1);
    gtk_tree_model_get (GTK_TREE_MODEL(config_file->tree_store), 
			&iter, COL_PARAMETER_VALUE, &param_value, -1);
    g_print ("assigning:\t%s: %s\n", param_name, param_value);

    model_parameter_import (param_name, param_value, model_params);

    g_free (param_name);
    g_free (param_value);
    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(config_file->tree_store), &iter);
  }  

}


void import_stroop_trial_data_to_treestore (GtkTreeStore * store, 
					    GtkTreeIter *iter, 
					    stroop_trial_data* data) {


  gtk_tree_store_set (store, iter, 
		      COL_TASK_ID, data->trial_id, 
		      COL_TASK_PATTERN_1, data->stim_word, 
		      COL_TASK_PATTERN_2, data->stim_colour,
		      COL_TASK_PATTERN_3, data->stim_task,
      		      -1);

}


// use task_store as PdpGuiObjects member as a buffer for importing tasks (task blocks)
FileData * create_task_import_objects() {

  FileData *config_file; // struct containing pointers to relevant file data
  GtkTreeStore *store;

  // first create memory for the file pointer

  config_file = g_malloc (sizeof(FileData)); 
  config_file->fp = NULL;

  char filename[FILENAME_MAX_LENGTH];
  strcpy (filename, "no file selected");
  config_file->filename_label = gtk_label_new(filename);

  // Need to do string to int conversion when transferring out of buffer to task store
  store = gtk_tree_store_new (N_TASK_COLUMNS,
			      G_TYPE_STRING,
			      G_TYPE_STRING,
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING);
  
  config_file->tree_store = store;

  return config_file;
}
