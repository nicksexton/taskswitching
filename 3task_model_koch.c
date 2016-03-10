// #define DEBUG_NETWORK_ACTIVATION
// #define ECHO
// #define ECHO_ACTIVATION
#define STRATEGIC_ADAPTATION_RATE 0.05

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "3task_procedure.h" // for init_type typedef
#include "3task_model_koch.h"
#include "3task_model_gs.h" // for some shared functions
#include "3task_koch_default_params.h"


void three_task_koch_conflict_parameters_htable_set_default (GHashTable * params_table) {

  double * activation_max = g_malloc(sizeof(double));
  *activation_max = ACTIVATION_MAX;
  g_hash_table_insert (params_table, "activation_max", activation_max);

  double * activation_min = g_malloc(sizeof(double));
  *activation_min = ACTIVATION_MIN;
  g_hash_table_insert (params_table, "activation_min", activation_min);

  double * response_threshold = g_malloc(sizeof(double));
  *response_threshold = RESPONSE_THRESHOLD;
  g_hash_table_insert (params_table, "response_threshold", response_threshold);

  double * step_size = g_malloc(sizeof(double));
  *step_size = STEP_SIZE;
  g_hash_table_insert (params_table, "step_size", step_size);

  double * squashing_param = g_malloc(sizeof(double));
  *squashing_param = SQUASHING_PARAM;
  g_hash_table_insert (params_table, "squashing_param", squashing_param);

  double * conflict_squashing_param = g_malloc(sizeof(double));
  *conflict_squashing_param = SQUASHING_PARAM;
  g_hash_table_insert (params_table, "conflict_squashing_param", conflict_squashing_param);

  double * rsi_scale_param = g_malloc(sizeof(double));
  *rsi_scale_param = RSI_SCALE_PARAM;
  g_hash_table_insert (params_table, "rsi_scale_param", rsi_scale_param);

  double * noise = g_malloc(sizeof(double));
  *noise = NOISE;
  g_hash_table_insert (params_table, "noise", noise);

  double * bias_outputunit = g_malloc(sizeof(double));
  *bias_outputunit = BIAS_OUTPUTUNIT;
  g_hash_table_insert (params_table, "bias_outputunit", bias_outputunit);

  double * bias_taskdemand = g_malloc(sizeof(double));
  *bias_taskdemand = BIAS_TASKDEMAND;
  g_hash_table_insert (params_table, "bias_taskdemand", bias_taskdemand);

  double * bias_conflict = g_malloc(sizeof(double));
  *bias_conflict = BIAS_CONFLICT;
  g_hash_table_insert (params_table, "bias_conflict", bias_conflict);

  double * bias_none = g_malloc(sizeof(double));
  *bias_none = BIAS_NONE;
  g_hash_table_insert (params_table, "bias_none", bias_none);

  double *stimulus_input_strength_0  = g_malloc(sizeof(double));
  *stimulus_input_strength_0 = STIMULUS_INPUT_STRENGTH_0;
  g_hash_table_insert (params_table, "stimulus_input_strength_0", stimulus_input_strength_0);

  double *stimulus_input_strength_1  = g_malloc(sizeof(double));
  *stimulus_input_strength_1 = STIMULUS_INPUT_STRENGTH_1;
  g_hash_table_insert (params_table, "stimulus_input_strength_1", stimulus_input_strength_1);

  double *stimulus_input_strength_2  = g_malloc(sizeof(double));
  *stimulus_input_strength_2 = STIMULUS_INPUT_STRENGTH_2;
  g_hash_table_insert (params_table, "stimulus_input_strength_2", stimulus_input_strength_2);

  double *taskdemand_output_inhibitory_wt  = g_malloc(sizeof(double));
  *taskdemand_output_inhibitory_wt = TASKDEMAND_OUTPUT_INHIBITORY_WT;
  g_hash_table_insert (params_table, "taskdemand_output_inhibitory_wt", taskdemand_output_inhibitory_wt);

  double *taskdemand_output_excitatory_wt  = g_malloc(sizeof(double));
  *taskdemand_output_excitatory_wt = TASKDEMAND_OUTPUT_EXCITATORY_WT;
  g_hash_table_insert (params_table, "taskdemand_output_excitatory_wt", taskdemand_output_excitatory_wt);

  double *taskdemand_lateral_inhibitory_wt  = g_malloc(sizeof(double));
  *taskdemand_output_excitatory_wt = TASKDEMAND_LATERAL_INHIBITORY_WT;
  g_hash_table_insert (params_table, "taskdemand_lateral_inhibitory_wt", taskdemand_lateral_inhibitory_wt);

  double *conflict_taskdemand_wt  = g_malloc(sizeof(double));
  *conflict_taskdemand_wt = CONFLICT_TASKDEMAND_WT;
  g_hash_table_insert (params_table, "conflict_taskdemand_wt", taskdemand_lateral_inhibitory_wt);

  double *conflict_gain  = g_malloc(sizeof(double));
  *conflict_gain = CONFLICT_GAIN;
  g_hash_table_insert (params_table, "conflict_gain", conflict_gain);

  int *conflict_negative = g_malloc(sizeof(conflict_negative_options));
  *conflict_negative = CONFLICT_NEGATIVE;
  g_hash_table_insert (params_table, "conflict_negative", conflict_negative);

  double *topdown_control_strength_0  = g_malloc(sizeof(double));
  *topdown_control_strength_0 = TOPDOWN_CONTROL_STRENGTH_0;
  g_hash_table_insert (params_table, "topdown_control_strength_0", topdown_control_strength_0);

  double *topdown_control_strength_1  = g_malloc(sizeof(double));
  *topdown_control_strength_1 = TOPDOWN_CONTROL_STRENGTH_1;
  g_hash_table_insert (params_table, "topdown_control_strength_1", topdown_control_strength_1);

  double *topdown_control_strength_2  = g_malloc(sizeof(double));
  *topdown_control_strength_2 = TOPDOWN_CONTROL_STRENGTH_2;
  g_hash_table_insert (params_table, "topdown_control_strength_2", topdown_control_strength_2);

  double *learning_rate  = g_malloc(sizeof(double));
  *learning_rate = LEARNING_RATE;
  g_hash_table_insert (params_table, "learning_rate", learning_rate);

  int *max_cycles  = g_malloc(sizeof(int));
  *max_cycles = MAX_CYCLES;
  g_hash_table_insert (params_table, "max_cycles", max_cycles);

  int *hebb_persist  = g_malloc(sizeof(hebbian_learning_persistence));
  *hebb_persist = HEBB_PERSIST;
  g_hash_table_insert (params_table, "hebb_persist", hebb_persist);

  return;
}


