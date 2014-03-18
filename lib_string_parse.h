#ifndef lib_string_parse_h
#define lib_string_parse_h

#define MAX_FIELDS 10
#define FIELD_SIZE 51
// MAX_LINE_LENGTH should be MAX_FIELDS * FIELD_SIZE
#define MAX_LINE_LENGTH 512

#define FILENAME_MAX_LENGTH 40

#include <stdbool.h>

// tree model for storing parameter names/values
enum {
  COL_PARAMETER_NAME,
  COL_PARAMETER_VALUE,
  N_COLUMNS
};


typedef struct file_data {
  GtkWidget * filename_label; // store filename in the text of a gtk widget
  char filename[FILENAME_MAX_LENGTH];

  FILE * fp; // file pointer itself
  GtkTreeStore * tree_store; // pointer to tree model that will store the data

} FileData;



/*
typedef struct generic_parameter {
  int id;
  char name[FIELD_SIZE];
  int data_int_1, data_int_2, data_int_3;
  double data_double_1, data_double_2, data_double_3;
  char data_text_1[FIELD_SIZE], data_text_2[FIELD_SIZE];
} GenericParameter;
*/

/*
typedef struct generic_parameter_set {

  GenericParameter parameter_1;
  GenericParameter parameter_2;
  GenericParameter parameter_3;
  GenericParameter parameter_4;
  GenericParameter parameter_5;

} GenericParameterSet;
*/

// void init_generic_parameter (GenericParameter * param);

// void init_generic_parameter_set (GenericParameterSet *param_set);

// void print_generic_parameter (GenericParameter * param);


// void print_generic_parameter_set (GenericParameterSet * param);



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


#endif


