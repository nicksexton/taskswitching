/*  Model-general functions associated with the gui. */ 
/*  Possibly merge into general pdpgui library if it ends up small? */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "pdp_import.h"
#include "pdpgui_import.h"
#include "gs_stroop_gui.h"
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






//<---------------------- MODEL TASK IMPORT FUNCTIONS ------------------------






// temp code - for translating task blocks (ie an array of stroop_trial_data objects) to treestore
/*
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
*/



