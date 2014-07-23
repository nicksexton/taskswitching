// same as gs_stroop_global_params.h
// except, gs_stroop_global_params should not now be used directly
// default parameters (ie., in this file) should be imported via a GsStroopParameters object

#ifndef three_task_global_params_h
#define three_task_global_params_h

/* Model parameters */
// NB see gs_stroop.h where model parameters structure is defined

#define ACTIVATION_MAX 1.0
#define ACTIVATION_MIN -1.0
#define RESPONSE_THRESHOLD 0.15
#define STEP_SIZE 0.0015
#define SQUASHING_PARAM 0.8

#define NOISE 0.006
#define OUTPUTUNIT_BIAS -6.0
#define TASKDEMAND_BIAS -4.0
#define BIAS_NONE 0
#define STIMULUS_INPUT_STRENGTH_0 3.5
#define STIMULUS_INPUT_STRENGTH_1 1.9
#define STIMULUS_INPUT_STRENGTH_2 1.9

#define TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 // default
#define TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 // default
#define TASKDEMAND_OUTPUT_INHIB_VARY_MAX 0.0
#define TASKDEMAND_OUTPUT_INHIB_VARY_MIN -8.0

#define TOPDOWN_CONTROL_STRENGTH_0 6.0
#define TOPDOWN_CONTROL_STRENGTH_1 15.0
#define TOPDOWN_CONTROL_STRENGTH_2 15.0
#define LEARNING_RATE 1.0
#define MAX_CYCLES 500 // how long to let model run - NB check G&S defaults
                        // TODO - need to track and handle 'no response' trials




/* Simulation Parameters */

#define NUM_SUBJECTS 500
#define NUM_TRIALS 100 // total number of trials


#endif
