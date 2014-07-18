#include <string.h>
#include "3task_model_gs.h"
#include "3task_default_params.h"



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




void init_model (pdp_model * this_model, GHashTable *model_params_htable) {
  // just allocate memory for simulation and run constructors  

  act_func_params * activation_parameters = g_malloc (sizeof(act_func_params));
  activation_parameters->type = ACT_GS;
  activation_parameters->params.gs.step_size = *(double *)g_hash_table_lookup(model_params_htable, "step_size");
  activation_parameters->params.gs.act_max = *(double *)g_hash_table_lookup(model_params_htable, "activation_max");
  activation_parameters->params.gs.act_min = *(double *)g_hash_table_lookup(model_params_htable, "activation_min");
  
  this_model->activation_parameters = activation_parameters;

  // now create the model
  // gs_stroop_model_build (this_model, model_params_htable); 
  printf ("in init_model, create the model\n");
}


void deinit_model (pdp_model * this_model) {
  // delete model components
  // can be re-initialised with init_model

  printf ("in deinit_model, free model\n");
  /*
  pdp_model_component_free (this_model->components);
  this_model->components = NULL;
  */

  g_free (this_model->activation_parameters);
}


