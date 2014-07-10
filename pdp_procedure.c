// Functions and objects for setting up and running a pdp simulation
// ie. presenting a series of trials to a pdp model
// All functions should be Non-gui, model general

#include <stdbool.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>
#include "random_generator_functions.h"
#include "pdp_objects.h"
#include "pdp_import.h"
#include "pdp_procedure.h"


gint model_current_trial_get (PdpSimulation *simulation) {
  
  gint depth = -1;
  gint *current_trial;

  current_trial = gtk_tree_path_get_indices_with_depth (simulation->current_trial_path, &depth);

  // printf ("model_current_trial_get, returning depth %d", depth);

  if (depth > -1) {
    // printf ("trial %d\n", current_trial[depth-1]);
    return current_trial[depth-1];
  }

  else {
    // printf ("\n");
    return -99;
  }
}


gboolean model_current_block_is_last (PdpSimulation *simulation) {

  int num_blocks;
  gint * trial_path;
  // get number of top level nodes
  num_blocks = gtk_tree_model_iter_n_children (GTK_TREE_MODEL(simulation->task_store), NULL); 
  
  if (num_blocks == 0) {
    printf ("no blocks, is the task store empty?");
    return true;
  }
  else {

   trial_path = gtk_tree_path_get_indices (simulation->current_trial_path);
   if (trial_path == NULL) {
     return true;
   }
   
   else if (trial_path[0] == num_blocks) {
     printf ("last block!\n");
     return true;
   }
   else return false;

  }
}


// returns false if there are no trials
gboolean model_current_trial_is_last (PdpSimulation *simulation) {

  // get number of children of current parent
  GtkTreeIter *current_trial = g_malloc (sizeof(GtkTreeIter));
  GtkTreeIter *parent = g_malloc (sizeof(GtkTreeIter));
  int num_trials;
  int this_trial;

  if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(simulation->task_store), 
			      current_trial, 
			      simulation->current_trial_path)){
    printf ("error in model_current_trial_is_last, there appear to be no trials loaded \n"); 
    return false;
  }
  else {
    if (!gtk_tree_model_iter_parent (GTK_TREE_MODEL(simulation->task_store),
				     parent,
				     current_trial)) {
      printf ("error in model_current_trial_is_last, " 
	      "current_trial appears to be top level (ie block header\n");
      return false;
    }
    else {
      num_trials = gtk_tree_model_iter_n_children (GTK_TREE_MODEL(simulation->task_store),
						   parent);
      this_trial = model_current_trial_get (simulation);
      // if (this_trial < (num_trials - 1)) {
      if ((this_trial + 1) < num_trials) {
	// printf ("%dth trial of %d, continuing\n", (this_trial + 1), num_trials);
	return false;
      }
      else {
	// printf ("current trial is last one\n");
	return true;
      }
    }
  }
}




PdpSimulation * create_simulation () {
  // just allocate memory for simulation and run constructors

  PdpSimulation *simulation = g_malloc (sizeof(PdpSimulation));




  simulation->random_generator = random_generator_create();

  // allocate memory for model parameters and set to default values
  simulation->model_params_htable = g_hash_table_new (g_str_hash, g_str_equal); // NEW way to store global params

  simulation->current_subject = 0;
  simulation->current_trial = 0;


  // set up the task store

  simulation->task_store = gtk_tree_store_new (N_TASK_COLUMNS,
					       G_TYPE_STRING,
					       G_TYPE_INT,
					       G_TYPE_INT, 
					       G_TYPE_INT, 
					       G_TYPE_INT, 
					       G_TYPE_STRING, 
					       G_TYPE_STRING);

  simulation->current_trial_data = NULL;
  simulation->current_trial_path = gtk_tree_path_new_first();

  return simulation;
}




void free_simulation (PdpSimulation * simulation) {
  // free memory for simulation

  free (simulation->model->activation_parameters); 
  g_hash_table_destroy(simulation->model_params_htable); // NEW 
  //  subject_popn_free (simulation->subjects);
  random_generator_free (simulation->random_generator);  

  g_free (simulation->current_trial_data);
  gtk_tree_path_free(simulation->current_trial_path);
  g_free (simulation);

}
