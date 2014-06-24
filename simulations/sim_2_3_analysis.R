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

library(car) # for Levene's test
library(lsr) # for Eta Squared


imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_2")


# nb see gs_stroop_print_trial_data in gs_stroop_analyse.c for output colunms
labels.data = c("trialpath", "trialid", "trialtype", "stim_task", "stim_word", "stim_colour",
           "correct", "response", "RT", "RSI")

data.raw <- read.delim("sim_2_3_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

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

# 18-06-14 : remove long_switch trials as not relevant to this analysis
data.plot <- subset (data.plot, RSI_lvl != "long_switch")



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
  ggtitle("Simulation 2.3:\nRestart effects in the Gilbert & Shallice model\nrestarts only, N=1000, \nlearning rate = 1.5, word reading control strength = 4.5") 

#imageFile <- file.path(imageDirectory, "sim_2.0_filtered.png")
imageFile <- file.path(imageDirectory, "sim_2.3.png") 
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

calc_restart_cost <- function(data) data$long[1] - data$short[1]


rt.colournaming.primed <- list (
                            short = stat.desc (subset(data.plot, SwitchDirection=="C-C" &
                              fac.priming=="primed" & RSI_lvl =="all_short")$RT)[c(9,13)],
                            long = stat.desc (subset(data.plot, SwitchDirection=="C-C" &
                              fac.priming=="primed" & RSI_lvl =="long_restart")$RT)[c(9,13)] )


rt.colournaming.unprimed <- list (
                            short = stat.desc (subset(data.plot, SwitchDirection=="C-C" &
                              fac.priming=="unprimed" & RSI_lvl =="all_short")$RT)[c(9,13)],
                            long = stat.desc (subset(data.plot, SwitchDirection=="C-C" &
                              fac.priming=="unprimed" & RSI_lvl =="long_restart")$RT)[c(9,13)] )

rt.wordreading.primed <- list (
                            short = stat.desc (subset(data.plot, SwitchDirection=="C-W" &
                              fac.priming=="primed" & RSI_lvl =="all_short")$RT)[c(9,13)],
                            long = stat.desc (subset(data.plot, SwitchDirection=="C-W" &
                              fac.priming=="primed" & RSI_lvl =="long_restart")$RT)[c(9,13)] )

rt.wordreading.unprimed <- list (
                            short = stat.desc (subset(data.plot, SwitchDirection=="C-W" &
                              fac.priming=="unprimed" & RSI_lvl =="all_short")$RT)[c(9,13)],
                            long = stat.desc (subset(data.plot, SwitchDirection=="C-W" &
                              fac.priming=="unprimed" & RSI_lvl =="long_restart")$RT)[c(9,13)] )

rt.colournaming.primed
calc_restart_cost (rt.colournaming.primed)
rt.wordreading.primed
calc_restart_cost (rt.wordreading.primed)
rt.colournaming.unprimed
calc_restart_cost (rt.colournaming.unprimed)
rt.wordreading.unprimed
calc_restart_cost (rt.wordreading.unprimed)

# ------------------------- STATISTICAL ANALYSIS --------------

# check this ANOVA is coded correctly!

model.all <- aov(RT ~ SwitchDirection +
             RSI_lvl + fac.priming +
             SwitchDirection:RSI_lvl +
             SwitchDirection:fac.priming +
             RSI_lvl:fac.priming +
             SwitchDirection:RSI_lvl:fac.priming, data = data.plot)

anova(model.all)



# filter data for 2-way ANOVAs

data.CC <- subset (data.plot, SwitchDirection == "C-C")
data.CW <- subset (data.plot, SwitchDirection == "C-W")


model.CC <- aov(RT ~ RSI_lvl + fac.priming + RSI_lvl:fac.priming, data = data.CC)
anova(model.CC)

model.CW <- aov(RT ~ RSI_lvl + fac.priming + RSI_lvl:fac.priming, data = data.CW)
anova(model.CW)
