#ifndef three_task_gui_h
#define three_task_gui_h


// dimensions of main gui widgets
#define MAIN_WINDOW_WIDTH_DEFAULT 800
#define MAIN_WINDOW_HEIGHT_DEFAULT 800
#define TASK_VIEW_WIDTH 500
#define TASK_VIEW_HEIGHT 800


// wrapper struct for simulation data and pointer to other objects on notepage
// eg to issue redraw events
typedef struct three_task_objects {

  // GUI objects that we want to refer to 
  ThreeTaskSimulation * simulation;

  int (*model_run)(pdp_model*, ThreeTaskSimulation*);
  int (*model_reinit)(pdp_model*, init_type, ThreeTaskSimulation*);

  GtkWidget * model_sub_notepage;
  GtkWidget * task_tree_view;

  // pointers to files & tree stores for parameter import
  // current implementation - single file treestore for all parameter imports
  FileData * param_config_file;
  FileData * task_config_file;

  // pointers to label widgets - for updating dashboard information

  /*
  GtkWidget * model_headerbar_label_subject;
  GtkWidget * model_headerbar_label_trial;
  GtkWidget * model_headerbar_label_trial_data;
  GtkWidget * model_headerbar_spin_trial;
  */
  

} ThreeTaskObjects;


static void main_quit (GtkWidget *window, ThreeTaskObjects *objects);

#endif
