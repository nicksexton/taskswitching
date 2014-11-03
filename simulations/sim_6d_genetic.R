rm (list = ls())

library (reshape2) # for colsplit
library (pastecs) # for stat.desc

source ("sim_6_analysis_functions.R")

blocksize <- 5
filename.conf.temp <- "sim_6d_params_temp.conf" # will be created
filename.output.data.temp <- "sim_6d_data_temp.txt"

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
BIAS_NONE 0 
STIMULUS_INPUT_STRENGTH_0 3.0 
STIMULUS_INPUT_STRENGTH_1 3.0 
STIMULUS_INPUT_STRENGTH_2 3.0 
TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 
TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 
TASKDEMAND_LATERAL_INHIBITORY_WT -2.0 
CONFLICT_NEGATIVE 1 
TOPDOWN_CONTROL_STRENGTH_0 12.0 
TOPDOWN_CONTROL_STRENGTH_1 12.0  
TOPDOWN_CONTROL_STRENGTH_2 12.0 
LEARNING_RATE 1.0 
MAX_CYCLES 500") # tailing spaces eliminates double newlines, for some reason
                                        # Leaf parameters: 
                                        # BIAS_CONFLICT -7.5 
                                        # CONFLICT_GAIN 39.0 
                                        # CONFLICT_TASKDEMAND_WT -14.0 


model.conf.makeleaf <- function (x) paste ("\nCONFLICT_GAIN", x$conflict.gain,
                                          "\nCONFLICT_TASKDEMAND_WT", x$conflict.tdwt,
                                          "\nBIAS_CONFLICT", x$conflict.bias)


model.run <- function (stem, leaf, conf.tempfile, output.tempfile) {

  system2 ("rm", args=conf.tempfile) # optional, just being tidy
  write (paste(stem, leaf), conf.tempfile, append=FALSE)

  system2 ("rm", args=output.tempfile) # optional, just being tidy
  system2 ("rm", args="3task_act.txt") # optional, just being tidy
  
  run.args = paste("-t sim_6d_trials.conf -m ", conf.tempfile, sep=" ")
  system2 ("../3task_basic_koch_conflict", args=run.args, stdout = "sim_6d_log.txt", stderr = NULL)

  system2 ("mv", args=paste("3task_data.txt", output.tempfile))  
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


run.individual <- function (leaf, # parameter leaf (a data frame)
                            stem, # parameter file stem (a (long) character vector)
                            conf.file.temp, #filename for temp conf file
                            output.file.temp) { # filename for temp data

  model.run (stem,
             model.conf.makeleaf (leaf),
             conf.file.temp,
             output.file.temp)
  data <- data.preprocess (filename.output.data.temp, data.lookuptable)
  results <- calculate.switchcost (data)
  return (results)
  
}


########################### Main algorithm #############################


model.conf.leaf.default <- data.frame (conflict.gain = 39.0, conflict.tdwt = -14.0, conflict.bias = -7.5)
model.conf.leaf.min <- data.frame (conflict.gain = 0.5, conflict.tdwt = -40.0, conflict.bias = -30.0)
model.conf.leaf.max <- data.frame (conflict.gain = 100.0, conflict.tdwt = -0.0, conflict.bias = -0.0)

generation <- data.frame (conflict.gain = c(30.0, 35.0, 39.0),
                          conflict.tdwt = c(-12.0, -14.0, -16.0),
                          conflict.bias = c(-7.5, -8.5, -9.5)
                          )


generate.random <- function (min, max) {
                                        # generates a random (uniform dist) indidual between min and max
  apply (X=rbind (min, max),
         MARGIN = 2, # apply over columns
         FUN = function (x) runif (1, x[1], x[2]))
}

generate.cross <- function (x, y) {
  apply (X=rbind (x, y),
         MARGIN = 2,
         FUN = function (x) ifelse (sample(0:1, 1) == 0, x[1], x[2]))
}

generate.mutation <- function (x, min, max) {
  apply (X=rbind (x, min, max),
         MARGIN = 2,
         FUN = function (x) {
           new <- rnorm (n=1, mean=x[1], sd = abs(x[2]-x[3])/3) # min - max = 3SDs
           ifelse (new < x[2],
                   x[2],
                   ifelse(new > x[3],
                          x[3],
                          new))
       })
}



calculate.fit <- function (results, target.0SW, target.1SW) {
                                        # sum squared error
  (results$mean.0SW - target.0SW)^2 + (results$mean.1SW - target.1SW)^2
}




run.individual (generation[1,],
                model.conf.stem,
                filename.conf.temp,
                filename.output.data.temp)

# create new data frame

n <- nrow(generation)
results <- data.frame (mean.0SW=numeric(n),
                       mean.1SW=numeric(n),
                       sc=numeric(n),
                       t=numeric(n),
                       df=numeric(n),
                       p=numeric(n))

for (i in 1:nrow(generation)) {
  results[i,] <- run.individual (generation[i,],
                                stem=model.conf.stem,
                                conf.file.temp = filename.conf.temp,
                                output.file.temp = filename.output.data.temp) }


