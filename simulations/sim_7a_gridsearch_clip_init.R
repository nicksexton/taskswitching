# Initialisation file for specific run of a simulation
# noise at .006 (default) for comparison of errors, c/w .004 for previous gridsearch

rm (list = ls())

path.simulation <- "/home/nickdbn/Programming/c_pdp_models/simulations/" 
setwd (path.simulation)



filename.output.results <- "sim_7a_gridsearch_results_highnoise_clip.txt" # where to store results


blocksize <- 100
n <- 15 # resolution of grid to explore param space (ie total observations is n^ncols(min)
max.cycles <- 500

# Bounds for gridsearch
model.conf.leaf.min <- c(task.input.str = 1.5, task.topdown.str = 6.0, conflict.tdwt = -30.0)
model.conf.leaf.max <- c(task.input.str = 5.0, task.topdown.str = 20.0, conflict.tdwt = 0.0)


path.ramdiskfolder <- "7a_clip_0"

# define stem in init file
model.conf.stem = (" 
ACTIVATION_MAX 1.0 
ACTIVATION_MIN -1.0 
RESPONSE_THRESHOLD 0.15 
STEP_SIZE 0.0015 
SQUASHING_PARAM 0.8 
CONFLICT_SQUASHING_PARAM 0.5 
NOISE 0.006 # default .006
HEBBIAN_LEARNING_PERSISTENCE 1 
RSI_SCALE_PARAM 1.0 
BIAS_OUTPUTUNIT -6.0 
BIAS_TASKDEMAND -4.0 
BIAS_NONE 0 
BIAS_CONFLICT -10.0 
CONFLICT_GAIN 75.0 
STIMULUS_INPUT_STRENGTH_1 3.0 
STIMULUS_INPUT_STRENGTH_2 3.0 
TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 
TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 
TASKDEMAND_LATERAL_INHIBITORY_WT -2.0 
CONFLICT_NEGATIVE 2 # clip
TOPDOWN_CONTROL_STRENGTH_1 12.0  
TOPDOWN_CONTROL_STRENGTH_2 12.0 
LEARNING_RATE 1.0 
MAX_CYCLES 500") # tailing spaces eliminates double newlines, for some reason
                                        # Leaf parameters: 
                                        # STIMULUS_INPUT_STRENGTH_0 3.0 
                                        # TOPDOWN_CONTROL_STRENGTH_0 12.0 
                                        # CONFLICT_TASKDEMAND_WT -14.0 




source ("sim_7a_gridsearch.R")
