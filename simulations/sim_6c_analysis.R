# R data analysis for thesis simulation 4 & 6
# analysing asymmetric switching between 3 tasks (simulation 4)
# see Arbuthnott (2008), thesis notes on same, and lab notebook for details

# simulation 6 uses same task structure as simulation 4 but with conflict monitoring model

# simulation 6c - comparison of 3 approaches to dealing with negative conflict - allow, clip and rescale.

# design:
# - four types of 3-trial run: no switch (BAA, BCC), one switch (AAB, BBC), two switch (CBA, ABC),
# alternating switch (ABA, CBC).
# Critical trial is the 3rd trial for each run.
# Allow assessment of relative difficulty (RTs and error rates for trial 3 of no-switch runs),
# switch costs (two switch vs. one switch), and n-2 repetition costs (two switch vs. alternating switch).
# switch cost asymmetry is the difference in switch costs between 

# distractor stimuli for all trials are incongruent/incongruent (see simulation 3.1)
# w.r.t. the correct response of the cued stim dimension.
# ie., if cued task is A, stim [0 1 1] would be II and [0 0 1] would be IC.
# these need to be counterbalanced across all 3 trials for both run types (ie., 8 conditions per run type)
# disregarding congruent/congruent trials for now.

# Basic hypothesis: Based on the hypothesis that n-2 repetition costs reflect a specific inhibition mechanism,
# lateral inhibition only (ie., modified gilbert & shallice model used in simulation 3) will not produce
# n-2 repetition costs

# Contrasts
# Trial 3 in CBA trials vs. trial 3 in ABA trials
# n-2 repetition cost - RT of trial 3 significantly longer in ABA sequences 

# Additional hypotheses:
# Congruency may affect/interact with task sequence.

rm (list = ls())
library(ggplot2) # for graphs
library(pastecs) # for descriptive statistics
library(reshape2) # for transform
# library (lineprof) # for profiling

source ("sim_6_analysis_functions.R")

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_6")
labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")
#
data.off <- read.delim("sim_6_data_conflict_off.txt",
                         header=FALSE, sep=c("", ":"), col.names=labels.data)
data.allow <- read.delim("sim_6_data_conflict_neg_allow.txt",
                         header=FALSE, sep=c("", ":"), col.names=labels.data)
data.clip <- read.delim("sim_6_data_conflict_neg_clip.txt",
                        header=FALSE, sep=c("", ":"), col.names=labels.data)
data.rescale <- read.delim("sim_6_data_conflict_neg_rescale.txt",
                           header=FALSE, sep=c("", ":"), col.names=labels.data)
                                        # now split trial path into block and trial ID
data.off <- cbind (data.off, conflict="no conflict")
data.allow <- cbind (data.allow, conflict="allow")
data.clip <- cbind (data.clip, conflict="clip")
data.rescale <- cbind (data.rescale, conflict="rescale")


# Need to clean data (ie work out block.correct) before binding multiple data sets together

# data.raw <- rbind (data.off, data.allow, data.clip, data.rescale) # commented while testing only single method

data.processed <- split.trialpath (data.off)
data.processed$correct.trial <- trial.is.correct (data.processed)

# l <- lineprof (data.off <- process.data (data.off))
system.time (data.off.1 <- block.is.correct (data.processed))

system.time (data.off.1 <- block.is.correct1 (data.processed))

system.time (data.off.1 <- block.is.correct2 (data.processed))

system.time (data.off.1 <- block.is.correct3 (data.processed))

                                        #
#
# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_6_lookup.txt", header = FALSE, col.names=labels.lookup)
data.raw <- merge(data.lookuptable, data.raw, by.x = "trialid", by.y = "trialid")
data.raw = subset(data.raw, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "conflict"))
#
# create labels for tasks making up whole sequence 
data.raw = transform (data.raw, seq = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))
#
# NB congruency analysis not needed for simulation 4 yet 
#data = transform (data, congruency = colsplit(congruency_seq, pattern = "/", names=c('1', '2', '3')))
#data$congruency.23 <- paste (data$congruency.2,  data$congruency.3, sep="/")
#data$congruency.12 <- paste (data$congruency.1,  data$congruency.2, sep="/")
#
#

#######################################################
################### FILTERING #########################
#######################################################
                                        # should filter for correct sequences only!
exclude.outliers <- function (x, field, max, min) subset(x, x$field > min || x$field < max)

data = subset (data.raw, correct == TRUE)




# exclude outliers (RTs +/- 3 SDs) for each task (irrespective of trial position or switch condition)
# exclude outliers (RTs +/- 3 SDs)
#descriptives <- by(data$cycles, data$cue, stat.desc)
#data <- subset (data,
#        !((cue == 0 ) & cycles > descriptives$"0"[9] + 3 * descriptives$"0"[13]))
#data <- subset (data,
#        !((PATH.trial == 1 ) & cycles > descriptives$"1"[9] + 3 * descriptives$"1"[13]))
#data <- subset (data,
#        !((PATH.trial == 2 ) & cycles > descriptives$"2"[9] + 3 * descriptives$"2"[13]))
#
#
# Now look only at trial 3
#
#
#
###task 0 (easiest)
#data.task0<- subset (data, PATH.trial == 2 & seq.3 == 0)
#by(data.task0$cycles, data.task0$seq.2, stat.desc)
#
##task 1 (intermediate)
#data.task1<- subset (data, PATH.trial == 2 & seq.3 == 1)
#by(data.task1$cycles, data.task1$seq.2, stat.desc)
#
##task 2 (hardest)
#data.task2<- subset (data, PATH.trial == 2 & seq.3 == 2)
#by(data.task2$cycles, data.task2$seq.2, stat.desc)
#
#
data$seq.3 <- factor(data$seq.3)
data$seq.2 <- factor(data$seq.2)
data$seq.1 <- factor(data$seq.1)
#
#
#
######################### DEBUG STUFF ####################################


