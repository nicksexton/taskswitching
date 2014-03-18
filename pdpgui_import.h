#ifndef pdpgui_import_h
#define pdpgui_import_h

#include "lib_string_parse.h"
#include "pdpgui.h"


// utility function for clearing all entries from a treeview
gboolean treestore_remove_all (GtkTreeStore * tree_store);


// callback function to read file contents
gboolean load_from_file (GtkWidget *widget, FileData *file_info);


void select_file (GtkComboBoxText *widget, FileData * config_file);


void destroy_notepage_fileselect(GtkWidget *notepage_fs, FileData *config_file);

// initialise the config file object
FileData * init_config_file (GtkTreeStore * tree_store);

void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data);

void setup_config_file_treeview (GtkTreeView * tree);

FileData * create_param_import_objects();

GtkWidget* create_notepage_import(PdpGuiObjects * objects);

#endif
