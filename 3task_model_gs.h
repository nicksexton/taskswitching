#include <gtk/gtk.h>
#include "pdp_objects.h"


void three_task_parameters_htable_set_default (GHashTable * params_table);

void init_model (pdp_model * this_model, GHashTable *model_params_htable);

void deinit_model (pdp_model * this_model);
