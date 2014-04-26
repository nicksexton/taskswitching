# R data analysis for thesis simulation 1
# Replication of Gilbert & Shallice (2002) item-specific priming simulation (p.319)
# Corresponds with Allport & Wylie (2000) experiment 5
# Hypothesis - primed stimuli (ie. associated with colour naming task) will produce
# larger RTs on word reading task

# Preparatory to simulation 2 (which tests the hypothesis that long-term interference
# AKA asymettric restart costs AKA rebound effects, is a long item-specific priming effect


# Operationalisation:
# Sequence (1) - ABCD (tasks: CCWW) Hebbian persistence - single trial only (1)
# Sequence (2) - ABBC (tasks: CCWW) Hebbian persistence - whole block (2)
# Sequence (3) - ABCB (tasks: CCWW) Hebbian persistence - whole block (2)
# all incongruent stimuli
# CONTRASTS: Baseline (positions 3 and 4 in (1)) - unprimed, word switch & repeat
# position 3 in (1) vs. (2) - effect of priming on word switch
# position 4 in (1) vs (2) - effect of priming on word repeat trials

# nb delayed vs. immediate repetition not relevant in this simulation as hebbian weights
# do not decrease


rm (list = ls())

library(ggplot2) # for graphs
library(pastecs) # for descriptive statistics
library(reshape2) # for transform

# nb see gs_stroop_print_trial_data in gs_stroop_analyse.c for output colunms
labels = c("trialpath", "trialid", "trialtype", "stim_task", "stim_word", "stim_colour",
           "correct", "response", "RT", "blank")

data.raw <- read.delim("sim_1_data.txt", header=FALSE, sep=c("", ":"), col.names=labels)

# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

# trim unwanted columns
data = subset(data.raw, select = c("PATH.block", "PATH.trial", "stim_task", "stim_word", "stim_colour", "correct", "response", "RT"))
