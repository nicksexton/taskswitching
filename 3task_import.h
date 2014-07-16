#ifndef triple_task_import_h
#define triple_task_import_h


#define MAX_FIELDS 10
#define FIELD_SIZE 51
// MAX_LINE_LENGTH should be MAX_FIELDS * FIELD_SIZE
#define MAX_LINE_LENGTH 512

#define FILENAME_MAX_LENGTH 40

#include <stdbool.h>
#include <gtk/gtk.h>

// tree model for storing parameter names/values
enum {
  COL_PARAMETER_NAME,
  COL_PARAMETER_VALUE,
  N_COLUMNS
};


// tree model for task viewer/importer
enum {
  COL_BLOCK_NAME,
  COL_TASK_ID,
  COL_TASK_PATTERN_1,
  COL_TASK_PATTERN_2,
  COL_TASK_PATTERN_3,
  COL_TASK_PATTERN_4,
  COL_TASK_PARAM_1,
  COL_TASK_PARAM_2,
  N_TASK_COLUMNS
};




typedef struct file_data {
  GtkWidget * filename_label; // store filename in the text of a gtk widget
  char filename[FILENAME_MAX_LENGTH];

  FILE * fp; // file pointer itself
  GtkTreeStore * tree_store; // pointer to tree model that will store the data

} FileData;


// <----------------- GENERAL UTILITY FUNCTIONS

// utility function for clearing all entries from a treeview
gboolean treestore_remove_all (GtkTreeStore * tree_store);

// <----------------- Functions for opening config files
// create a FileData object containing file pointer, filename, associated treestore, etc.
// non-gui, model general
void pdp_import_select_file (gchar *filename, FileData * config_file);

// non-gui, model general
FileData * create_param_import_objects();


// non-gui model-general function which parses a file into a treestore. 
// wrapped by load_from_file_short_cb 
gboolean pdp_load_from_file_short (FileData *file_info);

// non-gui model-general function which parses a file into a treestore. 
// wrapped by load_from_file_long_cb 
gboolean pdp_load_from_file_long (FileData *file_info);

// <----------------- File parse and import functions




void pdp_file_segmented_line_to_treestore (int max_fields, 
					   int field_size, 
					   char extracted_fields[max_fields][field_size],
					   GtkTreeStore * store );
  // GTK function - uses a treestore
  // replacement for pdp_file_parse_segmented_line
  // stores first two fields on line (param name and param value) into treestore
  // despite max_fields, only reads first two fields (param name and param value)
  // perhaps tidy this
  // defers conversion of fields from string to other data types - should be done
  // separately by a translator module which reads from the treestore



// imports a long line (ie., more than 2 fields) 
// really unsafe - likely to segfault ie. if max_fields is greater than number of fields in treestore
// need a better system for terminating imported line
void pdp_file_segmented_line_to_treestore_long (int max_fields, 
						int n_fields, // number of fields to extract
						int field_size, 
						char extracted_fields[max_fields][field_size],
						GtkTreeStore * store );


void pdp_file_segmented_line_to_treestore_entire (int max_fields, 
						  int field_size, 
						  char extracted_fields[max_fields][field_size],
						  GtkTreeStore * store );
// really unsafe - likely to segfault ie. if max_fields is greater than number of fields in treestore
// need a better system for terminating imported line



/*
bool pdp_file_parse_segmented_line (int max_fields, 
				    int field_size, 
				    char extracted_fields[max_fields][field_size],
				    GenericParameterSet * destination);
  // this function is program specific
  // contains switch statement instructing program how to parse extracted fields
  // (eg assign to parameters)
  // for a non-example program, break these out to separate functions
  // and put the whole thing in a separate import translator file
*/


int pdp_file_segment_new_line (FILE *input_file, 
			       int max_fields, 
			       int field_size, 
			       char extracted_fields[max_fields][field_size]);
  // dimensions - extracted_fields[max_fields][field_size]
  // parses input file, segments it, filters it for comments etc
  // returns number of fields successfully extracted
  // returns 0 if line is empty,
  // returns -1 if EOF
  // returns -2 if error


int pdp_file_parse_to_treestore (FileData *file_info);
  // alternative version to parse_file that extracts data into a treestore

/*
int parse_file (FILE *config_file, GenericParameterSet *my_params);
  // example code to test string_parse functions 
  // change GenericParameterSet type to a program-specific struct containing parameters
  */

// similar to pdp_file_parse_to_treestore but handles long format lines
int pdp_file_parse_to_treestore_long (FileData *file_info);


#endif


