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

  