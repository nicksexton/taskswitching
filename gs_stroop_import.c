/* Non-gui functions for importing parameters and tasks to gs stroop model */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "pdp_import.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"

// OLD, deprecated
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

// NEW
bool model_parameter_import_ht (gchar* param_name, gchar* param_value, GHashTable *model_params_ht) {

  bool return_value = true; // returns true if import succeeds

  if (!strcmp (param_name, "ACTIVATION_MAX")) { 
    //    model_params->activation_max = (double) g_ascii_strtod (param_value, NULL);
    gdouble * activation_max = g_malloc(sizeof(double));
    *activation_max = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "activation_max", activation_max);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "activation_max"));
  }

  else if (!strcmp (param_name, "ACTIVATION_MIN")) {
    gdouble * activation_min = g_malloc(sizeof(double));
    *activation_min = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "activation_min", activation_min);
    printf ("parameter %s now %4.2f\n", param_name,
	    *(double *)g_hash_table_lookup(model_params_ht, "activation_min"));
  }

  else if (!strcmp (param_name, "RESPONSE_THRESHOLD")) {
    gdouble * response_threshold = g_malloc(sizeof(double));
    *response_threshold = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "response_threshold", response_threshold);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "response_threshold"));

  }

  else if (!strcmp (param_name, "STEP_SIZE")) {
    gdouble * step_size = g_malloc(sizeof(double));
    *step_size = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "step_size", step_size);
    printf ("parameter %s now %4.4f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "step_size"));
  }

  else if (!strcmp (param_name, "SQUASHING_PARAM")) {
    gdouble * squashing_param = g_malloc(sizeof(double));
    *squashing_param = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "squashing_param", squashing_param);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "squashing_param"));
  }

  else if (!strcmp (param_name, "NOISE")) {
    gdouble * noise = g_malloc(sizeof(double));
    *noise = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "noise", noise);
    printf ("parameter %s now %4.2f\n", param_name,
	    *(double *)g_hash_table_lookup(model_params_ht, "noise"));
  }

  else if (!strcmp (param_name, "BIAS_OUTPUTUNIT")) {
    gdouble * bias_outputunit = g_malloc(sizeof(double));
    *bias_outputunit = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "bias_outputunit", bias_outputunit);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "bias_outputunit"));
  }

  else if (!strcmp (param_name, "BIAS_TASKDEMAND")) {
    gdouble * bias_taskdemand = g_malloc(sizeof(double));
    *bias_taskdemand = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "bias_taskdemand",  bias_taskdemand);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "bias_taskdemand"));
  }  

  else if (!strcmp (param_name, "BIAS_NONE")) {
    gdouble * bias_none = g_malloc(sizeof(double));
    *bias_none = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "bias_none", bias_none);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "bias_none"));
  }

  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_WORD")) {
    gdouble *stimulus_input_strength_word  = g_malloc(sizeof(double));
    *stimulus_input_strength_word = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "stimulus_input_strength_word", stimulus_input_strength_word);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "stimulus_input_strength_word"));
  }

  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_COLOUR")) {
    gdouble *stimulus_input_strength_colour  = g_malloc(sizeof(double));
    *stimulus_input_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "stimulus_input_strength_colour", stimulus_input_strength_colour);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "stimulus_input_strength_colour"));
  }

  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_INHIBITORY_WT")) {
    gdouble *taskdemand_output_inhibitory_wt  = g_malloc(sizeof(double));
    *taskdemand_output_inhibitory_wt = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "taskdemand_output_inhibitory_wt", taskdemand_output_inhibitory_wt);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "taskdemand_output_inhibitory_wt"));
  }

  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_EXCITATORY_WT")) {
    gdouble *taskdemand_output_excitatory_wt  = g_malloc(sizeof(double));
    *taskdemand_output_excitatory_wt = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "taskdemand_output_excitatory_wt", taskdemand_output_excitatory_wt);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "taskdemand_output_excitatory_wt"));
  }

  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_WORD")) {
    gdouble *topdown_control_strength_word  = g_malloc(sizeof(double));
    *topdown_control_strength_word = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "topdown_control_strength_word", topdown_control_strength_word);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "topdown_control_strength_word"));
  }

  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_COLOUR")) {
    gdouble *topdown_control_strength_colour  = g_malloc(sizeof(double));
    *topdown_control_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "topdown_control_strength_colour", topdown_control_strength_colour);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "topdown_control_strength_colour"));
  }

  else if (!strcmp (param_name, "LEARNING_RATE")) {
    gdouble *learning_rate  = g_malloc(sizeof(double));
    *learning_rate = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "learning_rate", learning_rate);
    printf ("parameter %s now %4.2f\n", param_name, 	    
	    *(double *)g_hash_table_lookup(model_params_ht, "learning_rate"));
  }

  else if (!strcmp (param_name, "MAX_CYCLES")) {
    gint *max_cycles  = g_malloc(sizeof(int));
    *max_cycles = (int) g_ascii_strtoll (param_value, NULL, 10);
    g_hash_table_insert (model_params_ht, "max_cycles", max_cycles);
    printf ("parameter %s now %d\n", param_name, 
	    *(int *)g_hash_table_lookup(model_params_ht, "max_cycles"));
  }

  else if (!strcmp (param_name, "HEBBIAN_LEARNING_PERSISTENCE")) {
    gint *hebb_persist  = g_malloc(sizeof(int));
    *hebb_persist = (double) g_ascii_strtoll (param_value, NULL, 10);
    g_hash_table_insert (model_params_ht, "hebb_persist", hebb_persist);
    printf ("parameter %s now %d\n", param_name,
	    *(int *)g_hash_table_lookup(model_params_ht, "hebb_persist"));    
  }

  else if (!strcmp (param_name, "RSI_SCALE_PARAM")) {
    gdouble *rsi_scale_param  = g_malloc(sizeof(double));
    *rsi_scale_param = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "rsi_scale_param", rsi_scale_param);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "rsi_scale_param"));    
  }

  else {
    printf ("warning! parameter %s not recognised\n", param_name);
    return_value = false;
  }

  return return_value;
}



