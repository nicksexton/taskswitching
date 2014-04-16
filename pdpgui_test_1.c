#define NUMBER_OF_SUBJECTS 1
// DATAFILE currently defined in gs_stroop_global_params.h

// inits a model with basic controls to run it
#include <stdbool.h>
#include <gtk/gtk.h>
#include "gs_stroop.h"
#include "pdp_objects.h"

#include <gsl/gsl_randist.h>
#include "random_generator_functions.h" // for gaussian noise
#include "gs_stroop_subjects.h"
#include "gs_stroop_global_params.h"
#include "gs_stroop_analyse.h"

#include "pdpgui_plot.h"
#include "pdpgui_import.h"
#include "pdpgui.h"
#include "pdpgui_test_1.h"

// temp!
// #include <math.h>


int pdpgui_print_current_trial_data (PdpSimulation * simulation) {
  FILE *fp;
  char *path;

  // first check response is not -666 (init value)

  if (simulation->current_trial_data->response == -666) {
    printf ("error! pdpgui_print_current_trial_data but current_trial_data not run (-666)\n");
    return (1);
  }
  else {
    // open file pointer here on new line at end of file (append)
    fp = fopen (DATAFILE, "a");

    // fprintf (fp, "\t"); // block ID
    // print path
    path = gtk_tree_path_to_string(simulation->current_trial_path);
    fprintf (fp, "%s\t", path);
    g_free (path);
    
    // print data
    gs_stroop_print_trial_data (fp, simulation->current_trial_data);
    fprintf (fp, "\n");
    fclose(fp);
    return 0;
    
  }
}



// takes an iter pointing to relevant row of task store, 
// returns a pointer to UNINITIALIZED stroop_trial_data,
int make_stroop_trial_data_from_task_store (GtkTreeStore *store, GtkTreeIter *trial, stroop_trial_data * data) {

  /*
  // free data buffer if it is not already free (null)
  if (data != NULL) {
    printf ("error, current trial buffer not set to null, new current trial not set");
    return 1;
  }

  else if (trial == NULL) {
    printf ("error, current trial iter is null, new current trial not set");
    return 1;
  }

  if (store == NULL) {
    printf ("error, task store is null, new current trial not set");
    return 1;
  }
  */

  
  // handle case where patterns are expressed as vectors

  // handle case where patterns are expressed as ints
  gtk_tree_model_get (GTK_TREE_MODEL(store), trial, 
		      COL_TASK_ID, &(data->trial_id),
		      COL_TASK_PATTERN_1, &(data->stim_word),
		      COL_TASK_PATTERN_2, &(data->stim_colour),
		      COL_TASK_PATTERN_3, &(data->stim_task),
		      -1);

  // stim_task is word naming if =0, colour otherwise 
  (data->stim_task == 0) ? 
    (data->stim_correct_response = data->stim_word) : 
    (data->stim_correct_response = data->stim_colour);


  if (data->stim_word == -1 || data->stim_colour == -1) { data->trial_type = NEUTRAL; }
  else if (data->stim_word == data->stim_colour ) { data->trial_type = CONGRUENT; }
  else data->trial_type = INCONGRUENT;

  // set responses to init values
  data->response = -666;
  data->response_time = -666;

  return 0;
}




