#ifndef triple_task_procedure_h
#define triple_task_procedure_h

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gsl/gsl_rng.h>
#include "3task_import.h"
#include "pdp_objects.h"


typedef struct triple_task_simulation {

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

} TripleTaskSimulation;


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


bool triple_task_model_parameter_import (gchar* param_name, gchar* param_value, GHashTable *model_params_ht);

void triple_task_model_parameters_import_commit (FileData *config_file, 
						 GHashTable *model_params);

void import_three_task_trial_data_to_treestore (GtkTreeStore * store, 
					   GtkTreeIter *iter, 
					   ThreeTaskData* data);

void triple_task_task_import_commit (FileData *task_config_file, 
				     GtkTreeStore *task_store);

FileData * triple_task_create_task_import_objects();

TripleTaskSimulation * create_simulation ();

void free_simulation (TripleTaskSimulation * simulation);

#endif
