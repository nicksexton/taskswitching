#include <stdio.h>
#include <string.h>
#include <math.h>
#include "3task_procedure.h" // for init_type typedef
#include "3task_model_gs.h"
#include "3task_default_params.h"


#define ID_INPUT_0 1
#define ID_INPUT_1 2
#define ID_INPUT_2 3
#define ID_OUTPUT_0 4
#define ID_OUTPUT_1 5
#define ID_OUTPUT_2 6
#define ID_TASKDEMAND 7
#define ID_TOPDOWNCONTROL 8

#define ECHO


void three_task_parameters_htable_set_default (GHashTable * params_table) {

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

  double * rsi_scale_param = g_malloc(sizeof(double));
  *rsi_scale_param = RSI_SCALE_PARAM;
  g_hash_table_insert (params_table, "rsi_scale_param", rsi_scale_param);

  double * noise = g_malloc(sizeof(double));
  *noise = NOISE;
  g_hash_table_insert (params_table, "noise", noise);

  double * bias_outputunit = g_malloc(sizeof(double));
  *bias_outputunit = OUTPUTUNIT_BIAS;
  g_hash_table_insert (params_table, "bias_outputunit", bias_outputunit);

  double * bias_taskdemand = g_malloc(sizeof(double));
  *bias_taskdemand = TASKDEMAND_BIAS;
  g_hash_table_insert (params_table, "bias_taskdemand", bias_taskdemand);

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


bool three_task_model_parameter_import_ht (gchar* param_name, gchar* param_value, GHashTable *model_params_ht) {

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




void three_task_gs_parameters_import_commit (FileData *config_file, 
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
    three_task_model_parameter_import_ht (param_name, param_value, model_params);

    g_free (param_name);
    g_free (param_value);
    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(config_file->tree_store), &iter);
  }  

}


void three_task_model_gs_run (pdp_model * model, ThreeTaskSimulation * simulation) {

  printf ("model_run is running model!\n");

}







int three_task_model_dummy_run (pdp_model * model,  
				ThreeTaskSimulation * simulation) {

  printf ("in three_task_model_dummy_run, running model\n");
// init inputs

  double input_0_initial_act[2]   = { 0.0,  0.0 };
  double input_1_initial_act[2]   = { 0.0,  0.0 };
  double input_2_initial_act[2]   = { 0.0,  0.0 };
  double topdown_control_initial_act[3]   = { 0.0,  0.0, 0.0 };
  double response_threshold, learning_rate;
  hebbian_learning_persistence hebb_persist;
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
    // fp = fopen (DATAFILE, "a");
    int trial_id, cue, stim_0, stim_1, stim_2;

    // now get params for current trial
    path = gtk_tree_path_to_string(simulation->current_trial_path);
    fprintf (simulation->datafile, "%s\t", path);
    g_free (path);

    gtk_tree_model_get (GTK_TREE_MODEL(simulation->task_store), iter, 
			COL_TASK_ID, &trial_id,
			COL_TASK_PATTERN_1, &cue, 
			COL_TASK_PATTERN_2, &stim_0,
			COL_TASK_PATTERN_3, &stim_1,
			COL_TASK_PATTERN_4, &stim_2,
			-1);

    // print trial params to output file
    fprintf (simulation->datafile, "%d\t", trial_id);
    //    fprintf (fp, "%d\t", trial_data->trial_type);
    fprintf (simulation->datafile, "%d\t", cue);
    fprintf (simulation->datafile, "%d\t", stim_0);
    fprintf (simulation->datafile, "%d\t", stim_1);
    fprintf (simulation->datafile, "%d\t", stim_2);
    //    fprintf (fp, "%d\t", trial_data->response);
    //    fprintf (fp, "%d\t", trial_data->response_time);
    //    fprintf (fp, "\n");

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
  // NOTE this diverges from real version of model

  response_threshold = *(double *)g_hash_table_lookup(simulation->model_params_htable, "response_threshold");
  learning_rate = *(double *)g_hash_table_lookup(simulation->model_params_htable, "learning_rate");
  hebb_persist = *(int *)g_hash_table_lookup(simulation->model_params_htable, 
					     "hebb_persist");

  // run_model_step returns true when stopping_condition evaluates to false
  do {
    three_task_model_dummy_run_step (model, simulation->random_generator, 
				     response_threshold, 
				     simulation->datafile);
    stopped = stopping_condition (model, response_threshold);
  } while (stopped == false && model->cycle < MAX_CYCLES);
  

  three_task_model_update_weights(model,
				  learning_rate,
				  hebb_persist);

  // Print activation of output units


  // Print RT
  fprintf (simulation->datafile, "%d\t", model->cycle); 
  fprintf (simulation->datafile, "%d", stopped); // response
  fprintf (simulation->datafile, "\n");

  return (1);
  }
  

}