bool three_task_model_koch_conflict_parameter_import_ht (gchar* param_name, 
							 gchar* param_value, 
							 GHashTable *model_params_ht) {

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

  else if (!strcmp (param_name, "CONFLICT_SQUASHING_PARAM")) {
    gdouble * conflict_squashing_param = g_malloc(sizeof(double));
    *conflict_squashing_param = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "conflict_squashing_param", conflict_squashing_param);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "conflict_squashing_param"));
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

  else if (!strcmp (param_name, "BIAS_CONFLICT")) {
    gdouble * bias_conflict = g_malloc(sizeof(double));
    *bias_conflict = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "bias_conflict",  bias_conflict);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "bias_conflict"));
  }  


  else if (!strcmp (param_name, "BIAS_NONE")) {
    gdouble * bias_none = g_malloc(sizeof(double));
    *bias_none = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "bias_none", bias_none);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "bias_none"));
  }

  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_0")) {
    gdouble *stimulus_input_strength_0  = g_malloc(sizeof(double));
    *stimulus_input_strength_0 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "stimulus_input_strength_0", stimulus_input_strength_0);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "stimulus_input_strength_0"));
  }

  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_1")) {
    gdouble *stimulus_input_strength_1  = g_malloc(sizeof(double));
    *stimulus_input_strength_1 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "stimulus_input_strength_1", stimulus_input_strength_1);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "stimulus_input_strength_1"));
  }

  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_2")) {
    gdouble *stimulus_input_strength_2  = g_malloc(sizeof(double));
    *stimulus_input_strength_2 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "stimulus_input_strength_2", stimulus_input_strength_2);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "stimulus_input_strength_2"));
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

  else if (!strcmp (param_name, "TASKDEMAND_LATERAL_INHIBITORY_WT")) {
    gdouble *taskdemand_lateral_inhibitory_wt  = g_malloc(sizeof(double));
    *taskdemand_lateral_inhibitory_wt = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "taskdemand_lateral_inhibitory_wt", taskdemand_lateral_inhibitory_wt);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "taskdemand_lateral_inhibitory_wt"));
  }

  else if (!strcmp (param_name, "CONFLICT_TASKDEMAND_WT")) {
    gdouble *conflict_taskdemand_wt  = g_malloc(sizeof(double));
    *conflict_taskdemand_wt = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "conflict_taskdemand_wt", conflict_taskdemand_wt);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "conflict_taskdemand_wt"));
  }

  else if (!strcmp (param_name, "CONFLICT_GAIN")) {
    gdouble *conflict_gain  = g_malloc(sizeof(double));
    *conflict_gain = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "conflict_gain", conflict_gain);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "conflict_gain"));
  }

  else if (!strcmp (param_name, "CONFLICT_NEGATIVE")) {
    gint *conflict_negative  = g_malloc(sizeof(gint));
    *conflict_negative = (int) g_ascii_strtoll (param_value, NULL, 10);
    g_hash_table_insert (model_params_ht, "conflict_negative", conflict_negative);
    printf ("parameter %s now %d\n", param_name, 
	    *(int *)g_hash_table_lookup(model_params_ht, "conflict_negative"));
  }


  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_0")) {
    gdouble *topdown_control_strength_0  = g_malloc(sizeof(double));
    *topdown_control_strength_0 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "topdown_control_strength_0", topdown_control_strength_0);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "topdown_control_strength_0"));
  }

  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_1")) {
    gdouble *topdown_control_strength_1  = g_malloc(sizeof(double));
    *topdown_control_strength_1 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "topdown_control_strength_1", topdown_control_strength_1);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "topdown_control_strength_1"));
  }

  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_2")) {
    gdouble *topdown_control_strength_2  = g_malloc(sizeof(double));
    *topdown_control_strength_2 = (double) g_ascii_strtod (param_value, NULL);
    g_hash_table_insert (model_params_ht, "topdown_control_strength_2", topdown_control_strength_2);
    printf ("parameter %s now %4.2f\n", param_name, 
	    *(double *)g_hash_table_lookup(model_params_ht, "topdown_control_strength_2"));
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
    *hebb_persist = (int) g_ascii_strtoll (param_value, NULL, 10);
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


void three_task_koch_conflict_parameters_import_commit (FileData *config_file, 
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
    three_task_model_koch_conflict_parameter_import_ht (param_name, param_value, model_params);

    g_free (param_name);
    g_free (param_value);
    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(config_file->tree_store), &iter);
  }  

}


// cf. model_parameter_import in pdpgui_import.c
bool three_task_koch_conflict_task_parameter_import (gchar* param, GHashTable *model_params_ht) {

  // printf ("debug: now in model_task_parameter_import\n");
  bool return_value = true;
  

  if (!strncmp (param, "HebP=", 5)) { // Hebbian learning persistence
    if (strlen (param) < 6) {
      printf ("error in model_task_parameter_import! HebP= param shorter than 6 characters, no value?");
    }
    else {
      gint *hebb_persist  = g_malloc(sizeof(int));
      *hebb_persist = (int) g_ascii_strtoll (&param[5], NULL, 10);
      g_hash_table_insert (model_params_ht, "hebb_persist", hebb_persist);
      printf ("trial parameter %s now %d\n", param, 
	      *(int *)g_hash_table_lookup(model_params_ht, "hebb_persist"));
    }
  }

  else if (!strncmp (param, "RSIs=", 5)) { // RSI scaling parameter
    if (strlen (param) < 6) {
      printf ("error in model_task_parameter_import! HebP= param shorter than 6 characters, no value?");
    }
    else {
      gdouble *rsi_scale_param  = g_malloc(sizeof(double));
      *rsi_scale_param = (double) g_ascii_strtod (&param[5], NULL);
      g_hash_table_insert (model_params_ht, "rsi_scale_param", rsi_scale_param);
      printf ("trial parameter %s now %4.2f\n", param, 
	      *(double *)g_hash_table_lookup(model_params_ht, "rsi_scale_param"));    
    }
  }

  // other parameter imports go here

  else {
    printf ("warning! trial parameter %s not recognised\n", param);
    return_value = false;
  }

  return return_value;
}



int three_task_koch_conflict_set_trial_params_from_task_store (GtkTreeStore *store, 
GtkTreeIter *trial, 
GHashTable *model_params_ht){

  gchar* task_param_1 = NULL;
  gchar* task_param_2 = NULL;

  gtk_tree_model_get (GTK_TREE_MODEL(store), trial, 
		      COL_TASK_PARAM_1, &task_param_1,
		      COL_TASK_PARAM_2, &task_param_2,
		      -1);

  // set trial parameters!

  // code here: if task param 1 or 2 is non-empty,
  if (task_param_1 != NULL) {
    // set trial parameters
    three_task_koch_conflict_task_parameter_import (task_param_1, model_params_ht);
  }

  if (task_param_2 != NULL) {
    // set trial parameters
    three_task_koch_conflict_task_parameter_import (task_param_2, model_params_ht);
  }

  return 0;
}