void gs_stroop_parameters_import_commit (FileData *config_file, 
					 GHashTable *model_params) {

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

    //    model_parameter_import (param_name, param_value, model_params);
    model_parameter_import_ht (param_name, param_value, model_params);

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


void gs_stroop_model_task_import_commit (FileData *task_config_file, 
					 GtkTreeStore *task_store) {


  // OLD CODE
  
  GtkTreeIter iter_import; 
  GtkTreeIter iter_parent;
  GtkTreeIter iter_import_append;

  gboolean more;
  gchar *block_id_prev;
  gchar *block_id_next;

  // get iterator at top of the import store 
  more = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(task_config_file->tree_store), &iter_import);



  // get block ID for first trial in import store
  gtk_tree_model_get (GTK_TREE_MODEL(task_config_file->tree_store), &iter_import,
		      COL_BLOCK_NAME, &block_id_prev,
		      -1);
    
  // get parent as a new top level iterator in task store
  gtk_tree_store_append (task_store, &iter_parent, NULL);

  gtk_tree_store_set (task_store, 
		      &iter_parent,
		      COL_BLOCK_NAME, block_id_prev, 
		      -1);
  

  while (more) {

    // insert new parent if BLOCK_ID changes
    gtk_tree_model_get (GTK_TREE_MODEL(task_config_file->tree_store), &iter_import,
			COL_BLOCK_NAME, &block_id_next,
			-1);

    if (strcmp (block_id_prev, block_id_next)) {
      // add a new top-level iterator (block)
      gtk_tree_store_append (task_store, &iter_parent, NULL);

      gtk_tree_store_set (task_store, 
			  &iter_parent,
			  COL_BLOCK_NAME, block_id_next, 
			  -1);

    }

    g_free (block_id_prev);
    block_id_prev = block_id_next;

    // create stroop trial data object
    stroop_trial_data imported_trial;
    gchar *import_trial_id;
    gchar *import_stim_task;
    gchar *import_stim_word;
    gchar *import_stim_colour;
    gchar *task_param_1;
    gchar *task_param_2;


    gtk_tree_model_get (GTK_TREE_MODEL(task_config_file->tree_store), &iter_import, 
			COL_TASK_ID, &import_trial_id, 
			COL_TASK_PATTERN_1, &import_stim_word, 
			COL_TASK_PATTERN_2, &import_stim_colour,
			COL_TASK_PATTERN_3, &import_stim_task, 
			COL_TASK_PARAM_1, &task_param_1, 
			COL_TASK_PARAM_2, &task_param_2, 
			-1);

    // need to set defaults here to prevent seg fault if data not imported??
    // or, are defaults set in task_config_file when importing from text file...?
    imported_trial.trial_id = atoi (import_trial_id);
    imported_trial.stim_task = atoi (import_stim_task);
    imported_trial.stim_word = atoi (import_stim_word);
    imported_trial.stim_colour = atoi (import_stim_colour);


    gtk_tree_store_append (task_store, &iter_import_append, &iter_parent);

    import_stroop_trial_data_to_treestore(task_store, &iter_import_append, &imported_trial);

    // set trial parameters
    gtk_tree_store_set (task_store, 
			&iter_import_append,
			COL_TASK_PARAM_1, task_param_1,
			COL_TASK_PARAM_2, task_param_2,
			-1);

    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(task_config_file->tree_store), &iter_import);

    g_free (import_trial_id);

  }  
  // model_reset_trial_markers (objects->simulation);

  g_free (block_id_next);
  // block_id_prev already freed

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
