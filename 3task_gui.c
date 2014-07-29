#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"
#include "3task_model_gs.h" // model
#include "3task_procedure.h"


#include "pdpgui_plot.h"
// #include "pdpgui_import.h"
#include "3task_gui.h"


static void main_quit (GtkWidget *window, ThreeTaskObjects  *objects) {

  pdp_model_free (objects->simulation->model);  
  free_simulation (objects->simulation);
  g_free (objects->param_config_file);
  g_free (objects->task_config_file);
  gtk_main_quit ();

}



int main (int argc, char *argv[]) {

  gtk_init (&argc, &argv);

  ThreeTaskSimulation * simulation = create_simulation();

  simulation->model = pdp_model_create (0, "3task_gs"); 
  three_task_parameters_htable_set_default (simulation->model_params_htable);

  // now build the model
  printf ("in main, building the model\n");
  init_model_simulation (simulation->model, simulation->model_params_htable); 


  ThreeTaskObjects * objects = g_malloc (sizeof(ThreeTaskObjects));
  // init function, set everything to null?
  objects->simulation = simulation;
  //  objects->model_sub_notepage = NULL;

  objects->param_config_file = create_param_import_objects();
  objects->task_config_file = triple_task_create_task_import_objects();


  // Draw the GUI

  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *notes;

  // Create a window with a title, default size, and set border width
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(window), "Three Task Switching: Model");
  gtk_window_set_default_size(GTK_WINDOW(window), MAIN_WINDOW_WIDTH_DEFAULT, MAIN_WINDOW_HEIGHT_DEFAULT);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width (GTK_CONTAINER(window), 10);
  g_signal_connect (window, "destroy", G_CALLBACK(main_quit), (gpointer) objects);


  // ------------- NOTEPAD -----------------

  notes = gtk_notebook_new();
  /*
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
