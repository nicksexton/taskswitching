
#include <stdbool.h>




typedef struct stroop_response {
  int this_node; 
    // 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  double activation;
} stroop_response;

stroop_response * make_stroop_response (int node, double activation);

bool stopping_condition (pdp_model * gs_stroop);

int model_init (pdp_model * gs_stroop_model);
