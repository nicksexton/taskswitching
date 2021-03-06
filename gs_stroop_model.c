/* Initialises architecture & parameters for Gilbert & Shallice Stroop
 model. Should do classic stroop effects (ie. response inhibition) and
 task switching */



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>
#include "pdp_objects.h"
#include "random_generator_functions.h" // for gaussian noise
// #include "simulated_subjects.h" // objects for representing subject params and data
#include "gs_stroop_subjects.h" // specialises simulated_subjects to
                                //stroop data/stimuli/params
#include "gs_stroop_analyse.h"

#include "gs_stroop.h"
#include "gs_stroop_global_params.h"

// #define ECHO
// #define ECHO_WEIGHTS


// set default parameters in GsStroopParameters object (defined in gs_stroop.h)
// used in the gui version of the model (pdpgui_test_1.c)
// not currently used in the console version

void gs_stroop_parameters_set_default (GsStroopParameters * params_object) {

  params_object->activation_max = ACTIVATION_MAX;
  params_object->activation_min = ACTIVATION_MIN;
  params_object->response_threshold = RESPONSE_THRESHOLD;
  params_object->step_size = STEP_SIZE;
  params_object->squashing_param = SQUASHING_PARAM;
  params_object->noise = NOISE;
  params_object->bias_outputunit = OUTPUTUNIT_BIAS;
  params_object->bias_taskdemand = TASKDEMAND_BIAS;
  params_object->bias_none = BIAS_NONE;
  params_object->stimulus_input_strength_word = STIMULUS_INPUT_STRENGTH_WORD;
  params_object->stimulus_input_strength_colour = STIMULUS_INPUT_STRENGTH_COLOUR;
  params_object->taskdemand_output_inhibitory_wt = TASKDEMAND_OUTPUT_INHIBITORY_WT;
  params_object->taskdemand_output_excitatory_wt = TASKDEMAND_OUTPUT_EXCITATORY_WT;
  params_object->topdown_control_strength_word = TOPDOWN_CONTROL_STRENGTH_WORD;
  params_object->topdown_control_strength_colour = TOPDOWN_CONTROL_STRENGTH_COLOUR;
  params_object->learning_rate = LEARNING_RATE;
  params_object->max_cycles = MAX_CYCLES;


  return;
}