void pdpgui_plot_network_activation (GtkWidget *widget, 
				     cairo_t *cr, 
				     PdpSimulation *simulation) {


  guint widget_width, widget_height;
  widget_width = gtk_widget_get_allocated_width (GTK_WIDGET(widget));
  widget_height = gtk_widget_get_allocated_height (GTK_WIDGET(widget));

  printf ("%d x %d\n", widget_width, widget_height);

  pdpgui_draw_graph_axes(cr, widget_width, widget_height, 10, 10, 
			 0.0, simulation->model->cycle * 1.0, 
			 -1.0, 0.5);


  PdpguiAxisDimensions axes = { 
    .x_min = 0.0, 
    .x_max = simulation->model->cycle * 1.0, 
    .y_min = -1.0, 
    .y_max = 0.5
  };

  PdpguiColourRgb plot_colour[3] = {{ 
      .r = 1.0, 
      .g = 0.0, 
      .b = 0.0 
    }, {
      .r = 0.0, 
      .g = 6.0, 
      .b = 0.0 
    }, {
      .r = 0.0, 
      .g = 0.0, 
      .b = 1.0 
    }};

  PdpguiColourRgb plot_td_mono[2] = {{ 
      .r = 0.5, 
      .g = 0.5, 
      .b = 0.5 
    }, {
      .r = 0.5, 
      .g = 0.5, 
      .b = 0.5 
    }};


  // now construct an arbitrary vector;
  double * units_activation; // check this doesn't overflow??


  // <------------ FIRST PLOT WORDOUT UNITS ------------
  pdp_layer * current_layer = pdp_model_component_find (simulation->model, 
							ID_WORDOUT)->layer; 
  int unit;


  for (unit = 0; unit < current_layer->size; unit ++) {

    units_activation = 
      pdp_layer_get_unit_activation_history (current_layer, unit, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
      break;
    }
    else {
      pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
			simulation->model->cycle, units_activation, &(plot_colour[unit]));

      // want more sophisticated rendering algorithm using buffering, 
      // this will probably cause flicker

      free(units_activation);
    }
  }


  // <------------ NEXT PLOT COLOUROUT UNITS
  current_layer = pdp_model_component_find (simulation->model, 
							ID_COLOUROUT)->layer; 

  for (unit = 0; unit < current_layer->size; unit ++) {

    units_activation = 
      pdp_layer_get_unit_activation_history (current_layer, unit, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
      break;
    }
    else {
      pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
			       simulation->model->cycle, units_activation, 
			       &(plot_colour[unit]));

      // want more sophisticated rendering algorithm using buffering, 
      // this will probably cause flicker

      free(units_activation);
    }
  }


  // <------------ NEXT PLOT TASKDEMAND UNITS  ------------------

  current_layer = pdp_model_component_find (simulation->model, 
							ID_TASKDEMAND)->layer; 

  // word task demand unit
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 0, simulation->model->cycle);
  
  if (units_activation == NULL) {
    free(units_activation);
  }
  else {
    pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 
		      simulation->model->cycle, units_activation, 
		      &(plot_td_mono[0]));
  }
    // colour task demand unit
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 1, simulation->model->cycle);
  
    if (units_activation == NULL) {
      free(units_activation);
    }
    else {
      pdpgui_plot_vector_dashed (cr, widget_width, widget_height, &axes, 
				 simulation->model->cycle, units_activation, 
				 &(plot_td_mono[1]));
      free(units_activation);
    }
  
}



static GtkWidget* 
create_sub_notepage_model_plot_activation (PdpGuiObjects * objects) {
  // plots network activation for current trial


  GtkWidget *grid;
  GtkWidget *drawing_area;
  GtkWidget *label;

  label = gtk_label_new("Network activation graph here");
  //  gtk_widget_set_hexpand (label, TRUE);
  //  gtk_widget_set_vexpand (label, TRUE);


  drawing_area = gtk_drawing_area_new();
  g_signal_connect (drawing_area, "draw", 
		    G_CALLBACK(pdpgui_plot_network_activation), objects->simulation);
  gtk_widget_set_hexpand (drawing_area, TRUE);
  gtk_widget_set_vexpand (drawing_area, TRUE);


  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area, 0, 1, 1, 1);

  gtk_widget_show_all(grid);
  return (grid);
}


static GtkWidget* 
create_sub_notepage_model_display_architecture (PdpGuiObjects * objects) {

  GtkWidget *grid;
  GtkWidget *label;

  label = gtk_label_new("Network architecture diagram here");
  gtk_widget_set_hexpand (label, TRUE);
  gtk_widget_set_vexpand (label, TRUE);

  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);


  gtk_widget_show_all(grid);
  return (grid);

}


