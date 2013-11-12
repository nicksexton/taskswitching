/* Initialises architecture & parameters for Gilbert & Shallice Stroop
 model. Should do classic stroop effects (ie. response inhibition) and
 task switching */

#include "activation_functions.h"
#include "pdp_objects.h"


/* Global parameters */
#define ACTIVATION_MAX = 1.0
#define ACTIVATION_MIN = -1.0
#define RESPONSE_THRESHOLD = 0.15
#define STEP_SIZE = 0.0015
#define SQUASHING_PARAM = 0.8

#define NOISE = 0.006
#define OUTPUTUNIT_BIAS = -6
#define TASKDEMAND_BIAS = -4
#define TOPDOWN_CONTROL_STRENGTH_WORD = 6
#define TOPDOWN_CONTROL_STRENGTH_COLOUR = 15
#define LEARNING_RATE = 1.0



int model_init (pdp_model * gs_stroop_model) {


  pdp_layer *word_input, *word_output, *colour_input, *colour_output, *taskdemand;

  word_input = pdp_layer_create(3);
  word_output = pdp_layer_create(3);
  colour_input = pdp_layer_create(3);
  colour_output = pdp_layer_create(3);
  taskdemand = pdp_layer_create(2);

  
  double initial_activation_wordin[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourin[3] = {0.0, 0.0, 0.0};
  double initial_activation_wordout[3] = {0.0, 0.0, 0.0};
  double initial_activation_colourout[3] = {0.0, 0.0, 0.0};
  double initial_activation_taskdemand[2] = {0.0, 0.0};
  

  /* set initial activation */
  pdp_layer_set_activation(word_input, 3, initial_activation_wordin);
  pdp_layer_set_activation(word_output, 3, initial_activation_wordout);
  pdp_layer_set_activation(colour_input, 3, initial_activation_colourin);
  pdp_layer_set_activation(colour_output, 3, initial_activation_colourout);
  pdp_layer_set_activation(taskdemand, 2, initial_activation_taskdemand);


  /* <------------------------------ SET WEIGHTS --------------------------------> */
  /* Input units -> output units */
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


  /* Output unit lateral inhibition */
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


  /* Output units -> task demand */
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


  /* Taskdemand -> output units */

  pdp_weights_matrix *wts_taskdemand_wordout, *wts_taskdemand_colourout;

  double wts_taskdemand_wordout_matrix[3][2] = {
    { 2.5,  0.0},
    { 2.5,  0.0},
    { 2.5,  0.0},
  };

  double wts_taskdemand_colourout_matrix[3][2] = {
    { 0.0,  2.5},
    { 0.0,  2.5},
    { 0.0,  2.5},
  };

  wts_taskdemand_wordout = pdp_weights_create (3,2);
  pdp_weights_set (wts_taskdemand_wordout, 3, 2, wts_taskdemand_wordout_matrix);
  pdp_input_connect (word_output, taskdemand, wts_taskdemand_wordout);

  wts_taskdemand_colourout = pdp_weights_create (3,2);
  pdp_weights_set (wts_taskdemand_colourout, 3, 2, wts_taskdemand_colourout_matrix);
  pdp_input_connect (colour_output, taskdemand, wts_taskdemand_colourout);


  /* Taskdemand -> taskdemand units */
  pdp_weights_matrix *wts_taskdemand_taskdemand;
  double wts_taskdemand_taskdemand_matrix[2][2] = {
    { 0.0, -2.0},
    {-2.0,  0.0},
  };

  wts_taskdemand_taskdemand = pdp_weights_create (2,2);
  pdp_weights_set (wts_taskdemand_taskdemand, 3, 2, wts_taskdemand_taskdemand_matrix);
  pdp_input_connect (taskdemand, taskdemand, wts_taskdemand_taskdemand);


  /* TODO - remember feed forward (hebbian learning) connections from inputs to task demand */


  /* Now init model object and push components */


  pdp_model_component_push(gs_stroop_model, word_input, 1); // ID of components
  pdp_model_component_push(gs_stroop_model, colour_input, 2);
  pdp_model_component_push(gs_stroop_model, word_output, 3);
  pdp_model_component_push(gs_stroop_model, colour_output, 4);
  pdp_model_component_push(gs_stroop_model, taskdemand, 5);

  return 0;
}

int main () {

  pdp_model * gs_stroop_model = pdp_model_create();
  model_init (gs_stroop_model);
  pdp_model_free (gs_stroop_model);

  return 0;
}