int three_task_model_koch_strategic_adaptation (pdp_model * model) {

  pdp_layer *task_demand, *conflict_total;
  pdp_weights_matrix * weights_to_update;
  double cum_conflict_this_trial, weights_scale; // value to multiply weights by
  int i, j;

  task_demand = pdp_model_component_find (model, ID_TASKDEMAND)->layer;
  conflict_total = pdp_model_component_find (model, ID_CONFLICT_TOTAL)->layer;
  weights_to_update = pdp_input_find (task_demand, ID_CONFLICT)->input_weights;
  
  // first calculate cumulative conflict

  cum_conflict_this_trial = 0;
  for (j = 0; j < 3; j ++) {
      cum_conflict_this_trial += 
	 conflict_total->units_latest->activations[j]; 
    } 

  

  if (cum_conflict_this_trial > model->last_trial_cum_conflict) {
    // if cum conflict went up, do opposite to last time
    if (model->last_trial_weight_change > 1.0) {
      weights_scale = 1 - STRATEGIC_ADAPTATION_RATE;      
    }
    else {
      weights_scale = 1 + STRATEGIC_ADAPTATION_RATE;      
    }
  } else {
    // if cum conflict went down, do the same as last time
    if (model->last_trial_weight_change > 1.0) {
      weights_scale = 1 + STRATEGIC_ADAPTATION_RATE;      
    }
    else {
      weights_scale = 1 - STRATEGIC_ADAPTATION_RATE;      
    }
  }
  printf ("\nCum conflict %3.2f, last trial %3.2f, Weight scale %3.2f",
    cum_conflict_this_trial, model->last_trial_cum_conflict, weights_scale); // debug

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      weights_to_update->weights[i][j] *= weights_scale;
    }
  }

  
  model->last_trial_cum_conflict = cum_conflict_this_trial;
  model->last_trial_weight_change = weights_scale;

  return 0;
  
}


/* Mirrors three_task_model_dummy_run in 3task_model_gs.c */
int three_task_model_koch_conflict_run (pdp_model * model,  
					ThreeTaskSimulation * simulation) {

  printf ("in three_task_model_koch_conflict_run, running model\n");

  double input_0_initial_act[2]   = { 0.0,  0.0 };
  double input_1_initial_act[2]   = { 0.0,  0.0 };
  double input_2_initial_act[2]   = { 0.0,  0.0 };
  double topdown_control_initial_act[3]   = { 0.0,  0.0, 0.0 };
  double response_threshold, learning_rate;
  hebbian_learning_persistence hebb_persist;
  conflict_negative_options conflict_negative;
  int stopped;

  // Get data for current trial 
  gchar *path;
  GtkTreeIter *iter = g_malloc (sizeof(GtkTreeIter));

  if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(simulation->task_store), iter, simulation->current_trial_path)) {

    g_free (iter);
    printf ("error! three_task_model_dummy_run  failed to acquire valid iter from current_trial_path,"
	    "returning FALSE\n");
    return -1;
  }

  else {
    // open file for writing data
    int trial_id, cue, stim_0, stim_1, stim_2;

    // now get params for current trial
    path = gtk_tree_path_to_string(simulation->current_trial_path);
    fprintf (simulation->datafile, "%s\t", path);
    printf ("trial: %s", path);

    three_task_koch_conflict_set_trial_params_from_task_store (simulation->task_store, 
							       iter, 
							       simulation->model_params_htable);

    gtk_tree_model_get (GTK_TREE_MODEL(simulation->task_store), iter, 
			COL_TASK_ID, &trial_id,
			COL_TASK_PATTERN_1, &cue, 
			COL_TASK_PATTERN_2, &stim_0,
			COL_TASK_PATTERN_3, &stim_1,
			COL_TASK_PATTERN_4, &stim_2,
			-1);

    // print trial params to output file
    fprintf (simulation->datafile, "%d\t", trial_id);
    fprintf (simulation->datafile, "%d\t", cue);
    fprintf (simulation->datafile, "%d\t", stim_0);
    fprintf (simulation->datafile, "%d\t", stim_1);
    fprintf (simulation->datafile, "%d\t", stim_2);

  // check that trial parameters are sensible

  if (stim_0 < -1 || stim_0 > 2) {
    printf ("subject data: input_0 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_0);
    return (0);
  }

  if (stim_1 < -1 || stim_1 > 2) {
    printf ("subject data: input_1 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_1);
    return (0);
  }

  if (stim_2 < -1 || stim_2 > 2) {
    printf ("subject data: input_2 %d out of range (should be -1 (neutral) or 0 - 1)!",
	    stim_2);
    return (0);
  }

  if (cue < 0 || cue > 3) {
    printf ("subject data: task input %d out of range (should be 0 - 2)!",
	    cue);
    return (0);
  }


  // set ON inputs
  if (stim_0 >= 0) { // check for neutral trial condition where stim is -1
      input_0_initial_act[stim_0] = 1.0;
  }

  if (stim_1 >= 0) { // check for neutral trial condition where stim is -1
      input_1_initial_act[stim_1] = 1.0;
  }

  if (stim_2 >= 0) { // check for neutral trial condition where stim is -1
      input_2_initial_act[stim_2] = 1.0;
  }

  topdown_control_initial_act[cue] = 1.0;

  pdp_layer_set_activation (pdp_model_component_find (model, ID_INPUT_0)->layer, 
			    2, input_0_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (model, ID_INPUT_1)->layer, 
			    2, input_1_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (model, ID_INPUT_2)->layer, 
			    2, input_2_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (
                                 model, ID_TOPDOWNCONTROL)->layer, 
	   		    3, topdown_control_initial_act);

  // <--------------------- RUN TRIAL ---------------------------->
  response_threshold = *(double *)g_hash_table_lookup(simulation->model_params_htable, "response_threshold");
  learning_rate = *(double *)g_hash_table_lookup(simulation->model_params_htable, "learning_rate");
  hebb_persist = *(int *)g_hash_table_lookup(simulation->model_params_htable, 
					     "hebb_persist");

  // check what to do (if anything) about negative conflict
  conflict_negative = *(gint *)g_hash_table_lookup(simulation->model_params_htable, 
									     "conflict_negative");  

#ifdef ECHO

    printf ("\ncyc:%d\t", model->cycle);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_0)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_1)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_2)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_TASKDEMAND)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_CONFLICT)->layer);

    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_CONFLICT_INPUT)->layer);