static void model_headerbar_update_labels (PdpGuiObjects * objects) {

  char textbuf[100];

  gchar* trial;

  trial = gtk_tree_path_to_string (objects->simulation->current_trial_path); 

  // printf ("updating headerbar with current trial path: %s\n", trial);

  // update subject label
  sprintf (textbuf, "Subject: %d", objects->simulation->current_subject);
  gtk_label_set_text (GTK_LABEL(objects->model_headerbar_label_subject), textbuf);
 
  // update trial label
  sprintf (textbuf, "Trial: %s", trial);
  gtk_label_set_text (GTK_LABEL(objects->model_headerbar_label_trial), textbuf);

  // update trial data label
  stroop_trial_data_print_as_string (textbuf, 100, 
				     objects->simulation->current_trial_data); 
  gtk_label_set_text (GTK_LABEL(objects->model_headerbar_label_trial_data), textbuf);

  // update spin button
  gtk_spin_button_set_value (GTK_SPIN_BUTTON(objects->model_headerbar_spin_trial), 
  			     model_current_trial_get(objects->simulation));

  g_free(trial);

}


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
	printf ("%dth trial of %d, continuing\n", (this_trial + 1), num_trials);
	return false;
      }
      else {
	printf ("current trial is last one\n");
	return true;
      }

    }
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


static gboolean model_change_trial (PdpSimulation *simulation, GtkTreeStore *store, GtkTreePath *new_trial_path) {

  // get iter to new path
  GtkTreeIter *iter = g_malloc (sizeof(GtkTreeIter));


  if (gtk_tree_model_get_iter(GTK_TREE_MODEL(simulation->task_store), iter, new_trial_path)) {


    // make stroop trial data
    g_free (simulation->current_trial_data);
    simulation->current_trial_data = g_malloc (sizeof(stroop_trial_data));

    make_stroop_trial_data_from_task_store (simulation->task_store, 
					    iter, 
					    simulation->current_trial_data);

    // update the path
    gtk_tree_path_free (simulation->current_trial_path);
    simulation->current_trial_path = new_trial_path;
    
    return TRUE;
  }

  else {
    g_free (iter);
    printf ("error! model_change_trial failed to acquire valid iter from current_trial_path, returning FALSE\n");
    return FALSE;
  }

}


// nb function as-is does not update current_data, just sets the path
static void model_change_trial_next (PdpSimulation *simulation) {

  GtkTreeIter *iter = g_malloc (sizeof(GtkTreeIter));

  // check next trial exists:
  gtk_tree_model_get_iter(GTK_TREE_MODEL(simulation->task_store), iter, 
			  simulation->current_trial_path);

  if (gtk_tree_model_iter_next (GTK_TREE_MODEL(simulation->task_store), iter)) {
  
    // gtk_tree_path_free(simulation->current_trial_path);
    // simulation->current_trial_path = gtk_tree_model_get_path (GTK_TREE_MODEL(simulation->task_store)
    //								     , iter);
  
    // advance the trial path
    gtk_tree_path_next(simulation->current_trial_path);

    // make stroop trial data
    g_free (simulation->current_trial_data);
    simulation->current_trial_data = g_malloc (sizeof(stroop_trial_data));

    make_stroop_trial_data_from_task_store (simulation->task_store, 
					    iter, 
					    simulation->current_trial_data);

  }

  printf ("new current trial path: %s\n", gtk_tree_path_to_string (simulation->current_trial_path));
  
}


// handle callback from change trial spin_button 
static void model_change_trial_cb (GtkWidget * spin_button, 
				   PdpGuiObjects * objects) {


  int new_trial;
  int current_trial_block;
  GtkTreePath *new_path;
  gchar *old_path_str;
  gchar *new_path_str;


  old_path_str = gtk_tree_path_to_string (objects->simulation->current_trial_path);
  printf ("in model_change_trial_cb, old path: %s\t", old_path_str);

  new_trial = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spin_button));
  current_trial_block = gtk_tree_path_get_indices(objects->simulation->current_trial_path)[0];



  // update path;
  new_path = gtk_tree_path_new_from_indices (current_trial_block, new_trial, -1);
  new_path_str = gtk_tree_path_to_string (new_path);
  printf ("new path: %s\n", new_path_str);


  if (model_change_trial (objects->simulation, objects->simulation->task_store, new_path)) {

    printf ("model_change_trial_cb acquired iter\n");
    //	    objects->simulation->current_trial_path); // ?? what is this intended to do?

    // now update text in headerbar widgets
    model_headerbar_update_labels (objects);

  }

  else {
    printf ("model_change_trial_cb failed to acquire iter, aborting\n");
  }


  g_free(old_path_str);
  g_free(new_path_str);

}


