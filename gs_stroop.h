
#include <stdbool.h>
#include <gsl/gsl_rng.h>



typedef struct stroop_response {
  int this_node; 
    // 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  double activation;
} stroop_response;


void add_noise_to_units (pdp_layer * some_layer, double noise_sd, gsl_rng *r);

stroop_response * make_stroop_response (int node, double activation);

bool stopping_condition (pdp_model * gs_stroop);

int model_init (pdp_model * gs_stroop_model);

int run_stroop_trial (stroop_trial_data * subject_data);
