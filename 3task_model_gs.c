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
  printf ("in init_model, create model\n");

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
