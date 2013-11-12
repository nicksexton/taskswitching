
typedef struct pdp_input pdp_input;
typedef struct pdp_layer pdp_layer;
typedef struct pdp_model_component pdp_model_component;
typedef struct pdp_model pdp_model;

/* linked list for tracking unit activation states over time */
typedef struct pdp_units {
  int cycle;
  double * activations;
  struct pdp_units * next;
  struct pdp_units * previous;
} pdp_units;
  

/* A layer of units */
typedef struct pdp_layer {

    int size;

  //     struct pdp_layer * previous; // past iterations
  //     struct pdp_layer * next;     // future iterations

  //     double * units; // pointer to array (ie. row matrix) of unit activation values, UNINITIALISED
    double * net_inputs; // accumulators for summing net input, initialised to zero
    struct pdp_units units_initial; // INITIAL ACTIVATION STATE ie. cycle 0, head of list
    struct pdp_units * units_latest; // MOST RECENT STATE ie. tail of list
    struct pdp_input * upstream_layers; // pointer to the first input (NULL by 
                                        // default and if nothing connected)

} pdp_layer;



/* A weights structure */
typedef struct pdp_weights_matrix {

  int size_input;
  int size_output;
  /* ie define a size_inputs rows x size_outputs cols matrix */

  double ** weights;

} pdp_weights_matrix;


/* container object for the input to a layer - contains links to the
   head of the input layer and the weights matrix */

typedef struct pdp_input {

  pdp_layer * input_layer;
  pdp_weights_matrix * input_weights;
  pdp_input * next;

} pdp_input;


typedef struct pdp_model_component {

  int id;
  pdp_layer * layer;
  pdp_model_component * next;
} pdp_model_component;


/* Union wrapper for pointer to an activation function needed as
   different activation functions take various arguments */
typedef union {
  double (*gs_activation_func) (double net_in, double old_act, 
				double step_sz, double act_max, double act_min);
  // proof of concept
  double (*dummy_act_func) (double some_param, double some_other_param); 

} pdp_activation_function;


typedef struct pdp_model {

  /* model global parameters */
  /* activation function */
  pdp_activation_function activation_function;

  /* components */
  pdp_model_component * components;
  /* pointers to access functions (ie. dump data) */
  /* stopping condition */

} pdp_model;






void pdp_units_free (pdp_units * some_units);

pdp_layer * pdp_layer_create(int size); 

void pdp_layer_free (pdp_layer * some_layer);

int pdp_layer_set_activation(pdp_layer * some_layer, int size, double init_array[size]);

void pdp_layer_print_current_output (pdp_layer * some_layer);

pdp_weights_matrix * pdp_weights_create(int size_output, int size_input);

void pdp_weights_set (struct pdp_weights_matrix * some_weights, 
			 int size_output, int size_input, double init_array[size_output][size_input]);

/* <-------------- Alternate version --------------------->
void pdp_weights_set_v1 (pdp_weights_matrix * some_weights, 
		      int size_output, int size_input, double * init_array);
*/

int pdp_calc_input_fromlayer (int size_output, struct pdp_layer * output, 
			      int size_input, struct pdp_layer * input, 
			      struct pdp_weights_matrix * weights);

void pdp_weights_print(struct pdp_weights_matrix * a_weights_matrix);

void pdp_weights_free (struct pdp_weights_matrix * some_weights);

int pdp_input_connect (struct pdp_layer * this_layer, 
		       struct pdp_layer * input_layer_to_connect, 
		       struct pdp_weights_matrix * weights_to_connect);
/* constructor function which adds a specified input layer & weights
   matrix to the pdp_input object associated with a layer */


void pdp_input_free (pdp_input * input_to_free);

int pdp_layer_cycle_inputs (pdp_layer * some_layer);
/* Model cycle consists of two stages - 1) sum all inputs, and 2)
   update all activations */

// int pdp_layer_cycle_activation (pdp_layer * some_layer); 
int pdp_layer_cycle_activation (pdp_layer * some_layer, pdp_activation_function);

/* calculate new iteration of the layer based on the current inputs of
   connected upstream layers */


pdp_model * pdp_model_create ();
void pdp_model_free (pdp_model * some_model);
pdp_model_component * pdp_model_component_create ();
void pdp_model_component_free (pdp_model_component * some_component);
void pdp_model_component_push (pdp_model * some_model, pdp_layer * layer_add_as_component, int id);
pdp_model_component * pdp_model_component_find(pdp_model * some_model, int id);

void pdp_model_cycle (pdp_model * some_model); 
/* updates inputs for all layers, then updates activation for all
   layers */
