/* Initialises architecture & parameters for Gilbert & Shallice Stroop
 model. Should do classic stroop effects (ie. response inhibition) and
 task switching */



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_randist.h>
#include "pdp_objects.h"
#include "random_generator_functions.h" // for gaussian noise
// #include "simulated_subjects.h" // objects for representing subject params and data
#include "gs_stroop_subjects.h" // specialises simulated_subjects to stroop data/stimuli/paramsi

#include "gs_stroop.h"


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
#define MAX_CYCLES 1500 // how long to let model run - NB check G&S defaults


#define ID_WORDIN 1
#define ID_COLOURIN 2
#define ID_WORDOUT 3
#define ID_COLOUROUT 4
#define ID_TASKDEMAND 5
#define ID_TOPDOWNCONTROL 6

#define NUM_TRIALS 100
#define PPN_CONGRUENT 50
#define PPN_INCONGRUENT 50
#define PPN_NEUTRAL 0
#define PPN_WORDREADING 50
#define PPN_COLOURNAMING 50



// #define ECHO // echo console output




void add_noise_to_units (pdp_layer * some_layer, double noise_sd, const gsl_rng *r) {
  
  int i, sz = some_layer->size;
  
  for (i = 0; i < sz; i ++) {
    some_layer->units_latest->activations[i] += gsl_ran_gaussian (r, noise_sd);
  }
}

  


stroop_response * make_stroop_response (int node, double activation) {
  // NB code for nodes: 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  stroop_response * response = malloc (sizeof(stroop_response));
  response->this_node = node;
  response->activation = activation;
  return response;
}


bool stopping_condition (const pdp_model * gs_stroop, stroop_trial_data * this_trial) {

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
	// RECORD RESPONSE
	// ((stroop_trial_data * )(gs_stroop->model_data))->response = biggest_act[0]->this_node;
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
      if (biggest_act[0]->activation - RESPONSE_THRESHOLD > biggest_act[1]->activation) {
	// RECORD RESPONSE
	// ((stroop_trial_data * )(gs_stroop->model_data))->response = biggest_act[0]->this_node;
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

int gs_stroop_model_build (pdp_model * gs_stroop_model) {

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


  /*+--------------------------------------+*/
  /*| Top down control -> taskdemand units |*/
  /*+--------------------------------------+*/

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

  pdp_model_component_push(gs_stroop_model, word_input, ID_WORDIN, FALSE); 
  pdp_model_component_push(gs_stroop_model, colour_input, ID_COLOURIN, FALSE);
  pdp_model_component_push(gs_stroop_model, word_output, ID_WORDOUT, TRUE);
  pdp_model_component_push(gs_stroop_model, colour_output, ID_COLOUROUT, TRUE);
  pdp_model_component_push(gs_stroop_model, taskdemand, ID_TASKDEMAND, TRUE);
  pdp_model_component_push(gs_stroop_model, topdown_control, ID_TOPDOWNCONTROL, FALSE);

  return 0;
}

// Zeros activation levels of all nodes
// DOES NOT RESET WEIGHTS!!
int model_init (pdp_model * gs_stroop_model) {

  pdp_layer *word_input, *word_output, *colour_input, *colour_output, *taskdemand, *topdown_control;

  // zero cycle counter
  gs_stroop_model->cycle = 0;

  word_input = pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer;
  colour_input = pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer;
  word_output = pdp_model_component_find (gs_stroop_model, ID_WORDOUT)->layer;
  colour_output = pdp_model_component_find (gs_stroop_model, ID_COLOUROUT)->layer;
  taskdemand = pdp_model_component_find (gs_stroop_model, ID_TASKDEMAND)->layer;
  topdown_control = pdp_model_component_find (gs_stroop_model, ID_TOPDOWNCONTROL)->layer;


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

  return 0;

}



/*****************************************************************/
/* Takes a pointer to a stroop_trial_data that has been init'd   */
/* with trial parameters. Fills in the response data and returns */
/*****************************************************************/
int run_stroop_trial (pdp_model * gs_stroop_model,  
		      stroop_trial_data * this_trial,
		      const gsl_rng * random_generator) {

  // if (gs_stroop_model->model_data == NULL) {
  //   printf ("run stroop trial error! model_data pointer is null\n");
  //   return 0;
  // }
  // stroop_trial_data * subject_data = gs_stroop_model->model_data;



// init inputs

  double word_input_initial_act[3]   = { 0.0,  0.0,  0.0 };
  double colour_input_initial_act[3] = { 0.0,  0.0,  0.0 };
  double topdown_control_initial_act[2]   = { 0.0,  0.0 };


  // check that subject parameters are sensible

  // TODO: HANDLE NEUTRAL TRIALS

  if (this_trial->stim_word < -1 || this_trial->stim_word > 3) {
    printf ("subject data: word input %d out of range (should be 0 - 2)!",
	    this_trial->stim_word);
    return (0);
  }

  if (this_trial->stim_colour < -1 || this_trial->stim_colour > 3) {
    printf ("subject data: colour input %d out of range (should be 0 - 2)!",
	    this_trial->stim_colour);
    return (0);
  }

  if (this_trial->stim_task < -1 || this_trial->stim_task > 2) {
    printf ("subject data: task input %d out of range (should be 0 - 1)!",
	    this_trial->stim_task);
    return (0);
  }


  // set ON inputs
  word_input_initial_act[this_trial->stim_word] = 1.0;
  colour_input_initial_act[this_trial->stim_colour] = 1.0;
  topdown_control_initial_act[this_trial->stim_task] = 1.0;

  pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_WORDIN)->layer, 
			    3, word_input_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (gs_stroop_model, ID_COLOURIN)->layer, 
			    3, colour_input_initial_act);
  pdp_layer_set_activation (pdp_model_component_find (
                                 gs_stroop_model, ID_TOPDOWNCONTROL)->layer, 
	   		    2, topdown_control_initial_act);



  // <--------------------- RUN TRIAL ---------------------------->

  while ((stopping_condition(gs_stroop_model, this_trial) != true && 
	  gs_stroop_model->cycle < MAX_CYCLES))  {


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

 
    
  }

  this_trial->response_time = gs_stroop_model->cycle;
  // nb subject_data->response_time already set by stopping_condition

  return (1);

}



