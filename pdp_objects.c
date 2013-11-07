#include <stdlib.h>
#include <stdio.h>
#include "pdp_objects.h"
#include "activation_functions.h"

/* only for offsetof macro */
#include <stddef.h>


#define ACT_MAX 1.0
#define ACT_MIN -1.0
#define STEP_SIZE 0.0015


/* Access functions: */
/* 1. reaction time (number of cycles to reach stopping condition) */
/* 2. response (ie. which node wins) */
/* 3. dump all activation values (ie. for drawing a graph of act vs. time) */


pdp_units * pdp_units_create (int size) {

    pdp_units * a_units_element;
    a_units_element = malloc(sizeof(pdp_units));
    a_units_element->activations = malloc (size * (sizeof(double)));

    return a_units_element;
}



void pdp_units_free (pdp_units * some_units) {
  /* frees from head (initial) -> tail (latest) of list */
  if (some_units == NULL) {
    printf ("end of (units) list reached, all done!\n");
    return;
  }
  else {
    pdp_units * tmp = some_units->next;
    printf ("freeing pdp_units struct from memory at %p\n", some_units);
    free (some_units->activations);
    free (some_units);
    pdp_units_free (tmp);

    return;
  }
}


pdp_layer * pdp_layer_create(int size) {

    pdp_layer * new_layer;
    int i;

    new_layer = malloc (sizeof(pdp_layer));
    new_layer->size = size;

    /* array of accumulators, to sum net input  */
    new_layer->net_inputs = malloc (size * (sizeof(double)));

    for (i = 0; i < size; i++) {
      new_layer->net_inputs[i] = 0;
    }

    /* instead of calling pdp_units_create, init the head of the units list here */
    new_layer->units_initial.previous = NULL;
    new_layer->units_initial.next = NULL;
    new_layer->units_initial.cycle = 0;
    new_layer->units_initial.activations = malloc (size * (sizeof(double)));
    new_layer->units_latest = &(new_layer->units_initial);
    
    /* inputs list pointer defaults to NULL */
    new_layer->upstream_layers = NULL;

    return new_layer;
}


    

void pdp_layer_free (pdp_layer * some_layer) {

    if (some_layer == NULL) {
      // printf ("end of list reached, all done!\n");
        return;
    }
    else {

      /* first free the upstream input list */
      if (some_layer->upstream_layers != NULL) {
	pdp_input_free (some_layer->upstream_layers);
      }
      some_layer->units_latest = NULL; // list no longer exists


      /* then free the units */
        pdp_units_free (some_layer->units_initial.next);
	free (some_layer->units_initial.activations);
        

	/* free the input accumulators */
	free (some_layer->net_inputs);
        
	free (some_layer);
	
	return;
    }
}


int pdp_layer_set_activation(pdp_layer * some_layer, int size, double init_array[size]){
  /* sets the activation of the unit_activation state referenced by
     the TAIL pointer ie. sets activation for MOST RECENT CYCLE
     only. If this is called on initialising the model then this is
     the initial state. */

  int i;
  
  if (some_layer->size != size) {
    /* raise an exception! */
    printf ("\nerror! tried to set unit activations to an incorrectly sized array\n");
    return 1;
  }

  for (i = 0; i < size; i++) {
    some_layer->units_latest->activations[i] = init_array[i];
  }
  return 0;
}


pdp_weights_matrix * pdp_weights_create(int size_output, int size_input) {

  /*  TODO could probably combine this function with an initializer,
      can't think of a context you would want a weights matrix but not
      initialise it */

  pdp_weights_matrix * new_weights;
  new_weights = malloc (sizeof(pdp_weights_matrix));
  new_weights->size_input = size_input;
  new_weights->size_output = size_output;
  new_weights->weights = malloc (size_output * sizeof(double*));
  
  int i;
  for (i = 0; i < size_output; i++) {
    new_weights->weights[i] = malloc(size_input * sizeof(double));
  }


  return new_weights;
}


void pdp_weights_set (pdp_weights_matrix * some_weights, 
		      int size_output, int size_input, double init_array[size_output][size_input]) {
  int out, in;

  for (out = 0; out < some_weights->size_output; out++) {
    for (in = 0; in < some_weights->size_input; in++) {
      some_weights->weights[out][in] = init_array[out][in];
    }
  }

  return;

}


/* <-------------- ALTERNATE VERSION ---------------> */
/*  
void pdp_weights_set_v1 (pdp_weights_matrix * some_weights, 
		      int size_output, int size_input, double * init_array) {
*/

/* initialises a weights matrix based on a specified array (ie. a
     calling function can set weights by using this function to point
     a weights matrix at a 2-dimensional array) */

  /* call like this:
       pdp_weights_set (some_weights, 3, 5, (double*)local_weights_matrix);
   */
/*
  int out, in;

  for (out = 0; out < some_weights->size_output; out++) {
    for (in = 0; in < some_weights->size_input; in++) {
      some_weights->weights[out][in] = init_array[size_input * out + in];
    }
  }

  return;
}
*/



void pdp_weights_print (struct pdp_weights_matrix * some_weights) {
/* mainly intended for debugging; print out weights matrix contained in the struct */
  int out, in;
  printf ("\n");
  for (out = 0; out < some_weights->size_output; out++) {
    printf ("|\t");
    for (in = 0; in < some_weights->size_input; in++) {
      printf ("%4.2f\t", some_weights->weights[out][in]);
    }
    printf ("|\n");
  }
  printf ("\n");
  return;
}

void pdp_weights_free (struct pdp_weights_matrix * some_weights) {
  int i;
  for (i = 0; i < some_weights->size_output; i ++) {
    free (some_weights->weights[i]);
  }
  free (some_weights->weights);
  free (some_weights);
  return;
}




