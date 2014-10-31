rm (list = ls())

source ("sim_6_analysis_functions.R")

blocksize <- 100
model.conf.tempfile <- "sim_6d_model_genetic.conf" # will be created
output.data.tempfile <- "sim_6d_data_temp.txt"

system2 ("rm", args="sim_6d_lookup.txt sim_6d_trials.conf")
system2 ("./sim_6d_genetic_trials.py", args=paste("-n", blocksize))

labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_6d_lookup.txt", header = FALSE, col.names=labels.lookup)

model.conf.stem = (" 
ACTIVATION_MAX 1.0 
ACTIVATION_MIN -1.0 
RESPONSE_THRESHOLD 0.15 
STEP_SIZE 0.0015 
SQUASHING_PARAM 0.8 
CONFLICT_SQUASHING_PARAM 0.5 
NOISE 0.006 
HEBBIAN_LEARNING_PERSISTENCE 1 
RSI_SCALE_PARAM 1.0 
BIAS_OUTPUTUNIT -6.0 
BIAS_TASKDEMAND -4.0 
BIAS_CONFLICT -7.5 
BIAS_NONE 0 
STIMULUS_INPUT_STRENGTH_0 3.0 
STIMULUS_INPUT_STRENGTH_1 3.0 
STIMULUS_INPUT_STRENGTH_2 3.0 
TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 
TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 
TASKDEMAND_LATERAL_INHIBITORY_WT -2.0 
CONFLICT_GAIN 39.0 
CONFLICT_TASKDEMAND_WT -14.0 
CONFLICT_NEGATIVE 1 
TOPDOWN_CONTROL_STRENGTH_0 12.0 
TOPDOWN_CONTROL_STRENGTH_1 12.0  
TOPDOWN_CONTROL_STRENGTH_2 12.0 
LEARNING_RATE 1.0 
MAX_CYCLES 500") # tailing spaces eliminates double newlines, for some reason


model.run <- function (model.conf) {

  write (model.conf, model.conf.tempfile)

  system2 ("rm", args=output.data.tempfile) # optional, just being tidy
  system2 ("rm", args="3task_act.txt") # optional, just being tidy
  
  run.args = paste("-t sim_6_trials.conf -m ", model.conf.tempfile, "> sim_6d_log.txt", sep=" ")
  system2 ("../3task_basic_koch_conflict", args=run.args, stdout = NULL, stderr = NULL)

  system2 ("mv", args=paste("3task_data.txt", output.data.tempfile))

  
}


data.preprocess <- function (datafile, lookuptable) {

  labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")
  data <- read.delim(datafile,
                     header=FALSE, sep=c("", ":"), col.names=labels.data)
  data <- split.trialpath (data)
  data <- process.data (data)

  data <- merge(lookuptable, data, by.x = "trialid", by.y = "trialid")
  data <- subset(data, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct.trial", "correct.block"))


### filter the data
  data <- subset (data, correct.block == TRUE)
                                        # Don't filter outliers here!!

 # split columns for processing
  data <- transform (data, seq = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))
  data$seq.3 <- factor(data$seq.3)
  data$seq.2 <- factor(data$seq.2)
  data$seq.1 <- factor(data$seq.1)


  return (data)
}



model.run (model.conf.stem)

data <- data.preprocess (output.data.tempfile, data.lookuptable)



data.desc <- descriptives (data)