void gs_stroop_parameters_htable_set_default (GHashTable * params_table) {

  double * activation_max = g_malloc(sizeof(double));
  *activation_max = ACTIVATION_MAX;
  g_hash_table_insert (params_table, "activation_max", activation_max);

  double * activation_min = g_malloc(sizeof(double));
  *activation_min = ACTIVATION_MIN;
  g_hash_table_insert (params_table, "activation_min", activation_min);

  double * response_threshold = g_malloc(sizeof(double));
  *response_threshold = RESPONSE_THRESHOLD;
  g_hash_table_insert (params_table, "response_threshold", response_threshold);

  double * step_size = g_malloc(sizeof(double));
  *step_size = STEP_SIZE;
  g_hash_table_insert (params_table, "step_size", step_size);

  double * squashing_param = g_malloc(sizeof(double));
  *squashing_param = SQUASHING_PARAM;
  g_hash_table_insert (params_table, "squashing_param", squashing_param);

  double * noise = g_malloc(sizeof(double));
  *noise = NOISE;
  g_hash_table_insert (params_table, "noise", noise);

  double * bias_outputunit = g_malloc(sizeof(double));
  *bias_outputunit = OUTPUTUNIT_BIAS;
  g_hash_table_insert (params_table, "bias_outputunit", bias_outputunit);

  double * bias_taskdemand = g_malloc(sizeof(double));
  *bias_taskdemand = TASKDEMAND_BIAS;
  g_hash_table_insert (params_table, "bias_taskdemand", bias_taskdemand);

  double * bias_none = g_malloc(sizeof(double));
  *bias_none = BIAS_NONE;
  g_hash_table_insert (params_table, "bias_none", bias_none);

  double *stimulus_input_strength_word  = g_malloc(sizeof(double));
  *stimulus_input_strength_word = STIMULUS_INPUT_STRENGTH_WORD;
  g_hash_table_insert (params_table, "stimulus_input_strength_word", stimulus_input_strength_word);

  double *stimulus_input_strength_colour  = g_malloc(sizeof(double));
  *stimulus_input_strength_colour = STIMULUS_INPUT_STRENGTH_COLOUR;
  g_hash_table_insert (params_table, "stimulus_input_strength_colour", stimulus_input_strength_colour);

  double *taskdemand_output_inhibitory_wt  = g_malloc(sizeof(double));
  *taskdemand_output_inhibitory_wt = TASKDEMAND_OUTPUT_INHIBITORY_WT;
  g_hash_table_insert (params_table, "taskdemand_output_inhibitory_wt", taskdemand_output_inhibitory_wt);

  double *taskdemand_output_excitatory_wt  = g_malloc(sizeof(double));
  *taskdemand_output_excitatory_wt = TASKDEMAND_OUTPUT_EXCITATORY_WT;
  g_hash_table_insert (params_table, "taskdemand_output_excitatory_wt", taskdemand_output_excitatory_wt);

  double *topdown_control_strength_word  = g_malloc(sizeof(double));
  *topdown_control_strength_word = TOPDOWN_CONTROL_STRENGTH_WORD;
  g_hash_table_insert (params_table, "topdown_control_strength_word", topdown_control_strength_word);

  double *topdown_control_strength_colour  = g_malloc(sizeof(double));
  *topdown_control_strength_colour = TOPDOWN_CONTROL_STRENGTH_COLOUR;
  g_hash_table_insert (params_table, "topdown_control_strength_colour", topdown_control_strength_colour);

  double *learning_rate  = g_malloc(sizeof(double));
  *learning_rate = LEARNING_RATE;
  g_hash_table_insert (params_table, "learning_rate", learning_rate);

  int *max_cycles  = g_malloc(sizeof(int));
  *max_cycles = MAX_CYCLES;
  g_hash_table_insert (params_table, "max_cycles", max_cycles);


  return;
}


