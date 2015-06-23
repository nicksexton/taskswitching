# R data analysis for thesis simulation 4 & 6
# analysing asymmetric switching between 3 tasks (simulation 4)
# see Arbuthnott (2008), thesis notes on same, and lab notebook for details

# simulation 6 uses same task structure as simulation 4 but with conflict monitoring model

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
imageDirectory <- file.path("/media/ramdisk/simulation_6d_diagnostic") # path to save images to
                                        #(eg ~/Dropbox/PhD/Thesis/simulations/etc

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")

                                        #Default, use this
data.raw <- read.delim("sim_10_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)



                                        # now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))
#
data.raw$correct <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                    ifelse (cue == 1, stim_1 == response %% 2,
                                    ifelse (cue == 2, stim_2 == response %% 2, NA))))
#
#
#
# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_6_lookup.txt", header = FALSE, col.names=labels.lookup)
data.raw <- merge(data.lookuptable, data.raw, by.x = "trialid", by.y = "trialid")
data.raw = subset(data.raw, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct"))
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
data <- data.raw
# filter trials for correct only
                                        # should filter for correct sequences only!
# data = subset (data.raw, correct == TRUE)


  
data.raw$exclude <- apply (data.raw, 1, function (x) all (data.raw[data.raw$PATH.block==x["PATH.block"],]$correct))

data.raw$cue.prev <- as.factor(c(NA, as.character(data.raw$cue[1:nrow(data.raw)-1])))
data.correct <- subset (data.raw, data.raw$exclude == FALSE)



                                        #
# exclude outliers (RTs +/- 3 SDs) for each task (irrespective of trial position or switch condition)
# exclude outliers (RTs +/- 3 SDs)
#descriptives <- by(data$cycles, data$cue, stat.desc)
#data <- subset (data,
#        !((cue == 0 ) & cycles > descriptives$"0"[9] + 3 * descriptives$"0"[13]))
#data <- subset (data,
#        !((PATH.trial == 1 ) & cycles > descriptives$"1"[9] + 3 * descriptives$"1"[13]))
#data <- subset (data,
#        !((PATH.trial == 2 ) & cycles > descriptives$"2"[9] + 3 * descriptives$"2"[13]))


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

# 0SW
data.0SW <- subset (data, sequence_cond == "0SW")
bargraph <- ggplot (subset(data.0SW, PATH.trial == 2), aes(x=seq.3, y=cycles, group=seq.1, fill=seq.1))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trials 2 & 3)", y = "RT", group = "task (trial 1)") +
  ggtitle("Simulation 4: 0SW only")



# RTs for 1SW condition
data.1SW <- subset (data, sequence_cond == "1SW")
by (data.1SW$cycles, data.1SW$seq.3, stat.desc)

bargraph <- ggplot (subset(data.1SW, PATH.trial == 2), aes(x=seq.3, y=cycles, group=seq.2, fill=seq.2))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
  ggtitle("Simulation 4: 1SW only")

#look at middle trial
bargraph <- ggplot (subset(data.1SW, PATH.trial == 1), aes(x=seq.2, y=cycles, group=seq.1, fill=seq.1))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 2)", y = "RT", group = "task (trial 1)") +
  ggtitle("Simulation 4: 1SW only, middle trial")


#================ RTs for 2SW condition
data.2SW <- subset (data, sequence_cond == "2SW")
by (data.2SW$cycles, data.2SW$seq.3, stat.desc)
bargraph <- ggplot (subset(data.2SW, PATH.trial ==1), aes(x=seq.3, y=cycles, group=seq.2, fill=seq.2))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
  ggtitle("Simulation 4: 2SW only")

#look at middle trial
bargraph <- ggplot (subset(data.2SW, PATH.trial == 1), aes(x=seq.2, y=cycles, group=seq.1, fill=seq.1))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 2)", y = "RT", group = "task (trial 1)") +
  ggtitle("Simulation 4: 2SW only, middle trial")

