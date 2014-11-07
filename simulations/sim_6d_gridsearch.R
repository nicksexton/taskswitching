# Executes a gridsearch over a bounded parameter space, generates a list of parameters, switch costs, and n-2 repetition costs, and reports stat tests. 
# IMPORTANT! run create_ramdisk.sh as root before executing script to create ramdisk for temporary files


rm (list = ls())


library (reshape2) # for colsplit
library (pastecs) # for stat.desc
library (plyr) # for ddply

#library (foreach)
#library (doParallel)


path.simulation <- "/home/nickdbn/Programming/c_pdp_models/simulations/" 
path.ramdisk <- "/media/ramdisk/"

source (paste(path.simulation, "sim_6_analysis_functions.R", sep=""))

# blocksize <- 50
n <- 10 # size of grid (ie total observations is n^ncols(min)
max.cycles <- 500

filename.conf.temp.stem <- "sim_6d_params_" # for parallel version
filename.output.temp.stem <- "sim_6d_data_" # for parallel version
filename.conf.temp <- "sim_6d_params_temp.conf" # will be created
filename.output.data.temp <- "sim_6d_data_temp.txt"
filename.output.genetic.results <- "sim_6d_genetic_results.txt" # results of GA


setwd (path.ramdisk)
system2 ("rm", args="sim_6d_lookup.txt sim_6d_trials.conf")
system2 (paste(path.simulation, "sim_6d_gridsearch_trials.py", sep=""), args=paste("-n", blocksize))

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
CONFLICT_NEGATIVE 3 
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
  data <- process.data (data, max.cycles=max.cycles)

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
  data <- data.preprocess (output.file.temp, data.lookuptable)
  # clean up output file
  system2 ("rm", args=c(output.file.temp, conf.file.temp, "3task_act.txt")) # run.individual cleans output tempfile  

  results <- calculate.switchcost (data)
  return (results)
  
}


########################### Main algorithm #############################


# test data frames
min <- c(a=1.0, b=0.0, c=-10.0)
max <- c(a=5.0, b=1.0, c=10.0)

interpolate.1d <- function (n, min, max) vect <- seq (from=min, to=max, length.out=n) 

expand.1d <- function (vect, d) {
# expands a vector n to size n*d by repeating each element d times
  as.vector(sapply(X=vect, FUN=function(x) rep(x, d)))
}

generate.grid <- function (n, min, max) {
# Generates a data frame containing a multidimensional grid with n divisions per dimension, ie n^(ncol(min)) rows

                                        # pre-allocate the data frame

  ndims <- length(min)
  grid <- matrix (nrow = n^ndims, ncol=ndims)

#  grid[,1] <- rep(interpolate.1d (n, min[1], max[1]), n^(ndims-1))
  grid[,1] <- (interpolate.1d (n, min[1], max[1]), n^(ndims-1))

  if (ndims > 1) {
    for (dimension in 2:ndims) {
      grid[,dimension] <- expand.1d(grid[1:n^(dimension-1),dimension-1], n)
    }
  }

  grid <- data.frame (grid)
  colnames (grid) <- names(min)
}




test.generation <- function (gen, iteration) {

  progress <- txtProgressBar (min=0, max=nrow(gen), style=3)

  for (i in 1:nrow(gen)) {

    conf <- paste (filename.conf.temp.stem, ".", iteration, ".", i, ".conf", sep="")
    data <- paste (filename.output.temp.stem, ".", iteration, ".", i, ".txt", sep="")
    
    gen[i,names(results)] <- run.individual (leaf=gen[i,names(model.conf.leaf.min)],
                                             stem=model.conf.stem,
                                             conf.file.temp=conf,
                                             output.file.temp=data)

    setTxtProgressBar(progress, i)
  }

  close (progress)
  
  gen$ssqerror <- calculate.fit (results=gen[names(results)],
                                 target.0SW=65.0, target.1SW=105.0)

  gen <- gen[order (gen$ssqerror),] # sort by ssqerror

#  head (gen)                            #output this generation to file here

  
  return (gen)

}



run.generation <- function (gen, iteration) {

  generation <- cbind (gen, results, data.frame("ssqerror"=0))

  generation.results <- test.generation (generation, iteration)
#  generation.results <- test.generation.parallel (generation, iteration)

  file <- paste(path.simulation, filename.output.genetic.results, sep="")
  
  # print here
  print (generation.results)
  write (paste("\nGENERATION ", iteration, ": \n"), file, append=TRUE)

  write.table (format(generation.results, digits=4),
               file,
               sep="\t",
               append=TRUE,
               quote=FALSE,
               col.names=TRUE)
  write ("\n", file, append=TRUE)
         
  gen <- evolve.generation (generation.results[names(model.conf.leaf.min)],
                            min=model.conf.leaf.min,
                            max=model.conf.leaf.max)
  return (gen)
}




# model.conf.leaf.default <- data.frame (conflict.gain = 39.0, conflict.tdwt = -14.0, conflict.bias = -7.5)


model.conf.leaf.min <- data.frame (conflict.gain = 1.0, conflict.tdwt = -30.0, conflict.bias = -40.0)
model.conf.leaf.max <- data.frame (conflict.gain = 100.0, conflict.tdwt = -1.0, conflict.bias = -1.0)

# model.conf.leaf.TEST <- data.frame (conflict.gain = NA, conflict.tdwt = NA, conflict.bias = NA)

params <- names (model.conf.leaf.min)

  results <- data.frame (mean.0SW=numeric(n),
                         mean.1SW=numeric(n),
                         sc=numeric(n),
                         t=numeric(n),
                         df=numeric(n),
                         p=numeric(n))

generation.seed <- generate.population.seed (n,
                                             model.conf.leaf.min,
                                             model.conf.leaf.max)

gen <- generation.seed

# temp
total.generations <- 40
for (i in 1:total.generations) {
    print (paste ("Generation", i, "of", total.generations))
  gen <- run.generation (gen, i)
  print (nrow(gen))
}
