# Executes a gridsearch over a bounded parameter space, generates a list of parameters, switch costs, and n-2 repetition costs, and reports stat tests. 
# IMPORTANT! run create_ramdisk.sh as root before executing script to create ramdisk for temporary files

# simulation 6f calculates & plots effect sizes (cohen's d) rather than significance

# rm (list = ls())

library (reshape2) # for colsplit
library (pastecs) # for stat.desc
library (plyr) # for ddply

path.simulation <- "/home/nickdbn/Programming/c_pdp_models/simulations/" 
setwd (path.simulation)

# source ("sim_6e_gridsearch_init.R") # init file SPECIFIC TO A RUN OF A SIMULATION
source(paste(path.simulation, "sim_6_analysis_functions_calcerrors.R", sep=""))

path.ramdisk <- paste("/media/ramdisk/", path.ramdiskfolder, "/", sep="")
system2("mkdir", args=path.ramdisk)





filename.conf.temp.stem <- "sim_6f_params_" # for parallel version
filename.output.temp.stem <- "sim_6f_data_" # for parallel version
filename.conf.temp <- "sim_6f_params_temp.conf" # will be created
filename.output.data.temp <- "sim_6f_data_temp.txt"
#Now specified in init file
#filename.output.genetic.results <- "sim_6d_gridsearch_results_clip_lownoise.txt" # where to store results


setwd (path.ramdisk)
system2 ("rm", args="sim_6d_lookup.txt sim_6d_trials.conf")
system2 (paste(path.simulation, "sim_6d_gridsearch_trials.py", sep=""), args=paste("-n", blocksize))

labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_6d_lookup.txt", header = FALSE, col.names=labels.lookup)

# define stem in init file
#model.conf.stem = (" 
#ACTIVATION_MAX 1.0 
#ACTIVATION_MIN -1.0 
#RESPONSE_THRESHOLD 0.15 
#STEP_SIZE 0.0015 
#SQUASHING_PARAM 0.8 
#CONFLICT_SQUASHING_PARAM 0.5 
#NOISE 0.004 # default .006
#HEBBIAN_LEARNING_PERSISTENCE 1 
#RSI_SCALE_PARAM 1.0 
#BIAS_OUTPUTUNIT -6.0 
#BIAS_TASKDEMAND -4.0 
#BIAS_NONE 0 
#STIMULUS_INPUT_STRENGTH_0 3.0 
#STIMULUS_INPUT_STRENGTH_1 3.0 
#STIMULUS_INPUT_STRENGTH_2 3.0 
#TASKDEMAND_OUTPUT_INHIBITORY_WT -2.5 
#TASKDEMAND_OUTPUT_EXCITATORY_WT 2.5 
#TASKDEMAND_LATERAL_INHIBITORY_WT -2.0 
#CONFLICT_NEGATIVE 1 
#TOPDOWN_CONTROL_STRENGTH_0 12.0 
#TOPDOWN_CONTROL_STRENGTH_1 12.0  
#TOPDOWN_CONTROL_STRENGTH_2 12.0 
#LEARNING_RATE 1.0 
#MAX_CYCLES 500") # tailing spaces eliminates double newlines, for some reason
                                        # Leaf parameters: 
                                        # BIAS_CONFLICT -7.5 
                                        # CONFLICT_GAIN 39.0 
                                        # CONFLICT_TASKDEMAND_WT -14.0 


model.conf.makeleaf <- function (x) paste ("\nCONFLICT_GAIN", x$conflict.gain,
                                          "\nCONFLICT_TASKDEMAND_WT", x$conflict.tdwt,
                                          "\nBIAS_CONFLICT", x$conflict.bias)


model.run <- function (stem, leaf, conf.tempfile, output.tempfile) {

  write (paste(stem, leaf), conf.tempfile, append=FALSE)
  
  run.args = paste("-t sim_6d_trials.conf -m ", conf.tempfile, sep=" ")
  system2 (paste(path.simulation, "/../3task_basic_koch_conflict", sep=""),
           args=run.args, stdout = NULL, stderr = NULL)

  system2 ("mv", args=paste("3task_data.txt", paste(path.ramdisk, output.tempfile, sep=""))) 
}


data.preprocess <- function (datafile, lookuptable) {

  labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")
  data <- read.delim(paste(path.ramdisk, datafile, sep=""),
                     header=FALSE, sep=c("", ":"), col.names=labels.data)
  data <- split.trialpath (data)
  data <- process.data (data, max.cycles=max.cycles) # filters data for trial 3 only!

  data <- merge(lookuptable, data, by.x = "trialid", by.y = "trialid")
  data <- subset(data, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "gaveresponse.trial", "correct.trial", "correct.block12", "gaveresponse.block12"))


### filter the data
#  data <- subset (data, correct.block == TRUE)
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
  data <- data.preprocess (output.file.temp, data.lookuptable)
  # clean up output file
  system2 ("rm", args=c(output.file.temp, conf.file.temp, "3task_act.txt")) # run.individual cleans output tempfile  

