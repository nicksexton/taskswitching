#ifndef gs_stroop_global_params_h
#define gs_stroop_global_params_h

/* Global parameters */
#define ACTIVATION_MAX 1.0
#define ACTIVATION_MIN -1.0
#define RESPONSE_THRESHOLD 0.15
#define STEP_SIZE 0.0015
#define SQUASHING_PARAM 0.8

#define NOISE 0.006
#define OUTPUTUNIT_BIAS -6.0
#define TASKDEMAND_BIAS -4.0
#define BIAS_NONE 0
#define STIMULUS_INPUT_STRENGTH_WORD 3.5
#define STIMULUS_INPUT_STRENGTH_COLOUR 1.9

#define TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 // default
#define TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 // default
#define TASKDEMAND_OUTPUT_INHIB_VARY_MAX 0.0
#define TASKDEMAND_OUTPUT_INHIB_VARY_MIN -8.0

#define TOPDOWN_CONTROL_STRENGTH_WORD 6.0
#define TOPDOWN_CONTROL_STRENGTH_COLOUR 15.0
#define LEARNING_RATE 1.0
#define MAX_CYCLES 500 // how long to let model run - NB check G&S defaults
                        // TODO - need to track and handle 'no response' trials

#define ID_WORDIN 1
#define ID_COLOURIN 2
#define ID_WORDOUT 3
#define ID_COLOUROUT 4
#define ID_TASKDEMAND 5
#define ID_TOPDOWNCONTROL 6

#define NUM_SUBJECTS 500
#define NUM_TRIALS 100 // total number of trials
#define MIXED_BLOCK_RUN_LENGTH 12 // must be multiple of 3??

// relative proportion of congruent, incongruent, neutral trials 
#define PPN_CONGRUENT 33 
#define PPN_INCONGRUENT 33
#define PPN_NEUTRAL 33

// relative proportion of word reading vs. colour naming trials
#define PPN_WORDREADING 50 
#define PPN_COLOURNAMING 50

#define DATAFILE "gs_stroop_data.txt"


// #define ECHO // echo console output

#endif