void add_noise_to_units (pdp_layer * some_layer, double noise_sd, const gsl_rng *r) {
  
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

  

stroop_response * make_stroop_response (int node, double activation) {
  // NB code for nodes: 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  stroop_response * response = malloc (sizeof(stroop_response));
  response->this_node = node;
  response->activation = activation;
  return response;
}


bool stopping_condition (const pdp_model * gs_stroop, stroop_trial_data * this_trial, double response_threshold) {

  /* evaluates whether model should stop on this cycle and returns
     true/false. CURRENT CRITERION: most active output node is > next
     most active output node that is not the same coloured node */

  // NB code for responses: 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  // ie. if abs(response[0] - response[1]) == 3, then the nodes correspond

  int outputnode;
  stroop_response * biggest_act[3] = {NULL, NULL, NULL}; // stores 3 biggest activations, to compare
                         // [0] vs. [1] or [2] if [0] and [1] are same colour

  int i; // does an insertion sort
  int o; // iterates output layers (0 or 1)
  pdp_layer * output_layers[2];

  output_layers[0] = (pdp_model_component_find (gs_stroop, ID_WORDOUT)->layer);  
  output_layers[1] = (pdp_model_component_find (gs_stroop, ID_COLOUROUT)->layer);  

  /* do both output layers */
  for (o = 0; o < 2; o ++) {

    /* outer loop iterates all the output nodes */
    for (outputnode = 0; outputnode < 3; outputnode ++) {
    
      /* inner loop does an insertion sort */
      for (i = 0; i < 3; i ++) {
      
	/*  if slot is empty, insert the new response right here */
	if (biggest_act[i] == NULL) {
	  biggest_act[i] = make_stroop_response (
		                 (3*o + outputnode), 
		                 output_layers[o]->units_latest->activations[outputnode]);
	  break;
	}
	
	/* else, compare size of activations */
	else {
	  if (biggest_act[i]->activation < output_layers[o]->units_latest->activations[outputnode]) {
	    /* insert new response here, move everything down */
	    if (i == 2) {
	      free (biggest_act[2]);
	      biggest_act[2] = make_stroop_response (
                                     (3*o + outputnode), 
				     output_layers[o]->units_latest->activations[outputnode]);
	      break;
	    }
	    else {
	      if (i == 1) {
		free (biggest_act[2]);
		biggest_act[2] = biggest_act[1];
		biggest_act[1] = make_stroop_response (
				       (3*o + outputnode), 
				       output_layers[o]->units_latest->activations[outputnode]);
		break;
	      }
	      else {
		// i == 0, presumably
		free (biggest_act[2]);
		biggest_act[2] = biggest_act[1];
		biggest_act[1] = biggest_act[0];
		biggest_act[0] = make_stroop_response (
				       (3*o + outputnode), 
				       output_layers[o]->units_latest->activations[outputnode]);
		break;
	      }
	    }
	  
	    
	  }
	  /* when i = 2, we're done */
	}


      } // <-- inner loop
    } // <-- outer loop
  } // <- output layers

  // now, we can evaluate stopping condition

  switch (abs(biggest_act[0]->this_node - biggest_act[1]->this_node)) {
    case 3: { 
      // contingency where [0] and [1] correspond
      if (biggest_act[0]->activation - response_threshold > biggest_act[2]->activation) {

	// RECORD RESPONSE
	this_trial->response = biggest_act[0]->this_node;
	

	for (i = 0; i < 3; i ++) { free (biggest_act[i]); }
	return true; // [0] and [1] correspond, but ([0] - 15) > [2]
      }
      else {
	for (i = 0; i < 3; i ++) { free (biggest_act[i]); }
	return false; // [0] and [1] correspond, and ([0] - 15) > [2]
      }
    }

    default: { 
     // contingency where [0] and [2] do not correspond
      if (biggest_act[0]->activation - response_threshold > biggest_act[1]->activation) {

	// RECORD RESPONSE
	this_trial->response = biggest_act[0]->this_node;
	for (i = 0; i < 3; i ++) { free (biggest_act[i]); }
	return true;
      }
      else {
	for (i = 0; i < 3; i ++) { free (biggest_act[i]); }
	return false;
      }
    }
  }
}


// fully creates the Gilbert & Shallice model, weights, connections etc.
// sets initial activations to 0 (init's model)

int gs_stroop_model_build (pdp_model * gs_stroop_model, GHashTable * model_params) {

  pdp_layer *word_input, *word_output, *colour_input, *colour_output, *taskdemand, *topdown_control;

  word_input = pdp_layer_create(ID_WORDIN, 3, 
				*(double *)g_hash_table_lookup(model_params, "bias_none"));
  word_output = pdp_layer_create(ID_WORDOUT, 3, 
				 *(double *)g_hash_table_lookup(model_params, "bias_outputunit"));
  colour_input = pdp_layer_create(ID_COLOURIN, 3, 
				  *(double *)g_hash_table_lookup(model_params, "bias_none"));
  colour_output = pdp_layer_create(ID_COLOUROUT, 3, 
				   *(double *)g_hash_table_lookup(model_params, "bias_outputunit")); 
  taskdemand = pdp_layer_create(ID_TASKDEMAND, 2, 
				*(double *)g_hash_table_lookup(model_params, "bias_taskdemand"));
  topdown_control = pdp_layer_create(ID_TOPDOWNCONTROL, 2, 
				     *(double *)g_hash_table_lookup(model_params, "bias_none"));

  
  double initial_activation_wordin[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourin[3] = {0.0, 0.0, 0.0};
  double initial_activation_wordout[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourout[3] = {0.0, 0.0, 0.0};
  double initial_activation_taskdemand[2] = {0.0, 0.0};
  double initial_activation_topdown_control[2] = {0.0, 0.0};

  double topdown_control_strength_word = *(double *)g_hash_table_lookup(model_params, 
									"topdown_control_strength_word");
  double topdown_control_strength_colour = *(double *)g_hash_table_lookup(model_params, 
									  "topdown_control_strength_colour");  


  /* set initial activation */
  pdp_layer_set_activation(word_input, 3, initial_activation_wordin);
  pdp_layer_set_activation(word_output, 3, initial_activation_wordout);
  pdp_layer_set_activation(colour_input, 3, initial_activation_colourin);
  pdp_layer_set_activation(colour_output, 3, initial_activation_colourout);
  pdp_layer_set_activation(taskdemand, 2, initial_activation_taskdemand);
  pdp_layer_set_activation(topdown_control, 2, initial_activation_topdown_control);


  /* <------------------------------ NETWORK CONNECTIVITY --------------------------------> */
  /****************************** */
  /* Input units -> output units */
  /***************************** */
  pdp_weights_matrix *wts_wordin_wordout, *wts_colourin_colourout;

  double stimulus_input_strength_word = *(double *)g_hash_table_lookup(model_params, 
								       "stimulus_input_strength_word");

  double wts_wordin_wordout_matrix[3][3] = {
    {stimulus_input_strength_word, 0.0, 0.0},
    {0.0, stimulus_input_strength_word, 0.0},
    {0.0, 0.0, stimulus_input_strength_word}, 
  };

  
  double stimulus_input_strength_colour = *(double *)g_hash_table_lookup(model_params, 
									 "stimulus_input_strength_colour");
  double wts_colourin_colourout_matrix[3][3] = {
    {stimulus_input_strength_colour, 0.0, 0.0},
    {0.0, stimulus_input_strength_colour, 0.0},
    {0.0, 0.0, stimulus_input_strength_colour}, 
  };

  wts_wordin_wordout = pdp_weights_create (3,3);
  pdp_weights_set (wts_wordin_wordout, 3, 3, wts_wordin_wordout_matrix);
  pdp_input_connect (word_output, word_input, wts_wordin_wordout);

  wts_colourin_colourout = pdp_weights_create (3,3);
  pdp_weights_set (wts_colourin_colourout, 3, 3, wts_colourin_colourout_matrix);
  pdp_input_connect (colour_output, colour_input, wts_colourin_colourout);

  /******************************** */
  /* Output unit lateral inhibition */
  /******************************** */

  pdp_weights_matrix *wts_wordout_wordout, *wts_colourout_colourout;

  double wts_alloutputs_lateral_matrix[3][3] = {
    { 0.0, -2.0, -2.0},
    {-2.0,  0.0, -2.0},
    {-2.0, -2.0,  0.0}, 
  };

  wts_wordout_wordout = pdp_weights_create (3,3);
  pdp_weights_set (wts_wordout_wordout, 3, 3, wts_alloutputs_lateral_matrix);
  pdp_input_connect (word_output, word_output, wts_wordout_wordout);

  wts_colourout_colourout = pdp_weights_create (3,3);
  pdp_weights_set (wts_colourout_colourout, 3, 3, wts_alloutputs_lateral_matrix);
  pdp_input_connect (colour_output, colour_output, wts_colourout_colourout);


  /*****************************************/
  /* Output units cross-module connections */
  /*****************************************/
  pdp_weights_matrix *wts_wordout_colourout, *wts_colourout_wordout;

  double wts_crossoutputs_matrix[3][3] = {
    { 2.0, -2.0, -2.0},
    {-2.0,  2.0, -2.0},
    {-2.0, -2.0,  2.0}, 
  };

  /* wordout -> colourout */
  wts_wordout_colourout = pdp_weights_create (3,3);
  pdp_weights_set (wts_wordout_colourout, 3, 3, wts_crossoutputs_matrix);
  pdp_input_connect (colour_output, word_output, wts_wordout_colourout);


  /* colourout -> wordout */
  wts_colourout_wordout = pdp_weights_create (3,3);
  pdp_weights_set (wts_colourout_wordout, 3, 3, wts_crossoutputs_matrix);
  pdp_input_connect (word_output, colour_output, wts_colourout_wordout);


  /***************************** */
  /* Output units -> task demand */
  /***************************** */
  pdp_weights_matrix *wts_wordout_taskdemand, *wts_colourout_taskdemand;

  double wts_wordout_taskdemand_matrix[2][3] = {
    { 1.0,  1.0,  1.0},
    {-1.0, -1.0, -1.0},
  };
  // see G&S - should be 1.0 and 0.0 or 1.0 and -1.0?!
  double wts_colourout_taskdemand_matrix[2][3] = {
    {-1.0, -1.0, -1.0},
    { 1.0,  1.0,  1.0},
  };

  wts_wordout_taskdemand = pdp_weights_create (2,3);
  pdp_weights_set (wts_wordout_taskdemand, 2, 3, wts_wordout_taskdemand_matrix);
  pdp_input_connect (taskdemand, word_output, wts_wordout_taskdemand);

  wts_colourout_taskdemand = pdp_weights_create (2,3);
  pdp_weights_set (wts_colourout_taskdemand, 2, 3, wts_colourout_taskdemand_matrix);
  pdp_input_connect (taskdemand, colour_output, wts_colourout_taskdemand);


  /**************************** */
  /* Taskdemand -> output units */
  /**************************** */

  pdp_weights_matrix *wts_taskdemand_wordout, *wts_taskdemand_colourout;

  double wts_taskdemand_wordout_matrix[3][2] = {
    { 2.5, -2.5},
    { 2.5, -2.5},
    { 2.5, -2.5},
  };

  double wts_taskdemand_colourout_matrix[3][2] = {
    {-2.5,  2.5},
    {-2.5,  2.5},
    {-2.5,  2.5},
  };

  wts_taskdemand_wordout = pdp_weights_create (3,2);
  pdp_weights_set (wts_taskdemand_wordout, 3, 2, wts_taskdemand_wordout_matrix);
  pdp_input_connect (word_output, taskdemand, wts_taskdemand_wordout);

  wts_taskdemand_colourout = pdp_weights_create (3,2);
  pdp_weights_set (wts_taskdemand_colourout, 3, 2, wts_taskdemand_colourout_matrix);
  pdp_input_connect (colour_output, taskdemand, wts_taskdemand_colourout);

  /******************************** */
  /* Taskdemand -> taskdemand units */
  /******************************** */
  pdp_weights_matrix *wts_taskdemand_taskdemand;
  double wts_taskdemand_taskdemand_matrix[2][2] = {
    { 0.0, -2.0},
    {-2.0,  0.0},
  };

  wts_taskdemand_taskdemand = pdp_weights_create (2,2);
  pdp_weights_set (wts_taskdemand_taskdemand, 2, 2, wts_taskdemand_taskdemand_matrix);
  pdp_input_connect (taskdemand, taskdemand, wts_taskdemand_taskdemand);


  /*  +--------------------------------------+  */
  /*  | Top down control -> taskdemand units |  */
  /*  +--------------------------------------+  */



  pdp_weights_matrix *wts_topdown_taskdemand;
  double wts_topdown_taskdemand_matrix[2][2] = {
    { topdown_control_strength_word,  0.0 },
    { 0.0, topdown_control_strength_colour },
  };

  wts_topdown_taskdemand = pdp_weights_create (2,2);
  pdp_weights_set (wts_topdown_taskdemand, 2, 2, wts_topdown_taskdemand_matrix);
  pdp_input_connect (taskdemand, topdown_control, wts_topdown_taskdemand);



  /*  TODO - remember feed forward (hebbian learning) connections from inputs to task demand */
  //  +----------------------------------------+//
  //  | Associative learning FF weights        |
  //  +----------------------------------------+

  pdp_weights_matrix *wts_wordin_taskdemand, *wts_colourin_taskdemand;

  double wts_inputgeneric_taskdemand_matrix[2][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
  };
  // ok to use generic initialiser as pdp_weights_create creates copy of array anyway

  
  wts_wordin_taskdemand = pdp_weights_create (2,3);
  wts_colourin_taskdemand = pdp_weights_create (2,3);
  pdp_weights_set (wts_wordin_taskdemand, 2, 3, wts_inputgeneric_taskdemand_matrix);
  pdp_weights_set (wts_colourin_taskdemand, 2, 3, wts_inputgeneric_taskdemand_matrix);
  pdp_input_connect (taskdemand, word_input, wts_wordin_taskdemand);
  pdp_input_connect (taskdemand, colour_input, wts_colourin_taskdemand);


  /* Now init model object and push components */

  pdp_model_component_push(gs_stroop_model, word_input, ID_WORDIN, FALSE); 
  pdp_model_component_push(gs_stroop_model, colour_input, ID_COLOURIN, FALSE);
  pdp_model_component_push(gs_stroop_model, word_output, ID_WORDOUT, TRUE);
  pdp_model_component_push(gs_stroop_model, colour_output, ID_COLOUROUT, TRUE);
  pdp_model_component_push(gs_stroop_model, taskdemand, ID_TASKDEMAND, TRUE);
  pdp_model_component_push(gs_stroop_model, topdown_control, ID_TOPDOWNCONTROL, FALSE);

  //debug
  // printf ("gs_stroop model created!\n");

  return 0;
}


// OLD (deprecated!) parameter setting!!!
// sets model parameters (eg., weights) specific to gs_stroop_params defined in 
// gs_stroop_subjects.c
int model_init_params (pdp_model * gs_stroop_model, gs_stroop_params * my_params) {

  pdp_layer *word_output, *colour_output;

  word_output = pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer;
  colour_output = pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer;

  double wts_taskdemand_wordout_matrix[3][2] = {
    { my_params->taskdemand_weights_excitatory, my_params->taskdemand_weights_inhibitory},
    { my_params->taskdemand_weights_excitatory, my_params->taskdemand_weights_inhibitory},
    { my_params->taskdemand_weights_excitatory, my_params->taskdemand_weights_inhibitory},
  };

  double wts_taskdemand_colourout_matrix[3][2] = {
    { my_params->taskdemand_weights_inhibitory,  my_params->taskdemand_weights_excitatory },
    { my_params->taskdemand_weights_inhibitory,  my_params->taskdemand_weights_excitatory },
    { my_params->taskdemand_weights_inhibitory,  my_params->taskdemand_weights_excitatory },
  };


  pdp_weights_set (pdp_input_find (word_output, ID_TASKDEMAND)->input_weights, 
		   3, 2, wts_taskdemand_wordout_matrix);
  pdp_weights_set (pdp_input_find (colour_output, ID_TASKDEMAND)->input_weights, 
		   3, 2, wts_taskdemand_colourout_matrix);

  return 0;
  
}


// Zeros activation levels of all nodes
// also resets & frees activation history of all units in all layers
// DOES NOT RESET WEIGHTS!!
// persist_taskdemand_activation sets proportion of TD activation to carry over to
// next trial ie. .20 = 20% of activation on previous
int model_init_activation (pdp_model * gs_stroop_model, double persist_taskdemand_activation) {

  pdp_layer *word_input, *word_output, *colour_input, *colour_output, *taskdemand, *topdown_control;
  int i;

  // zero cycle counter
  gs_stroop_model->cycle = 0;


  double initial_activation_wordin[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourin[3] = {0.0, 0.0, 0.0};
  double initial_activation_wordout[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourout[3] = {0.0, 0.0, 0.0};
  double initial_activation_topdown_control[2] = {0.0, 0.0};
  double initial_activation_taskdemand[2];


  word_input = pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer;
  colour_input = pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer;
  word_output = pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer;
  colour_output = pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer;
  taskdemand = pdp_model_component_find (gs_stroop_model, ID_TASKDEMAND)->layer;
  topdown_control = pdp_model_component_find (gs_stroop_model, ID_TOPDOWNCONTROL)->layer;


  // calculate new task demand activations
  // debug
  /*
  printf ("new task demand activation:\t");
  for (i = 0; i < taskdemand->size; i ++) {

    initial_activation_taskdemand[i] = taskdemand->units_latest->activations[i] * 
                                       persist_taskdemand_activation;
    printf ("%4.3f\t", initial_activation_taskdemand[i]);
  } 
  printf ("\n");
  */

  // clear & free history

  pdp_model_component * comp_i;
  for (comp_i = gs_stroop_model->components; comp_i != NULL; comp_i = comp_i->next) {
    pdp_units_free (comp_i->layer->units_initial.next);
    comp_i->layer->units_latest = &(comp_i->layer->units_initial);
    comp_i->layer->units_initial.next = NULL;
  }



  /* set initial activation */
  pdp_layer_set_activation(word_input, 3, initial_activation_wordin);
  pdp_layer_set_activation(word_output, 3, initial_activation_wordout);
  pdp_layer_set_activation(colour_input, 3, initial_activation_colourin);
  pdp_layer_set_activation(colour_output, 3, initial_activation_colourout);
  pdp_layer_set_activation(taskdemand, 2, initial_activation_taskdemand);
  pdp_layer_set_activation(topdown_control, 2, initial_activation_topdown_control);

  return 0;

}


int update_associative_weights (pdp_model * gs_stroop_model, 
				double learning_rate, 
				hebbian_learning_persistence persist) {
  // default hebbian persistence (1, NEXT_TRIAL) - weights persist for next trial only
  // NB running this function immediately after initing model SHOULD zero associative weights
  // (only if persist is set to NEXT_TRIAL)

  int i, j;

  if (persist == OFF) {
    return 0;
  }

  pdp_layer *word_input, *colour_input, *task_demand;
  word_input = pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer;
  colour_input = pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer;
  task_demand = pdp_model_component_find (gs_stroop_model, ID_TASKDEMAND)->layer;
  

  double wts_wordinput_taskdemand_matrix[2][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
  };

  double wts_colourinput_taskdemand_matrix[2][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
  };
  
 
  for (i = 0; i < 2; i ++) { // outer loop, output unit (i)
    for (j = 0; j < 3; j ++) {// inner loop, input unit (j)
      wts_wordinput_taskdemand_matrix[i][j] = 
	task_demand->units_latest->activations[i] *
	word_input->units_latest->activations[j] * learning_rate;


      wts_colourinput_taskdemand_matrix[i][j] = 
	task_demand->units_latest->activations[i] *
	colour_input->units_latest->activations[j] * learning_rate;

    }
  }


  if (persist == BLOCK || persist == FOREVER) {
    // increment existing weights
    pdp_weights_increment (pdp_input_find (task_demand, ID_WORDIN)->input_weights, 
			   2, 3, wts_wordinput_taskdemand_matrix);
    pdp_weights_increment (pdp_input_find (task_demand, ID_COLOURIN)->input_weights, 
			   2, 3, wts_colourinput_taskdemand_matrix);

  }

  else if (persist == NEXT_TRIAL) {
    pdp_weights_set (pdp_input_find (task_demand, ID_WORDIN)->input_weights, 
		     2, 3, wts_wordinput_taskdemand_matrix);
    pdp_weights_set (pdp_input_find (task_demand, ID_COLOURIN)->input_weights, 
		     2, 3, wts_colourinput_taskdemand_matrix);
  }

  return 0;
}



// returns true while model is still running (does not satisfy stopping condition), false otherwise
bool run_model_step (pdp_model * gs_stroop_model, 
		     stroop_trial_data * this_trial, 
		     const gsl_rng * random_generator, 
		     double response_threshold) {


  if (stopping_condition(gs_stroop_model, this_trial, response_threshold) == true || 
      gs_stroop_model->cycle > MAX_CYCLES)  {
    return false;
  }
  else {

    // recalculate activation 
  pdp_model_cycle (gs_stroop_model);


    // add noise to units 
  add_noise_to_units (pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer, 
		      NOISE, random_generator);
  add_noise_to_units (pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer, 
			NOISE, random_generator);
  add_noise_to_units (pdp_model_component_find (gs_stroop_model, ID_TASKDEMAND)->layer, 
			NOISE, random_generator);
    
    

#if defined ECHO

  printf ("\ncyc:%d\t", gs_stroop_model->cycle);
  pdp_layer_print_current_output (
				  pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer);
  pdp_layer_print_current_output (
				  pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer);

#endif


#if defined ECHO_WEIGHTS

  printf ("\ncyc:%d\t", gs_stroop_model->cycle);
  pdp_weights_print (pdp_input_find (pdp_model_component_find (gs_stroop_model, 
							       ID_TASKDEMAND)
				     ->layer, ID_WORDIN)->input_weights); 
  printf ("\t||\t");
  pdp_weights_print (pdp_input_find (pdp_model_component_find (gs_stroop_model, 
							       ID_TASKDEMAND)
				     ->layer, ID_COLOURIN)->input_weights); 

#endif


    return true;
  }


}




/*****************************************************************/
/* Takes a pointer to a stroop_trial_data that has been init'd   */
/* with trial parameters. Fills in the response data and returns */
/*****************************************************************/
int run_stroop_trial (pdp_model * gs_stroop_model,  
		      stroop_trial_data * this_trial,
		      const gsl_rng * random_generator,
		      double response_threshold,
		      hebbian_learning_persistence persist,
		      double learning_rate) {

// init inputs

  double word_input_initial_act[3]   = { 0.0,  0.0,  0.0 };
  double colour_input_initial_act[3] = { 0.0,  0.0,  0.0 };
  double topdown_control_initial_act[2]   = { 0.0,  0.0 };


  // check that subject parameters are sensible

  if (this_trial->stim_word < -1 || this_trial->stim_word > 3) {
    printf ("subject data: word input %d out of range (should be -1 (neutral) or 0 - 2)!",
	    this_trial->stim_word);
    return (0);
  }

  if (this_trial->stim_colour < -1 || this_trial->stim_colour > 3) {
    printf ("subject data: colour input %d out of range (should be -1 (neutral) or 0 - 2)!",
	    this_trial->stim_colour);
    return (0);
  }

  if (this_trial->stim_task < 0 || this_trial->stim_task > 2) {
    printf ("subject data: task input %d out of range (should be 0 or 1)!",
	    this_trial->stim_task);
    return (0);
  }


  // set ON inputs
  if (this_trial->stim_word >= 0) { // check for neutral trial condition where stim is -1
      word_input_initial_act[this_trial->stim_word] = 1.0;
  }

  if (this_trial->stim_colour >= 0) {
      colour_input_initial_act[this_trial->stim_colour] = 1.0;
  }

  topdown_control_initial_act[this_trial->stim_task] = 1.0;

  pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer, 
			    3, word_input_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer, 
			    3, colour_input_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (
                                 gs_stroop_model, ID_TOPDOWNCONTROL)->layer, 
	   		    2, topdown_control_initial_act);



  // <--------------------- RUN TRIAL ---------------------------->

  // run_model_step returns true when stopping_condition evaluates to false
  while (run_model_step (gs_stroop_model, this_trial, random_generator, response_threshold));
  
  update_associative_weights(gs_stroop_model,
			     learning_rate,
			     persist);

  this_trial->response_time = gs_stroop_model->cycle;


  return (1);

}