# browser()


  errors.trial12 <- calculate.errors (data$correct.block12, data$sequence_cond) 
  names(errors.trial12) <- c("err.12.0SW", "err.12.1SW", "err.12.2SW", "err.12.ALT")  

  timeouts.trial12 <- calculate.errors (data$gaveresponse.block12, data$sequence_cond)
  names(timeouts.trial12) <- c("timeout.12.0SW", "timeout.12.1SW", "timeout.12.2SW", "timeout.12.ALT")  
                                        # now we can filter out blocks where there was an error on trials 1 or 2
  data <- subset (data, correct.block12 == TRUE)
  
  
  errors.trial3 <- calculate.errors (data$correct.trial, data$sequence_cond)
  timeouts.trial3 <- calculate.errors (data$gaveresponse.trial, data$sequence_cond)
  names(timeouts.trial12) <- c("timeout.3.0SW", "timeout.3.1SW", "timeout.3.2SW", "timeout.3.ALT")   
  names(errors.trial3) <- c("err.3.0SW", "err.3.1SW", "err.3.2SW", "err.3.ALT")

  
  
                                        # filter the errors out to calc switchcost 
  data <- subset (data, PATH.trial == 2 & correct.trial == TRUE)

  
  
  results <- cbind (calculate.switchcost (data),
                    calculate.n2rc(data),
                    t(errors.trial12),
                    t(timeouts.trial12),
                    t(errors.trial3),
                    t(timeouts.trial3))


#  browser ()
  
  return (results)
  
}


########################### Main algorithm #############################




generate.grid <- function (n, min, max) {
# Generates a data frame containing a multidimensional grid with n divisions per dimension, ie n^(ncol(min)) rows

  interpolate.1d <- function (n, min, max) vect <- seq (from=min, to=max, length.out=n) 
  expand.1d <- function (vect, d) as.vector(sapply(X=vect, FUN=function(x) rep(x, d)))
  
                                        # pre-allocate the data frame

  ndims <- length(min)
  grid <- matrix (nrow = n^ndims, ncol=ndims)
  fill <- function (n, min, max, dimensions) {
    if (dimensions == 1) return (interpolate.1d (n, min, max))
#  else expand.1d (fill(n, min, max, dimensions-1), 2)
    else return (expand.1d (fill(n, min, max, dimensions-1), n))
  }

  for (dimension in 1:ndims) {
    grid[,dimension] <- fill (n, min[dimension], max[dimension], dimension)
  }

  grid <- data.frame (grid)
  colnames (grid) <- names(min)

  return (grid)

}



test.population <- function (pop) {

  progress <- txtProgressBar (min=0, max=nrow(pop), style=3)

  for (i in 1:nrow(pop)) {

    conf <- paste (filename.conf.temp.stem, ".", i, ".conf", sep="")
    data <- paste (filename.output.temp.stem, ".", i, ".txt", sep="")
    
    pop[i,names(results)] <- run.individual (leaf=pop[i,names(model.conf.leaf.min)],
                                             stem=model.conf.stem,
                                             conf.file.temp=conf,
                                             output.file.temp=data)

    setTxtProgressBar(progress, i)
  }

  close (progress)
  
  return (pop)

}



run <- function (pop) {

  pop <- cbind (pop, results)
  pop <- test.population (pop)

  file <- paste(path.simulation, filename.output.genetic.results, sep="")
  
  # print here
  print (pop)

  write.table (format(pop, digits=4),
               file,
               sep="\t",
               append=TRUE,
               quote=FALSE,
               col.names=TRUE)
  write ("\n", file, append=TRUE)
         
  return (pop)
}






params <- names (model.conf.leaf.min)

results <- data.frame (mean.0SW=numeric(n),
                       mean.1SW=numeric(n),
                       sc=numeric(n),
                       sc.t=numeric(n),
                       sc.df=numeric(n),
                       sc.p=numeric(n),
                       mean.2SW=numeric(n),
                       mean.ALT=numeric(n),
                       n2rc=numeric(n),
                       n2rc.t=numeric(n),
                       n2rc.df=numeric(n),
                       n2rc.p=numeric(n),
                       err.12.0SW=numeric(n),
                       err.12.1SW=numeric(n),
                       err.12.2SW=numeric(n),
                       err.12.ALT=numeric(n),
                       timeout.12.0SW=numeric(n),
                       timeout.12.1SW=numeric(n),
                       timeout.12.2SW=numeric(n),
                       timeout.12.ALT=numeric(n),
                       err.3.0SW=numeric(n),
                       err.3.1SW=numeric(n),
                       err.3.2SW=numeric(n),
                       err.3.ALT=numeric(n),
                       timeout.3.0SW=numeric(n),
                       timeout.3.1SW=numeric(n),
                       timeout.3.2SW=numeric(n),
                       timeout.3.ALT=numeric(n)
                       )

grid <- generate.grid (n,
                       model.conf.leaf.min,
                       model.conf.leaf.max)


  grid <- run (grid)
