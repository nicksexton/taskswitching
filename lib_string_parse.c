/* TODO:
   fix parsing to use sscanf or similar to assign numeric variables 
   fix parsing so that space or comma can also be used as a delimiter */

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lib_string_parse.h"


/*
void init_generic_parameter (GenericParameter * param) {

  // init default parameter settings here
  param->id = 0;
  strcpy (param->name, "blank");
  param->data_int_1 = 0;
  param->data_int_2 = 0;
  param->data_int_3 = 0;
  param->data_double_1 = 0.0;
  param->data_double_2 = 0.0;
  param->data_double_3 = 0.0;
  strcpy (param->data_text_1, "");
  strcpy (param->data_text_2, "");

}
*/

/*
void init_generic_parameter_set (GenericParameterSet *param_set) {

  init_generic_parameter (&(param_set->parameter_1));
  init_generic_parameter (&(param_set->parameter_2));
  init_generic_parameter (&(param_set->parameter_3));
  init_generic_parameter (&(param_set->parameter_4));
  init_generic_parameter (&(param_set->parameter_5));


  param_set->parameter_1.id = 1;
  strcpy (param_set->parameter_1.name, "NUMERIC_VAR1");

  param_set->parameter_2.id = 2;
  strcpy (param_set->parameter_2.name, "NUMERIC_VAR2");

  param_set->parameter_3.id = 3;
  strcpy (param_set->parameter_3.name, "NUMERIC_VAR3");

  param_set->parameter_4.id = 4;
  strcpy (param_set->parameter_3.name, "TEXT_VAR1");

  param_set->parameter_5.id = 5;
  strcpy (param_set->parameter_3.name, "TEXT_VAR2");


}
*/

/*
void print_generic_parameter (GenericParameter * param) {

  printf ("\nParameter %s, id %d:\n", param->name, param->id);
  printf ("Int data:\t1: %d\t2: %d\t 3: %d\n", 
	  param->data_int_1, param->data_int_2, param->data_int_3);
  printf ("Double data:\t1: %4.2f\t2: %4.2f\t 3: %4.2f\n", 
	  param->data_double_1, param->data_double_2, param->data_double_3);
  printf ("Text data:\t1: %s\t2: %s\n", 
	  param->data_text_1, param->data_text_2);

}
*/

/*
void print_generic_parameter_set (GenericParameterSet * param) {

  print_generic_parameter (&(param->parameter_1));
  print_generic_parameter (&(param->parameter_2));
  print_generic_parameter (&(param->parameter_3));
  print_generic_parameter (&(param->parameter_4));
  print_generic_parameter (&(param->parameter_5));

}
*/


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


/*
bool pdp_file_parse_segmented_line (int max_fields, 
				    int field_size, 
				    char extracted_fields[max_fields][field_size],
				    GenericParameterSet * destination) {
  // this function is program specific
  // for a non-example program, break these out to separate functions
  // and put the whole thing in a separate import translator file

  // THIS CODE (!strcmp) IS SCREWY!!!   
  // decide how to process line based on field 0
  if (!strcmp (extracted_fields[0], "NUMERIC_VAR1")) {
    
    if (strcmp (extracted_fields[1], "") != 0) {
      destination->parameter_1.data_int_1 = atoi (extracted_fields[1]);
      if (strcmp (extracted_fields[2], "") != 0) {
	destination->parameter_1.data_int_2 = atoi (extracted_fields[2]);
      }
    }
    return true;
  }


  else if (!strcmp (extracted_fields[0], "NUMERIC_VAR2")) {
    
    if (strcmp (extracted_fields[1], "") != 0) {
      destination->parameter_2.data_int_1 = atoi (extracted_fields[1]);
      if (strcmp (extracted_fields[2], "") != 0) {
	destination->parameter_2.data_int_2 = atoi (extracted_fields[2]);
      }
    }
    return true;
  }


  else if (!strcmp (extracted_fields[0], "NUMERIC_VAR3")) {
    
    if (strcmp (extracted_fields[1], "") != 0) {
      destination->parameter_3.data_int_1 = atoi (extracted_fields[1]);
      if (strcmp (extracted_fields[2], "") != 0) {
	destination->parameter_3.data_int_2 = atoi (extracted_fields[2]);
      }
    }
    return true;
  }


  else if (!strcmp (extracted_fields[0], "TEXT_VAR1")) {
    
    if (strcmp (extracted_fields[1], "") != 0) {
      strcpy (destination->parameter_4.data_text_1, extracted_fields[1]); 
      if (strcmp (extracted_fields[2], "") != 0) {
	strcpy (destination->parameter_4.data_text_2, extracted_fields[2]); 
      }
    }
    return true;
  }

  else if (!strcmp (extracted_fields[0], "TEXT_VAR2")) {
    
    if (strcmp (extracted_fields[1], "") != 0) {
      strcpy (destination->parameter_5.data_text_1, extracted_fields[1]);
      if (strcmp (extracted_fields[2], "") != 0) {
	strcpy (destination->parameter_5.data_text_2, extracted_fields[2]); 
      }
    }
    return true;
  }


  return false;
}
*/


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

/*
// example code to test string_parse functions 
// change GenericParameterSet type to a program-specific struct containing parameters
int parse_file (FILE *config_file, GenericParameterSet *my_params) {


  char fields [MAX_FIELDS][FIELD_SIZE];
  int line_counter = 0;
  int fields_extracted;
  bool more_lines = true;

  while (more_lines) {
    line_counter ++;
    fields_extracted = pdp_file_segment_new_line (config_file, 
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

	if (pdp_file_parse_segmented_line (MAX_FIELDS, 
					   FIELD_SIZE, 
					   fields,
					   my_params)) {
	  printf ("line imported\n");
	}
	else {
	  printf ("warning! line not understood, not imported\n");
	}

	// else do nothing
      }
      else {
	printf ("blank line or comment, ignoring\n");
      }
    }
    }

  }

  return 0;
}
*/

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



/*
int main () {


  FILE *config_file;

  config_file = fopen("gtk_config_file.conf", "r");
  if (config_file == NULL) {
    printf ("error! gtk_config_file.conf not opened!\n");
    return 1;
  }

  GenericParameterSet my_params;
  init_generic_parameter_set (&my_params);
  
  my_params.parameter_1.id = 1;
  strcpy (my_params.parameter_1.name, "NUMERIC_VAR1");

  my_params.parameter_2.id = 2;
  strcpy (my_params.parameter_2.name, "NUMERIC_VAR2");

  my_params.parameter_3.id = 3;
  strcpy (my_params.parameter_3.name, "NUMERIC_VAR3");

  my_params.parameter_4.id = 4;
  strcpy (my_params.parameter_3.name, "TEXT_VAR1");

  my_params.parameter_5.id = 5;
  strcpy (my_params.parameter_3.name, "TEXT_VAR2");


  printf ("starting parameter settings:\n");
  print_generic_parameter_set (&my_params);
  

  parse_file (config_file, &my_params);


  printf ("new parameter settings:\n");
  print_generic_parameter_set (&my_params);

  return 0;

}

*/
