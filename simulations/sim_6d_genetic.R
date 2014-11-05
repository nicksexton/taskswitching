rm (list = ls())

# IMPORTANT! run create_ramdisk.sh as root before executing script to create ramdisk for temporary files



library (reshape2) # for colsplit
library (pastecs) # for stat.desc
library (plyr) # for ddply

#library (foreach)
#library (doParallel)


path.simulation <- "/home/nickdbn/Programming/c_pdp_models/simulations/" 
path.ramdisk <- "/media/ramdisk/"

source (paste(path.simulation, "sim_6_analysis_functions.R", sep=""))

blocksize <- 50
n <- 200 # population size
max.cycles <- 500

filename.conf.temp.stem <- "sim_6d_params_" # for parallel version
filename.output.temp.stem <- "sim_6d_data_" # for parallel version
filename.conf.temp <- "sim_6d_params_temp.conf" # will be created
filename.output.data.temp <- "sim_6d_data_temp.txt"
filename.output.genetic.results <- "sim_6d_genetic_results.txt" # results of GA


setwd (path.ramdisk)
system2 ("rm", args="sim_6d_lookup.txt sim_6d_trials.conf")
system2 (paste(path.simulation, "sim_6d_genetic_trials.py", sep=""), args=paste("-n", blocksize))

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




generate.random <- function (min, max) {
                                        # generates a random (uniform dist) indidual between min and max
  apply (X=rbind (min, max),
         MARGIN = 2, # apply over columns
         FUN = function (x) runif (1, x[1], x[2]))
}

generate.cross <- function (x, y) apply (X=rbind (x, y),
                                         MARGIN = 2,
                                         FUN = function (x) ifelse (sample(0:1, 1) == 0, x[1], x[2]))


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

generate.mutation1 <- function (x, min, max) {

  apply (X=rbind (x, min, max),
         MARGIN = 2,
         FUN = function (x) rnorm (n=1, mean=x[1], sd = abs(x[2]-x[3])/3)) # min - max = 3SDs

}


generate.population.seed <- function (n, min, max) {
                                        # pre-allocate the data frame
  seed <- data.frame (matrix(nrow=n, ncol=ncol(min)))
  colnames (seed) <- names(min)

  for (i in seq(from=1, to=n))
    seed[i,] <- apply (X=seed[i,], MARGIN=2, FUN=function (x) generate.random (min, max))

  return (seed)
}


calculate.fit <- function (results, target.0SW, target.1SW) {
                                        # sum squared error
  (results$mean.0SW - target.0SW)^2 + (results$mean.1SW - target.1SW)^2
}


evolve.generation <- function (gen, minimum, maximum) {
#gen - only parameters from generation (ie., generation[names(params)]
  
  # Q1 = best 25% of current population
  # Q2 = crossed 25% of current popn
  # Q3 = mutated best 25%
  # Q4 = new randomly generated

  q.size <- ceiling(as.numeric(nrow(gen)/4)) # to avoid decreasing population due to rounding problems
  q1 <- gen[1:q.size,] 

  # debug!
#  q1 <- rbind (q1, q1[3,])
#  print (q1)
  
#  q2 <- ddply (.data=q1,
#               .variables=c("id", names(minimum)),
#               .fun=function (x) {
#                 y <- sample(1:q.size)
#                 generate.cross (x, q1[y,])
#               })
  q2 <- data.frame(t(apply (X=q1, MARGIN=1, FUN=function (x) {
                  y <- sample(1:q.size, 1)
                 generate.cross (x, q1[y,])
               })))

#  print (q2)

#  browser()

#  q3 <- ddply (.data=q1,
#               .variables=c("id", names(minimum)),
#               .fun= function(x) generate.mutation (x, minimum, maximum))
  q3 <- data.frame(t(apply (X=q1,
                            MARGIN=1,
                            FUN = function(x) generate.mutation (x, minimum, maximum))))

#  print (q3)
  
  q4 <- generate.population.seed (q.size, minimum, maximum)
#  print (q4)
  
#  size <- data.frame (segment=c("q1", "q2", "q3", "q4"), pop=c(nrow(q1), nrow(q2), nrow(q3), nrow(q4)))
#  print (size)
#  ifelse (size$pop != q.size, browser(), print ("ok")) 
  
  # new <- rbind (q1, q2, q3, q4)[1:nrow(gen),] # trim in case nrow(gen) wasn't divisible by 4
  new <- rbind (q1, q2, q3, q4) 

#  print (paste ("total:", nrow(new)))
  print (new)
  
  rownames (new) <- 1:nrow(new)

  return (new)
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

test.generation.parallel <- function (gen, iteration) {

#  progress <- txtProgressBar (min=0, max=nrow(gen), style=3)

  foreach (i=1:nrow(gen), .packages=c("reshape2", "pastecs", "plyr")) %dopar% {

    conf <- paste ("sim_6d_params_.", iteration, ".", i, ".conf", sep="")
    data <- paste ("sim_6d_data_.", iteration, ".", i, ".txt", sep="")
    
    gen[i,names(results)] <- run.individual (leaf=gen[i,names(model.conf.leaf.min)],
                                             stem=model.conf.stem,
                                             conf.file.temp=conf,
                                             output.file.temp=data)

#    setTxtProgressBar(progress, icount)
  }

#  close (progress)
  
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
