
typedef struct pdp_input pdp_input;
typedef struct pdp_layer pdp_layer;

/* A layer of units */
typedef struct pdp_layer {

    struct pdp_layer * previous; // past iterations
    struct pdp_layer * next;     // future iterations
    int size;
    double * units; // pointer to array (ie. row matrix) of unit activation values, UNINITIALISED
    double * net_inputs; // accumulators for summing net input, initialised to zero

    struct pdp_input * upstream_layers; // pointer to the first input (NULL by default and if nothing connected)

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





pdp_layer * pdp_layer_create(int size); 
void pdp_layer_free_fromtail(pdp_layer * some_layer);
int pdp_layer_set_activation(pdp_layer * some_layer, int size, double init_array[size]);


pdp_weights_matrix * pdp_weights_create(int size_output, int size_input);
void pdp_weights_set (pdp_weights_matrix * some_weights, 
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

int pdp_input_connect (pdp_layer * this_layer, 
		       pdp_layer * input_layer_to_connect, pdp_weights_matrix * weights_to_connect);


/* constructor function which adds a specified input layer & weights matrix to the pdp_input object associated with a layer */


void pdp_input_free (pdp_input * input_to_free);


struct pdp_layer * pdp_layer_cycle (pdp_layer * some_layer); // calculate new iteration of the layer based on the current inputs of connected upstream layers