# Look at initial trial (broken out by task.sequence
bargraph <- ggplot (subset(data.2SW, PATH.trial == 0), aes(x=seq.1, y=cycles, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 0)", y = "RT", group = "sequence") +
  ggtitle("Simulation 4: 2SW only, initial trial")



#=============== RTs for ALT condition
data.ALT <- subset (data, sequence_cond == "ALT")
by (data.ALT$cycles, data.ALT$seq.3, stat.desc)
bargraph <- ggplot (data.ALT, aes(x=seq.3, y=cycles, group=seq.2, fill=seq.2))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
  ggtitle("Simulation 4: Alt Switch only")


boxplot.task0 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 0), aes(x=sequence_cond, y=cycles))
boxplot.task0 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 0")
#imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task0_boxplot.png") 
#ggsave(imageFile)

boxplot.task1 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 1), aes(x=sequence_cond, y=cycles))
boxplot.task1 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 1")
#imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task1_boxplot.png") 
#ggsave(imageFile)

boxplot.task2 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 2), aes(x=sequence_cond, y=cycles))
boxplot.task2 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 2")
#imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task2_boxplot.png") 
#ggsave(imageFile)


##########################################################################




# Plot graph for switches between tasks 0 and 1
data.task01 <- subset (data, PATH.trial == 2 & (
                               (seq.3 == 0 & (seq.2 == 0 | seq.2 == 1)) |
                               (seq.3 == 1 & (seq.2 == 1 | seq.2 == 0)) )
                       )

linegraph <- ggplot (data.task01, aes(x=sequence_cond, y=cycles, group=seq.3, fill=seq.3))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "RT", group = "Sequence") +
  ggtitle("Simulation 6: Asymmetric switch costs and n-2 repetition costs\n Switches between tasks 0 and 1")
imageFile <- file.path(imageDirectory, "sim_6_0_tasks01.png") 
ggsave(imageFile)



# Plot graph for switches between tasks 0 and 2
data.task02 <- subset (data, PATH.trial == 2 & (
                               (seq.3 == 0 & (seq.2 == 0 | seq.2 == 2)) |
                               (seq.3 == 2 & (seq.2 == 2 | seq.2 == 0)) )
                       )

linegraph <- ggplot (data.task02, aes(x=sequence_cond, y=cycles, group=seq.3, fill=seq.3))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "RT", group = "Sequence") +
  ggtitle("Simulation 6: Asymmetric switch costs and n-2 repetition costs\n Switches between tasks 0 and 2")
imageFile <- file.path(imageDirectory, "sim_6_0_tasks02.png") 
ggsave(imageFile)


# Plot graph for switches between tasks 1 and 2
data.task12 <- subset (data, PATH.trial == 2 & (
                               (seq.3 == 1 & (seq.2 == 1 | seq.2 == 2)) |
                               (seq.3 == 2 & (seq.2 == 2 | seq.2 == 1)) )
                       )

linegraph <- ggplot (data.task12, aes(x=sequence_cond, y=cycles, group=seq.3, fill=seq.3))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "RT", group = "Sequence") +
  ggtitle("Simulation 6: Asymmetric switch costs and n-2 repetition costs\n Switches between tasks 1 and 2")
imageFile <- file.path(imageDirectory, "sim_6_0_tasks12.png") 
ggsave(imageFile)


# descriptive statistics, tasks 0 (easy) and 1 (intermediate)
data.task01.1 <- subset(data.task01, data.task01$seq.3==1) # harder task (1)
by(data.task01.1$cycles, data.task01.1$sequence_cond, stat.desc)

data.task01.0 <- subset(data.task01, data.task01$seq.3==0) # easier task (0)
by(data.task01.0$cycles, data.task01.0$sequence_cond, stat.desc)


# descriptive statistics, tasks 0 (easy) and 2 (hard)
data.task02.2 <- subset(data.task02, data.task02$seq.3==2) # hard task (2)
by(data.task02.2$cycles, data.task02.2$sequence_cond, stat.desc)

data.task02.0 <- subset(data.task02, data.task02$seq.3==0) # easy task (0)
by(data.task02.0$cycles, data.task02.0$sequence_cond, stat.desc)

