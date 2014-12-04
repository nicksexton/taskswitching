# Executes a gridsearch over a bounded parameter space, generates a list of parameters, switch costs, and n-2 repetition costs, and reports stat tests. 
# IMPORTANT! run create_ramdisk.sh as root before executing script to create ramdisk for temporary files

# Version of the gridsearch that uses resampling to improve data from each voxel

# rm (list = ls())

library (reshape2) # for colsplit
library (pastecs) # for stat.desc
library (plyr) # for ddply

path.simulation <- "/home/nickdbn/Programming/c_pdp_models/simulations/" 
setwd (path.simulation)

# source ("sim_6e_gridsearch_init.R") # init file SPECIFIC TO A RUN OF A SIMULATION
source (paste(path.simulation, "sim_6_analysis_functions_calcerrors.R", sep=""))

path.ramdisk <- paste("/media/ramdisk/", path.ramdiskfolder, "/", sep="")
system2("mkdir", args=path.ramdisk)





filename.conf.temp.stem <- "sim_7a_params_" # for parallel version
filename.output.temp.stem <- "sim_7a_data_" # for parallel version
filename.conf.temp <- "sim_7a_params_temp.conf" # will be created
filename.output.data.temp <- "sim_7a_data_temp.txt"


setwd (path.ramdisk)
system2 ("rm", args="sim_7a_lookup.txt sim_7a_trials.conf")
system2 (paste(path.simulation, "sim_7a_gridsearch_trials.py", sep=""), args=paste("-n", blocksize))

labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_7a_lookup.txt", header = FALSE, col.names=labels.lookup)


## Note, model.conf.stem defined in init file

model.conf.makeleaf <- function (x) paste ("\nSTIMULUS_INPUT_STRENGTH_0", x$task.input.str,
                                          "\nCONFLICT_TASKDEMAND_WT", x$conflict.tdwt,
                                          "\nTOPDOWN_CONTROL_STRENGTH_0", x$task.topdown.str)


model.run <- function (stem, leaf, conf.tempfile, output.tempfile) {

  write (paste(stem, leaf), conf.tempfile, append=FALSE)
  
  run.args = paste("-t sim_7a_trials.conf -m ", conf.tempfile, sep=" ")
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

  
 # split columns for processing
  data <- transform (data, seq = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))
  data$seq.3 <- factor(data$seq.3)
  data$seq.2 <- factor(data$seq.2)
  data$seq.1 <- factor(data$seq.1)

  return (data)
}


