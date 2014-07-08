// definitions for structs used to communicate between callback functions
#ifndef gs_stroop_gui_h
#define gs_stroop_gui_h

#include <gtk/gtk.h>
#include <gsl/gsl_randist.h>

#include "pdp_objects.h"
#include "pdp_procedure.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop.h"

// dimensions of main gui widgets
#define MAIN_WINDOW_WIDTH_DEFAULT 800
#define MAIN_WINDOW_HEIGHT_DEFAULT 800
#define TASK_VIEW_WIDTH 750
#define TASK_VIEW_HEIGHT 500




// wrapper struct for simulation data and pointer to other objects on notepage
// eg to issue redraw events
typedef struct pdpgui_objects {


  // GUI objects that we want to refer to 
  PdpSimulation * simulation;
  GtkWidget * model_sub_notepage;


  // pointers to files & tree stores for parameter import
  // current implementation - single file treestore for all parameter imports
  FileData * config_file;
  FileData * task_config_file;



  // pointers to label widgets - for updating dashboard information
  GtkWidget * model_headerbar_label_subject;
  GtkWidget * model_headerbar_label_trial;
  GtkWidget * model_headerbar_label_trial_data;
  GtkWidget * model_headerbar_spin_trial;

} PdpGuiObjects;


// --------------------------------------------------------------------------------------
// <------------------------ IMPORT FUNCTIONS (transferred from old pdpgui_import) ------
// --------------------------------------------------------------------------------------


// callback which reads parameters from treestore and calls the translator function (model_parameter_import)
// gui, model specific. move to gs_stroop_gui or similar. 
// Wraps gs_stroop_parameters_import_commit
// static void model_parameters_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);

/*
// just proof of concept for now
// gui, model specific. not sure what this function actually does??
static void config_file_treeview_selection_changed_cb (GtkTreeSelection *selection, gpointer data);


// specifies how the treeview will look  (renderers etc)
// gui, model specific.
static void setup_model_params_treeview (GtkTreeView * tree);

*/


// create the import model params notepage
// gui, specialised to interface of a specific model. should be in gs_stroop_gui.
GtkWidget* create_notepage_import_model_params(PdpGuiObjects * objects);

// gui, model general. move to pdpgui?
void model_reset_trial_markers (PdpSimulation *simulation);

// gui, model specific
// static void setup_task_viewer_treeview (GtkTreeView * tree);

// gui, model specific
GtkWidget* create_notepage_view_trials(PdpGuiObjects * objects);

// non-gui, model specific. wraps gs_stroop_model_task_import_commit
// static void model_task_import_commit_cb (GtkWidget * button, PdpGuiObjects * objects);

// gui, model specific
GtkWidget* create_notepage_import_trials (PdpGuiObjects *objects);


// ---------------------------------------------------------------------------------
// <--------------------- Model functions (old pdpgui_test_1) ----------------------
// ---------------------------------------------------------------------------------


// prints current trial data to a file (#defined as DATAFILE)
// non-gui, model-specific
int pdpgui_print_current_trial_data (PdpSimulation * simulation);


// sets model parameters for current trial based on trial params in task_store
// non-gui, model-specific
bool model_task_parameter_import (gchar* param, GsStroopParameters *model_params);

// sets parameters for current trial
// non-gui, model-specific
int model_set_trial_params_from_task_store (GtkTreeStore *store, 
					    GtkTreeIter *trial, 
					    GsStroopParameters *model_params);

// non-gui, model-specific
int make_stroop_trial_data_from_task_store (GtkTreeStore *store, 
					GtkTreeIter *trial, 
					stroop_trial_data * data);


// gui, model-specific
void pdpgui_plot_network_activation (GtkWidget *widget, 
				cairo_t *cr, 
				PdpSimulation *simulation);

// gui, model-specific
// static GtkWidget* 
// create_sub_notepage_model_plot_activation (PdpGuiObjects * objects);

// gui, model-specific
// static GtkWidget* 
// create_sub_notepage_model_display_architecture (PdpGuiObjects * objects);

// gui, model-specific
// static void 
// model_headerbar_update_labels (PdpGuiObjects * objects);

/* // Moved to pdp_procedure.h

// non-gui, model-general, needs modification
gint model_current_trial_get (PdpSimulation *simulation);

// non-gui, model-general, needs modification
gboolean model_current_trial_is_last (PdpSimulation *simulation);
*/



// non-gui, model-general, needs modification
gboolean model_current_block_is_last (PdpSimulation *simulation);

// non-gui, model-general, needs modification
gboolean model_change_trial (PdpSimulation *simulation, 
			     GtkTreeStore *store, 
			     GtkTreePath *new_trial_path);

// nb function as-is does not update current_data, just sets the path
// non-gui, model-general, needs modification
void model_change_trial_next (PdpSimulation *simulation);

// gui, model-general. leave here
// static void model_change_trial_cb (GtkWidget * spin_button, 
//				   PdpGuiObjects * objects);

// non-gui, model-general
gboolean model_change_trial_first (PdpSimulation *simulation, 
				   GtkTreeStore *store);

// non-gui, model-general
gboolean model_change_trial_first_of_block (PdpSimulation *simulation, 
					    GtkTreeStore *store);

// non-gui, model specific (belongs in gs_stroop_model?)
// de-inits and re-builds model to implement new parameters
void model_initialise (PdpSimulation *simulation);

// gui, model-specific
// static void model_controls_initialise_cb (GtkToolItem * tool_item, 
//					  PdpGuiObjects * objects);

// gui, model-specific
// static void model_controls_step_once_cb (GtkToolItem * tool_item, 
//					 PdpGuiObjects * objects);

// gui, model-specific
// static void model_controls_step_many_cb (GtkToolItem * tool_item, 
//					 PdpGuiObjects * objects);

// gui, model-specific
// static void model_controls_run_cb (GtkToolItem * tool_item, 
//				   PdpGuiObjects * objects);

// gui, model specific
// static void model_controls_continue_cb (GtkToolItem * tool_item, 
//					PdpGuiObjects * objects);

// non-gui, model specific
void model_run_block (PdpSimulation *simulation);

// gui, model specific
// static void model_controls_run_block_cb (GtkToolItem * tool_item,
//					 PdpGuiObjects * objects);

// non-gui, model specific
void model_run_all_blocks (PdpSimulation * simulation );

// gui, model specific
// static void model_controls_run_all_blocks_cb (GtkToolItem * tool_item,
//					      PdpGuiObjects * objects);

// gui, model specific
// static GtkWidget* create_notepage_model_main(PdpGuiObjects * objects);

// allocates memory for simulation, parameters etc  and runs constructors
// non-gui, model specific
void init_model (pdp_model * this_model, GsStroopParameters *model_params);

// non-gui, model specific
void deinit_model (pdp_model * this_model);



// gui, model-specific
// static void main_quit (GtkWidget *window, PdpGuiObjects  *objects);


#endif