# descriptive statistics, tasks 1 (intermediate) and 2 (hard)
data.task12.2 <- subset(data.task12, data.task12$seq.3==2) # hard task (2)
by(data.task12.2$cycles, data.task12.2$sequence_cond, stat.desc)

data.task12.1 <- subset(data.task12, data.task12$seq.3==1) # easy task (1)
by(data.task12.1$cycles, data.task12.1$sequence_cond, stat.desc)



######################################### Stat Tests ######################################


############## FOR SYMMETRIC SWITCHING ###################

data.aggregate.trial3 <-subset (data, PATH.trial==2)
by (data.aggregate.trial3$cycles, data.aggregate.trial3$sequence_cond, stat.desc) # RT on trial 3 only

# t-test for switch cost
model.aggregate.trial3.switchcost <- t.test (cycles ~ sequence_cond,
                                             data = subset (data.aggregate.trial3,
                                                 sequence_cond == "0SW" | sequence_cond == "1SW"))
model.aggregate.trial3.switchcost


# t-test for n-2 repetition cost
model.aggregate.trial3.n2rc <- t.test (cycles ~ sequence_cond,
                                             data = subset (data.aggregate.trial3,
                                                 sequence_cond == "2SW" | sequence_cond == "ALT"))
model.aggregate.trial3.n2rc



############## FOR ASYMMETRIC SWITCHING ##################


############## 0 and 1 ##################

# Switch cost - assessing whether switch costs are reversed
# 2X2 ANOVA:
# Factors: sequence_cond (levels: 0SW, 1SW only), seq.3 (levels: 0, 2)
# Hypothesis:
#    main effect of sequence cond (ie., switch cost)
#    main effect of seq.3 (ie., different RTs for different tasks)
#    seq.3 x sequence_cond interaction: signf. asymmetric switch cost


# switch cost
model.task01.sc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task01, sequence_cond == "0SW" | sequence_cond == "1SW"))
anova(model.task01.sc)

#n2 repetition cost
model.task01.n2rc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task01, sequence_cond == "ALT" | sequence_cond == "2SW"))
anova(model.task01.n2rc)

# post-hoc test: is there a significant n-2 repetition cost in each condition
# task 0
model.task01.n2rc.0 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task01, seq.3 == 0),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task01.n2rc.0

# task 1
model.task01.n2rc.1 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task01, seq.3 == 1),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task01.n2rc.1



# Test for reversed switch cost asymmetry???



############## 1 and 2 ##################

# switch cost
model.task12.sc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task12, sequence_cond == "0SW" | sequence_cond == "1SW"))
anova(model.task12.sc)

#n2 repetition cost
model.task12.n2rc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task12, sequence_cond == "ALT" | sequence_cond == "2SW"))
anova(model.task12.n2rc)

# post-hoc test: is there a significant n-2 repetition cost in each condition
# task 1
model.task12.n2rc.1 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task12, seq.3 == 1),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task12.n2rc.1

# task 2
model.task12.n2rc.2 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task12, seq.3 == 2),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task12.n2rc.2

############## 0 and 2 ##################

# switch cost
model.task02.sc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task02, sequence_cond == "0SW" | sequence_cond == "1SW"))
anova(model.task02.sc)

#n2 repetition cost
model.task02.n2rc <- aov(cycles ~ sequence_cond +
                       seq.3 + 
                       sequence_cond:seq.3,
                       data = subset(data.task02, sequence_cond == "ALT" | sequence_cond == "2SW"))
anova(model.task02.n2rc)

# post-hoc test: is there a significant n-2 repetition cost in each condition
# task 1
model.task02.n2rc.0 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task02, seq.3 == 0),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task02.n2rc.0

# task 2
model.task02.n2rc.2 <- t.test (cycles ~ sequence_cond,
                               data = subset (data.task02, seq.3 == 2),
                               sequence_cond == "2SW" | sequence_cond == "ALT")
model.task02.n2rc.2

