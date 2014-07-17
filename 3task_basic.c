
#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"





// main function - basic test version of the model


int main (int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  TripleTaskSimulation * simulation = create_simulation();
  FileData *param_config_file, *task_config_file;

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);

  // now build the model
  printf ("in main, building the model\n");
  init_model (simulation->model, simulation->model_params_htable); // needs to be defined in 3_task_model.c
                                                                   // see gs_stroop_model for example

  // Import parameters
  param_config_file = create_param_import_objects();
  pdp_import_select_file ("3task_model_gs_params_default.conf", param_config_file);
  pdp_load_from_file_short (param_config_file);
  three_task_gs_parameters_import_commit (param_config_file, simulation->model_params_htable);

  // Import tasks
  task_config_file = triple_task_create_task_import_objects();
  pdp_import_select_file ("task_import_test.conf", task_config_file);
  pdp_load_from_file_long (task_config_file);
  triple_task_task_import_commit (task_config_file, simulation->task_store);


  // Now do stuff with the model and the parameters - eg. run all blocks?


  // free memory
  printf ("in main, now freeing memory\n");
  pdp_model_free (simulation->model);  
  free_simulation (simulation);
  // g_free (objects->param_config_file);
  g_free (task_config_file);
  
  return 0;
}
