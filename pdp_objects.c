#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_randist.h>
#include "pdp_activation_funcs.h"
#include "pdp_objects.h"
#include "random_generator_functions.h" // for gaussian noise

#define MODEL_NAME_MAX_LENGTH 30


/* TODO - connect bias nodes as inputs */


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
  /*
  if (some_units == NULL) {
    // printf ("end of (units) list reached, all done!\n");
    return;
  }
  else {
    pdp_units * tmp = some_units->next;
    // printf ("freeing pdp_units struct from memory at %p\n", some_units);
    free (some_units->activations);
    free (some_units);
    pdp_units_free (tmp);

    return;

  
  }
  */
  // Non-recursive version
  while (some_units != NULL) {
    pdp_units * tmp = some_units->next;
    free (some_units->activations);
    free (some_units);
    some_units = tmp;
  }
  return;

}


pdp_layer * pdp_layer_create(const int id, const int size, double bias) {

    pdp_layer init = { .id = id, .size = size };

    pdp_layer * new_layer;
    int i;  

    new_layer = malloc (sizeof(pdp_layer));
    memcpy (new_layer, &init, sizeof (*new_layer));
  
    new_layer->input_bias = bias;

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

/* eg. for adding noise to activation levels of PDP units */
/* does vector addition on activation values of the latest units */
int pdp_layer_modulate_activation (pdp_layer * some_layer, int size, double modulating_array[size]){
  
  int i;

  if (some_layer->size != size) {
    /* raise an exception! */
    printf ("\nerror! tried to set unit activations to an incorrectly sized array\n");
    return 1;
  }

  for (i = 0; i < size; i++) {
    some_layer->units_latest->activations[i] += modulating_array[i];
  }
  return 0;
}


void pdp_layer_print_current_output (const pdp_layer * some_layer) {
  int i;
  for (i = 0; i < some_layer->size; i++) {
    // printf ("[%d]: %4.2f  ", i, some_layer->units_latest->activations[i]);
    printf ("%5.4f\t", some_layer->units_latest->activations[i]);
  }
  // printf ("\n");
}

void pdp_layer_fprintf_current_output (const pdp_layer * some_layer, FILE *fp) {
  int i;
  for (i = 0; i < some_layer->size; i++) {
    // printf ("[%d]: %4.2f  ", i, some_layer->units_latest->activations[i]);
    fprintf (fp, "%5.4f\t", some_layer->units_latest->activations[i]);
  }
  // printf ("\n");
}

  

void pdp_layer_print_activation (const pdp_layer * some_layer) {
  int i;
  const pdp_units *units_i = &(some_layer->units_initial);
  
  printf ("\n");

  while (units_i != NULL) {
    printf ("cycle:%d\t", units_i->cycle);
    for (i = 0; i < some_layer->size; i++) {
      printf ("%4.2f\t", units_i->activations[i]);
    }
    printf ("\n");
    units_i = units_i->next;

  }
}


double* pdp_layer_get_unit_activation_history (const pdp_layer * some_layer, int unit_index, int current_cycle) {

// returns a pointer to a vector of unit activations for specified unit in the layer, 
// up to current_cycle 

  if (current_cycle > 0) {

    double *activations = malloc (current_cycle * sizeof(double));
    const pdp_units *units_i = &(some_layer->units_initial);
    int i;
  
    for (i = 0; i < current_cycle; i ++) {
      if (units_i == NULL) {
	return NULL;
      }

      activations[i] = units_i->activations[unit_index];
      units_i = units_i->next;

    }

    return activations;
  }
  else return NULL;
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

void pdp_weights_increment (pdp_weights_matrix * some_weights, 
			    int size_output, int size_input, double init_array[size_output][size_input]) {
  int out, in;

  for (out = 0; out < some_weights->size_output; out++) {
    for (in = 0; in < some_weights->size_input; in++) {
      some_weights->weights[out][in] += init_array[out][in];
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
    printf (" | ");
  }
  // printf ("\n");
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
			      int size_input, const struct pdp_layer * input, 
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
      
      for (j = 0; j < size_input; j++) { /* calculate weighted input from jth input neuron */
	output->net_inputs[i] += input->units_latest->activations[j] * weights->weights[i][j];
      }
    }

  return 0;
  }
}
      
int pdp_input_connect (pdp_layer * downstream_layer, const pdp_layer * upstream_layer, 
		       pdp_weights_matrix * upstream_weights) {
  /* connects a single upstream layer (via set of connection weights) */
  
  /* TODO - code to check size of matrix and layers corresponds */

  pdp_input init = { .id_of_input_layer = upstream_layer->id, 
		     .input_layer = upstream_layer };

  pdp_input * p_input_tmp = malloc (sizeof (pdp_input));
  memcpy (p_input_tmp, &init, sizeof (*p_input_tmp));

  p_input_tmp->next = downstream_layer->upstream_layers;
  p_input_tmp->input_weights = upstream_weights;

  downstream_layer->upstream_layers = p_input_tmp;

  return 0;
}


// returns pointer to an upstream connected input 
pdp_input * pdp_input_find (const pdp_layer * some_layer, int id) {
  pdp_input * input_i = some_layer->upstream_layers;
  while (input_i != NULL) {
    if (input_i->id_of_input_layer == id) {
      return (input_i);
    }
    input_i = input_i->next;
  }
  return input_i;
}


void pdp_input_free (pdp_input * input_to_free) {
  if (input_to_free == NULL) {
    return;
  }
  else {
    // printf ("freeing input list item\n");
    pdp_input * tmp = input_to_free->next;
    pdp_weights_free (input_to_free->input_weights);
    free (input_to_free);    
    pdp_input_free (tmp);
  }
}


int pdp_layer_cycle_inputs (pdp_layer * some_layer) {

  pdp_input * input_iterator; 
  input_iterator = some_layer->upstream_layers;

  /* remember to zero the accumulators, then add bias now */
  int j;
  for (j = 0; j < some_layer->size; j++) {
    // some_layer->net_inputs[j] = 0;
    some_layer->net_inputs[j] = some_layer->input_bias;
  }

  /* update the inputs */
  while (input_iterator != NULL) {
    
    pdp_calc_input_fromlayer (some_layer->size, some_layer, 
			      input_iterator->input_layer->size, input_iterator->input_layer, 
			      input_iterator->input_weights);
	
    input_iterator = input_iterator->next;
  }

  /* <--------------- DEBUG ------------------- */
  /*
  printf ("net inputs:\n");
  for (j = 0; j < some_layer->size; j++) {
    printf ("[%d]:, %4.2f  ", j, some_layer->net_inputs[j]);
  }
  printf ("\n");
  */

  return 0;
}


int pdp_layer_cycle_activation (pdp_layer * some_layer, 
				act_func_params * activation_parameters) {

  /* TODO - activation function should be pointer to a function */
  /* calculates a new iteration of the layer based on its upstream inputs */
      /* create new units element */
  
  pdp_units * units_new = pdp_units_create (some_layer->size);
  int j; // iterates across output units

  units_new->next = NULL;
  units_new->previous = some_layer->units_latest;      
  units_new->cycle = (units_new->previous->cycle) + 1;  

  /* make new linkages */
  some_layer->units_latest->next = units_new;
  some_layer->units_latest = units_new;

  switch (activation_parameters->type) {
  case ACT_GS : {

    /* now update activation */
    for (j = 0; j < some_layer->size; j ++) {
      some_layer->units_latest->activations[j] = 
      act_gs(some_layer->net_inputs[j],
	     some_layer->units_latest->previous->activations[j],
	     activation_parameters->params.gs.step_size, 
	     activation_parameters->params.gs.act_max,
	     activation_parameters->params.gs.act_min);  
    }
    break;
  }
    case DUMMY : {
      // other activation functions can be slotted in like so
      printf ("\nerror! DUMMY is proof of concept,");
      printf ("not a real activation function\n");
      break;
    }      
  }

  return 0;
}


void pdp_layer_add_noise_to_units (pdp_layer * some_layer, double noise_sd, const gsl_rng *r) {
  
  int i, sz = some_layer->size;
  
  for (i = 0; i < sz; i ++) {
    some_layer->units_latest->activations[i] += gsl_ran_gaussian (r, noise_sd);


    // remember to clip values to between -1.0 and +1.0
    if (some_layer->units_latest->activations[i] > 1.0) 
      some_layer->units_latest->activations[i] = 1.0;
    else if (some_layer->units_latest->activations[i] < -1.0) 
      some_layer->units_latest->activations[i] = -1.0;
  
  }

  return;
}




pdp_model * pdp_model_create (int id, char *model_name) {
  pdp_model * this_model = malloc (sizeof(pdp_model));
  this_model->name = malloc (MODEL_NAME_MAX_LENGTH * sizeof (char));

  this_model->next = NULL;
  this_model->prev = NULL;
  this_model->activation_parameters = NULL;

  this_model->id = id;
  strncpy (this_model->name, model_name, MODEL_NAME_MAX_LENGTH);
  
  this_model->components = NULL;
  this_model->cycle = 0;
  return (this_model);
  
}

// creates a new pdp model and inserts it into a list of models
pdp_model * pdp_model_insert_new (pdp_model * insert_after, int id, char *model_name) {
  pdp_model * this_model = malloc (sizeof(pdp_model));
  this_model->name = malloc (sizeof(*model_name));

  this_model->next = insert_after->next;
  this_model->prev = insert_after;

  this_model->id = id;
  strcpy (this_model->name, model_name);
  
  this_model->components = NULL;
  this_model->cycle = 0;
  return (this_model);
  
}


void pdp_model_free (pdp_model * some_model) {
  /* TODO - free all other members */

  /* free the components */
  // printf ("freeing components\n");
  pdp_model_component_free (some_model->components);
  some_model->components = NULL; // poss optional?

  // Remember to fix up the links!

  if (some_model->prev != NULL) {
    // printf ("reassigning prev links\n");
    some_model->prev->next = some_model->next;
  }
  if (some_model->next != NULL) {
    // printf ("reassigning next links\n");
    some_model->next->prev = some_model->prev;
  }



  // now free the model itself
  // printf ("freeing model name\n");
  free (some_model->name);  
  // printf ("freeing model\n");
  free (some_model);
  // printf ("model freed, returning...\n");
  return;
}


void pdp_model_free_list (pdp_model * some_model) {

  // two iterators
  pdp_model *this, *prev, *next;
  
  prev = some_model->prev;

  while (prev != NULL) {
    this = prev;
    prev = prev->prev;
    pdp_model_free (this);
  }

  next = some_model->next;

  while (next != NULL) {
    this = next;
    next = next->next;
    pdp_model_free (next);
  }

  some_model->next = NULL;
  some_model->prev = NULL;
  pdp_model_free (some_model);

}

// void pdp_model_set_data (pdp_model * some_model, void * some_data) {
//   // deprecated???
//   some_model->model_data = some_data;
//   return;
// }


pdp_model_component * pdp_model_component_create () {
  pdp_model_component * this_component;
  this_component = malloc (sizeof(pdp_model_component));
  this_component->layer = NULL;
  this_component->next = NULL;
  return (this_component);
}


void pdp_model_component_free (pdp_model_component * some_component) {
  if (some_component == NULL) {
    // printf ("no more components to free, returning...\n");
    return;
  }
  else {
    pdp_model_component * next;
    pdp_layer_free (some_component->layer);
    next = some_component->next;
    // printf ("freeing component id %d at %p\n", some_component->id, some_component);
    free (some_component);
    pdp_model_component_free (next);  
    return;
  }
}


void pdp_model_component_push (pdp_model * some_model, 
			       pdp_layer * layer_add_as_component, 
			       int id, 
			       bool update_activation) {
  /* check id does not already exist in model */
  if (pdp_model_component_find(some_model, id) != NULL) {
    // printf ("\nError! adding component id: %d to model, id already exists in model\n", id);
    return;
  }
  else {
    pdp_model_component * new_component;
      /* add data members */
    new_component = pdp_model_component_create();
    new_component->layer = layer_add_as_component;
    new_component->id = id;
    new_component->update_activation = update_activation;
    // printf ("adding component id %d: location %p\n", id, new_component);
    
    /* linkages */
    new_component->next = some_model->components;
    some_model->components = new_component;
    // printf ("some_model->components now points to %p\n", some_model->components); 
    return;
  }
}



pdp_model_component * pdp_model_component_find(const pdp_model * some_model, int id) {
  pdp_model_component * component_i; // iterator
  component_i = some_model->components;
  while (component_i != NULL) {
    if (component_i->id == id) {
      return (component_i);
    }
    component_i = component_i->next;
  }
  return component_i;
}



void pdp_model_cycle (pdp_model * some_model) {
  pdp_model_component * component_i;

  /* first calc the summed inputs for all components*/
  component_i = some_model->components;
  while (component_i != NULL) {
    pdp_layer_cycle_inputs (component_i->layer);
    component_i = component_i->next;
  }

  /* now update activations */
  component_i = some_model->components;
  while (component_i != NULL) {

    if (component_i->update_activation == true) {
      pdp_layer_cycle_activation (component_i->layer, 
				  some_model->activation_parameters);
    }

    component_i = component_i->next;
  }

  some_model->cycle ++;
  return;
}


/* TEST CODE */
 
/*   
int main () {

    pdp_layer * an_input;
    pdp_layer * an_output;

    an_input = pdp_layer_create(5);
    an_output = pdp_layer_create(3);

    double initial_input_activations[5] = {0.1, 1.0, 0.1, 0.5, 0.1};
    double initial_output_activations[3] = {0.1, 0.9, 0.1};

    pdp_layer_set_activation(an_input, 5, initial_input_activations);
    pdp_layer_set_activation(an_output, 3, initial_output_activations);

    // init some weights 
    pdp_weights_matrix * some_weights;
    some_weights = pdp_weights_create (3,5);

    double local_weights_matrix[3][5] = {
      {0.0,  1.0, -4.0,  3.1,  2.5},
      {5.3, -5.3,  0.4, -5.2,  1.4},
      {3.0, -7.3, -3.5,  1.2,  7.1},
    };


    pdp_weights_set (some_weights, 3, 5, local_weights_matrix);
    pdp_input_connect (an_output, an_input, some_weights);

    pdp_model * test_model;
    test_model = pdp_model_create();
    pdp_model_component_push(test_model, an_input, 1);
    pdp_model_component_push(test_model, an_output, 2);

    // cycle the model 10 times 

    int i;
    for (i = 0; i < 50; i++) {
      // pdp_layer_cycle_inputs (an_output);
      // pdp_layer_cycle_activation (an_output, gs_activation);
      pdp_model_cycle (test_model);

      // print output activation
      pdp_layer_print_current_output (pdp_model_component_find(test_model, 2)->layer); 
    }

    pdp_weights_print (some_weights);
    pdp_model_free (test_model);

    return 0;
}
*/