// returns 0 (false) if model is still running, or response (true) if it has stopped
int stopping_condition (const pdp_model * model, 
			 double response_threshold) {

  /* evaluates whether model should stop on this cycle and returns
     true/false. CURRENT CRITERION: most active output node is > next
     most active output node that is not the same coloured node */

  // NB code for responses: 1 = task_0:L, 2 = task_0:R, 3 = task_1:L, 4 = task_1:R, 5 = task_2:L, 6 = task_2:R
  // ie. if (abs(response[0] - response[1])) % 2 == 0, then the nodes correspond

  int outputnode;
  int biggest_node[4] = {0, 0, 0, 0}; // stores 3 biggest activations, to compare
                         // [0] vs. [1], or [0] vs. [2] if [0],[1] are same colour, 
                         // or [0] vs. [3] if [0], [1], [2] are all the same colour
  double biggest_node_act[4] = {-1.0, -1.0, -1.0, -1.0};

  int i; // does an insertion sort
  int o; // iterates output layers (0, 1, 2)
  pdp_layer * output_layers[3];

  output_layers[0] = (pdp_model_component_find (model, ID_OUTPUT_0)->layer);  
  output_layers[1] = (pdp_model_component_find (model, ID_OUTPUT_1)->layer);  
  output_layers[2] = (pdp_model_component_find (model, ID_OUTPUT_2)->layer);  


  /* do three output layers */
  for (o = 0; o < 3; o ++) {

    /* outer loop iterates all the output nodes */
    for (outputnode = 0; outputnode < 2; outputnode ++) {
    
      /* inner loop does an insertion sort */
      i = 0;
      while (biggest_node_act[i] > output_layers[o]->units_latest->activations[outputnode] && i < 4) 
	i ++;
      
      if (i == 4) // if unit activation is smaller than activation in 4th slot
	continue;

      /*  if slot is empty, insert the new response right here */
      if (biggest_node[i] == 0) {
	biggest_node[i] = 2*o + outputnode +1; 
	biggest_node_act[i] = output_layers[o]->units_latest->activations[outputnode];
	continue;
      }
	
      /* else, compare size of activations */
      else {
	  /* insert new response here, move everything down */

	switch (i) {
	case 3: 
	  biggest_node[3] = 2*o + outputnode +1; 
	  biggest_node_act[3] = output_layers[o]->units_latest->activations[outputnode];
	  break;
	    
	    
	case 2: 
	  biggest_node[3] = biggest_node[2];
	  biggest_node_act[3] = biggest_node_act[2];
	  
	  biggest_node[2] = 2*o + outputnode +1; 
	  biggest_node_act[2] = output_layers[o]->units_latest->activations[outputnode];
	  
	  break;
	    

	case 1:
	  biggest_node[3] = biggest_node[2];
	  biggest_node_act[3] = biggest_node_act[2];
	      
	  biggest_node[2] = biggest_node[1];
	  biggest_node_act[2] = biggest_node_act[1];
	      
	  biggest_node[1] = 2*o + outputnode +1; 
	  biggest_node_act[1] = output_layers[o]->units_latest->activations[outputnode];


	  break;

	case 0:
	  biggest_node[3] = biggest_node[2];
	  biggest_node_act[3] = biggest_node_act[2];
	  
	  biggest_node[2] = biggest_node[1];
	  biggest_node_act[2] = biggest_node_act[1];
	      
	  biggest_node[1] = biggest_node[0];
	  biggest_node_act[1] = biggest_node_act[0];
	      
	  biggest_node[0] = 2*o + outputnode +1; 
	  biggest_node_act[0] = output_layers[o]->units_latest->activations[outputnode];

	  break;

	default:
	  printf ("stopping condition: weird, no cases seem to match\n");
	} // <-- close switch
      } // < -- close else

    } // <-- outer loop
  }// <- output layers

  // now, we can evaluate stopping condition


  i = 1;
  while (abs(biggest_node[0] - biggest_node[i]) % 2 == 0 && i < 4) {
    i ++;
  }

  if (biggest_node_act[0] - response_threshold > biggest_node_act[i]) {
    //    printf ("stopping condition met, returning %d", biggest_node[0]);
    return (biggest_node[0]);
  }
  
  /*
  for (i = 0; i < 4; i ++) {
    printf ("%d ", biggest_node[i]);
  }
  */
  return 0; 
  
}




