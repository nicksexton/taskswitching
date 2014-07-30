
#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"

#define DATAFILE "3task_test.txt"
// defined elsewhere? 3task_basic.c for now


// main function - basic test version of the model


int main (int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  ThreeTaskSimulation * simulation = create_simulation();
  simulation->datafile = fopen (DATAFILE, "a");
  FileData *param_config_file, *task_config_file;

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);

  // build the model
  printf ("in main, building the model\n");
  init_model_simulation (simulation->model, simulation->model_params_htable); 
                                                                   // see gs_stroop_model for example
  // define function pointer to model_run function 
  int (*model_run)(pdp_model*, ThreeTaskSimulation*);
  model_run = &three_task_model_dummy_run; // (model-specific, def in 3task_model_gs.c)

  int (*model_reinit)(pdp_model*, init_type, ThreeTaskSimulation*);
  model_reinit = &three_task_model_dummy_reinit; // (model-specific, def in 3task_model_gs.c)

  // Import parameters
  param_config_file = create_param_import_objects();
  pdp_import_select_file ("3task_model_gs_params_default.conf", param_config_file);
  pdp_load_from_file_short (param_config_file);
  three_task_gs_parameters_import_commit (param_config_file, simulation->model_params_htable);

  // Import tasks
  task_config_file = triple_task_create_task_import_objects();
  pdp_import_select_file ("3task_import_test.conf", task_config_file);
  pdp_load_from_file_long (task_config_file);
  triple_task_task_import_commit (task_config_file, simulation->task_store);


  // Now do stuff with the model and the parameters - eg. run all blocks?
  //  procedure_run_all_blocks (simulation->model, simulation, model_run(simulation->model, simulation));
  procedure_run_all_blocks (simulation->model, simulation, model_run, model_reinit);


  // free memory
  free (simulation->model->activation_parameters); 
  pdp_model_free (simulation->model);  
  fclose(simulation->datafile);
  free_simulation (simulation);
  g_free (param_config_file);
  g_free (task_config_file);
  
  return 0;
}
