#include <gtk/gtk.h>
#include "pdp_objects.h"
#include "3task_import.h"


void three_task_parameters_htable_set_default (GHashTable * params_table);
void three_task_gs_parameters_import_commit (FileData *config_file, 
					     GHashTable *model_params);

void init_model (pdp_model * this_model, GHashTable *model_params_htable);

void deinit_model (pdp_model * this_model);
