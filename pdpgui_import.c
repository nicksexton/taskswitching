
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "lib_string_parse.h"
#include "pdpgui.h"

#define CONFIG_FILE gtk_config_file.conf

static bool model_parameter_import (gchar* param_name, gchar* param_value, GsStroopParameters *model_params) {

  bool return_value = true; // returns true if import succeeds

  if (!strcmp (param_name, "ACTIVATION_MAX")) { 
    model_params->activation_max = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->activation_max);
  }
  else if (!strcmp (param_name, "ACTIVATION_MIN")) {
    model_params->activation_min = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->activation_min);
  }
  else if (!strcmp (param_name, "RESPONSE_THRESHOLD")) {
    model_params->response_threshold = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->response_threshold);
  }
  else if (!strcmp (param_name, "STEP_SIZE")) {
    model_params->step_size = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.4f\n", param_name, model_params->step_size);
  }
  else if (!strcmp (param_name, "SQUASHING_PARAM")) {
    model_params->squashing_param = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->squashing_param);
  }
  else if (!strcmp (param_name, "NOISE")) {
    model_params->noise = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->noise);
  }
  else if (!strcmp (param_name, "BIAS_OUTPUTUNIT")) {
    model_params->bias_outputunit = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_outputunit);
  }
  else if (!strcmp (param_name, "BIAS_TASKDEMAND")) {
    model_params->bias_taskdemand = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_taskdemand);
  }  
  else if (!strcmp (param_name, "BIAS_NONE")) {
    model_params->bias_none = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->bias_none);
  }
  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_WORD")) {
    model_params->stimulus_input_strength_word = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->stimulus_input_strength_word);
  }
  else if (!strcmp (param_name, "STIMULUS_INPUT_STRENGTH_COLOUR")) {
    model_params->stimulus_input_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->stimulus_input_strength_colour);
  }
  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_INHIBITORY_WT")) {
    model_params->taskdemand_output_inhibitory_wt = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->taskdemand_output_inhibitory_wt);
  }
  else if (!strcmp (param_name, "TASKDEMAND_OUTPUT_EXCITATORY_WT")) {
    model_params->taskdemand_output_excitatory_wt = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->taskdemand_output_excitatory_wt);
  }
  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_WORD")) {
    model_params->topdown_control_strength_word = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->topdown_control_strength_word);
  }
  else if (!strcmp (param_name, "TOPDOWN_CONTROL_STRENGTH_COLOUR")) {
    model_params->topdown_control_strength_colour = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->topdown_control_strength_colour);
  }
  else if (!strcmp (param_name, "LEARNING_RATE")) {
    model_params->learning_rate = (double) g_ascii_strtod (param_value, NULL);
    printf ("parameter %s now %4.2f\n", param_name, model_params->learning_rate);
  }
  else if (!strcmp (param_name, "MAX_CYCLES")) {
    model_params->max_cycles = (double) g_ascii_strtoll (param_value, NULL, 10);
    printf ("parameter %s now %d\n", param_name, model_params->max_cycles);
  }
  else {
    printf ("warning! parameter %s not recognised\n", param_name);
    return_value = false;
  }

  return return_value;
}


// read parameters from the tree store, apply to model parameters struct
static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects) {

  GtkTreeIter iter; 
  gboolean more;

  more = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(objects->config_file->tree_store), &iter);

  while (more) {

    // tree is not empty, process 
    gchar *param_name;
    gchar *param_value;

   
    gtk_tree_model_get (GTK_TREE_MODEL(objects->config_file->tree_store), 
			&iter, COL_PARAMETER_NAME, &param_name, -1);
    gtk_tree_model_get (GTK_TREE_MODEL(objects->config_file->tree_store), 
			&iter, COL_PARAMETER_VALUE, &param_value, -1);
    g_print ("assigning:\t%s: %s\n", param_name, param_value);

    model_parameter_import (param_name, param_value, objects->simulation->model_params);

    g_free (param_name);
    g_free (param_value);
    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(objects->config_file->tree_store), &iter);
  }  

}



// utility function for clearing all entries from a treeview
gboolean treestore_remove_all (GtkTreeStore * tree_store) {

  // GtkTreeIter * iter = g_malloc (sizeof(GtkTreeIter));
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL(tree_store), &iter)) {
    // tree is not empty, proceed to remove all items

    while (gtk_tree_store_remove (tree_store, &iter)) {
      // tree_store still contains rows
    }
    return TRUE;
  }
  else {
    // tree is already empty,
    return FALSE;
  }
}



// callback function to read file contents
gboolean load_from_file_cb (GtkWidget *widget, FileData *file_info) {

  file_info->fp = fopen(file_info->filename, "r");
  if (file_info->fp == NULL) {
    printf ("error! gtk_config_file.conf does not exist\n");
    return FALSE;
  }
  else {
    printf ("success! config file opened.\n");

    treestore_remove_all (file_info->tree_store);
    pdp_file_parse_to_treestore (file_info);

    fclose(file_info->fp);
    printf ("success! config file closed.\n");
  }

  return TRUE;
}


void select_file (GtkComboBoxText *widget, FileData * config_file) {

  GtkComboBoxText *combo_box = widget;
  // char *tmp[FILENAME_MAX_LENGTH];

  gchar *filename = gtk_combo_box_text_get_active_text (combo_box);

    // just print filename for now

  g_print ("file %s selected", filename);
  strcpy (config_file->filename, filename);
  gtk_label_set_text(GTK_LABEL(config_file->filename_label), filename);

  g_free (filename);

}


/*
FileData * init_config_file (GtkTreeStore * tree_store){

  // first create memory for the file pointer
  FileData *config_file; // struct containing pointers to relevant file data
  config_file = g_malloc (sizeof(FileData)); 
  config_file->fp = NULL;

  char filename[FILENAME_MAX_LENGTH];
  strcpy (filename, "no file selected");
  config_file->filename_label = gtk_label_new(filename);
  
  config_file->tree_store = tree_store;

  return config_file;
}
*/



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


FileData * create_param_import_objects() {

  FileData *config_file; // struct containing pointers to relevant file data
  GtkTreeStore *store;
  //  GtkWidget *tree;



  // first create memory for the file pointer

  config_file = g_malloc (sizeof(FileData)); 
  config_file->fp = NULL;

  char filename[FILENAME_MAX_LENGTH];
  strcpy (filename, "no file selected");
  config_file->filename_label = gtk_label_new(filename);

  store = gtk_tree_store_new (N_COLUMNS,
			      G_TYPE_STRING,
			      G_TYPE_STRING);
  
  config_file->tree_store = store;

  return config_file;
}




GtkWidget* create_notepage_import(PdpGuiObjects * objects) {

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
  g_signal_connect (file_select, "changed", G_CALLBACK(select_file), (gpointer)(objects->config_file) );

  // aesthetic: give this a standard icon
  button_process_configfile = gtk_button_new_with_label ("Load from file");
  g_signal_connect (button_process_configfile, "clicked", 
		    G_CALLBACK(load_from_file_cb), (gpointer)(objects->config_file));

  
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