#endif

    three_task_model_koch_fprintf_current_state (model, path, simulation->datafile_act); // print activations to log file

  // run_model_step returns true when stopping_condition evaluates to false
  do {
    three_task_model_koch_conflict_run_step (model, simulation->random_generator, 
					     response_threshold, 
					     simulation->datafile,
					     simulation->datafile_act,
					     path,
					     conflict_negative);
    stopped = stopping_condition (model, response_threshold);
  } while (stopped == false && model->cycle < MAX_CYCLES);
  

  // associative weights
  three_task_model_update_weights(model,
				  learning_rate,
				  hebb_persist);

  // strategic adaptation
  three_task_model_koch_strategic_adaptation (model); 

  
  // Print activation of output units


  // Print RT
  fprintf (simulation->datafile, "%d\t", model->cycle); 
  fprintf (simulation->datafile, "%d\t", stopped - 1); // response
  // Simulation 14 - print conflict unit output
  pdp_layer_fprintf_current_output (pdp_model_component_find (model, ID_CONFLICT_TOTAL)->layer,
    simulation->datafile);
  fprintf (simulation->datafile, "%3.2f\t", model->last_trial_weight_change);
  double current_weight = pdp_input_find (pdp_model_component_find (model, ID_TASKDEMAND)->layer, ID_CONFLICT)->input_weights->weights[0][0];
  fprintf (simulation->datafile, "%4.2f\t", current_weight);
  fprintf (simulation->datafile, "\n");

  g_free (path);

  return (1);
  }

}


void three_task_model_koch_fprintf_current_state (pdp_model *model, gchar *path, FILE * fp_act) {



  fprintf (fp_act, "%s\t%d\t", path, model->cycle);


  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_TOPDOWNCONTROL)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_INPUT_0)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_INPUT_1)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_INPUT_2)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_0)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_1)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_2)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_TASKDEMAND)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_CONFLICT)->layer, fp_act);
  pdp_layer_fprintf_current_output (
				    pdp_model_component_find (model, ID_CONFLICT_INPUT)->layer, fp_act);
    fprintf (fp_act, "\n");
    
    return;
 

}


// replaces pdp_layer_cycle_inputs. Only used when layer->id is TASKDEMAND and conflict_negative == CLIP
// checks whether input is from a conflict monitoring unit: if so, clips activation at zero
int three_task_koch_pdp_layer_cycle_inputs (pdp_layer * some_layer, conflict_negative_options conflict_negative) {

  pdp_input * input_iterator; 
  input_iterator = some_layer->upstream_layers;

  /* remember to zero the accumulators, then add bias now */
  int j;
  for (j = 0; j < some_layer->size; j++) {
    // some_layer->net_inputs[j] = 0;
    some_layer->net_inputs[j] = some_layer->input_bias;
  }

  /* update the inputs */
  while (input_iterator != NULL) {

#ifdef DEBUG_NETWORK_ACTIVATION
    printf ("\ninput to layer %d from %d: ", some_layer->id, input_iterator->input_layer->id);
#endif

    if ((some_layer->id == ID_TASKDEMAND) && 
	(input_iterator->input_layer->id == ID_CONFLICT) &&
	(conflict_negative == NO_CONFLICT || conflict_negative == CLIP || conflict_negative == RESCALE)) {

      switch (conflict_negative) {
      case CLIP:
	pdp_calc_input_fromlayer_clip (some_layer->size, some_layer, 
				       input_iterator->input_layer->size, input_iterator->input_layer, 
				       input_iterator->input_weights,
				       TRUE // clip_negative, TRUE clips negative 
				       );
	break;

      case RESCALE:
	pdp_calc_input_fromlayer_rescale ( some_layer->size, some_layer, 
					   input_iterator->input_layer->size, input_iterator->input_layer, 
					   input_iterator->input_weights,
					   0.5, 0.5); // scales by x * 0.5 + 0.5
	break;

      case OFF:
	// disregard input from conflict monitoring units, do nothing!
	break;

      default:
	printf ("in three_task_koch_pdp_layer_cycle_inputs, uh oh\n");
      }

    }
    else {
      pdp_calc_input_fromlayer (some_layer->size, some_layer, 
				input_iterator->input_layer->size, input_iterator->input_layer, 
				input_iterator->input_weights);
    }
    

	
    input_iterator = input_iterator->next;
  }
  
  /* <--------------- DEBUG ------------------- */
  /*
  printf ("net inputs:\n");
  for (j = 0; j < some_layer->size; j++) {
    printf ("[%d]:, %4.2f  ", j, some_layer->net_inputs[j]);
  }
  printf ("\n");
  */

  return 0;
}


// replaces pdp_model_cycle in pdp_objects.c
// needed as input from conflict monitoring to TD units might need to be clipped
void three_task_koch_model_cycle (pdp_model * some_model, conflict_negative_options conflict_negative) {
  pdp_model_component * component_i;

  /* first calc the summed inputs for all components*/
  component_i = some_model->components;
  while (component_i != NULL) {

    three_task_koch_pdp_layer_cycle_inputs (component_i->layer, conflict_negative); 
    // specifically checks for CM->TD input and clips/rescales
    
    component_i = component_i->next;
  }


  /* now update activations */
  component_i = some_model->components;
  while (component_i != NULL) {

    if (component_i->update_activation == true) {
      pdp_layer_cycle_activation (component_i->layer, 
				  some_model->activation_parameters);
    }

    component_i = component_i->next;
  }

  some_model->cycle ++;
  return;
}



int three_task_model_koch_conflict_run_step (pdp_model * model, 
					     const gsl_rng * random_generator, 
					     double response_threshold, 
					     FILE * fp_trial,
					     FILE * fp_act,
					     gchar * path, 
					     conflict_negative_options conflict_negative) {


    // cycle the model
    // pdp_model_cycle (model);
   three_task_koch_model_cycle (model, conflict_negative);

    // calculate conflict inputs
    pdp_layer * conflict_input =  pdp_model_component_find (model, ID_CONFLICT_INPUT)->layer;
    pdp_layer * taskdemand =  pdp_model_component_find (model, ID_TASKDEMAND)->layer;

    pdp_layer_create_units (conflict_input); // append new units history to units_latest linked list

    double conflict_input_values[3] = {0.0, 0.0, 0.0};
    int i;
    for (i = 0; i < 3; i ++) {
      conflict_input_values[i] = 
	(taskdemand->units_latest->activations[i] + 1) * 0.5 * // td unit i
	(taskdemand->units_latest->activations[(i+1)%3] + 1) * 0.5; // td unit i+1
    } 
    pdp_layer_set_activation (conflict_input, 3, conflict_input_values);

    
    // calculate cumulative conflict
    pdp_layer * conflict_total = pdp_model_component_find (model, ID_CONFLICT_TOTAL)->layer;
    pdp_layer_create_units (conflict_total); // append new units history 

    double conflict_total_values[3] = {0.0, 0.0, 0.0};
        int j;
    for (j = 0; j < 3; j ++) {
      conflict_total_values[j] = 
	(conflict_input->units_latest->activations[j] +
	 conflict_total->units_latest->previous->activations[j]); // td unit i

    } 
    pdp_layer_set_activation (conflict_total, 3, conflict_total_values);


    
    // add noise to units 
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_0)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_1)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_2)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_TASKDEMAND)->layer, 
				    NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_CONFLICT)->layer, 
				    NOISE, random_generator);
    // note doesn't add noise to conflict inputs (otherwise adding noise twice)    
    

