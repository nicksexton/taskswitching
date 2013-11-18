#ifndef gs_stroop_h
#define gs_stroop_h

#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include "gs_stroop_subjects.h"



void add_noise_to_units (pdp_layer * some_layer, double noise_sd, gsl_rng *r);

stroop_response * make_stroop_response (int node, double activation);

bool stopping_condition (pdp_model * gs_stroop);

int model_init (pdp_model * gs_stroop_model);

int run_stroop_trial (stroop_trial_data * subject_data);


#endif