static gboolean model_change_trial_first (PdpSimulation *simulation, 
					  GtkTreeStore *store) {

  GtkTreeIter first_block, first_trial;
  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL(simulation->task_store), &first_block)) {
    // move to child node (also check this succeeds)
    if (gtk_tree_model_iter_children (GTK_TREE_MODEL(simulation->task_store), 
				      &first_trial, 
				      &first_block)) {
      // iter now positioned for first trial, set trial buffer
      model_change_trial (simulation, simulation->task_store, 
			  gtk_tree_model_get_path(GTK_TREE_MODEL(simulation->task_store), &first_trial));
      //      simulation->current_trial_iter = first_trial; // this line might not work

      gtk_tree_path_free (simulation->current_trial_path);
      simulation->current_trial_path = 
                      gtk_tree_model_get_path(GTK_TREE_MODEL(simulation->task_store), 
					      &first_trial);
      
      return true;
    }
    else return false;
  }
  else return false;
}

static gboolean model_change_trial_first_of_block (PdpSimulation *simulation, 
						   GtkTreeStore *store) {

  GtkTreeIter trial, block;

  if (!gtk_tree_model_get_iter (GTK_TREE_MODEL(simulation->task_store), &trial, simulation->current_trial_path)) {
    printf ("error! model_change_trial_first_of_block could not acquire iter from current_trial_path\n");
    return false;
  }
  else {

    if (!gtk_tree_model_iter_parent(GTK_TREE_MODEL(simulation->task_store), &block, &trial)) {
      printf ("error! model_change_trial_first_of_block could not acquire parent iter, "
	      "is current_trial_path at top level?\n");
      return false;
    }
    else {

      if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(simulation->task_store), &trial, &block)) {
	printf ("error! model_change_trial_first_of_block could not acquire child iter of parent, "
		"really weird?\n");
	return false;
      }

      else {
       
      // iter now positioned for first trial, set trial buffer
      model_change_trial (simulation, simulation->task_store, 
			  gtk_tree_model_get_path(GTK_TREE_MODEL(simulation->task_store), &trial));
      //      simulation->current_trial_iter = first_trial; // this line might not work

      gtk_tree_path_free (simulation->current_trial_path);
      simulation->current_trial_path = 
                      gtk_tree_model_get_path(GTK_TREE_MODEL(simulation->task_store), 
					      &trial);

	return true;

      }

      
    }

  }

}



void model_initialise (PdpSimulation *simulation) {

  // de-init and re-build model to implement new parameters
  deinit_model (simulation->model);
  init_model (simulation->model, simulation->model_params);
  model_init_activation (simulation->model, 0.0); // zero activations 

  printf ("model simulation %s initialised\n", simulation->model->name);

}


static void model_controls_initialise_cb (GtkToolItem * tool_item, 
					  PdpGuiObjects * objects) {


  /*
  // de-init and re-build model to implement new parameters
  deinit_model (objects->simulation->model);
  init_model (objects->simulation->model, objects->simulation->model_params);


  model_init_activation (objects->simulation->model, 0.0); // zero activations 
  */

  model_initialise (objects->simulation);

  // current trial and subject now controlled by spin buttons
  // simulation->current_subject = 0;
  // simulation->current_trial = 0;

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }



}

static void model_controls_step_once_cb (GtkToolItem * tool_item, 
					 PdpGuiObjects * objects) {

  PdpSimulation * simulation = objects->simulation;

  // printf ("model %s step once\n", simulation->model->name);

  // check current_trial_data is init'd
  if (simulation->current_trial_data == NULL) {
    model_change_trial_first (simulation, simulation->task_store); 
  }

  bool running = run_model_step (simulation->model, 
				 simulation->current_trial_data, 
				 simulation->random_generator, 
				 simulation->model_params->response_threshold);
  

  if (running) {
    // do something?
    // print activations?
  }

  else {
    printf ("model stopped\n");
  }


  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

}