#if defined ECHO
    
    printf ("\ncyc:%d\t", model->cycle);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_0)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_1)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_OUTPUT_2)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_TASKDEMAND)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_CONFLICT)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_CONFLICT_INPUT)->layer);
    pdp_layer_print_current_output (
				    pdp_model_component_find (model, ID_CONFLICT_TOTAL)->layer);

#endif

  /*
#if defined ECHO_WEIGHTS

  printf ("\ncyc:%d\t", gs_stroop_model->cycle);
  pdp_weights_print (pdp_input_find (pdp_model_component_find (gs_stroop_model, 
							       ID_TASKDEMAND)
				     ->layer, ID_WORDIN)->input_weights); 
  printf ("\t||\t");
  pdp_weights_print (pdp_input_find (pdp_model_component_find (gs_stroop_model, 
							       ID_TASKDEMAND)
				     ->layer, ID_COLOURIN)->input_weights); 

#endif
  */
    // Output this to a second logfile

#if defined ECHO_ACTIVATION
    if (path != NULL) {
      // fprintf (fp_act, "%s\t%d\t", path, model->cycle);
      three_task_model_koch_fprintf_current_state (model, path, fp_act);
    }
#endif
    return 0; 
  
}


int three_task_model_koch_conflict_build (pdp_model * model, GHashTable * model_params) {


  pdp_layer *input_0, *input_1, *input_2, *output_0, *output_1, *output_2, *taskdemand, *topdown_control, 
    *conflict, *conflict_input, *conflict_total;

  printf ("in three_task_model_koch_conflict_build, building the koch conflict monitoring model\n");


// first initialise the strategic adaptation variables
  model->last_trial_cum_conflict = 0;
  model->last_trial_weight_change = 1.0;


  printf ("ID_INPUT_0 = %d ", ID_INPUT_0);
  printf ("ID_INPUT_1 = %d ", ID_INPUT_1);
  printf ("ID_INPUT_2 = %d ", ID_INPUT_2);
  printf ("ID_OUTPUT_0 = %d ", ID_OUTPUT_0);
  printf ("ID_OUTPUT_1 = %d ", ID_OUTPUT_1);
  printf ("ID_OUTPUT_2 = %d ", ID_OUTPUT_2);
  printf ("ID_TASKDEMAND = %d ", ID_TASKDEMAND);
  printf ("ID_TOPDOWNCONTROL = %d ", ID_TOPDOWNCONTROL);
  printf ("ID_CONFLICT = %d ", ID_CONFLICT );
  printf ("ID_CONFLICT_INPUT = %d ", ID_CONFLICT_INPUT);
  printf ("ID_CONFLICT_TOTAL = %d ", ID_CONFLICT_TOTAL);

  input_0 = pdp_layer_create(ID_INPUT_0, 2, 
				*(double *)g_hash_table_lookup(model_params, "bias_none"));
  input_1 = pdp_layer_create(ID_INPUT_1, 2, 
				*(double *)g_hash_table_lookup(model_params, "bias_none"));
  input_2 = pdp_layer_create(ID_INPUT_2, 2, 
				*(double *)g_hash_table_lookup(model_params, "bias_none"));

  output_0 = pdp_layer_create(ID_OUTPUT_0, 2, 
				 *(double *)g_hash_table_lookup(model_params, "bias_outputunit"));
  output_1 = pdp_layer_create(ID_OUTPUT_1, 2, 
				 *(double *)g_hash_table_lookup(model_params, "bias_outputunit"));
  output_2 = pdp_layer_create(ID_OUTPUT_2, 2, 
				 *(double *)g_hash_table_lookup(model_params, "bias_outputunit"));

  taskdemand = pdp_layer_create(ID_TASKDEMAND, 3, 
				*(double *)g_hash_table_lookup(model_params, "bias_taskdemand"));
  topdown_control = pdp_layer_create(ID_TOPDOWNCONTROL, 3, 
				     *(double *)g_hash_table_lookup(model_params, "bias_none"));


  conflict_input = pdp_layer_create(ID_CONFLICT_INPUT, 3, 
				     *(double *)g_hash_table_lookup(model_params, "bias_none"));

  conflict = pdp_layer_create(ID_CONFLICT, 3, 
				     *(double *)g_hash_table_lookup(model_params, "bias_conflict"));
    conflict_total = pdp_layer_create(ID_CONFLICT_TOTAL, 3, 
				     *(double *)g_hash_table_lookup(model_params, "bias_none"));


  
  double initial_activation_in_0[2] = {0.0, 0.0};
  double initial_activation_in_1[2] = {0.0, 0.0};
  double initial_activation_in_2[2] = {0.0, 0.0};
  double initial_activation_out_0[2] = {0.0, 0.0};
  double initial_activation_out_1[2] = {0.0, 0.0};
  double initial_activation_out_2[2] = {0.0, 0.0};
  double initial_activation_taskdemand[3] = {0.0, 0.0, 0.0};
  double initial_activation_topdown_control[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict_input[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict_total[3] = {0.0, 0.0, 0.0};

  
  double topdown_control_strength_0 = *(double *)g_hash_table_lookup(model_params, 
									"topdown_control_strength_0");
  double topdown_control_strength_1 = *(double *)g_hash_table_lookup(model_params, 
									  "topdown_control_strength_1");  
  double topdown_control_strength_2 = *(double *)g_hash_table_lookup(model_params, 
									  "topdown_control_strength_2");  
  



  /* set initial activation */
  pdp_layer_set_activation(input_0, 2, initial_activation_in_0);
  pdp_layer_set_activation(input_1, 2, initial_activation_in_1);
  pdp_layer_set_activation(input_2, 2, initial_activation_in_2);
  pdp_layer_set_activation(output_0, 2, initial_activation_out_0);
  pdp_layer_set_activation(output_1, 2, initial_activation_out_1);
  pdp_layer_set_activation(output_2, 2, initial_activation_out_2);
  pdp_layer_set_activation(taskdemand, 3, initial_activation_taskdemand);
  pdp_layer_set_activation(topdown_control, 3, initial_activation_topdown_control);
  pdp_layer_set_activation(conflict, 3, initial_activation_conflict);
  pdp_layer_set_activation(conflict_input, 3, initial_activation_conflict_input);
  pdp_layer_set_activation(conflict_total, 3, initial_activation_conflict_total);


  /* <------------------------------ NETWORK CONNECTIVITY --------------------------------> */
  /****************************** */
  /* Input units -> output units */
  /***************************** */
  pdp_weights_matrix *wts_in0_out0, *wts_in1_out1, *wts_in2_out2;

  double stimulus_input_strength_0 = *(double *)g_hash_table_lookup(model_params, 
								       "stimulus_input_strength_0");
  double stimulus_input_strength_1 = *(double *)g_hash_table_lookup(model_params, 
								       "stimulus_input_strength_1");
  double stimulus_input_strength_2 = *(double *)g_hash_table_lookup(model_params, 
								       "stimulus_input_strength_2");

  double wts_in0_out0_matrix[2][2] = {
    {stimulus_input_strength_0, 0.0},
    {0.0, stimulus_input_strength_0},
  };

  double wts_in1_out1_matrix[2][2] = {
    {stimulus_input_strength_1, 0.0},
    {0.0, stimulus_input_strength_1},
  };

  double wts_in2_out2_matrix[2][2] = {
    {stimulus_input_strength_2, 0.0},
    {0.0, stimulus_input_strength_2},
  };

  printf ("model_build, stim input strenghts %2.1f; %2.1f; %2.1f\n",
	  stimulus_input_strength_0,
	  stimulus_input_strength_1,
	  stimulus_input_strength_2);
  
  wts_in0_out0 = pdp_weights_create (2,2);
  pdp_weights_set (wts_in0_out0, 2, 2, wts_in0_out0_matrix);
  pdp_input_connect (output_0, input_0, wts_in0_out0);

  wts_in1_out1 = pdp_weights_create (2,2);
  pdp_weights_set (wts_in1_out1, 2, 2, wts_in1_out1_matrix);
  pdp_input_connect (output_1, input_1, wts_in1_out1);

  wts_in2_out2 = pdp_weights_create (2,2);
  pdp_weights_set (wts_in2_out2, 2, 2, wts_in2_out2_matrix);
  pdp_input_connect (output_2, input_2, wts_in2_out2);


  /******************************** */
  /* Output unit lateral inhibition */
  /******************************** */

  pdp_weights_matrix *wts_out0_out0, *wts_out1_out1, *wts_out2_out2;

  double wts_alloutputs_lateral_matrix[2][2] = {
    { 0.0, -2.0},
    {-2.0,  0.0},
  };

  wts_out0_out0 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out0_out0, 2, 2, wts_alloutputs_lateral_matrix);
  pdp_input_connect (output_0, output_0, wts_out0_out0);

  wts_out1_out1 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out1_out1, 2, 2, wts_alloutputs_lateral_matrix);
  pdp_input_connect (output_1, output_1, wts_out1_out1);

  wts_out2_out2 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out2_out2, 2, 2, wts_alloutputs_lateral_matrix);
  pdp_input_connect (output_2, output_2, wts_out2_out2);



  /*****************************************/
  /* Output units cross-module connections */
  /*****************************************/
  pdp_weights_matrix *wts_out0_out1, *wts_out0_out2, *wts_out1_out0, *wts_out1_out2, *wts_out2_out0, *wts_out2_out1; 

  double wts_crossoutputs_matrix[2][2] = {
    { 2.0, -2.0},
    {-2.0,  2.0},
  };

  /* out_0 -> out_1 */
  wts_out0_out1 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out0_out1, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_0, output_1, wts_out0_out1);

  /* out_0 -> out_2 */
  wts_out0_out2 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out0_out2, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_0, output_2, wts_out0_out2);

  /* out_1 -> out_0 */
  wts_out1_out0 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out1_out0, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_1, output_0, wts_out1_out0);

  /* out_1 -> out_2 */
  wts_out1_out2 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out1_out2, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_1, output_2, wts_out1_out2);

  /* out_2 -> out_0 */
  wts_out2_out0 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out2_out0, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_2, output_0, wts_out2_out0);

  /* out_2 -> out_1 */
  wts_out2_out1 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out2_out1, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_2, output_1, wts_out2_out1);


  /***************************** */
  /* Output units -> task demand */
  /***************************** */
  pdp_weights_matrix *wts_out0_taskdemand, *wts_out1_taskdemand, *wts_out2_taskdemand;

  double wts_out0_taskdemand_matrix[3][2] = {
    { 1.0,  1.0},
    {-1.0, -1.0},
    {-1.0, -1.0},
  };

  double wts_out1_taskdemand_matrix[3][2] = {
    {-1.0, -1.0},
    { 1.0,  1.0},
    {-1.0, -1.0},
  };

  double wts_out2_taskdemand_matrix[3][2] = {
    {-1.0, -1.0},
    {-1.0, -1.0},
    { 1.0,  1.0},
  };

  wts_out0_taskdemand = pdp_weights_create (3,2);
  pdp_weights_set (wts_out0_taskdemand, 3, 2, wts_out0_taskdemand_matrix);
  pdp_input_connect (taskdemand, output_0, wts_out0_taskdemand);

  wts_out1_taskdemand = pdp_weights_create (3,2);
  pdp_weights_set (wts_out1_taskdemand, 3, 2, wts_out1_taskdemand_matrix);
  pdp_input_connect (taskdemand, output_1, wts_out1_taskdemand);

  wts_out2_taskdemand = pdp_weights_create (3,2);
  pdp_weights_set (wts_out2_taskdemand, 3, 2, wts_out2_taskdemand_matrix);
  pdp_input_connect (taskdemand, output_2, wts_out2_taskdemand);


  /**************************** */
  /* Taskdemand -> output units */
  /**************************** */

  double td_out_excit_wt = *(double *)g_hash_table_lookup(model_params, 
								       "taskdemand_output_excitatory_wt");

  double td_out_inhib_wt = *(double *)g_hash_table_lookup(model_params, 
								       "taskdemand_output_inhibitory_wt");

  pdp_weights_matrix *wts_taskdemand_out0, *wts_taskdemand_out1, *wts_taskdemand_out2;

  double wts_taskdemand_out0_matrix[2][3] = {
    { td_out_excit_wt, td_out_inhib_wt, td_out_inhib_wt},
    { td_out_excit_wt, td_out_inhib_wt, td_out_inhib_wt},
  };

  double wts_taskdemand_out1_matrix[2][3] = {
    {td_out_inhib_wt,  td_out_excit_wt, td_out_inhib_wt},
    {td_out_inhib_wt,  td_out_excit_wt, td_out_inhib_wt},
  };

  double wts_taskdemand_out2_matrix[2][3] = {
    {td_out_inhib_wt, td_out_inhib_wt,  td_out_excit_wt},
    {td_out_inhib_wt, td_out_inhib_wt,  td_out_excit_wt},
  };

  wts_taskdemand_out0 = pdp_weights_create (2,3);
  pdp_weights_set (wts_taskdemand_out0, 2, 3, wts_taskdemand_out0_matrix);
  pdp_input_connect (output_0, taskdemand, wts_taskdemand_out0);

  wts_taskdemand_out1 = pdp_weights_create (2,3);
  pdp_weights_set (wts_taskdemand_out1, 2, 3, wts_taskdemand_out1_matrix);
  pdp_input_connect (output_1, taskdemand, wts_taskdemand_out1);

  wts_taskdemand_out2 = pdp_weights_create (2,3);
  pdp_weights_set (wts_taskdemand_out2, 2, 3, wts_taskdemand_out2_matrix);
  pdp_input_connect (output_2, taskdemand, wts_taskdemand_out2);



  /******************************** */
  /* Taskdemand -> taskdemand units */
  /******************************** */

  double taskdemand_lateral_inhibitory_wt = *(double *)g_hash_table_lookup(model_params, 
								       "taskdemand_lateral_inhibitory_wt");

  pdp_weights_matrix *wts_taskdemand_taskdemand;
  double wts_taskdemand_taskdemand_matrix[3][3] = {
    { 0.0, taskdemand_lateral_inhibitory_wt, taskdemand_lateral_inhibitory_wt},
    {taskdemand_lateral_inhibitory_wt,  0.0, taskdemand_lateral_inhibitory_wt},
    {taskdemand_lateral_inhibitory_wt, taskdemand_lateral_inhibitory_wt,  0.0},
  };

  wts_taskdemand_taskdemand = pdp_weights_create (3,3);
  pdp_weights_set (wts_taskdemand_taskdemand, 3, 3, wts_taskdemand_taskdemand_matrix);
  pdp_input_connect (taskdemand, taskdemand, wts_taskdemand_taskdemand);


  /******************************** */
  /* Conflict Input -> Conflict units */
  /******************************** */

  double conflict_gain = *(double *)g_hash_table_lookup(model_params, 
							"conflict_gain");

  pdp_weights_matrix *wts_conflict_input_conflict;
  double wts_conflict_input_conflict_matrix[3][3] = {
    { conflict_gain, 0.0, 0.0 },
    { 0.0, conflict_gain, 0.0 },
    { 0.0, 0.0, conflict_gain },
  };

  wts_conflict_input_conflict = pdp_weights_create (3,3);
  pdp_weights_set (wts_conflict_input_conflict, 3, 3, wts_conflict_input_conflict_matrix);
  pdp_input_connect (conflict, conflict_input, wts_conflict_input_conflict);


  /******************************** */
  /* Conflict -> Taskdemand units */
  /******************************** */

  double conflict_taskdemand_wt = *(double *)g_hash_table_lookup(model_params, 
								 "conflict_taskdemand_wt");

  pdp_weights_matrix *wts_conflict_taskdemand;
  double wts_conflict_taskdemand_matrix[3][3] = {
    //    { conflict_taskdemand_wt * topdown_control_strength_0, 0.0, conflict_taskdemand_wt * topdown_control_strength_0 },
    //    { conflict_taskdemand_wt * topdown_control_strength_1, conflict_taskdemand_wt * topdown_control_strength_1, 0.0 },
    //    { 0.0, conflict_taskdemand_wt * topdown_control_strength_2, conflict_taskdemand_wt * topdown_control_strength_2 },
    { conflict_taskdemand_wt, 0.0, conflict_taskdemand_wt },
    { conflict_taskdemand_wt, conflict_taskdemand_wt, 0.0 },
    { 0.0, conflict_taskdemand_wt, conflict_taskdemand_wt },
  };

  wts_conflict_taskdemand = pdp_weights_create (3,3);
  pdp_weights_set (wts_conflict_taskdemand, 3, 3, wts_conflict_taskdemand_matrix);
  pdp_input_connect (taskdemand, conflict, wts_conflict_taskdemand);

  /*  +--------------------------------------+  */
  /*  | Top down control -> taskdemand units |  */
  /*  +--------------------------------------+  */

  pdp_weights_matrix *wts_topdown_taskdemand;
  double wts_topdown_taskdemand_matrix[3][3] = {
    { topdown_control_strength_0, 0.0, 0.0 },
    { 0.0, topdown_control_strength_1, 0.0 },
    { 0.0, 0.0, topdown_control_strength_2 },
  };

  wts_topdown_taskdemand = pdp_weights_create (3,3);
  pdp_weights_set (wts_topdown_taskdemand, 3, 3, wts_topdown_taskdemand_matrix);
  pdp_input_connect (taskdemand, topdown_control, wts_topdown_taskdemand);







  //  +----------------------------------------+//
  //  | Associative learning FF weights        |
  //  +----------------------------------------+

  pdp_weights_matrix *wts_in0_taskdemand, *wts_in1_taskdemand, *wts_in2_taskdemand;

  double wts_inputgeneric_taskdemand_matrix[3][2] = {
    {0.0, 0.0 },
    {0.0, 0.0 },
    {0.0, 0.0 },
  };
  // ok to use generic initialiser as pdp_weights_create creates copy of array anyway

  
  wts_in0_taskdemand = pdp_weights_create (3,2);
  wts_in1_taskdemand = pdp_weights_create (3,2);
  wts_in2_taskdemand = pdp_weights_create (3,2);

  pdp_weights_set (wts_in0_taskdemand, 3, 2, wts_inputgeneric_taskdemand_matrix);
  pdp_weights_set (wts_in1_taskdemand, 3, 2, wts_inputgeneric_taskdemand_matrix);
  pdp_weights_set (wts_in2_taskdemand, 3, 2, wts_inputgeneric_taskdemand_matrix);

  pdp_input_connect (taskdemand, input_0, wts_in0_taskdemand);
  pdp_input_connect (taskdemand, input_1, wts_in1_taskdemand);
  pdp_input_connect (taskdemand, input_2, wts_in2_taskdemand);






  /*********************************************/
  /* Now init model object and push components */
  /*********************************************/

  pdp_model_component_push(model, input_0, ID_INPUT_0, FALSE); 
  pdp_model_component_push(model, input_1, ID_INPUT_1, FALSE); 
  pdp_model_component_push(model, input_2, ID_INPUT_2, FALSE); 
  pdp_model_component_push(model, output_0, ID_OUTPUT_0, TRUE);
  pdp_model_component_push(model, output_1, ID_OUTPUT_1, TRUE);
  pdp_model_component_push(model, output_2, ID_OUTPUT_2, TRUE);
  pdp_model_component_push(model, taskdemand, ID_TASKDEMAND, TRUE);
  pdp_model_component_push(model, topdown_control, ID_TOPDOWNCONTROL, FALSE);
  pdp_model_component_push(model, conflict, ID_CONFLICT, TRUE); 
  pdp_model_component_push(model, conflict_input, ID_CONFLICT_INPUT, FALSE); // do not update activation, do this manually
  pdp_model_component_push(model, conflict_total, ID_CONFLICT_TOTAL, FALSE); // do not update activation, do this manually

  //debug
  printf ("basic 3 task model created! Limited connectivity, just for test\n");

  return 0;

}

