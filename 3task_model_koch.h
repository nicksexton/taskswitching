#ifndef three_task_model_koch_h
#define three_task_model_koch_h

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
#define ID_CONFLICT 9
#define ID_CONFLICT_INPUT 10
#define ID_CONFLICT_TOTAL 11 // accumulators for tracking cumulative conflict


typedef enum {NO_CONFLICT, ALLOW, CLIP, RESCALE} conflict_negative_options; 
/*  off = no conflict, 
   allow = allow negative conflict, 
   clip = clip below zero conflict, 
   rescale = rescale -1 to 1 as 0-1 */

void three_task_koch_conflict_parameters_htable_set_default (GHashTable * params_table);

bool three_task_model_koch_conflict_parameter_import_ht (gchar* param_name, 
							 gchar* param_value, 
							 GHashTable *model_params_ht);

void three_task_koch_conflict_parameters_import_commit (FileData *config_file, 
							GHashTable *model_params);

int three_task_model_koch_strategic_adaptation (pdp_model * model);
int three_task_model_koch_conflict_run (pdp_model * model, ThreeTaskSimulation * simulation);

int three_task_koch_pdp_layer_cycle_inputs (pdp_layer * some_layer, conflict_negative_options conflict_negative);

void three_task_koch_model_cycle (pdp_model * some_model, conflict_negative_options conflict_negative);

int three_task_model_koch_conflict_run_step (pdp_model * model, 
					     const gsl_rng * random_generator, 
					     double response_threshold, 
					     FILE * fp_trial,
					     FILE * fp_act,
					     gchar * path,
					     conflict_negative_options conflict_negative);

void three_task_model_koch_fprintf_current_state (pdp_model *model, gchar *path, FILE * ofp_act);

int three_task_model_koch_conflict_build (pdp_model * model, GHashTable * model_params);

int three_task_model_koch_conflict_reinit (pdp_model * model, init_type init, ThreeTaskSimulation * simulation);

void init_model_koch_conflict_simulation (pdp_model * this_model, GHashTable *model_params_htable);
// void deinit_model (pdp_model * this_model);

#endif
