#ifndef pdp_test_1_h
#define pdp_test_1_h


// takes an iter pointing to relevant row of task store, 
// returns a pointer to stroop_trial_data, need to free data once it is done
int make_stroop_trial_data_from_task_store (GtkTreeStore *store, GtkTreeIter *trial, stroop_trial_data * data);


void pdpgui_plot_network_activation (GtkWidget *widget, 
				     cairo_t *cr, 
				     PdpSimulation *simulation);

static GtkWidget* 
create_sub_notepage_model_plot_activation (PdpGuiObjects * objects);


static GtkWidget* 
create_sub_notepage_model_display_architecture (PdpGuiObjects * objects);

static void model_headerbar_update_labels (PdpGuiObjects * objects);


// access function, reuturns current trial as a gint
gint model_current_trial_get (PdpSimulation *simulation);





static void model_change_trial_cb (GtkWidget * spin_button, 
				   PdpGuiObjects * objects);

static void model_controls_initialise_cb (GtkToolItem * tool_item, 
					  PdpGuiObjects * objects);

static void model_controls_step_once_cb (GtkToolItem * tool_item, 
					 PdpGuiObjects * objects);

static void model_controls_step_many_cb (GtkToolItem * tool_item, 
					 PdpGuiObjects * objects);

static void model_controls_run_cb (GtkToolItem * tool_item, 
				   PdpGuiObjects * objects);

static GtkWidget* create_notepage_model_main(PdpGuiObjects * objects);

static void init_model (pdp_model * this_model, GsStroopParameters *model_params);
// to be run after create_simulation to build & initialise model



static void deinit_model (pdp_model * this_model);
// not a full free! just frees model components and activaiton parameters
// can be re-initialised with init_model


PdpSimulation * create_simulation ();

void free_simulation (PdpSimulation * simulation);

static void main_quit (GtkWidget *window, PdpGuiObjects  *objects);

int main (int argc, char *argv[]);


#endif