int three_task_model_koch_conflict_reinit (pdp_model * model, init_type init, ThreeTaskSimulation * simulation) {
  // resets activation
  // resets weights
  // persist_taskdemand_activation sets proportion of TD activation to carry over to
  // next trial ie. .20 = 20% of activation on previous

  pdp_layer *input_0, *input_1, *input_2, *output_0, *output_1, *output_2, *taskdemand, *topdown_control, *conflict, *conflict_input, *conflict_total;



  double initial_activation_in_0[2] = {0.0, 0.0};
  double initial_activation_in_1[2] = {0.0, 0.0};
  double initial_activation_in_2[2] = {0.0, 0.0};

  double initial_activation_out_0[2] = {0.0, 0.0};
  double initial_activation_out_1[2] = {0.0, 0.0};
  double initial_activation_out_2[2] = {0.0, 0.0};
  double initial_activation_topdown_control[3] = {0.0, 0.0, 0.0};
  double initial_activation_taskdemand[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict_input[3] = {0.0, 0.0, 0.0};
  double initial_activation_conflict_total[3] = {0.0, 0.0, 0.0};
  int i;
  double squashing_param, rsi_scale_param, conflict_squashing_param;
  hebbian_learning_persistence hebb_persist;

  // zero cycle counter
  model->cycle = 0;


  input_0 = pdp_model_component_find (model, ID_INPUT_0)->layer;
  input_1 = pdp_model_component_find (model, ID_INPUT_1)->layer;
  input_2 = pdp_model_component_find (model, ID_INPUT_2)->layer;
  output_0 = pdp_model_component_find (model, ID_OUTPUT_0)->layer;
  output_1 = pdp_model_component_find (model, ID_OUTPUT_1)->layer;
  output_2 = pdp_model_component_find (model, ID_OUTPUT_2)->layer;
  taskdemand = pdp_model_component_find (model, ID_TASKDEMAND)->layer;
  conflict = pdp_model_component_find (model, ID_CONFLICT)->layer;
  conflict_input = pdp_model_component_find (model, ID_CONFLICT_INPUT)->layer;
  topdown_control = pdp_model_component_find (model, ID_TOPDOWNCONTROL)->layer;
  conflict_total = pdp_model_component_find (model, ID_CONFLICT_TOTAL)->layer;


  /* Reset Weights */
  // check to make sure this effectively zeroes weights...
  if (init == BLOCK) {
    hebb_persist = *(int *)g_hash_table_lookup(simulation->model_params_htable, "hebb_persist");
    printf ("three_task_model_koch_conflict_reinit retrieved Hebbian Persistence parameter, = %d\n", hebb_persist);
    three_task_model_reset_weights(model, hebb_persist);

    // reinit the strategic adaptation variables
    model->last_trial_cum_conflict = 0;
    model->last_trial_weight_change = 1.0;

    double conflict_taskdemand_wt = *(double *)g_hash_table_lookup(simulation->
								   model_params_htable,
								   "conflict_taskdemand_wt");
    double wts_conflict_taskdemand_matrix[3][3] = {
      { conflict_taskdemand_wt, 0.0, conflict_taskdemand_wt },
      { conflict_taskdemand_wt, conflict_taskdemand_wt, 0.0 },
      { 0.0, conflict_taskdemand_wt, conflict_taskdemand_wt },
    };

    pdp_layer *task_demand;
    task_demand = pdp_model_component_find (model, ID_TASKDEMAND)->layer;
    pdp_weights_set (pdp_input_find (task_demand, ID_CONFLICT)->input_weights, 
		     3, 3, wts_conflict_taskdemand_matrix);

    
  }

  // Get and squash TD activations
  if (init == TRIAL) {

    conflict_squashing_param = *(double *)g_hash_table_lookup(simulation->model_params_htable, 
							      "conflict_squashing_param");
    squashing_param = *(double *)g_hash_table_lookup(simulation->model_params_htable, 
						     "squashing_param");
    rsi_scale_param = *(double *)g_hash_table_lookup(simulation->model_params_htable, 
						     "rsi_scale_param");
    printf ("\nsquashing TD activations by (1 - %2.1f ) ^ %2.1f:\t", squashing_param, rsi_scale_param);
    
    for (i = 0; i < 3; i++) {
      initial_activation_taskdemand[i] = taskdemand->units_latest->activations[i] *
	pow(1-squashing_param, rsi_scale_param);
      printf ("%4.2f -> %4.2f\t", taskdemand->units_latest->activations[i], initial_activation_taskdemand[i]);
    }
    printf ("\n");

    // Squash conflict activations
    printf ("\nsquashing Conflict activations by (1 - %2.1f ) ^ %2.1f:\t", 
	    conflict_squashing_param, rsi_scale_param);
    
    for (i = 0; i < 3; i++) {
      initial_activation_conflict[i] = conflict->units_latest->activations[i] *
	pow(1-conflict_squashing_param, rsi_scale_param);
      printf ("%4.2f -> %4.2f\t", conflict->units_latest->activations[i], initial_activation_conflict[i]);
    }
    printf ("\n");
  }
 
  pdp_layer_set_activation_starting(taskdemand, 3, initial_activation_taskdemand);
  pdp_layer_set_activation_starting(conflict, 3, initial_activation_conflict);

  // clear & free history


  pdp_model_component * comp_i;
  for (comp_i = model->components; comp_i != NULL; comp_i = comp_i->next) {
    pdp_units_free (comp_i->layer->units_initial.next);
    comp_i->layer->units_latest = &(comp_i->layer->units_initial);
    comp_i->layer->units_initial.next = NULL;
  }


  /* set initial activation */
  pdp_layer_set_activation(input_0, 2, initial_activation_in_0);
  pdp_layer_set_activation(input_1, 2, initial_activation_in_1);
  pdp_layer_set_activation(input_2, 2, initial_activation_in_2);
  pdp_layer_set_activation(output_0, 2, initial_activation_out_0);
  pdp_layer_set_activation(output_1, 2, initial_activation_out_1);
  pdp_layer_set_activation(output_2, 2, initial_activation_out_2);
  pdp_layer_set_activation(topdown_control, 3, initial_activation_topdown_control);
  pdp_layer_set_activation(conflict_input, 3, initial_activation_conflict_input);


  return 0;
}


void init_model_koch_conflict_simulation (pdp_model * this_model, GHashTable *model_params_htable) {
  // just allocate memory for simulation and run constructors  


  act_func_params * activation_parameters = g_malloc (sizeof(act_func_params));
  activation_parameters->type = ACT_GS;
  activation_parameters->params.gs.step_size = *(double *)g_hash_table_lookup(model_params_htable, "step_size");
  activation_parameters->params.gs.act_max = *(double *)g_hash_table_lookup(model_params_htable, "activation_max");
  activation_parameters->params.gs.act_min = *(double *)g_hash_table_lookup(model_params_htable, "activation_min");
  
  this_model->activation_parameters = activation_parameters;

  // now create the model
  printf ("in init_model_koch_conflict_simulation, creating dummy model\n");
  three_task_model_koch_conflict_build (this_model, model_params_htable); 
}



