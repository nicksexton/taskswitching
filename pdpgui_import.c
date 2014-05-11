/*  Model-general functions associated with the gui. */ 
/*  Possibly merge into general pdpgui library if it ends up small? */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "pdp_import.h"
#include "pdpgui_import.h"
#include "pdpgui.h"
#include "gs_stroop_import.h"


#define CONFIG_FILE gtk_config_file.conf


// callback function to read file contents
// uses short format (ie key - value pairs)
gboolean load_from_file_short_cb (GtkWidget *widget, FileData *file_info) {

  return (pdp_load_from_file_short (file_info));
}


// similar to load_from_file_cb but imports long format data (ie multiple columns)
gboolean load_from_file_long_cb (GtkWidget *widget, FileData *file_info) {

  return (pdp_load_from_file_long (file_info));
}


void select_file_cb (GtkComboBoxText *widget, FileData * config_file) {

  GtkComboBoxText *combo_box = widget;

  gchar *filename = gtk_combo_box_text_get_active_text (combo_box);

  g_print ("file %s selected", filename);
  strcpy (config_file->filename, filename);
  gtk_label_set_text(GTK_LABEL(config_file->filename_label), filename);

  g_free (filename);

}

// <------------------------------- MODEL PARAMETER IMPORT FUNCTIONS -----------------------





// read parameters from the tree store, apply to model parameters struct
static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects) {

  gs_stroop_parameters_import_commit (objects->config_file, objects->simulation->model_params);

}



// selection handling
// prototype for selection handler callback
static void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data) {

  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *param_name;
  gchar *param_value;

  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    gtk_tree_model_get (model, &iter, COL_PARAMETER_NAME, &param_name, -1);
    gtk_tree_model_get (model, &iter, COL_PARAMETER_VALUE, &param_value, -1);
    g_print ("%s: %s\n", param_name, param_value);
    g_free (param_name);
    g_free (param_value);
    
  }
}


static void setup_model_params_treeview (GtkTreeView * tree) {

  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  // aesthetic properties:
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);


  // create a cell renderer, arbitrarily make it red for demonstration purposes
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "foreground", "red", NULL);

  // create a column, associate the "text" attribute of the cell renderer to the
  // first column of the model, parameter name
  column = gtk_tree_view_column_new_with_attributes ("Parameter", renderer,
						     "text", COL_PARAMETER_NAME,
						     NULL);

  // add the column to the view
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);

  // second column... parameter value
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Value", renderer, "text",
						     COL_PARAMETER_VALUE,
						     NULL);

  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);


  // set up the selection handler
  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);

  g_signal_connect (G_OBJECT(select), "changed", 
		    G_CALLBACK(config_file_treeview_selection_changed_cb), NULL);

}






GtkWidget* create_notepage_import_model_params(PdpGuiObjects * objects) {

  GtkWidget *grid_main;
  GtkWidget *grid_controls;
  GtkWidget *label1; // *label2;
  GtkWidget *file_select;
  GtkWidget *button_process_configfile;
  GtkWidget *button_import_commit;

  GtkWidget *tree;
  tree = gtk_tree_view_new();
  gtk_tree_view_set_model (GTK_TREE_VIEW(tree), GTK_TREE_MODEL(objects->config_file->tree_store));
  setup_model_params_treeview(GTK_TREE_VIEW(tree));



  label1 = gtk_label_new("Select config file");
  gtk_label_set_line_wrap(GTK_LABEL(label1), TRUE);
  
  file_select = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "gtk_config_file_1.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "gtk_config_file_2.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "pdpgui_gs_params_default.conf");
  g_signal_connect (file_select, "changed", G_CALLBACK(select_file_cb), (gpointer)(objects->config_file) );

  // aesthetic: give this a standard icon
  button_process_configfile = gtk_button_new_with_label ("Load from file");
  g_signal_connect (button_process_configfile, "clicked", 
		    G_CALLBACK(load_from_file_short_cb), (gpointer)(objects->config_file));

  
  // TODO - make button only active when there are parameters to commit
  button_import_commit = gtk_button_new_with_label ("Import Commit");
  g_signal_connect (button_import_commit, "clicked", 
		    G_CALLBACK(model_parameters_import_commit_cb), (gpointer)(objects));


  grid_main = gtk_grid_new();
  grid_controls = gtk_grid_new();

  gtk_grid_attach (GTK_GRID(grid_controls), label1, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_controls), file_select, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_controls), objects->config_file->filename_label, 0, 2, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_controls), button_process_configfile, 0, 3, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_controls), button_import_commit, 0, 4, 1, 1);


  //  gtk_widget_set_vexpand (GTK_WIDGET(grid_controls), TRUE);
  
  gtk_grid_attach (GTK_GRID(grid_main), grid_controls, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_main), tree, 1, 0, 1, 1);
  gtk_grid_set_column_spacing (GTK_GRID(grid_main), 30);
  gtk_widget_set_vexpand (GTK_WIDGET(grid_main), TRUE);
  gtk_widget_set_hexpand (GTK_WIDGET(grid_main), TRUE);


  gtk_widget_show_all(grid_main);


  return (grid_main);
  
}

//<---------------------- MODEL TASK IMPORT FUNCTIONS ------------------------







void model_reset_trial_markers (PdpSimulation *simulation) {

  gchar *path;

  gtk_tree_path_free(simulation->current_trial_path);
  simulation->current_trial_path = gtk_tree_path_new_first();

  path = gtk_tree_path_to_string (simulation->current_trial_path);

  printf ("in model_reset_trial_markers, current path is: %s\n", path);

  g_free (path);

}


