


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
