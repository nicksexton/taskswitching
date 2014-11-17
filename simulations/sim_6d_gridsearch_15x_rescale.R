# Initialisation file for specific run of a simulation

# rm (list = ls())

filename.output.genetic.results <- "sim_6d_gridsearch_results_15x_rescale_3.txt" # where to store results


blocksize <- 100
n <- 15 # resolution of grid to explore param space (ie total observations is n^ncols(min)
max.cycles <- 500
model.conf.leaf.min <- c(conflict.gain = 1.0, conflict.tdwt = -30.0, conflict.bias = -40.0)
model.conf.leaf.max <- c(conflict.gain = 100.0, conflict.tdwt = 0.0, conflict.bias = -1.0)

path.ramdiskfolder <- "rescale_lownoise_3"

model.conf.stem = (" 
ACTIVATION_MAX 1.0 
ACTIVATION_MIN -1.0 
RESPONSE_THRESHOLD 0.15 
STEP_SIZE 0.0015 
SQUASHING_PARAM 0.8 
CONFLICT_SQUASHING_PARAM 0.5 
NOISE 0.004 # default .006
HEBBIAN_LEARNING_PERSISTENCE 1 
RSI_SCALE_PARAM 1.0 
BIAS_OUTPUTUNIT -6.0 
BIAS_TASKDEMAND -4.0 
BIAS_NONE 0 
STIMULUS_INPUT_STRENGTH_0 3.0 
STIMULUS_INPUT_STRENGTH_1 3.0 
STIMULUS_INPUT_STRENGTH_2 3.0 
TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 
TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 
TASKDEMAND_LATERAL_INHIBITORY_WT -2.0 
CONFLICT_NEGATIVE 3 # rescale
TOPDOWN_CONTROL_STRENGTH_0 12.0 
TOPDOWN_CONTROL_STRENGTH_1 12.0  
TOPDOWN_CONTROL_STRENGTH_2 12.0 
LEARNING_RATE 1.0 
MAX_CYCLES 500") # tailing spaces eliminates double newlines, for some reason
                                        # Leaf parameters: 
                                        # BIAS_CONFLICT -7.5 
                                        # CONFLICT_GAIN 39.0 
                                        # CONFLICT_TASKDEMAND_WT -14.0 


source ("sim_6d_gridsearch_run.R")
