# An Interactive activation model of human task switching, with a specialized model of the n-2 repetition cost.
This model of task switching developed the model of Gilbert & Shallice 2002 (Cognitive Psychology). 
It includes a complete re-implementation of that model, in set of relatively high-level object-oriented C libraries that implement neural networks in general.
I spent some time exploring the use of interactive activation networks to model reaction-time effects in human task switching, and the repository includes a model of backward inhibition and the human n-2 repetition cost that was published in Sexton & Cooper (2014). 

The model has both a graphic interface (built on the GTK framework) and a console interface, and both depend on the GTK toolkit and runs on a GNU/Linux platform, The code probably needs modification in order to run on Mac or Windows. The repo also contains quite a lot of code for running batch experiments using Python and R as interfaces.


## References
Gilbert & Shallice (2002) Task Switching: A PDP Model *Cognitive Psychology*. 

Sexton & Cooper (2017) Task inhibition, conflict, and the n-2 repetition cost: A combined computational and empirical approach *Cognitive Psychology*





Libraries for general interactive activation networks
===========================
Files with the pdp_ prefix contain an object-oriented general neural networks library of high level functions for specifying neural networks, and taking care of the moving memory around and multiplying matrices together.

(eg., see pdp_objects.h and pdp_objects.c) intended to be general to PDP models. Could be useful for any PDP style
connectionist model, and doing things like running a trial or block of trials, creating a population of models, plus miscellaneous functions for creating stimuli, analysing data etc.


####index####
gs_stroop files refer to the implementation of the Gilbert & Shallice (2002) task switching model. Current gs_stroop file uses gs_stroop model to analyse individual differences in correlations between DVs of stroop interference and task swtching (cf Miyake et al 2000). 

pdpgui_ files implement GUIs for pdp models in general.

3task_ and three_task files implement the backward inhibition model and the n-2 repetition cost. 