// returns true while model is still running (does not satisfy stopping condition), false otherwise
int three_task_model_dummy_run_step (pdp_model * model, 
				      const gsl_rng * random_generator, 
				      double response_threshold, 
				      FILE * fp) {


    pdp_model_cycle (model);


    // add noise to units 
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_0)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_1)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_OUTPUT_2)->layer, 
				  NOISE, random_generator);
    pdp_layer_add_noise_to_units (pdp_model_component_find (model, ID_TASKDEMAND)->layer, 
				    NOISE, random_generator);
    
    

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
    /* Output this to a second logfile?
    pdp_layer_fprintf_current_output (
				      pdp_model_component_find (model, ID_OUTPUT_0)->layer, fp);
    pdp_layer_fprintf_current_output (
				      pdp_model_component_find (model, ID_OUTPUT_1)->layer, fp);
    pdp_layer_fprintf_current_output (
				      pdp_model_component_find (model, ID_OUTPUT_2)->layer, fp);
      
    */

    return 0; 

  
}




int three_task_model_dummy_build (pdp_model * model, GHashTable * model_params) {

  pdp_layer *input_0, *input_1, *input_2, *output_0, *output_1, *output_2, *taskdemand, *topdown_control;


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


  
  double initial_activation_in_0[2] = {0.0, 0.0};
  double initial_activation_in_1[2] = {0.0, 0.0};
  double initial_activation_in_2[2] = {0.0, 0.0};
  double initial_activation_out_0[2] = {0.0, 0.0};
  double initial_activation_out_1[2] = {0.0, 0.0};
  double initial_activation_out_2[2] = {0.0, 0.0};
  double initial_activation_taskdemand[3] = {0.0, 0.0, 0.0};
  double initial_activation_topdown_control[3] = {0.0, 0.0, 0.0};

  
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
  pdp_input_connect (output_0, output_2, wts_out0_out1);

  /* out_1 -> out_0 */
  wts_out1_out0 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out1_out0, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_1, output_0, wts_out1_out0);

  /* out_1 -> out_2 */
  wts_out1_out2 = pdp_weights_create (2,2);
  pdp_weights_set (wts_out1_out2, 2, 2, wts_crossoutputs_matrix);
  pdp_input_connect (output_1, output_2, wts_out1_out0);

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

  pdp_weights_matrix *wts_taskdemand_out0, *wts_taskdemand_out1, *wts_taskdemand_out2;

  double wts_taskdemand_out0_matrix[2][3] = {
    { 2.5, -2.5, -2.5},
    { 2.5, -2.5, -2.5},
  };

  double wts_taskdemand_out1_matrix[2][3] = {
    {-2.5,  2.5, -2.5},
    {-2.5,  2.5, -2.5},
  };

  double wts_taskdemand_out2_matrix[2][3] = {
    {-2.5, -2.5,  2.5},
    {-2.5, -2.5,  2.5},
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

  pdp_weights_matrix *wts_taskdemand_taskdemand;
  double wts_taskdemand_taskdemand_matrix[3][3] = {
    { 0.0, -2.0, -2.0},
    {-2.0,  0.0, -2.0},
    {-2.0, -2.0,  0.0},
  };

  wts_taskdemand_taskdemand = pdp_weights_create (3,3);
  pdp_weights_set (wts_taskdemand_taskdemand, 3, 3, wts_taskdemand_taskdemand_matrix);
  pdp_input_connect (taskdemand, taskdemand, wts_taskdemand_taskdemand);



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

  //debug
  printf ("basic 3 task model created! Limited connectivity, just for test\n");

  return 0;

}


