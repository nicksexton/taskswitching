#define NUMBER_OF_SUBJECTS 1

// inits a model with basic controls to run it
#include <stdbool.h>
#include <gtk/gtk.h>
#include "gs_stroop.h"
#include "pdp_objects.h"

#include <gsl/gsl_randist.h>
#include "random_generator_functions.h" // for gaussian noise
#include "gs_stroop_subjects.h"
#include "gs_stroop_global_params.h"

#include "pdpgui_plot.h"
#include "pdpgui.h"


// temp!
#include <math.h>

void pdpgui_plot_network_activation (GtkWidget *widget, 
				     cairo_t *cr, 
				     PdpSimulation *simulation) {

  // cairo_scale (cr, WINDOW_WIDTH, WINDOW_HEIGHT);
  // set colour for background
  // cairo_set_source_rgb (cr, 1, 1, 1);
  // fill background colour
  // cairo_paint (cr);

  guint widget_width, widget_height;
  widget_width = gtk_widget_get_allocated_width (GTK_WIDGET(widget));
  widget_height = gtk_widget_get_allocated_height (GTK_WIDGET(widget));

  printf ("%d x %d\n", widget_width, widget_height);

  pdpgui_draw_graph_axes(cr, widget_width, widget_height, 10, 10, 0.0, 200.0, 0.0, 5.0);


  PdpguiAxisDimensions axes = { 
    .x_min = 0.0, 
    .x_max = 200, 
    .y_min = 0.0, 
    .y_max = 5.0
  };

  PdpguiColourRgb plot_colour = { 
    .r = 1.0, 
    .g = 0.0, 
    .b = 0.0 
  };

  // now construct an arbitrary vector;

  double log_function[40];
  int i;

  printf ("log function: ");

  for (i = 0; i < 40; i ++) {
    log_function[i] = log (i + 1);
    printf( "%4.2f ", log_function[i]);
  }
  printf ("\n");

  pdpgui_plot_vector (cr, widget_width, widget_height, &axes, 40, log_function, &plot_colour);
  
}



static GtkWidget* 
create_sub_notepage_model_plot_activation (PdpSimulation * simulation) {
  // plots network activation for current trial


  GtkWidget *grid;
  GtkWidget *drawing_area;
  GtkWidget *label;

  label = gtk_label_new("Network activation graph here");
  //  gtk_widget_set_hexpand (label, TRUE);
  //  gtk_widget_set_vexpand (label, TRUE);


  drawing_area = gtk_drawing_area_new();
  g_signal_connect (drawing_area, "draw", 
		    G_CALLBACK(pdpgui_plot_network_activation), simulation);
  gtk_widget_set_hexpand (drawing_area, TRUE);
  gtk_widget_set_vexpand (drawing_area, TRUE);


  grid = gtk_grid_new();
  gtk_grid_attach (GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid), drawing_area, 0, 1, 1, 1);

  gtk_widget_show_all(grid);
  return (grid);
}


