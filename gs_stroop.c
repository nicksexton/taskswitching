/* Initialises architecture & parameters for Gilbert & Shallice Stroop
 model. Should do classic stroop effects (ie. response inhibition) and
 task switching */


#include "pdp_objects.h"
#include "gs_stroop.h"
#include "random_generator_functions.h" // for gaussian noise
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



/* Global parameters */
#define ACTIVATION_MAX 1.0
#define ACTIVATION_MIN -1.0
#define RESPONSE_THRESHOLD 0.15
#define STEP_SIZE 0.0015
#define SQUASHING_PARAM 0.8

#define NOISE 0.006
#define OUTPUTUNIT_BIAS -6.0
#define TASKDEMAND_BIAS -4.0
#define BIAS_NONE 0
#define TOPDOWN_CONTROL_STRENGTH_WORD 6.0
#define TOPDOWN_CONTROL_STRENGTH_COLOUR 15.0
#define LEARNING_RATE 1.0

#define ID_WORDIN 1
#define ID_COLOURIN 2
#define ID_WORDOUT 3
#define ID_COLOUROUT 4
#define ID_TASKDEMAND 5
#define ID_TOPDOWNCONTROL 6


#define ECHO // echo console output






stroop_response * make_stroop_response (int node, double activation) {
  // NB code for nodes: 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  stroop_response * response = malloc (sizeof(stroop_response));
  response->this_node = node;
  response->activation = activation;
  return response;
}


bool stopping_condition (pdp_model * gs_stroop) {

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
      if (biggest_act[0]->activation - RESPONSE_THRESHOLD > biggest_act[2]->activation) {
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
      if (biggest_act[0]->activation - RESPONSE_THRESHOLD > biggest_act[1]->activation) {
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

  




int model_init (pdp_model * gs_stroop_model) {

  pdp_layer *word_input, *word_output, *colour_input, *colour_output, *taskdemand, *topdown_control;

  word_input = pdp_layer_create(3, BIAS_NONE);
  word_output = pdp_layer_create(3, OUTPUTUNIT_BIAS);
  colour_input = pdp_layer_create(3, BIAS_NONE);
  colour_output = pdp_layer_create(3, OUTPUTUNIT_BIAS);
  taskdemand = pdp_layer_create(2, TASKDEMAND_BIAS);
  topdown_control = pdp_layer_create(2, BIAS_NONE);

  
  double initial_activation_wordin[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourin[3] = {0.0, 0.0, 0.0};
  double initial_activation_wordout[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourout[3] = {0.0, 0.0, 0.0};
  double initial_activation_taskdemand[2] = {0.0, 0.0};
  double initial_activation_topdown_control[2] = {0.0, 0.0};
  


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

  double wts_wordin_wordout_matrix[3][3] = {
    {3.5, 0.0, 0.0},
    {0.0, 3.5, 0.0},
    {0.0, 0.0, 3.5}, 
  };

  double wts_colourin_colourout_matrix[3][3] = {
    {1.9, 0.0, 0.0},
    {0.0, 1.9, 0.0},
    {0.0, 0.0, 1.9}, 
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
    {1.0, 1.0, 1.0},
    {0.0, 0.0, 0.0},
  };

  double wts_colourout_taskdemand_matrix[2][3] = {
    {0.0, 0.0, 0.0},
    {1.0, 1.0, 1.0},
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


  /************************************** */
  /* Top down control -> taskdemand units */
  /************************************** */

  pdp_weights_matrix *wts_topdown_taskdemand;
  double wts_topdown_taskdemand_matrix[2][2] = {
    { TOPDOWN_CONTROL_STRENGTH_WORD,  0.0},
    { 0.0, TOPDOWN_CONTROL_STRENGTH_COLOUR},
  };

  wts_topdown_taskdemand = pdp_weights_create (2,2);
  pdp_weights_set (wts_topdown_taskdemand, 2, 2, wts_topdown_taskdemand_matrix);
  pdp_input_connect (taskdemand, topdown_control, wts_topdown_taskdemand);



  /* TODO - remember feed forward (hebbian learning) connections from inputs to task demand */


  /* Now init model object and push components */


  pdp_model_component_push(gs_stroop_model, word_input, ID_WORDIN); 
  pdp_model_component_push(gs_stroop_model, colour_input, ID_COLOURIN);
  pdp_model_component_push(gs_stroop_model, word_output, ID_WORDOUT);
  pdp_model_component_push(gs_stroop_model, colour_output, ID_COLOUROUT);
  pdp_model_component_push(gs_stroop_model, taskdemand, ID_TASKDEMAND);
  pdp_model_component_push(gs_stroop_model, topdown_control, ID_TOPDOWNCONTROL);

  return 0;
}

int main () {

  gsl_rng * random_generator = random_generator_create();




  pdp_model * gs_stroop_model = pdp_model_create();

  // Specify activation function
  act_func_params * activation_parameters = malloc (sizeof(act_func_params));
  activation_parameters->params.gs.step_size = STEP_SIZE;
  activation_parameters->params.gs.act_max = ACTIVATION_MAX;
  activation_parameters->params.gs.act_min = ACTIVATION_MIN;

  gs_stroop_model->activation_parameters = activation_parameters;

  // set up model
  model_init (gs_stroop_model);

  // test trial
  double word_input_initial_act[3]   = { 1.0,  0.0,  0.0 };
  double colour_input_initial_act[3] = { 0.0,  1.0,  0.0 };
  double topdown_control_initial_act[2]   = { 0.0,  1.0 };



  while ((stopping_condition(gs_stroop_model) != true && gs_stroop_model->cycle < 1500))  {

    /* TODO - introduce flags in pdp_layer for whether you want
       activation free (to update) or clamped(ie. does not update) */

    pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer, 
			      3, word_input_initial_act);
    pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer, 
			      3, colour_input_initial_act);
    pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_TOPDOWNCONTROL)->layer, 
			      2, topdown_control_initial_act);

    /* recalculate activation */

    pdp_model_cycle (gs_stroop_model);


    /* add noise to units */
    


#if defined ECHO

    printf ("\ncyc:%d\t", gs_stroop_model->cycle);
    pdp_layer_print_current_output (
		    pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer);
    pdp_layer_print_current_output (
		    pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer);

    
#endif

    // TODO:
    // 0.5) sort out activation function so that it takes parameters
    //      which can be specified inside this file (ie as an act_params
    //      union?) and passed in to the relevant pdp_objects function (pdp_model_cycle)
    // 1) access function which dumps unit activation output to screen or a plottable format
    // 2) pango functions which draw a nice graph
    // 3) implement stopping condition
    // 4) noise 

    // pdp_layer_print_current_output (pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer);
    // pdp_layer_print_current_output (pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer);
 
    
  }

  printf ("\n");

  free (gs_stroop_model->activation_parameters);
  pdp_model_free (gs_stroop_model);
  random_generator_free (random_generator);  
  
  return 0;
}