int three_task_model_dummy_reinit (pdp_model * model, init_type init, ThreeTaskSimulation * simulation) {
  // resets activation
  // does not reset weights
  // persist_taskdemand_activation sets proportion of TD activation to carry over to
  // next trial ie. .20 = 20% of activation on previous

  pdp_layer *input_0, *input_1, *input_2, *output_0, *output_1, *output_2, *taskdemand, *topdown_control;



  double initial_activation_in_0[2] = {0.0, 0.0};
  double initial_activation_in_1[2] = {0.0, 0.0};
  double initial_activation_in_2[2] = {0.0, 0.0};

  double initial_activation_out_0[2] = {0.0, 0.0};
  double initial_activation_out_1[2] = {0.0, 0.0};
  double initial_activation_out_2[2] = {0.0, 0.0};
  double initial_activation_topdown_control[3] = {0.0, 0.0, 0.0};
  double initial_activation_taskdemand[3] = {0.0, 0.0, 0.0};
  int i;
  double squashing_param, rsi_scale_param;

  // zero cycle counter
  model->cycle = 0;


  input_0 = pdp_model_component_find (model, ID_INPUT_0)->layer;
  input_1 = pdp_model_component_find (model, ID_INPUT_1)->layer;
  input_2 = pdp_model_component_find (model, ID_INPUT_2)->layer;
  output_0 = pdp_model_component_find (model, ID_OUTPUT_0)->layer;
  output_1 = pdp_model_component_find (model, ID_OUTPUT_1)->layer;
  output_2 = pdp_model_component_find (model, ID_OUTPUT_2)->layer;
  taskdemand = pdp_model_component_find (model, ID_TASKDEMAND)->layer;
  topdown_control = pdp_model_component_find (model, ID_TOPDOWNCONTROL)->layer;


  // Get and squash TD activations
  if (init == TRIAL) {

    squashing_param = *(double *)g_hash_table_lookup(simulation->model_params_htable, "squashing_param");
    rsi_scale_param = *(double *)g_hash_table_lookup(simulation->model_params_htable, "rsi_scale_param");
    printf ("\nsquashing TD activations by (1 - %2.1f ) ^ %2.1f:\t", squashing_param, rsi_scale_param);
    
    for (i = 0; i < 3; i++) {
      initial_activation_taskdemand[i] = taskdemand->units_latest->activations[i] *
	pow(1-squashing_param, rsi_scale_param);
      printf ("%4.2f -> %4.2f\t", taskdemand->units_latest->activations[i], initial_activation_taskdemand[i]);
    }
    printf ("\n");
    pdp_layer_set_activation (taskdemand, 3, initial_activation_taskdemand);
  }

    pdp_layer_set_activation(taskdemand, 3, initial_activation_taskdemand);

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




  return 0;
}


