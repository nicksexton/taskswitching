
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"




// main function - basic test version of the model


int main (int argc, char *argv[]) {

  TripleTaskSimulation * simulation = create_simulation();

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);

  // now build the model
  init_model (simulation->model, simulation->model_params_htable); // needs to be defined in 3_task_model.c
                                                                   // see gs_stroop_model for example
  //  param_config_file = create_param_import_objects();
  FileData * task_config_file = triple_task_create_task_import_objects();
  pdp_import_select_file ("task_import_test.conf", task_config_file);

  // do the task/param import into the simulation here
  pdp_load_from_file_long (task_config_file);
  triple_task_task_import_commit (task_config_file, simulation->task_store);


  // free memory
  pdp_model_free (simulation->model);  
  free_simulation (simulation);
  // g_free (objects->param_config_file);
  g_free (task_config_file);
  
  return 0;
}
