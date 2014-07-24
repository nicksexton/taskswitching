#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_procedure.h"

void three_task_parameters_htable_set_default (GHashTable * params_table);
bool three_task_model_parameter_import_ht (gchar* param_name, gchar* param_value, GHashTable *model_params_ht);
void three_task_gs_parameters_import_commit (FileData *config_file, 
					     GHashTable *model_params);

// master 'run model' function
void three_task_model_gs_run (pdp_model * model, ThreeTaskSimulation * simulation);

int three_task_model_dummy_run (pdp_model * model,  
				ThreeTaskSimulation * simulation);

bool three_task_model_dummy_run_step (pdp_model * model, 
				      const gsl_rng * random_generator, 
				      FILE * fp);

int three_task_model_dummy_build (pdp_model * model, GHashTable * model_params);

int three_task_model_dummy_reinit (pdp_model * model, init_type init, ThreeTaskSimulation * simulation);

void init_model_simulation (pdp_model * this_model, GHashTable *model_params_htable);
void deinit_model (pdp_model * this_model);

