#include <ctype.h> // for processing command line arguments
#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"

#define DATAFILE "3task_data.txt"
#define DATAFILE_ACT "3task_act.txt"
// defined elsewhere? 3task_basic.c for now


// main function - basic test version of the model


int main (int argc, char *argv[]) {

  int c;
  char *model_confname = NULL, *task_confname = NULL;

  // process command line arguments (ie specify conf files)
    while ((c = getopt (argc, argv, "m:t:")) != -1) {
      switch (c) 
	{
	case 'm':
	  model_confname = optarg;
	  break;
	case 't':
	  task_confname = optarg;
	  break;
	case '?':
	  if (optopt == 'm' || optopt  == 't') {
	    printf ("option -%c requires name of a .conf file as an argument\n", optopt);
	  }
	  else if (isprint (optopt))
	    printf ("Unknown option -%c\n", optopt);
	  else
	    printf ("Unknown option character \\x%x \n", optopt);
	  return 1;
	}
    }


  gtk_init (&argc, &argv);

  ThreeTaskSimulation * simulation = create_simulation();
  simulation->datafile = fopen (DATAFILE, "a");
  simulation->datafile_act = fopen (DATAFILE_ACT, "a");
  FileData *param_config_file, *task_config_file;

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);


  // define function pointer to model_run function 
  int (*model_run)(pdp_model*, ThreeTaskSimulation*);
  model_run = &three_task_model_dummy_run; // (model-specific, def in 3task_model_gs.c)

  int (*model_reinit)(pdp_model*, init_type, ThreeTaskSimulation*);
  model_reinit = &three_task_model_dummy_reinit; // (model-specific, def in 3task_model_gs.c)

  // Import parameters
  param_config_file = create_param_import_objects();
  if (model_confname != NULL) 
    pdp_import_select_file (model_confname, param_config_file);
  else 
    pdp_import_select_file ("3task_model_gs_params_default.conf", param_config_file);
  pdp_load_from_file_short (param_config_file);
  three_task_gs_parameters_import_commit (param_config_file, simulation->model_params_htable);

  // Import tasks
  task_config_file = triple_task_create_task_import_objects();
  if (task_confname != NULL) 
    pdp_import_select_file (task_confname, task_config_file);
  else
    pdp_import_select_file ("3task_import_test.conf", task_config_file);
  pdp_load_from_file_long (task_config_file);
  triple_task_task_import_commit (task_config_file, simulation->task_store);

  // build the model
  printf ("in main, building the model\n");
  init_model_simulation (simulation->model, simulation->model_params_htable); 

  // Now do stuff with the model and the parameters - eg. run all blocks?
  //  procedure_run_all_blocks (simulation->model, simulation, model_run(simulation->model, simulation));
  procedure_run_all_blocks (simulation->model, simulation, model_run, model_reinit);


  // free memory
  free (simulation->model->activation_parameters); 
  pdp_model_free (simulation->model);  
  fclose(simulation->datafile);
  fclose(simulation->datafile_act);
  free_simulation (simulation);
  g_free (param_config_file);
  g_free (task_config_file);
  
  return 0;
}