static void model_controls_step_many_cb (GtkToolItem * tool_item, 
					 PdpGuiObjects * objects) {

  PdpSimulation * simulation = objects->simulation;

  if (simulation->current_trial_data == NULL) {
    model_change_trial_first (simulation, simulation->task_store); 
  }

  int i = 0;
  bool model_running = true;
  while (i < 10 && model_running) {

    model_running = run_model_step (simulation->model, 
				    simulation->current_trial_data, 
				    simulation->random_generator, 
				    simulation->model_params->response_threshold);

    i ++;
  }


  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }
}


static void model_controls_run_cb (GtkToolItem * tool_item, 
				   PdpGuiObjects * objects) {

  PdpSimulation * simulation = objects->simulation;

  if (simulation->current_trial_data == NULL) {
    model_change_trial_first (simulation, simulation->task_store); 
  }

  run_stroop_trial (simulation->model, 
		    simulation->current_trial_data, 
		    simulation->random_generator,
		    simulation->model_params->response_threshold);

  printf ("model %s run trial \n", simulation->model->name);
  pdpgui_print_current_trial_data (simulation);

  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }

}


static void model_controls_continue_cb (GtkToolItem * tool_item, 
					PdpGuiObjects * objects) {

  PdpSimulation * simulation = objects->simulation;

  if (simulation->current_trial_data == NULL) {
    model_change_trial_first (simulation, simulation->task_store); 
  }

  run_stroop_trial (simulation->model, 
		    simulation->current_trial_data, 
		    simulation->random_generator,
		    simulation->model_params->response_threshold);

  printf ("model %s run trial \n", simulation->model->name);
  // do not need to pdpgui_print_current_trial_data as this is done by model_controls_run_cb

  // draw activations for last trial:
  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }


  if (model_current_trial_is_last (simulation)) {
    printf ("last trial on block!\n");
    return;
  }
  else {

    // set new trial

    model_change_trial_next(objects->simulation);
    model_headerbar_update_labels(objects);

    // squash activation values
    model_init_activation (simulation->model, 1-(simulation->model_params->squashing_param));
  }
}


void model_run_block (PdpSimulation *simulation) {

  //  if (simulation->current_trial_data == NULL) {
    model_change_trial_first_of_block (simulation, simulation->task_store); 
    //}

  do {
    
    run_stroop_trial (simulation->model, 
		      simulation->current_trial_data, 
		      simulation->random_generator,
		      simulation->model_params->response_threshold);

    // log output
    pdpgui_print_current_trial_data (simulation);

    // set new trial
    model_change_trial_next(simulation);


    // squash activation values
    model_init_activation (simulation->model, 1-(simulation->model_params->squashing_param));

  } while (model_current_trial_is_last (simulation) == false);
}


static void model_controls_run_block_cb (GtkToolItem * tool_item,
					 PdpGuiObjects * objects) {

  // PdpSimulation * simulation = objects->simulation;

  model_run_block (objects->simulation);  
  model_headerbar_update_labels(objects);

}


void model_run_all_blocks (PdpSimulation * simulation ) {

  gint current_block = 0;
  
    // change to block 0, trial 0
    
    model_change_trial_first (simulation, simulation->task_store);
  

    // for each block
    do {

      // init model (zero activations)
      model_initialise (simulation);

      // run block
      model_run_block (simulation);

      //  gtk_tree_model_iter_next (GTK_TREE_MODEL(simulation->task_store), 
      //			&first_trial);

      current_block ++;
      printf ("in model_run_all_blocks: done with block %d", current_block);
      gtk_tree_path_free (simulation->current_trial_path);
      simulation->current_trial_path = gtk_tree_path_new_from_indices(current_block, 0, -1);
      // likely to segfault if there are blocks but no trials loaded (can this happen?)
    } while (model_current_block_is_last(simulation) == false);

    model_change_trial_first (simulation, simulation->task_store);
}


static void model_controls_run_all_blocks_cb (GtkToolItem * tool_item,
					      PdpGuiObjects * objects) {

  model_run_all_blocks (objects->simulation);

  
  model_headerbar_update_labels(objects);

  // draw activations for last trial:
  if (objects->model_sub_notepage != NULL) {
    gtk_widget_queue_draw(objects->model_sub_notepage);
  }
  

}

