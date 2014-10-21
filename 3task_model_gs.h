#ifndef three_task_model_gs_h
#define three_task_model_gs_h

#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_procedure.h"

#define ID_INPUT_0 1
#define ID_INPUT_1 2
#define ID_INPUT_2 3
#define ID_OUTPUT_0 4
#define ID_OUTPUT_1 5
#define ID_OUTPUT_2 6
#define ID_TASKDEMAND 7
#define ID_TOPDOWNCONTROL 8




typedef struct three_task_response {
  int this_node; 
  // NB code for responses: 1 = task_0:L, 2 = task_0:R, 3 = task_1:L, 4 = task_1:R, 5 = task_2:L, 6 = task_2:R
  double activation;
} ThreeTaskResponse;



void three_task_parameters_htable_set_default (GHashTable * params_table);
bool three_task_model_parameter_import_ht (gchar* param_name, gchar* param_value, GHashTable *model_params_ht);
void three_task_gs_parameters_import_commit (FileData *config_file, 
					     GHashTable *model_params);

// master 'run model' function
void three_task_model_gs_run (pdp_model * model, ThreeTaskSimulation * simulation);

ThreeTaskResponse * make_three_task_response (int node, double activation);

int stopping_condition (const pdp_model * model, 
			 double response_threshold);

int three_task_model_dummy_run (pdp_model * model,  
				ThreeTaskSimulation * simulation);

void three_task_model_fprintf_current_state (pdp_model *model, gchar * path, FILE * fp_act);

int three_task_model_dummy_run_step (pdp_model * model, 
				     const gsl_rng * random_generator, 
				     double response_threshold,
				     FILE * fp, // datafile for trial-by-trial history
				     FILE * fp_act, // datafile for cycle-by-cycle activation history
				     gchar * path); // string of current path, for writing to fp_act

int three_task_model_update_weights (pdp_model * gs_stroop_model, 
				     double learning_rate, 
				     hebbian_learning_persistence persist);

int three_task_model_reset_weights (pdp_model * gs_stroop_model,
				    hebbian_learning_persistence persist);

int three_task_model_dummy_build (pdp_model * model, GHashTable * model_params);

int three_task_model_dummy_reinit (pdp_model * model, init_type init, ThreeTaskSimulation * simulation);

void init_model_simulation (pdp_model * this_model, GHashTable *model_params_htable);
void deinit_model (pdp_model * this_model);

#endif
