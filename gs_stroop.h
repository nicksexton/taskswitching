#ifndef gs_stroop_h
#define gs_stroop_h

#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include "pdp_objects.h"
#include "gs_stroop_subjects.h"

typedef enum {OFF, NEXT_TRIAL, BLOCK, FOREVER} hebbian_learning_persistence;

typedef struct gs_stroop_parameters {

  double activation_max;
  double activation_min;
  double response_threshold;
  double step_size;
  double squashing_param;
  double noise;
  double bias_outputunit;
  double bias_taskdemand;
  double bias_none;
  double stimulus_input_strength_word;
  double stimulus_input_strength_colour;
  double taskdemand_output_inhibitory_wt;
  double taskdemand_output_excitatory_wt;
  double topdown_control_strength_word;
  double topdown_control_strength_colour;
  double learning_rate;
  hebbian_learning_persistence hebb_persist;
  double rsi_scale_param;
  int max_cycles;

} GsStroopParameters;

void gs_stroop_parameters_set_default (GsStroopParameters * params_object);

void add_noise_to_units (pdp_layer * some_layer, double noise_sd, const gsl_rng *r);

stroop_response * make_stroop_response (int node, double activation);

bool stopping_condition (const pdp_model * gs_stroop, stroop_trial_data * this_trial, double response_thresholdo);

int gs_stroop_model_build (pdp_model * gs_stroop_model, GsStroopParameters * model_params);

int model_init_params (pdp_model * gs_stroop_model, gs_stroop_params * some_params);

int model_init_activation (pdp_model * gs_stroop_model, double persist_taskdemand_activation);
// persist_taskdemand_activation sets proportion of TD activation to carry over to
// next trial ie. .20 = 20% of final activation on previous trial

int update_associative_weights (pdp_model * gs_stroop_model, 
				double learning_rate, 
				hebbian_learning_persistence persist);
  // NB running this function immediately after initing model SHOULD zero associative weights

bool run_model_step (pdp_model * gs_stroop_model, 
		     stroop_trial_data * this_trial, 
		     const gsl_rng * random_generator, 
		     double response_threshold);

int run_stroop_trial (pdp_model * gs_stroop_model,  
		      stroop_trial_data * this_trial,
		      const gsl_rng * random_generator,
		      double response_threshold,
		      hebbian_learning_persistence persist,
		      double learning_rate);


#endif
