# R data analysis for thesis simulation 2
# Extends Gilbert & Shallice (2002) item-specific priming simulation (p.319)
# to long term priming effects
# Corresponds with Allport & Wylie (2000) experiments 3, 4

# Hypothesis - long-term interference AKA asymettric restart costs AKA rebound effects,
# is a long item-specific priming effect


# Operationalisation:


# design:
# extends simulation 1 (item-specific priming)
# manipulates following factors
#  - Priming (primed vs unprimed)
#  - RSI (long vs short)
#  - Task transition (switch vs. repeat)
#  - Task (dominant vs. non-dominant)

# base      switch     restart
# ABCD   vs AB-CD   vs ABC-D   # baseline (unprimed)
# ABBC   vs AB-BC   vs ABB-C   # primed switch
# ABCB   vs AB-CB   vs ABC-B   # primed nonswitch

# Basic contrasts as in simulation 1:
# (positions 3 and 4 in ABCD) - unprimed, word switch & repeat
# position 3 in ABCD vs. ABBC - effect of priming on word switch
# position 4 in ABCD vs ABCB - effect of priming on word repeat trials

# RSI levels - ABBC vs. AB-BC vs. ABB-C :
#              all short, long RSI on switch, long RSI on repeat trials.
# Task transition : CN -> WR vs. WR -> CN


rm (list = ls())

library(ggplot2) # for graphs
library(pastecs) # for descriptive statistics
library(reshape2) # for transform

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_2")


# nb see gs_stroop_print_trial_data in gs_stroop_analyse.c for output colunms
labels.data = c("trialpath", "trialid", "trialtype", "stim_task", "stim_word", "stim_colour",
           "correct", "response", "RT", "RSI")

data.raw <- read.delim("sim_2_1_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "stim_task", "stim_word", "stim_colour", "correct", "response", "RT"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "Sequence", "SwitchDirection", "RSI_lvl", "position")
data.lookuptable = read.delim("sim_2_lookup.txt", header = FALSE, col.names=labels.lookup)
data <- merge(data.lookuptable, data)


# --------------------------- DATA TO PLOT ---------------
# ---- Define appropriate contrasts and apply factor levels

# only looking at repeat trials for now

#data.unprimed.switch = subset (data, Sequence == 'ABCD' & PATH.trial == 2)
#fac.priming <- gl(1, nrow(data.unprimed.switch), labels="unprimed")
#fac.taskTrans<- gl(1, nrow(data.unprimed.switch), labels="switch")
#data.unprimed.switch <- cbind (data.unprimed.switch, fac.priming, fac.taskTrans)

data.unprimed.repeat = subset (data, Sequence == 'ABCD' & PATH.trial == 3)
fac.priming <- gl(1, nrow(data.unprimed.repeat), labels="unprimed")
fac.taskTrans<- gl(1, nrow(data.unprimed.repeat), labels="repeat")
data.unprimed.repeat <- cbind (data.unprimed.repeat, fac.priming, fac.taskTrans)

#data.primed.switch = subset (data, Sequence == 'ABBC' & PATH.trial == 2)
#fac.priming <- gl(1, nrow(data.primed.switch), labels="primed")
#fac.taskTrans<- gl(1, nrow(data.primed.switch), labels="switch")
#data.primed.switch <- cbind (data.primed.switch, fac.priming, fac.taskTrans)

data.primed.repeat = subset (data, Sequence == 'ABCB' & PATH.trial == 3)
fac.priming <- gl(1, nrow(data.primed.repeat), labels="primed")
fac.taskTrans<- gl(1, nrow(data.primed.repeat), labels="repeat")
data.primed.repeat <- cbind (data.primed.repeat, fac.priming, fac.taskTrans)

#data.plot <- rbind (data.unprimed.switch, data.unprimed.repeat, data.primed.switch, data.primed.repeat)
data.plot <- rbind (data.unprimed.repeat, data.primed.repeat)


# --------------------------- Filter the data

# correct trials only
data.plot <- subset (data.plot, response %% 3 == correct)


# exclude outliers (RTs +/- 3 SDs)
descriptives <- by(data.plot$RT, data.plot$SwitchDirection, stat.desc, basic = FALSE, norm = TRUE)

                                        # exclude C-W trials w/ RT < mean + 3 SDs 
data.plot <- subset (data.plot,
        !((SwitchDirection == "C-W" ) & RT > descriptives$"C-W"[2] + 3 * descriptives$"C-W"[6]))

                                        # exclude C-W trials w/ RT < mean + 3 SDs 
data.plot <- subset (data.plot,
        !((SwitchDirection == "C-W" ) & RT < descriptives$"C-W"[2] - 3 * descriptives$"C-W"[6]))

                                        # exclude C-W trials w/ RT < mean + 3 SDs 
data.plot <- subset (data.plot,
        !((SwitchDirection == "C-C" ) & RT > descriptives$"C-C"[2] + 3 * descriptives$"C-C"[6]))

                                        # exclude C-W trials w/ RT < mean + 3 SDs 
data.plot <- subset (data.plot,
        !((SwitchDirection == "C-C" ) & RT < descriptives$"C-C"[2] - 3 * descriptives$"C-C"[6]))
           







# --------------------------- GRAPH ----------------------
# now plot basic line graph showing interaction
#linegraph <- ggplot (data.plot, aes(x=fac.taskTrans, y=RT, group=fac.priming, colour=fac.priming))
#linegraph +
#  facet_grid (RSI_lvl ~ SwitchDirection) +
#  stat_summary(fun.y = mean, geom = "point") +
#  stat_summary(fun.y = mean, geom = "line") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
#  labs (x = "Task Transition", y = "RT", group = "Priming") +
#  ggtitle("Simulation 2: Restart effects in the Gilbert & Shallice model")

#imageFile <- file.path(imageDirectory, "sim_2_linegraph_1.png") 
#ggsave(imageFile)


# now plot basic line graph showing interaction
linegraph <- ggplot (data.plot, aes(x=RSI_lvl, y=RT, group=fac.priming, colour=fac.priming))
linegraph +
  facet_grid (. ~ SwitchDirection) +
  stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.y = mean, geom = "line") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", width = 0.2) + 
  labs (x = "Task Transition", y = "RT", group = "Priming") +
#  ggtitle("Simulation 2:\nRestart effects in the Gilbert & Shallice model\nrestarts only")
  ggtitle("Simulation 2.1:\nRestart effects in the Gilbert & Shallice model\nrestarts only")

#imageFile <- file.path(imageDirectory, "sim_2.0_filtered.png")
imageFile <- file.path(imageDirectory, "sim_2.1_filtered.png") 
ggsave(imageFile)


# ----------------------- DESCRIPTIVE STATISTICS -----------

# Colour naming trials
stat.desc (subset(data.plot, SwitchDirection=="C-C" & fac.priming=="primed" & RSI_lvl =="all_short")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-C" & fac.priming=="primed" & RSI_lvl =="long_restart")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-C" & fac.priming=="unprimed" & RSI_lvl =="all_short")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-C" & fac.priming=="unprimed" & RSI_lvl =="long_restart")$RT)

# Word reading trials
stat.desc (subset(data.plot, SwitchDirection=="C-W" & fac.priming=="primed" & RSI_lvl =="all_short")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-W" & fac.priming=="primed" & RSI_lvl =="long_restart")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-W" & fac.priming=="unprimed" & RSI_lvl =="all_short")$RT)
stat.desc (subset(data.plot, SwitchDirection=="C-W" & fac.priming=="unprimed" & RSI_lvl =="long_restart")$RT)

