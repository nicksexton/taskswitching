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

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_1")


# nb see gs_stroop_print_trial_data in gs_stroop_analyse.c for output colunms
labels = c("trialpath", "trialid", "trialtype", "stim_task", "stim_word", "stim_colour",
           "correct", "response", "RT", "blank")

data.raw <- read.delim("sim_1_data.txt", header=FALSE, sep=c("", ":"), col.names=labels)

# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "stim_task", "stim_word", "stim_colour", "correct", "response", "RT"))

# column for sequence types (100 x ABCD, 100 x ABBC, 100 x ABCB)
# NB issue #19 - occasionally data not written to log file, hence only 399 trials logged per sequence (?!)
sequence <- gl(3, 399, labels = c("ABCD", "ABBC", "ABCB"))
data <- cbind (sequence, data)


# create data frame for plotting graph (Gilbert & Shallice 2002, page 20)

data.unprimed.switch = subset (data, sequence == 'ABCD' & PATH.trial == 2)
data.unprimed.repeat = subset (data, sequence == 'ABCD' & PATH.trial == 3)
data.primed.switch = subset (data, sequence == 'ABBC' & PATH.trial == 2)
data.primed.repeat = subset (data, sequence == 'ABCB' & PATH.trial == 3)

data.plot <- rbind (data.unprimed.switch, data.unprimed.repeat, data.primed.switch, data.primed.repeat)

# create the factor levels
fac.priming <- gl(2, 200, labels=c("unprimed", "primed"))
fac.switch <- gl(2, 100, labels=c("switch", "repeat"))
data.plot <- cbind (fac.priming, fac.switch, data.plot)

# --------------------------- GRAPH ----------------------
# now plot basic line graph showing interaction
linegraph <- ggplot (data.plot, aes(x=fac.switch, y=RT, group=fac.priming, colour=fac.priming))
linegraph +
  stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.y = mean, geom = "line") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
  labs (x = "Task Transition", y = "RT", group = "Priming")

imageFile <- file.path(simulation.imageDirectory, "simulation1.3 means RNG.png") 
ggsave(imageFile)
