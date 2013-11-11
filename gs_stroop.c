/* Initialises architecture & parameters for Gilbert & Shallice Stroop
 model. Should do classic stroop effects (ie. response inhibition) and
 task switching */


int model_init () {




int main () {
  
  pdp_layer * word_input, word_output, colour_input, colour_output, taskdemand;

  word_input = pdp_layer_create(3);
  word_output = pdp_layer_create(3);
  colour_input = pdp_layer_create(3);
  colour_output = pdp_layer_create(3);
  taskdemand = pdp_layer_create(2);

  return 0;
 
}
