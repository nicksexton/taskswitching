# R data analysis for thesis simulation 3
# analysing n-2 repetition cost

# design:
# - two types of 3-trial run: ABA, CBA
# each trial can be either incongruent/congruent or incongruent/incongruent 
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

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_3")

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")

data.raw <- read.delim("sim_3_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_3_lookup.txt", header = FALSE, col.names=labels.lookup)
data <- merge(data.lookuptable, data)


# filter trials for correct only


# exclude outliers (RTs +/- 3 SDs)

# now plot basic line graph showing interaction
linegraph <- ggplot (data, aes(x=trial_pos, y=cycles, group=sequence, colour=sequence))
linegraph +
  facet_wrap ( ~ congruency_seq) +
  stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.y = mean, geom = "line") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
  labs (x = "Trial Position", y = "RT", group = "Sequence") +
  ggtitle("Simulation 3: n-2 repetition costs in the 3-task switching model with lateral inhibition")
