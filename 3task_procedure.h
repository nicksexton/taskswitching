#ifndef triple_task_procedure_h
#define triple_task_procedure_h

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gsl/gsl_rng.h>
#include "3task_import.h"
#include "pdp_objects.h"

typedef enum {INITIAL, TRIAL, BLOCK} init_type;
typedef enum {OFF, NEXT_TRIAL, THIS_BLOCK, FOREVER} hebbian_learning_persistence;

typedef struct three_task_simulation {

  pdp_model *model; // head of a possible list of models
  gsl_rng * random_generator;

  GHashTable *model_params_htable; // NEW way to store global params

  // current state of the simulation
  int current_subject;
  int current_trial;

  // task store
  // In new version, move toward using the task_store as a repository for trial info
  // subjects becomes a repo for data only (and can maybe be stored in text file?)
  GtkTreeStore * task_store;
  GtkTreePath * current_trial_path;
  GtkTreeIter * current_trial_iter;

  void * current_trial_data;  // should be cast into whatever model-specific data type
  FILE * datafile;

} ThreeTaskSimulation;


typedef struct three_task_data {

  int trial_id;
  int cue; // task type: 0, 1, 2
  int stim_0;
  int stim_1;
  int stim_2;
  int stim_correct_response;
  int response;
  int response_time;

} ThreeTaskData;



gint procedure_current_trial_get (ThreeTaskSimulation *simulation);
gboolean procedure_current_block_is_last (ThreeTaskSimulation *simulation);
gboolean procedure_current_trial_is_last (ThreeTaskSimulation *simulation);


bool procedure_run_current_trial (pdp_model * model, ThreeTaskSimulation * simulation, 
				  int (*model_run)(pdp_model*, ThreeTaskSimulation*),
				  int (*model_reinit)(pdp_model*, init_type init, ThreeTaskSimulation*));

void procedure_run_block (pdp_model * model, ThreeTaskSimulation *simulation, 
			  int (*model_run)(pdp_model*, ThreeTaskSimulation*), 
			  int (*model_reinit)(pdp_model*, init_type init, ThreeTaskSimulation*) );

void procedure_run_all_blocks (pdp_model * model, ThreeTaskSimulation * simulation, 
			       int (*model_run)(pdp_model*, ThreeTaskSimulation*), 
			       int (*model_reinit)(pdp_model*, init_type init, ThreeTaskSimulation*) );


gboolean procedure_change_trial (ThreeTaskSimulation *simulation, GtkTreeStore *store, GtkTreePath *new_trial_path);
void procedure_change_trial_next (ThreeTaskSimulation *simulation);
gboolean procedure_change_trial_first (ThreeTaskSimulation *simulation, 
				       GtkTreeStore *store);
gboolean procedure_change_trial_first_of_block (ThreeTaskSimulation *simulation, 
						GtkTreeStore *store);


bool procedure_print_current_trial_data (ThreeTaskSimulation * simulation);

bool triple_task_model_parameter_import (gchar* param_name, gchar* param_value, GHashTable *model_params_ht);

void triple_task_model_parameters_import_commit (FileData *config_file, 
						 GHashTable *model_params);

void import_three_task_trial_data_to_treestore (GtkTreeStore * store, 
					   GtkTreeIter *iter, 
					   ThreeTaskData* data);

void triple_task_task_import_commit (FileData *task_config_file, 
				     GtkTreeStore *task_store);

FileData * triple_task_create_task_import_objects();

ThreeTaskSimulation * create_simulation ();

void free_simulation (ThreeTaskSimulation * simulation);

#endif
