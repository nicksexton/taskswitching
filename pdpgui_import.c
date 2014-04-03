
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "lib_string_parse.h"
#include "pdpgui_import.h"
#include "pdpgui.h"

#define CONFIG_FILE gtk_config_file.conf




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
// uses short format (ie key - value pairs)
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


// similar to load_from_file_cb but imports long format data (ie multiple columns)
gboolean load_from_file_long_cb (GtkWidget *widget, FileData *file_info) {

  file_info->fp = fopen(file_info->filename, "r");
  if (file_info->fp == NULL) {
    printf ("error! gtk_config_file.conf does not exist\n");
    return FALSE;
  }
  else {
    printf ("success! config file opened.\n");

    // currently removes all from treestore before import, want to fix this ultimately
    treestore_remove_all (file_info->tree_store); 

    pdp_file_parse_to_treestore_long (file_info);

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

// <------------------------------- MODEL PARAMETER IMPORT FUNCTIONS -----------------------


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

//<---------------------- MODEL TASK IMPORT FUNCTIONS ------------------------


void model_reset_trial_markers (PdpSimulation *simulation) {

  GtkTreePath * current_trial_path = gtk_tree_path_new_from_string ("0:0");
  simulation->current_trial_path = current_trial_path;
  // causes memory leak??

  
  GtkTreeIter * iter = g_malloc (sizeof(GtkTreeIter));
  gtk_tree_model_get_iter (GTK_TREE_MODEL(simulation->task_store), iter, current_trial_path);
  simulation->current_trial_iter = iter;

}


static void import_stroop_trial_data_to_treestore (GtkTreeStore * store, 
						   GtkTreeIter *iter, 
						   stroop_trial_data* data) {

  //  GtkTreeIter iter1;
  //  gtk_tree_store_append (store, &iter1, NULL);

  // defer string conversion, save everything as a string

  // write a translator to convert from number into binary pattern
  gtk_tree_store_set (store, iter, 
		      COL_TASK_ID, data->trial_id, 
		      COL_TASK_PATTERN_1, data->stim_word, 
		      COL_TASK_PATTERN_2, data->stim_colour,
		      COL_TASK_PATTERN_3, data->stim_task,
      		      -1);

  /*
enum {
  COL_TASK_ID,
  COL_TASK_PATTERN_1,
  COL_TASK_PATTERN_2,
  COL_TASK_PATTERN_3,
  COL_TASK_PARAM_1,
  COL_TASK_PARAM_2,
  N_TASK_COLUMNS
};
  */
}


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




// use task_store as PdpGuiObjects member as a buffer for importing tasks (task blocks)
FileData * create_task_import_objects() {

  FileData *config_file; // struct containing pointers to relevant file data
  GtkTreeStore *store;
  //  GtkWidget *tree;

  // first create memory for the file pointer

  config_file = g_malloc (sizeof(FileData)); 
  config_file->fp = NULL;

  char filename[FILENAME_MAX_LENGTH];
  strcpy (filename, "no file selected");
  config_file->filename_label = gtk_label_new(filename);

  // Need to do string to int conversion when transferring out of buffer to task store
  store = gtk_tree_store_new (N_TASK_COLUMNS,
			      G_TYPE_STRING,
			      G_TYPE_STRING,
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING, 
			      G_TYPE_STRING);
  
  config_file->tree_store = store;

  return config_file;
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


  /*
  stroop_trial_data mock_data;
  mock_data.trial_id = 999;
  mock_data.stim_task = 0;
  mock_data.stim_word = 2;
  mock_data.stim_colour = 1;

  import_stroop_trial_data_to_treestore (objects->task_config_file->tree_store, &mock_data);
  */



  /*
  subject * this_subject = objects->simulation->subjects->subj[objects->simulation->current_subject];

  import_task_block_new_to_treestore (objects->simulation->task_store,
				      "random stroop",
				      this_subject->num_fixed_trials,
				      this_subject->fixed_trials);
  */


  // TEMP CODE: set path to first trial of first block

  model_reset_trial_markers (objects->simulation);

  return (grid_main);

}




// read tasks from the tree store, add to task store
// key function for task import!
static void model_task_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects) {

  // OLD CODE
  
  GtkTreeIter iter_import; 
  GtkTreeIter iter_import_append;
  gboolean more;


  // get iterator at top of the import store 
  more = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(objects->task_config_file->tree_store), &iter_import);



  while (more) {

    // create stroop trial data object
    stroop_trial_data imported_trial;
    gchar *import_trial_id;
    gchar *import_stim_task;
    gchar *import_stim_word;
    gchar *import_stim_colour;


    gtk_tree_model_get (GTK_TREE_MODEL(objects->task_config_file->tree_store), &iter_import, 
			COL_TASK_ID, &import_trial_id, 
			COL_TASK_PATTERN_1, &import_stim_word, 
			COL_TASK_PATTERN_2, &import_stim_colour,
			COL_TASK_PATTERN_3, &import_stim_task, 
			-1);

    // need to set defaults here to prevent seg fault if data not imported??
    // or, are defaults set in task_config_file when importing from text file...?
    imported_trial.trial_id = atoi (import_trial_id);
    imported_trial.stim_task = atoi (import_stim_task);
    imported_trial.stim_word = atoi (import_stim_word);
    imported_trial.stim_colour = atoi (import_stim_colour);


    gtk_tree_store_append (objects->simulation->task_store, &iter_import_append, NULL);
    import_stroop_trial_data_to_treestore(objects->simulation->task_store, &iter_import_append, &imported_trial);
    
    more = gtk_tree_model_iter_next(GTK_TREE_MODEL(objects->task_config_file->tree_store), &iter_import);

    g_free (import_trial_id);

  }  
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
				 "gtk_config_file_1.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "gtk_config_file_2.conf");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(file_select), 
				 "task_import_test.conf");


  g_signal_connect (file_select, "changed", G_CALLBACK(select_file), (gpointer)(objects->task_config_file) );

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