data.analyse <- function (data) {

  errors.trial12 <- calculate.errors (data$correct.block12, data$sequence_cond) 
  names(errors.trial12) <- c("err.12.0SW", "err.12.1SW", "err.12.2SW", "err.12.ALT")  

  timeouts.trial12 <- calculate.errors (data$gaveresponse.block12, data$sequence_cond)
  names(timeouts.trial12) <- c("timeout.12.0SW", "timeout.12.1SW", "timeout.12.2SW", "timeout.12.ALT")  
                                        # now we can filter out blocks where there was an error on trials 1 or 2
  data <- subset (data, correct.block12 == TRUE)
  
  
  errors.trial3 <- calculate.errors (data$correct.trial, data$sequence_cond)
  timeouts.trial3 <- calculate.errors (data$gaveresponse.trial, data$sequence_cond)
  names(timeouts.trial3) <- c("timeout.3.0SW", "timeout.3.1SW", "timeout.3.2SW", "timeout.3.ALT")   
  names(errors.trial3) <- c("err.3.0SW", "err.3.1SW", "err.3.2SW", "err.3.ALT")
  

  # Debug: what is going on with NAs in trial 3 errors
  data <- subset (data, PATH.trial == 2)
#  browser()
  if (is.na(errors.trial3[1]) | is.na (errors.trial3[2])) { browser() }
  ## End debug
  
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


  # How we classify trials according to which tasks the 2nd trial in each seq belongs to
  switchdir <- data.frame(sequence=c("0/0/1", "0/0/2", "0/1/0", "0/1/1", "0/1/2", "0/2/0",
                                     "0/2/1", "0/2/2", "1/0/0", "1/0/1", "1/0/2", "1/1/0",
                                     "1/1/2", "1/2/0", "1/2/1", "1/2/2", "2/0/0", "2/0/1",
                                     "2/0/2", "2/1/0", "2/1/1", "2/1/2", "2/2/0", "2/2/1"),
                          alternation=c("AB", "AC", "BA", "AB", "BC", "CA",
                                        "CB", "AC", "BA", "AB", "AC", "BA",
                                        "BC", "CA", "CB", "BC", "CA", "AB",
                                        "AC", "BA", "CB", "BC", "CA", "CB"))

  data <- merge (data, switchdir)
                                        # create an empty data frame to paste the data into
  results <- data.frame (
               switchdir = c("AB", "AC", "BA", "BC", "CA", "CB"),
               mean.0SW=vector(mode="numeric", length=6),
               mean.1SW=vector(mode="numeric", length=6),
               sc=vector(mode="numeric", length=6),
               sc.t=vector(mode="numeric", length=6),
               sc.df=vector(mode="numeric", length=6),
               sc.p=vector(mode="numeric", length=6),
               mean.2SW=vector(mode="numeric", length=6),
               mean.ALT=vector(mode="numeric", length=6),
               n2rc=vector(mode="numeric", length=6),
               n2rc.t=vector(mode="numeric", length=6),
               n2rc.df=vector(mode="numeric", length=6),
               n2rc.p=vector(mode="numeric", length=6),
               err.12.0SW=vector(mode="numeric", length=6),
               err.12.1SW=vector(mode="numeric", length=6),
               err.12.2SW=vector(mode="numeric", length=6),
               err.12.ALT=vector(mode="numeric", length=6),
               timeout.12.0SW=vector(mode="numeric", length=6),
               timeout.12.1SW=vector(mode="numeric", length=6),
               timeout.12.2SW=vector(mode="numeric", length=6),
               timeout.12.ALT=vector(mode="numeric", length=6),
               err.3.0SW=vector(mode="numeric", length=6),
               err.3.1SW=vector(mode="numeric", length=6),
               err.3.2SW=vector(mode="numeric", length=6),
               err.3.ALT=vector(mode="numeric", length=6),
               timeout.3.0SW=vector(mode="numeric", length=6),
               timeout.3.1SW=vector(mode="numeric", length=6),
               timeout.3.2SW=vector(mode="numeric", length=6),
               timeout.3.ALT=vector(mode="numeric", length=6))


  
  results <- ddply (.data=data, .(alternation), .fun=data.analyse)
#  browser ()
  if(any(is.na(results))) {browser()}
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
    else return (expand.1d (fill(n, min, max, dimensions-1), n))
  }

  for (dimension in 1:ndims) {
    grid[,dimension] <- fill (n, min[dimension], max[dimension], dimension)
  }

  grid <- data.frame (grid)
  colnames (grid) <- names(min)

  return (grid)

}



test.population <- function (pop, pop.results) {

  # how many levels in pop.results?
  nlevels <- nrow(pop.results) / nrow(pop)
  
  progress <- txtProgressBar (min=0, max=nrow(pop), style=3)

  for (i in 1:nrow(pop)) {

    conf <- paste (filename.conf.temp.stem, ".", i, ".conf", sep="")
    data <- paste (filename.output.temp.stem, ".", i, ".txt", sep="")

    results.indiv <- run.individual (leaf=pop[i,names(model.conf.leaf.min)],
                                     stem=model.conf.stem,
                                     conf.file.temp=conf,
                                     output.file.temp=data)

    #debug
#    browser()
    if (is.na(results.indiv$err.3.0SW) | 
        is.na(results.indiv$err.3.1SW) |
        is.na(results.indiv$err.3.2SW) |
        is.na(results.indiv$err.3.ALT)) { browser() }


    
    pop.results[(1+(nlevels*(i-1))):(i*nlevels),names(results)] <- results.indiv[,2:ncol(results.indiv)]

    # browser()
    setTxtProgressBar(progress, i)
  }

  close (progress)
  
  return (pop.results)

}



run <- function (pop, levels) {

  expand.1d <- function (vect, d) as.vector(sapply(X=vect, FUN=function(x) rep(x, d)))

#  browser()
  
  pop.expanded <- apply (X=pop, MARGIN=2, FUN=function(x) expand.1d(x, length(levels)))
  pop.expanded <- cbind (data.frame(pop.expanded), alternation=rep(levels, nrow(pop)))
#  pop <- cbind (pop, apply (X=results, MARGIN=2, FUN=function(x) expand.1d(x, length(levels))))
  pop.results <- cbind (pop.expanded, results)

  
  pop.results <- test.population (pop, pop.results)

  file <- paste(path.simulation, filename.output.results, sep="")
  
  # print here
  print (pop.results)

  write.table (format(pop.results, digits=4),
               file,
               sep="\t",
               append=TRUE,
               quote=FALSE,
               col.names=TRUE)
  write ("\n", file, append=TRUE)
         
  return (pop.results)
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

levels <- c("AB", "AC", "BA", "BC", "CA", "CB")

grid <- generate.grid (n,
                       model.conf.leaf.min,
                       model.conf.leaf.max)

# Now expand the results grid to make data frame for results




results <- run (grid, levels)
