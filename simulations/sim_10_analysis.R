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
imageDirectory <- file.path("/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_10/") # path to save images to
                                        #(eg ~/Dropbox/PhD/Thesis/simulations/etc

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")

                                        #Default, use this
data.raw <- read.delim("sim_10_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)



                                        # now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))


                                        #
#
#
# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_10_lookup.txt", header = FALSE, col.names=labels.lookup)
data.raw <- merge(data.lookuptable, data.raw, by.x = "trialid", by.y = "trialid")
data.raw = subset(data.raw, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles"))
#
# create labels for tasks making up whole sequence 
data.raw = transform (data.raw, seq = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))

# Sort out correctness
data.raw$correct <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                    ifelse (cue == 1, stim_1 == response %% 2,
                                    ifelse (cue == 2, stim_2 == response %% 2, NA))))
data.raw$correct.nminus1[2:nrow(data.raw)] <- data.raw$correct[1:nrow(data.raw) - 1]
data.raw$correct.nminus2[2:nrow(data.raw)] <- data.raw$correct.nminus1[1:nrow(data.raw) - 1]


# Filter for trial 3, correct sequences only
data <- subset (data.raw, data.raw$PATH.trial == 2 &
                data.raw$correct == TRUE & 
                data.raw$correct.nminus1 == TRUE & 
                data.raw$correct.nminus2 == TRUE)


# Don't exclude outliers for rt distribution analysis??

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
data.task0<- subset (data, seq.3 == 0)
by(data.task0$cycles, data.task0$seq.2, stat.desc)
#
##task 1 (intermediate)
data.task1<- subset (data, seq.3 == 1)
by(data.task1$cycles, data.task1$seq.2, stat.desc)
#
##task 2 (hardest)
data.task2<- subset (data, seq.3 == 2)
by(data.task2$cycles, data.task2$seq.2, stat.desc)
#
#

data$seq.3 <- factor(data$seq.3)
data$seq.2 <- factor(data$seq.2)
data$seq.1 <- factor(data$seq.1)
#
#
#



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
#imageFile <- file.path(imageDirectory, "rts_tasks01.png") 
#ggsave(imageFile)



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
#imageFile <- file.path(imageDirectory, "sim_6_0_tasks02.png") 
#ggsave(imageFile)


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
#imageFile <- file.path(imageDirectory, "sim_6_0_tasks12.png") 
#ggsave(imageFile)


### Inspect error rates
with (data.raw, table (cue, correct))



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




############### Exgaussian analysis
library (retimes)

data.task01.EHE <- subset (data.task01, seq.3 == 0)
data.task01.HEH <- subset (data.task01, seq.3 == 1)


fit.exgaussian <- function (data) {
#    params <- timefit (data$resp.RT, iter=100) # number of bootstrap iterations, 1000 in Grange, Juvina etc.
  params <- timefit (data$cycles, iter=1000) # no bootstrap for now
    return (params@par)
#  return (params)
}


calc.n2rc.exg <- function (data) {
#  n2rc <- mean (subset (data, data$switch.condition == "ALT")$resp.RT)-
#    mean (subset(data, data$switch.condition == "2SW")$resp.RT)
  n2rc.params <- fit.exgaussian (subset (data, data$sequence_cond == "ALT"))-
    fit.exgaussian (subset (data, data$sequence_cond == "2SW"))  
  return (n2rc.params)
}


calc.n2rc.exg (data.task01.HEH)
calc.n2rc.exg (data.task01.EHE)


fit.exgaussian (subset (data.task01.HEH, sequence_cond == "2SW"))
fit.exgaussian (subset (data.task01.HEH, sequence_cond == "ALT"))

fit.exgaussian (subset (data.task01.EHE, sequence_cond == "2SW"))
fit.exgaussian (subset (data.task01.EHE, sequence_cond == "ALT"))