int three_task_model_update_weights (pdp_model * gs_stroop_model, 
				     double learning_rate, 
				     hebbian_learning_persistence persist) {
  // default hebbian persistence (1, NEXT_TRIAL) - weights persist for next trial only
  // NB running this function immediately after initing model SHOULD zero associative weights
  // (only if persist is set to NEXT_TRIAL)

  int i, j;

  if (persist == OFF) {
    return 0;
  }

  pdp_layer *input_0, *input_1, *input_2, *task_demand;
  input_0 = pdp_model_component_find (gs_stroop_model, ID_INPUT_0)->layer;
  input_1 = pdp_model_component_find (gs_stroop_model, ID_INPUT_1)->layer;
  input_2 = pdp_model_component_find (gs_stroop_model, ID_INPUT_2)->layer;
  task_demand = pdp_model_component_find (gs_stroop_model, ID_TASKDEMAND)->layer;
  

  double wts_in0_taskdemand_matrix[3][2] = {
    {0.0, 0.0 },
    {0.0, 0.0 },
    {0.0, 0.0 },
  };

  double wts_in1_taskdemand_matrix[3][2] = {
    {0.0, 0.0 },
    {0.0, 0.0 },
    {0.0, 0.0 },
  };

  double wts_in2_taskdemand_matrix[3][2] = {
    {0.0, 0.0 },
    {0.0, 0.0 },
    {0.0, 0.0 },
  };

  
 
  for (i = 0; i < 3; i ++) { // outer loop, output unit (i)
    for (j = 0; j < 2; j ++) {// inner loop, input unit (j)
      wts_in0_taskdemand_matrix[i][j] = 
	task_demand->units_latest->activations[i] *
	input_0->units_latest->activations[j] * learning_rate;

      wts_in1_taskdemand_matrix[i][j] = 
	task_demand->units_latest->activations[i] *
	input_1->units_latest->activations[j] * learning_rate;

      wts_in2_taskdemand_matrix[i][j] = 
	task_demand->units_latest->activations[i] *
	input_2->units_latest->activations[j] * learning_rate;

    }
  }


  if (persist == THIS_BLOCK || persist == FOREVER) {
    // increment existing weights
    pdp_weights_increment (pdp_input_find (task_demand, ID_INPUT_0)->input_weights, 
			   3, 2, wts_in0_taskdemand_matrix);
    pdp_weights_increment (pdp_input_find (task_demand, ID_INPUT_1)->input_weights, 
			   3, 2, wts_in1_taskdemand_matrix);
    pdp_weights_increment (pdp_input_find (task_demand, ID_INPUT_2)->input_weights, 
			   3, 2, wts_in2_taskdemand_matrix);
  }

  else if (persist == NEXT_TRIAL) {
    pdp_weights_set (pdp_input_find (task_demand, ID_INPUT_0)->input_weights, 
		     3, 2, wts_in0_taskdemand_matrix);
    pdp_weights_set (pdp_input_find (task_demand, ID_INPUT_1)->input_weights, 
		     3, 2, wts_in1_taskdemand_matrix);
    pdp_weights_set (pdp_input_find (task_demand, ID_INPUT_2)->input_weights, 
		     3, 2, wts_in2_taskdemand_matrix);
  }

  return 0;
}




void init_model_simulation (pdp_model * this_model, GHashTable *model_params_htable) {
  // just allocate memory for simulation and run constructors  


  act_func_params * activation_parameters = g_malloc (sizeof(act_func_params));
  activation_parameters->type = ACT_GS;
  activation_parameters->params.gs.step_size = *(double *)g_hash_table_lookup(model_params_htable, "step_size");
  activation_parameters->params.gs.act_max = *(double *)g_hash_table_lookup(model_params_htable, "activation_max");
  activation_parameters->params.gs.act_min = *(double *)g_hash_table_lookup(model_params_htable, "activation_min");
  
  this_model->activation_parameters = activation_parameters;

  // now create the model
  printf ("in init_model, creating dummy model\n");
  three_task_model_dummy_build (this_model, model_params_htable); 
}


void deinit_model (pdp_model * this_model) {
  // delete model components
  // can be re-initialised with init_model

  printf ("in deinit_model, free model\n");

  pdp_model_component_free (this_model->components);
  this_model->components = NULL;


  g_free (this_model->activation_parameters);
}



