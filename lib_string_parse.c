/* TODO:
   fix parsing to use sscanf or similar to assign numeric variables 
   fix parsing so that space or comma can also be used as a delimiter */

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lib_string_parse.h"





// GTK function - uses a treestore
// replacement for pdp_file_parse_segmented_line
// stores first two fields on line (param name and param value) into treestore
// despite max_fields, only reads first two fields (param name and param value)
// perhaps tidy this
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