static GtkWidget* create_notepage_model_main(PdpGuiObjects * objects) {

  GtkWidget *grid;
  GtkWidget *toolbar;
  GtkToolItem *tool_item;

  GtkWidget *grid_headerbar;
  GtkWidget *label1;
  GtkAdjustment *current_trial_adjustment;
  GtkWidget *spin_button;

  GtkWidget *sub_notepage;

  PdpSimulation *simulation = objects->simulation;


  int position = 0; // toolbar position

  // ------------- TOOLBAR ----------------
  // Create a basic toolbar
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  // initialise
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_initialise_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Reinitialise model from parameters");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step once
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_step_once_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process single model iteration");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step many
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_FORWARD);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_step_many_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process multiple model iterations");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_run_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Run model trial to end");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end, continue
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_OK);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_continue_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run to end, continue to next trial (carry over residual activation)");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run block
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GO_FORWARD);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_run_block_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run block from current trial");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run all blocks
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  		    G_CALLBACK(model_controls_run_all_blocks_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run all blocks from start");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);


  // options for toolbar
  gtk_widget_set_hexpand (toolbar, TRUE);
  gtk_widget_set_vexpand (toolbar, FALSE);

  // -------------- page header with current status -------

  grid_headerbar = gtk_grid_new();
  gtk_grid_set_column_spacing (GTK_GRID(grid_headerbar), 10);

  // spin button for controlling current trial  

  current_trial_adjustment = gtk_adjustment_new (simulation->current_trial, 0, 
						 (simulation->subjects->
						 subj[simulation->current_subject]->num_fixed_trials)-1,
						 1, 0, 0);
  spin_button = gtk_spin_button_new (current_trial_adjustment, 1, 0);
  g_signal_connect (G_OBJECT(spin_button), "value-changed", G_CALLBACK(model_change_trial_cb), (gpointer) objects);
  objects->model_headerbar_spin_trial = spin_button;

  gtk_grid_attach (GTK_GRID(grid_headerbar), spin_button, 0, 0, 1, 1);


  char textbuf[100];
  sprintf (textbuf, "Subject: %d", simulation->current_subject);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_subject = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 1, 0, 1, 1);

  sprintf (textbuf, "Trial: %d", simulation->current_trial);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_trial = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 2, 0, 1, 1);

  // display input pattern, trialtype, ...?
  stroop_trial_data_print_as_string (textbuf, 100, 
				     fixed_block_trial_data_get(simulation->subjects,
								simulation->current_subject,
								simulation->current_trial));
  // printf ("%s", textbuf);
  label1 = gtk_label_new (textbuf);
  gtk_widget_set_size_request (label1, 300, 30); // max size so spin button is always in same place
  objects->model_headerbar_label_trial_data = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 3, 0, 1, 1);


  

  // --------------- SUB-NOTEPAGE ------------------------    
  // Sub-notepage here: tabs for viewing network architecture and 
  // plotting single-trial activation
  sub_notepage = gtk_notebook_new();

  // keep a pointer to widgets so we can issue redraw signals on it
  objects->model_sub_notepage = sub_notepage;



  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_plot_activation(objects),
			   gtk_label_new("Plot Network Activation"));
  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_display_architecture(objects),
			   gtk_label_new("Display Network Architecture"));

			   
  

  // -------------- SHOW WIDGETS --------------------

  grid = gtk_grid_new();

  gtk_grid_attach (GTK_GRID(grid), grid_headerbar, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), toolbar, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), sub_notepage, 0, 2, 1, 1);

  gtk_widget_set_vexpand (GTK_WIDGET(grid), TRUE);

  gtk_widget_show_all(grid);
  return (grid);
}


static void init_model (pdp_model * this_model, GsStroopParameters *model_params) {
  // just allocate memory for simulation and run constructors  

  act_func_params * act_params = g_malloc (sizeof(act_func_params));
  act_params->type = ACT_GS;
  act_params->params.gs.step_size = model_params->step_size;
  act_params->params.gs.act_max = model_params->activation_max;
  act_params->params.gs.act_min = model_params->activation_min;
  
  this_model->activation_parameters = act_params;

  // now create the model
  gs_stroop_model_build (this_model, model_params); 
  // probably defer building the model in later versions

}

