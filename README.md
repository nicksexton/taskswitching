


pdp models implemented in C
===========================

implemented as a library of C functions (eg., see pdp_objects.h and
pdp_objects.c) intended to be general to PDP models. This project is
intended to form a general basis for implementing any PDP style
connectionist model, running a trial or block of trials, creating a
population of models, plus miscellaneous functions for creating
stimuli, analysing data etc.


Various models implemented within generic PDP library

####index####
* gs_stroop - Gilbert & Shallice (2002) task switching model. Current gs_stroop file uses gs_stroop model to analyse individual differences in correlations between DVs of stroop interference and task swtching (cf Miyake et al 2000). 

  ######todo 
  * re-create simulations from Gilbert & Shallice (2002) and Gilbert (2002) thesis. ie. create and document a fairly faithful replication.
  * use gs_stroop model out-of-the-box to explore empirical effects not discussed in published material
  * extend gs_stroop model (eg with inhibition/conflict monitoring mechanisms, to implement n-2 inhibition effects.


#### gui branch####

* pdpgui_test_1.c
initial test of a gui
needs to init and run a model
simple model controls (re-initialise, step, run)
output to console for now

  ######TODO
  * current model params are very simply defined.
  * model should store params in a generic format (g_hash_table?)
  * model currently initialises params inside model_controls_initialise_cb. hash table of params should be
    stored in pdp_model


* pdpgui_plot.c 
functions for plotting applictation-specific graphs
eg., plot of network activations
plot of DVs for group of subjects


#### gui_import_direct branch ####
trying to make parameter import simpler by using the treestore directly

* GsStroopParameters object - model parameters
defined within gs_stroop.h
function for setting default parameters (ie., from gs_stroop_default_parameters.h) defined in gs_stroop_model.c
used as a data member of PdpSimulation
future development might require a number of params sets - need to include some linked list functionality?
however - parameters are envisaged to be portable across alternative models of task switching

#### gui_import branch ####
some functions (pdp_import etc) written to implement parameter import via a hash table.
 



###### UPDATE 10-07-14 ######
Adapting model to 3 task switching. Uses generic code from previous (gilbert & shallice) model.


=============================== Generic code ===========================
lib_cairox - cairo functions
lib_string - string read/write functions
pdp_objects - generic, pdp functions
pdp_activation_funcs.c - generic, pdp functions
pdp_procedure - generic, functions for building simulations
pdp_import - generic, for importing data (ie, parameters, tasks)
pdpgui_import.c - generic, for importing 


=============================== Gilbert & Shallice model ===============================
gs_stroop_subjects - functions specific to the model (data types for trials, etc)
gs_stroop_sim_indiffs - simulation (0)
gs_stroop_model - cognitive model
gs_stroop_gui - gui for running simulations using the gs_stroop model.
gs_stroop_import - importing data (ie parameters, tasks) into the model
gs_stroop_analyse - (deprecated, only used in simulation 0)
gs_stroop_default_params - deprecated, now set using the import functions


============================== 3-task switching models ==================================
3task_procedure - build the simulation, define tasks, trial data, etc.
3task_model_gs - gilbert & shalllice GRAIN model adapted to 3 tasks
3task_gui - gui for implementing all 3-task switching models