static GtkWidget* 
create_sub_notepage_model_display_architecture (PdpSimulation * simulation) {

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


static void model_controls_initialise_cb (GtkToolItem * tool_item, PdpSimulation *simulation) {

  int n;
  for (n = 0; n < NUMBER_OF_SUBJECTS; n ++) {

    // initialise gs_stroop_params here for now
    model_init_params (simulation->model, 
		     ((gs_stroop_params *)(simulation->subjects->subj[n]->params)));
  }

  model_init_activation (simulation->model, 0.0); // zero activations 

  // init current trial and current subject
  simulation->current_subject = 0;
  simulation->current_trial = 0;

  printf ("model simulation %s initialised\n", simulation->model->name);

}

static void model_controls_step_once_cb (GtkToolItem * tool_item, PdpSimulation *simulation) {

  // printf ("model %s step once\n", simulation->model->name);
  bool running = run_model_step (simulation->model, 
				 &(simulation->subjects->subj[simulation->current_subject]
				   ->fixed_trials[simulation->current_trial]), 
				 simulation->random_generator);

  if (running) {
    // do something?
    // print activations?
  }

  else {
    printf ("model stopped\n");
  }

}

static void model_controls_step_many_cb (GtkToolItem * tool_item, PdpSimulation *simulation) {

  printf ("model %s step many (not implemented)\n", simulation->model->name);
}


static void model_controls_run_cb (GtkToolItem * tool_item, PdpSimulation *simulation) {

  run_stroop_trial (simulation->model, 
		    &(simulation->subjects->subj[simulation->current_subject]
		      ->fixed_trials[simulation->current_trial]), 
		    simulation->random_generator);


  printf ("model %s run trial \n", simulation->model->name);

  // current version only runs a single trial; uncomment this when checks are in place that current_trial does not
  // exceed max trials
  // simulation->current_trial ++;

}




static GtkWidget* create_notepage_model_main(PdpSimulation * simulation) {

  GtkWidget *grid;
  GtkWidget *toolbar;
  GtkToolItem *tool_item;
  GtkWidget *grid_headerbar;
  GtkWidget *label1;

  GtkWidget *sub_notepage;


  int position = 0; // toolbar position

  // ------------- TOOLBAR ----------------
  // Create a basic toolbar
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  // initialise
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PREVIOUS);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_initialise_cb), (gpointer) simulation);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step once
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_step_once_cb), (gpointer) simulation);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // step many
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_FORWARD);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_step_many_cb), (gpointer) simulation);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // run to end
  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_NEXT);
  g_signal_connect (G_OBJECT(tool_item), "clicked", 
		    G_CALLBACK(model_controls_run_cb), (gpointer) simulation);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);


  // code for close button on toolbar:
  // tool_item = gtk_separator_tool_item_new();
  // gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(tool_item), FALSE);
  // gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  //  tool_item = gtk_tool_button_new_from_stock (GTK_STOCK_CLOSE);
  // connect application quit callback here
  //  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, position ++);

  // options for toolbar
  gtk_widget_set_hexpand (toolbar, TRUE);
  gtk_widget_set_vexpand (toolbar, FALSE);

  // -------------- page header with current status -------

  grid_headerbar = gtk_grid_new();

  char textbuf[100];
  sprintf (textbuf, "Subject: %d", simulation->current_subject);
  label1 = gtk_label_new (textbuf);
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 0, 0, 1, 1);

  sprintf (textbuf, "Trial: %d", simulation->current_trial);
  label1 = gtk_label_new (textbuf);
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 1, 0, 1, 1);

  // display input pattern, trialtype, ...?
  stroop_trial_data_print_as_string (textbuf, 100, 
				     fixed_block_trial_data_get(simulation->subjects,
								simulation->current_subject,
								simulation->current_trial));
  printf ("%s", textbuf);
  label1 = gtk_label_new (textbuf);
  gtk_grid_attach (GTK_GRID(grid_headerbar), label1, 2, 0, 1, 1);
  

  // --------------- SUB-NOTEPAGE ------------------------    
  // Sub-notepage here: tabs for viewing network architecture and 
  // plotting single-trial activation
  sub_notepage = gtk_notebook_new();
  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_plot_activation(simulation),
			   gtk_label_new("Plot Network Activation"));
  gtk_notebook_append_page(GTK_NOTEBOOK(sub_notepage),
			   create_sub_notepage_model_display_architecture(simulation),
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





PdpSimulation * init_simulation () {
  // just allocate memory for simulation and run constructors

  PdpSimulation *simulation = g_malloc (sizeof(PdpSimulation));
  int n;

  simulation->random_generator = random_generator_create();
  simulation->model = pdp_model_create (0, "gs_stroop");

  gs_stroop_model_build (simulation->model); // probably defer building the model in later versions

  act_func_params * act_params = act_params = g_malloc (sizeof(act_func_params));

  act_params->type = ACT_GS;
  act_params->params.gs.step_size = STEP_SIZE;
  act_params->params.gs.act_max = ACTIVATION_MAX;
  act_params->params.gs.act_min = ACTIVATION_MIN;

  simulation->model->activation_parameters = act_params;


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


  return simulation;
}


void free_simulation (PdpSimulation * simulation) {
  // free memory for simulation

  free (simulation->model->activation_parameters); 
  pdp_model_free (simulation->model);

  subject_popn_free (simulation->subjects);
  random_generator_free (simulation->random_generator);  


  g_free (simulation);

}

static void main_quit (GtkWidget *window, PdpSimulation *simulation) {

  
  free_simulation (simulation);
  gtk_main_quit ();

}


int main (int argc, char *argv[]) {


  PdpSimulation * simulation = init_simulation();


  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *notes;

  gtk_init (&argc, &argv);

  // Create a window with a title, default size, and set border width
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "GUI: notebook");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  g_signal_connect (window, "destroy", G_CALLBACK(main_quit), (gpointer) simulation);


  // ------------- NOTEPAD -----------------

  notes = gtk_notebook_new();

  gtk_notebook_append_page(GTK_NOTEBOOK(notes), 
			   create_notepage_model_main(simulation), 
			   gtk_label_new("Model"));

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
