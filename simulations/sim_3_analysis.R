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

data.raw$correct <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                    ifelse (cue == 1, stim_1 == response %% 2,
                                    ifelse (cue == 2, stim_2 == response %% 2, NA))))


# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_3_lookup.txt", header = FALSE, col.names=labels.lookup)
data <- merge(data.lookuptable, data)


# split congruency_seq
# data.raw$congruency_seq <- as.character(data.raw$congruency_seq)
data = transform (data, congruency = colsplit(congruency_seq, pattern = "/", names=c('1', '2', '3')))
data$congruency.23 <- paste (data$congruency.2,  data$congruency.3, sep="/")


# filter trials for correct only
                                        # should filter for correct sequences only!
data = subset (data, correct == TRUE)


# exclude outliers (RTs +/- 3 SDs)



# Graphs!
# Collapsing all congruency conditions

linegraph <- ggplot (data, aes(x=trial_pos, y=cycles, group=sequence, colour=sequence))
linegraph +
  stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.y = mean, geom = "line") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
  labs (x = "Trial Position", y = "RT", group = "Sequence") +
  ggtitle("Simulation 3: n-2 repetition costs in the 3-task switching model with lateral inhibition")

# now plot basic line graph showing interaction
linegraph.facet <- ggplot (data, aes(x=trial_pos, y=cycles, group=sequence, colour=sequence))
linegraph.facet +
  facet_grid (congruency.1 ~ congruency.23) +
  stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.y = mean, geom = "line") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
  labs (x = "Trial Position", y = "RT", group = "Sequence") +
  ggtitle("Simulation 3: n-2 repetition costs in the 3-task switching model with lateral inhibition")