/* calculates the input to layer i from layer j by multiplying a
   weights matrix (W_ij) with a vector of activations reflecting an
   input layer. Implements safety checks that the size of the
   matrix & the layer correspond. The result (input) is written to a
   set of accumulators in a specified output matrix. */

int pdp_calc_input_fromlayer (int size_output, struct pdp_layer * output, 
			      int size_input, struct pdp_layer * input, 
			       struct pdp_weights_matrix * weights) {

  /* check sizes */
  int size_inputs, size_outputs, i, j;

  size_inputs = weights->size_input;
  size_outputs = weights->size_output;

  if (input->size != size_inputs) {
    /* raise an exception here!! */
    printf ("\nERROR! input size (%d) and weights matrix (%d) do not agree, terminating\n",
	    input->size, size_inputs);

    return 1;
  }

  else if  (output->size != size_outputs) {
    /* raise an exception here!! */
    printf ("\nERROR! output size (%d) and weights matrix (%d) do not agree, terminating\n",
	    output->size, size_outputs);

    return 1;
  }

  else {
    /* do the matrix multiplication */


    

    for (i = 0; i < size_output; i++) { /* calculate input to the ith output neuron */

    /* remember to zero the accumulators! */
      output->net_inputs[i] = 0;

      for (j = 0; j < size_input; j++) { /* calculate weighted input from jth input neuron */
	output->net_inputs[i] += input->units_latest->activations[j] * weights->weights[i][j];
      }
    }

  return 0;
  }

}
      
int pdp_input_connect (pdp_layer * downstream_layer, pdp_layer * upstream_layer, 
		       pdp_weights_matrix * upstream_weights) {
  /* connects a single upstream layer (via set of connection weights) */
  
  /* TODO - code to check size of matrix and layers corresponds */

  pdp_input * p_input_tmp;
  p_input_tmp = malloc (sizeof (pdp_input));
  
  p_input_tmp->next = downstream_layer->upstream_layers;
  p_input_tmp->input_layer = upstream_layer;
  p_input_tmp->input_weights = upstream_weights;

  downstream_layer->upstream_layers = p_input_tmp;

  return 0;
}

void pdp_input_free (pdp_input * input_to_free) {
  if (input_to_free == NULL) {
    return;
  }
  else {
    printf ("freeing input list item\n");
    pdp_input * tmp = input_to_free->next;
    free (input_to_free);
    pdp_input_free (tmp);
  }
}

int pdp_layer_cycle (pdp_layer * some_layer) {



  /* TODO - activation function should be pointer to a function */

  /* calculates a new iteration of the layer based on its upstream inputs */
      /* create new units element */
  
  pdp_input * input_iterator; 
  pdp_units * units_new = pdp_units_create (some_layer->size);


  units_new->next = NULL;
  units_new->previous = some_layer->units_latest;      
  units_new->cycle = (units_new->previous->cycle) + 1;
  
 


      /* make new linkages */
  some_layer->units_latest->next = units_new;
  some_layer->units_latest = units_new;
      
  /* get the input iterator */
  input_iterator = some_layer->upstream_layers;
      

      /* calculate inputs */
  while (input_iterator != NULL) {

    pdp_calc_input_fromlayer (some_layer->size, some_layer, 
			      input_iterator->input_layer->size, input_iterator->input_layer, 
			      input_iterator->input_weights);


	/* <--------- test code: -----------> */
	printf ("output unit activations: ");

	int outputunit = 0;
	for (outputunit = 0; outputunit < some_layer->size; outputunit ++) {
	  some_layer->units_latest->activations[outputunit] = 
	    act_gs(some_layer->net_inputs[outputunit],
		   some_layer->units_latest->previous->activations[outputunit],
		   STEP_SIZE, ACT_MAX, ACT_MIN);
	  
	  printf ("\t[%d]: %4.2f\t", outputunit, some_layer->units_latest->activations[outputunit]);
	}


	printf ("\n");

	/* <--------- end test code ---------> */
	input_iterator = input_iterator->next;
      }

      return 0;
}




int main () {
 
    pdp_layer * an_input;
    pdp_layer * an_output;


    an_input = pdp_layer_create(5);
    an_output = pdp_layer_create(3);

    double initial_input_activations[5] = {0.1, 1.0, 0.1, 0.5, 0.1};
    double initial_output_activations[3] = {0.1, 0.1, 0.1};

    pdp_layer_set_activation(an_input, 5, initial_input_activations);
    pdp_layer_set_activation(an_output, 3, initial_output_activations);

    /* init some weights */
    pdp_weights_matrix * some_weights;
    some_weights = pdp_weights_create (3,5);


    double local_weights_matrix[3][5] = {
      {0.0,  1.0, -4.0,  3.1,  2.5},
      {5.3, -1.3,  0.4, -5.2,  1.4},
      {3.0, -7.3, -3.5,  1.2,  7.1},
    };


    pdp_weights_set (some_weights, 3, 5, local_weights_matrix);

    pdp_input_connect (an_output, an_input, some_weights);



    /* cycle the model 10 times */

    int i;
    for (i = 0; i < 10; i++) {

      pdp_layer_cycle (an_output);

    }


    //    pdp_weights_print (some_weights);

    pdp_layer_free (an_input);
    pdp_layer_free (an_output);
    pdp_weights_free (some_weights);

    //    printf ("size of pdp_input: %lu", sizeof(pdp_input));
    // printf ("size of pdp_layer: %lu", sizeof(pdp_layer));
    // printf ("size of pdp_weights: %lu", sizeof(pdp_weights_matrix));

    return 0;

}
