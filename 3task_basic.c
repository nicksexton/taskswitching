
#include "pdp_objects.h"
#include "pdp_procedure.h"
#include "pdp_import.h"
#include "3task_model_gs.h" // model





// main function - basic test version of the model


int main (int argc, char *argv[]) {

  PdpSimulation * simulation = create_simulation();

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);

  // now build the model
  init_model (simulation->model, simulation->model_params_htable); // needs to be defined in 3_task_model.c
                                                                   // see gs_stroop_model for example
  param_config_file = create_param_import_objects();
  task_config_file = create_task_import_objects();

  // =================================================
  // do the task/param import into the simulation here
  // =================================================


  // free memory
  pdp_model_free (simulation->model);  
  free_simulation (objects->simulation);
  g_free (objects->param_config_file);
  g_free (objects->task_config_file);

}
