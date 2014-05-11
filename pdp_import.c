/* pdp_import */
/* library of model-generic functions for importing text file data to pdp models */
/* functions for parsing and importing task data and parameterisation data */
/* needs to be specialised to import data in a model-specific format */
/* Model import uses gtk treestore to store task and parameter data */
/* These functions are usable in both gui and non-gui versions of the model */
/* See pdpgui_import for functions which adapt these to a gui format */



/* (old) TODO:
   fix parsing to use sscanf or similar to assign numeric variables 
   fix parsing so that space or comma can also be used as a delimiter */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pdp_import.h"



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

// <--------------------- Functions for opening config files

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



// GTK function - uses a treestore
// replacement for pdp_file_parse_segmented_line
// stores first two fields on line (param name and param value) into treestore
// despite max_fields, only reads first two fields (param name and param value)
// perhaps tidy this
// NB used by parameter import (format- key: value)
void pdp_file_segmented_line_to_treestore (int max_fields, 
					   int field_size, 
					   char extracted_fields[max_fields][field_size],
					   GtkTreeStore * store ) {

  GtkTreeIter iter1;

  gtk_tree_store_append (store, &iter1, NULL);

  // defer string conversion, save everything as a string
  gtk_tree_store_set (store, &iter1, 
		      COL_PARAMETER_NAME, extracted_fields[0], -1);

  gtk_tree_store_set (store, &iter1, 
		      COL_PARAMETER_VALUE, extracted_fields[1], -1);

}


// imports a long line (ie., more than 2 fields) 
// really unsafe - likely to segfault ie. if max_fields is greater than number of fields in treestore
// need a better system for terminating imported line
void pdp_file_segmented_line_to_treestore_long (int max_fields,
						int n_fields, // number of fields to extract
						int field_size, 
						char extracted_fields[max_fields][field_size],
						GtkTreeStore * store ) {



  GtkTreeIter iter1;

  gtk_tree_store_append (store, &iter1, NULL);

  int n;
  for (n = 0; n < n_fields; n ++) {

    // defer string conversion, save everything as a string
    gtk_tree_store_set (store, &iter1, 
			n, extracted_fields[n], -1);

  }
}




int pdp_file_segment_new_line (FILE *input_file, 
			     int max_fields, 
			     int field_size, 
			     char extracted_fields[max_fields][field_size]) {
  // dimensions - extracted_fields[max_fields][field_size]
  // parses input file, segments it, filters it for comments etc
  // returns number of fields successfully extracted
  // returns 0 if line is empty,
  // returns -1 if EOF
  // returns -2 if error

  // clears contents of char array


  char input_line [MAX_LINE_LENGTH];
  char *ptr, *ptr_eol; // current position, end of line
  char *comment_marker_hash = "#"; // everything after hash to end of line 
                                   // interpreted as comment

  int n, f;

  // clear the array
  for (f = 0; f < max_fields; f++) {
    strcpy(extracted_fields[f], "");
  }

  fgets(input_line, sizeof input_line, input_file);
  
  if (input_line == NULL) {
    return 0;
  }
  else {

    ptr = input_line;

    if (feof(input_file)) {
      printf ("end of file reached, all is good\n");
      return -1;
    }
  
    if (ferror (input_file)) {
      printf ("a file read error occurred, exiting\n");
      return -2;
    } 


    // check if line contains a comment, position end of line pointer at the comment
    ptr_eol = strstr(input_line, comment_marker_hash);
    if (ptr_eol == NULL)
      ptr_eol = ptr + strlen(input_line);
    
    
    f = 0;
    while (ptr < ptr_eol) {

      // This line does not work very well
      // field delimiters - tabs, newlines
      sscanf (ptr, "%50[^\t\n ]%n", extracted_fields[f], &n);

      if ((ptr + n) > ptr_eol) {
	return f;
      }
      else {
	ptr += n;

	if ((*ptr != '\t') && (*ptr != '\n') && (*ptr != ' ')) break; // no delimiter, end of file reached?
	// need to handle condition here where \t is followed by a \n 
      

	ptr += 1; // skip the delimiter
	while ((*ptr == '\t') || (*ptr == '\n') || (*ptr == ' ')) {
	  ptr += 1; 
	  f++;
	}
	f ++;
      }
    }
    return f;
  }
}



// more general code - parses an entire config file and transfers first two fields
// on each line to a to treestore
int pdp_file_parse_to_treestore (FileData *file_info) {



  char fields [MAX_FIELDS][FIELD_SIZE];
  int line_counter = 0;
  int fields_extracted;
  bool more_lines = true;

  while (more_lines) {
    line_counter ++;
    fields_extracted = pdp_file_segment_new_line (file_info->fp, 
						  MAX_FIELDS, 
						  FIELD_SIZE, 
						  fields);

    switch (fields_extracted) {
    case -2: {
      printf ("File read error, exiting!\n");
      exit(EXIT_FAILURE);
    }
  
    case -1: {
      printf ("End of file reached, no more lines to get\n");
      more_lines = false;
      break;
    }

    default: {
      printf ("processing line %d\t", line_counter);
      // process the data
      if (fields_extracted > 0) { // was any data extracted?
	// if so, process the data

	pdp_file_segmented_line_to_treestore (MAX_FIELDS, 
					      FIELD_SIZE, 
					      fields,
					      file_info->tree_store );

	printf ("imported - %s:\t%s\n", fields[0], fields[1]);

      }
      else {
	printf ("blank line or comment, ignoring\n");
      }
    }
    }

  }

  return 0;
}



// similar to pdp_file_parse_to_treestore but handles long format lines
int pdp_file_parse_to_treestore_long (FileData *file_info) {



  char fields [MAX_FIELDS][FIELD_SIZE];
  int line_counter = 0;
  int fields_extracted;
  bool more_lines = true;

  while (more_lines) {
    line_counter ++;
    fields_extracted = pdp_file_segment_new_line (file_info->fp, 
						  MAX_FIELDS, 
						  FIELD_SIZE, 
						  fields);

    switch (fields_extracted) {
    case -2: {
      printf ("File read error, exiting!\n");
      exit(EXIT_FAILURE);
    }
  
    case -1: {
      printf ("End of file reached, no more lines to get\n");
      more_lines = false;
      break;
    }

    default: {
      printf ("processing line %d\t", line_counter);
      // process the data
      if (fields_extracted > 0) { // was any data extracted?
	// if so, process the data

	pdp_file_segmented_line_to_treestore_long (MAX_FIELDS,
						   fields_extracted,
						   FIELD_SIZE, 
						   fields,
						   file_info->tree_store );

	printf ("imported - %s:\t%s\n", fields[0], fields[1]);

      }
      else {
	printf ("blank line or comment, ignoring\n");
      }
    }
    }

  }

  return 0;
}