static void deinit_model (pdp_model * this_model) {
  // delete model components
  // can be re-initialised with init_model

  pdp_model_component_free (this_model->components);
  this_model->components = NULL;
  
  g_free (this_model->activation_parameters);

}



PdpSimulation * create_simulation () {
  // just allocate memory for simulation and run constructors

  PdpSimulation *simulation = g_malloc (sizeof(PdpSimulation));
  int n;

  simulation->random_generator = random_generator_create();
  simulation->model = pdp_model_create (0, "gs_stroop");

  // allocate memory for model parameters and set to default values
  simulation->model_params = g_malloc (sizeof(GsStroopParameters));
  gs_stroop_parameters_set_default (simulation->model_params);


  // now build the model
  // gs_stroop_model_build (simulation->model, simulation->model_params); 
  // probably defer building the model in later versions

  init_model (simulation->model, simulation->model_params);



  // initialise subjects
  simulation->subjects = subject_popn_create (NUMBER_OF_SUBJECTS);

  for (n = 0; n < simulation->subjects->number_of_subjects; n++) {

    simulation->subjects->subj[n] = subject_create (NUM_TRIALS, NUM_TRIALS, MIXED_BLOCK_RUN_LENGTH);

    // parameterise subject
    subject_params_vary (simulation->subjects->subj[n], 
			 TASKDEMAND_OUTPUT_INHIBITORY_WT,
			 TASKDEMAND_OUTPUT_EXCITATORY_WT);

    // create subject data
    subject_init_trialblock_fixed (simulation->random_generator, simulation->subjects->subj[n], 
				 PPN_NEUTRAL, PPN_CONGRUENT, PPN_INCONGRUENT,
				 PPN_WORDREADING, PPN_COLOURNAMING);

    // don't do mixed trials yet in this simulation				 
    // subject_init_trialblock_mixed (my_subjects->subj[n]);
  }

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
  pdp_model_free (simulation->model);
  g_free (simulation->model_params);

  subject_popn_free (simulation->subjects);
  random_generator_free (simulation->random_generator);  

  //  g_free (simulation->task_store);
  g_free (simulation->current_trial_data);
  gtk_tree_path_free(simulation->current_trial_path);
  g_free (simulation);

}


static void main_quit (GtkWidget *window, PdpGuiObjects  *objects) {
  
  free_simulation (objects->simulation);
  g_free (objects->config_file);
  gtk_main_quit ();

}


int main (int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  PdpSimulation * simulation = create_simulation();

  PdpGuiObjects * objects = g_malloc (sizeof(PdpGuiObjects));
  // init function, set everything to null?
  objects->simulation = simulation;
  objects->model_sub_notepage = NULL;

  objects->config_file = create_param_import_objects();
  objects->task_config_file = create_task_import_objects();




  // Draw the GUI

  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *notes;

  // Create a window with a title, default size, and set border width
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "GUI: notebook");
  gtk_window_set_default_size(GTK_WINDOW(window), MAIN_WINDOW_WIDTH_DEFAULT, MAIN_WINDOW_HEIGHT_DEFAULT);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  g_signal_connect (window, "destroy", G_CALLBACK(main_quit), (gpointer) objects);


  // ------------- NOTEPAD -----------------

  notes = gtk_notebook_new();

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_model_main(objects), 
			   gtk_label_new("Model"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_model_params(objects), 
			   gtk_label_new("Parameter Import"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_view_trials(objects), 
			   gtk_label_new("Task View"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_trials(objects), 
			   gtk_label_new("Task Import"));



  // Create a full-window grid to contain toolbar and the notebook
  grid = gtk_grid_new();
  
  // options for notebook
  gtk_widget_set_hexpand (notes, TRUE);
  gtk_widget_set_vexpand (notes, TRUE);
  gtk_grid_attach (GTK_GRID(grid), notes, 0, 1, 1, 1);

  gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));

  gtk_widget_show_all (window);

  gtk_main();


  return 0;
}