boxplot.clip <- ggplot (subset(data, PATH.trial == 2 & conflict=="clip"), aes(x=sequence_cond, y=cycles))
boxplot.clip + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") +
  ggtitle ("Simulation 6c, negative conflict clipped")

boxplot.allow <- ggplot (subset(data, PATH.trial == 2 & conflict=="allow"), aes(x=sequence_cond, y=cycles))
boxplot.allow + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") +
  ggtitle ("Simulation 6c, negative conflict allowed")

boxplot.rescale <- ggplot (subset(data, PATH.trial == 2 & conflict=="rescale"), aes(x=sequence_cond, y=cycles))
boxplot.rescale + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") +
  ggtitle ("Simulation 6c, negative conflict rescaled")



##########################################################################
##########################################################################
##########################################################################



# Plot graph for switches between tasks 0 and 1
#data.clip.task01 <- subset (data,
#                            conflict == "clip" &
#                            PATH.trial == 2 & (
#                              (seq.3 == 0 & (seq.2 == 0 | seq.2 == 1)) |
#                              (seq.3 == 1 & (seq.2 == 1 | seq.2 == 0)) )
#                       )

data.all <- subset (data,
                    PATH.trial == 2)

linegraph <- ggplot (data.all, aes(x=sequence_cond, y=cycles, group=conflict, fill=conflict))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "RT", group = "Sequence") +
  ggtitle("Simulation 6c: Switch costs and n-2 repetition costs\n Effect of negative conflict treatments")
 imageFile <- file.path(imageDirectory, "sim_6c_treatment_0.png") 
 ggsave(imageFile)



# descriptive statistics, tasks 0 (easy) and 1 (intermediate)
descriptives <- function (x) by (x$cycles, x$sequence_cond, stat.desc)

calculate.RT.mean <- function (x, condition) descriptives(x)[[condition]][["mean"]]
calculate.RT.sd <- function (x, condition) descriptives(x)[[condition]][["std.dev"]]


tabulate.RT <- function (data) {

  output <- data.frame (sequence = c("0SW", "1SW", "2SW", "ALT"))
  for (i in 1:4) {
    output$mean[i] <- calculate.RT.mean (data, output$sequence[i])
    output$sd[i] <- calculate.RT.sd (data, output$sequence[i])
  }

  return (output)
}


# calculate.switchcost <- function (x) calculate.RT.mean(x, "1SW") - calculate.RT.mean(x, "0SW")
calculate.n2rc <- function (x) calculate.RT.mean(x, "ALT") - calculate.RT.mean(x, "2SW")

test.switchcost <- function (x)
  t.test (cycles ~ sequence_cond, data = subset(x, sequence_cond == "0SW" | sequence_cond =="1SW"))

test.n2rc <- function (x)
  t.test (cycles ~ sequence_cond, data = subset(x, sequence_cond == "2SW" | sequence_cond =="ALT"))

calculate.switchcost <- function (x){
  data.frame (
    mean.0SW = calculate.RT.mean (x, "0SW"),
    mean.1SW = calculate.RT.mean (x, "1SW"),
    sc = calculate.RT.mean (x, "0SW") - calculate.RT.mean (x, "1SW"),
    t = test.switchcost(x)[[1]][[1]],
    df = test.switchcost(x)[[2]][[1]],
    p = test.switchcost(x)[[3]][[1]]
   )     
}

calculate.n2rc <- function (x){
  data.frame (
    mean.2SW = calculate.RT.mean (x, "2SW"),
    mean.ALT = calculate.RT.mean (x, "ALT"),
    n2rc = calculate.RT.mean (x, "ALT") - calculate.RT.mean (x, "2SW"),
    t = test.n2rc(x)[[1]][[1]],
    df = test.n2rc(x)[[2]][[1]],
    p = test.n2rc(x)[[3]][[1]]
   ) 
}


data.off <- subset(data.all, conflict=="no conflict")
tabulate.RT (data.off)
calculate.switchcost (data.off)
calculate.n2rc (data.off)


data.allow <- subset(data.all, conflict=="allow")
tabulate.RT (data.allow)
calculate.switchcost (data.allow)
calculate.n2rc (data.allow)


data.clip <- subset(data.all, conflict=="clip") # 
tabulate.RT (data.clip)
calculate.switchcost (data.clip)
calculate.n2rc (data.clip)


data.rescale <- subset(data.all, conflict=="rescale") # 
tabulate.RT (data.rescale)
calculate.switchcost (data.rescale)
calculate.n2rc (data.rescale)