// temp code - for translating task blocks (ie an array of stroop_trial_data objects) to treestore
static void import_task_block_new_to_treestore (GtkTreeStore * store, 
						gchar *name,
						int num_trials, 
						stroop_trial_data * trial_array) {

    GtkTreeIter iter1;
    GtkTreeIter *iter2 = g_malloc(sizeof(GtkTreeIter));

    // set block name as parent
    // top level iterator
    gtk_tree_store_append (store, &iter1, NULL);
    gtk_tree_store_set (store, &iter1, COL_BLOCK_NAME, name, -1);


  // set trials as children


  int n;
  for (n = 0; n < num_trials; n ++) {

    gtk_tree_store_append (store, iter2, &iter1);    
    import_stroop_trial_data_to_treestore (store, iter2, &(trial_array[n]));

  }

}



static void setup_task_viewer_treeview (GtkTreeView * tree) {

  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  // aesthetic properties:
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);

  // COlumns:
  // Block name
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Block", renderer,
						     "text", COL_BLOCK_NAME,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);

  // Trial ID
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Trial ID", renderer,
						     "text", COL_TASK_ID,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW(tree), column);


  // Input Patterns 
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Pattern 1", renderer, "text",
						     COL_TASK_PATTERN_1,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Pattern 2", renderer, "text",
						     COL_TASK_PATTERN_2,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Pattern 3", renderer, "text",
						     COL_TASK_PATTERN_3,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);


  // Trial parameters
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Parameter 1", renderer, "text",
						     COL_TASK_PARAM_1,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (G_OBJECT (renderer), "family", "monospace", NULL);
  column = gtk_tree_view_column_new_with_attributes ("Parameter 2", renderer, "text",
						     COL_TASK_PARAM_2,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

}







GtkWidget* create_notepage_view_trials(PdpGuiObjects * objects) {

  GtkWidget *grid_main;
  GtkWidget *scrollwindow;

  grid_main = gtk_grid_new();
  scrollwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request (scrollwindow, TASK_VIEW_WIDTH, TASK_VIEW_HEIGHT);

  GtkWidget *tree;
  tree = gtk_tree_view_new();
  gtk_tree_view_set_model (GTK_TREE_VIEW(tree), GTK_TREE_MODEL(objects->simulation->task_store));
  setup_task_viewer_treeview(GTK_TREE_VIEW(tree));

  // file import buttons here


  // tree view here
  gtk_container_add (GTK_CONTAINER(scrollwindow), tree);
  gtk_grid_attach (GTK_GRID(grid_main), scrollwindow, 0, 0, 1, 1);
  gtk_widget_show_all(grid_main);




  // TEMP CODE: set path to first trial of first block

  model_reset_trial_markers (objects->simulation);

  return (grid_main);

}




// read tasks from the tree store, add to task store
// key function for task import!
static void model_task_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects) {

  gs_stroop_model_task_import_commit (objects->task_config_file, 
				      objects->simulation->task_store);

  model_reset_trial_markers (objects->simulation);

}



GtkWidget* create_notepage_import_trials(PdpGuiObjects * objects) {

  GtkWidget *grid_main;
  GtkWidget *scrollwindow;

  GtkWidget *label1; 
  GtkWidget *file_select;
  GtkWidget *button_process_configfile;
  GtkWidget *button_import_commit;

 
  label1 = gtk_label_new("Select config file for task import");
  gtk_label_set_line_wrap(GTK_LABEL(label1), TRUE);
  
  file_select = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "task_import_test.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "simulations/sim_1_trials.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "simulations/sim_2_trials.conf");


  g_signal_connect (file_select, "changed", G_CALLBACK(select_file_cb), (gpointer)(objects->task_config_file) );

  // aesthetic: give this a standard icon
  button_process_configfile = gtk_button_new_with_label ("Load from file");
  g_signal_connect (button_process_configfile, "clicked", 
		    G_CALLBACK(load_from_file_long_cb), (gpointer)(objects->task_config_file));

  
  // TODO - make button only active when there are parameters to commit
  button_import_commit = gtk_button_new_with_label ("Import Commit");


  // todo - callback function to import task blocks 
  
  g_signal_connect (button_import_commit, "clicked", 
		    G_CALLBACK(model_task_import_commit_cb), (gpointer)(objects));
  




  GtkWidget *tree;
  tree = gtk_tree_view_new();
  gtk_tree_view_set_model (GTK_TREE_VIEW(tree), GTK_TREE_MODEL(objects->task_config_file->tree_store));
  setup_task_viewer_treeview(GTK_TREE_VIEW(tree)); // same task viewer as task view page

  grid_main = gtk_grid_new();

  // file import buttons here
  gtk_grid_attach (GTK_GRID(grid_main), label1, 0, 0, 2, 1);
  gtk_grid_attach (GTK_GRID(grid_main), file_select, 0, 1, 2, 1);
  gtk_grid_attach (GTK_GRID(grid_main), objects->task_config_file->filename_label, 0, 2, 2, 1);
  gtk_grid_attach (GTK_GRID(grid_main), button_process_configfile, 0, 3, 1, 1);
  gtk_grid_attach (GTK_GRID(grid_main), button_import_commit, 1, 3, 1, 1);


  // tree view here
  scrollwindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request (scrollwindow, TASK_VIEW_WIDTH, TASK_VIEW_HEIGHT);
  gtk_container_add (GTK_CONTAINER(scrollwindow), tree);

  gtk_grid_attach (GTK_GRID(grid_main), scrollwindow, 0, 5, 2, 1);
  gtk_widget_show_all(grid_main);

  return (grid_main);

}