int main () {

  gsl_rng * random_generator = random_generator_create();


  // TODO - can optimise model creation & initialisation - ie. use single model rather than 
  // re-initialising for each trial
  // <-------------------- MODEL INIT ---------------------->
  pdp_model * gs_stroop_model = pdp_model_create();

  // Specify activation function
  act_func_params * activation_parameters = malloc (sizeof(act_func_params));
  activation_parameters->params.gs.step_size = STEP_SIZE;
  activation_parameters->params.gs.act_max = ACTIVATION_MAX;
  activation_parameters->params.gs.act_min = ACTIVATION_MIN;
  gs_stroop_model->activation_parameters = activation_parameters;


  // create the network & set weights
  gs_stroop_model_build (gs_stroop_model); // also inits the model for 1st sim

  // <-------------------- MODEL INIT -------------------->

  /* set up subjects structure here */
  
  subject * subject_1 = subject_create (NUM_TRIALS);

  // write trials data to the array
  // stroop_trial_data some_data = stroop_trial_data_create (0, FIXED, 1, 0, 2); 
  // g_array_append_val (subject_1->trials, some_data);

  subject_init_trialblock_fixed (random_generator, subject_1, 
				 PPN_NEUTRAL, PPN_CONGRUENT, PPN_INCONGRUENT,
				 PPN_WORDREADING, PPN_COLOURNAMING);
				 


  int trial;
  for (trial = 0; trial < NUM_TRIALS; trial++) {
    
    model_init (gs_stroop_model); // zero activations

    // associate the data for THIS TRIAL with the model
    // pdp_model_set_data (gs_stroop_model, 
    //		&(g_array_index(subject_1->fixed_trials, stroop_trial_data, trial))); 

    /* run stroop trial(s) */
    run_stroop_trial (gs_stroop_model, 
		      &(subject_1->fixed_trials[trial]), 
		      random_generator);


  /* prove it's worked */
    // TODO - save and analyse data
    printf ("\n");
    printf ("response %d: %d", 
	    subject_1->fixed_trials[trial].trial_id,
	    subject_1->fixed_trials[trial].response);
    printf ("\tafter %d cycles\n", 
	    subject_1->fixed_trials[trial].response_time);
    

    printf ("\n");


  }


  subject_free (subject_1);
  free (gs_stroop_model->activation_parameters);
  pdp_model_free (gs_stroop_model);
  random_generator_free (random_generator);  
  
  return 0;
}
