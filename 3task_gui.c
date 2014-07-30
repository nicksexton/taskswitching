#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"


#include "pdpgui_plot.h"
// #include "pdpgui_import.h"
#include "3task_gui.h"
#include "3task_model_gs.h"


void three_task_gui_plot_network_activation (GtkWidget *widget, 
					     cairo_t *cr, 
					     ThreeTaskSimulation *simulation) {


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


  // <------------ OUTPUT 0 ------------
  pdp_layer * current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_0)->layer; 
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


  // <------------ OUTPUT 1 ------------
  current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_1)->layer; 


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

  // <------------ OUTPUT 1 ------------
  current_layer = pdp_model_component_find (simulation->model, 
							ID_OUTPUT_2)->layer; 


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




  // <------------ NEXT PLOT TASKDEMAND UNITS  ------------------

  current_layer = pdp_model_component_find (simulation->model, 
							ID_TASKDEMAND)->layer; 

  //  task demand unit 1
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

    // task demand unit 2
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

    // task demand unit 3
  units_activation = 
    pdp_layer_get_unit_activation_history (current_layer, 2, simulation->model->cycle);
  
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
create_sub_notepage_model_plot_activation (ThreeTaskObjects * objects) {
  // plots network activation for current trial


  GtkWidget *grid;
  GtkWidget *drawing_area;
  GtkWidget *label;

  label = gtk_label_new("Network activation graph here");
  //  gtk_widget_set_hexpand (label, TRUE);
  //  gtk_widget_set_vexpand (label, TRUE);


  drawing_area = gtk_drawing_area_new();
  g_signal_connect (drawing_area, "draw", 
		    G_CALLBACK(three_task_gui_plot_network_activation), objects->simulation);
  gtk_widget_set_hexpand (drawing_area, TRUE);
  gtk_widget_set_vexpand (drawing_area, TRUE);


  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area, 0, 1, 1, 1);

  gtk_widget_show_all(grid);
  return (grid);
}




static GtkWidget* 
create_sub_notepage_model_display_architecture (ThreeTaskObjects * objects) {

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




static GtkWidget* create_notepage_model_main(ThreeTaskObjects * objects) {

  GtkWidget *grid;
  GtkWidget *toolbar;
  GtkToolItem *tool_item;

  GtkWidget *grid_headerbar;
  //  GtkWidget *label1;
  // GtkAdjustment *current_trial_adjustment;
  // GtkWidget *spin_button;

  GtkWidget *sub_notepage;

  //  ThreeTaskSimulation *simulation = objects->simulation;


  int position = 0; // toolbar position

  // ------------- TOOLBAR ----------------
  // Create a basic toolbar
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  // initialise
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH);
  //  g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_initialise_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Reinitialise model from parameters");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step once
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_step_once_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process single model iteration");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step many
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_FORWARD);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_step_many_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Process multiple model iterations");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_run_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), "Run model trial to end");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end, continue
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_OK);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_continue_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run to end, continue to next trial (carry over residual activation)");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run block
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GO_FORWARD);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_run_block_cb), (gpointer) objects);
  gtk_widget_set_tooltip_text(GTK_WIDGET(tool_item), 
			      "Run block from current trial");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // button here for run all blocks
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_GOTO_LAST);
  //g_signal_connect (G_OBJECT(tool_item), "clicked", 
  //		    G_CALLBACK(model_controls_run_all_blocks_cb), (gpointer) objects);
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
  /* // NB commented out to remove dependency on simulation->subjects?
  current_trial_adjustment = gtk_adjustment_new (simulation->current_trial, 0, 
						 (simulation->subjects->
						 subj[simulation->current_subject]->num_fixed_trials)-1,
						 1, 0, 0);
  spin_button = gtk_spin_button_new (current_trial_adjustment, 1, 0);
  g_signal_connect (G_OBJECT(spin_button), "value-changed", G_CALLBACK(model_change_trial_cb), (gpointer) objects);
  objects->model_headerbar_spin_trial = spin_button;

  gtk_grid_attach (GTK_GRID(grid_headerbar), spin_button, 0, 0, 1, 1);
  */


  /*
  char textbuf[100];
  sprintf (textbuf, "Subject: %d", simulation->current_subject);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_subject = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 1, 0, 1, 1);

  sprintf (textbuf, "Trial: %d", simulation->current_trial);
  label1 = gtk_label_new (textbuf);
  objects->model_headerbar_label_trial = label1; // keep track of label so we can update it
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 2, 0, 1, 1);
  */


  /*
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
  */

  

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



/*
static void main_quit (GtkWidget *window, ThreeTaskObjects *objects) {

  printf ("freeing objects\n");
  free (objects->simulation->model->activation_parameters); 
  pdp_model_free (objects->simulation->model);  // irritating segfault!

  printf ("freeing simulation\n");
  free_simulation (objects->simulation);

  printf ("freeing config files\n");
  g_free (objects->param_config_file);
  g_free (objects->task_config_file);

  printf ("freeing objects\n");
  g_free (objects);

  printf ("gtk main quit\n");
  gtk_main_quit ();

}
*/


int main (int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  ThreeTaskObjects * objects = g_malloc (sizeof(ThreeTaskObjects));
  // init function, set everything to null?

  objects->simulation = create_simulation();
  three_task_parameters_htable_set_default (objects->simulation->model_params_htable);


  // now create and build the model
  printf ("in main, building the model\n");
  objects->simulation->model = pdp_model_create (0, "3task_gs"); 
  init_model_simulation (objects->simulation->model, objects->simulation->model_params_htable); 



  //  objects->model_sub_notepage = NULL;

  objects->param_config_file = create_param_import_objects();
  objects->task_config_file = triple_task_create_task_import_objects();


  // Draw the GUI

  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *notes;

  // Create a window with a title, default size, and set border width
  if (!(window = gtk_window_new (GTK_WINDOW_TOPLEVEL))) {
    printf ("error, could not create toplevel window!\n");
    return 0;
  }
  gtk_window_set_title (GTK_WINDOW(window), "Three Task Switching: Model");
  gtk_window_set_default_size(GTK_WINDOW(window), MAIN_WINDOW_WIDTH_DEFAULT, MAIN_WINDOW_HEIGHT_DEFAULT);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  // g_signal_connect (window, "destroy", G_CALLBACK(main_quit), (gpointer) objects); // causes irritating segfault
  g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // 


  // ------------- NOTEPAD -----------------

  notes = gtk_notebook_new();
 
  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_model_main(objects), 
			   gtk_label_new("Model"));

  /*
  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_model_params(objects), 
			   gtk_label_new("Parameter Import"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_view_trials(objects), 
			   gtk_label_new("Task View"));

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_import_trials(objects), 
			   gtk_label_new("Task Import"));
  */


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
