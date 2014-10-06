# R data analysis for thesis simulation 4
# analysing asymmetric switching between 3 tasks
# see Arbuthnott (2008), thesis notes on same, and lab notebook for details

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

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_4")

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")

data.raw <- read.delim("sim_4_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw$correct <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                    ifelse (cue == 1, stim_1 == response %% 2,
                                    ifelse (cue == 2, stim_2 == response %% 2, NA))))



# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
data.lookuptable = read.delim("sim_4_lookup.txt", header = FALSE, col.names=labels.lookup)
data.raw <- merge(data.lookuptable, data.raw)
data.raw = subset(data.raw, select = c("trialid", "sequence", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct"))

# NB congruency analysis not needed for simulation 4 yet 
#data = transform (data, congruency = colsplit(congruency_seq, pattern = "/", names=c('1', '2', '3')))
#data$congruency.23 <- paste (data$congruency.2,  data$congruency.3, sep="/")
#data$congruency.12 <- paste (data$congruency.1,  data$congruency.2, sep="/")


# filter trials for correct only
                                        # should filter for correct sequences only!
data = subset (data, correct == TRUE)

# exclude outliers (RTs +/- 3 SDs)
# exclude outliers (RTs +/- 3 SDs)
descriptives <- by(data$cycles, data$trial_pos, stat.desc)
                                        # exclude C-W trials w/ RT < mean + 3 SDs 
data <- subset (data,
        !((trial_pos == 0 ) & cycles > descriptives$"0"[9] + 3 * descriptives$"0"[13]))
data <- subset (data,
        !((trial_pos == 1 ) & cycles > descriptives$"1"[9] + 3 * descriptives$"1"[13]))
data <- subset (data,
        !((trial_pos == 2 ) & cycles > descriptives$"2"[9] + 3 * descriptives$"2"[13]))






# ------------------------- STATISTICAL ANALYSIS --------------
# First do analysis collapsed across condition

model.collapsed <- aov(cycles ~ sequence +
                       trial_pos + 
                       sequence:trial_pos,
                       data = data)

anova(model.collapsed)

# T-test for position 3 only
data.pos_3 <- subset (data, trial_pos == 2)
t.test (data.pos_3$cycles ~ data.pos_3$sequence)


# Now break down by congruency condition.
# Diagnostic 1 - 2x2 ANOVA, sequence x congruency_1 on RT_1 - ie. analysing position 1 trials only.
# sequence should NOT be significant
data.pos_1 <- subset (data, trial_pos == 0)
model.pos_1 <- aov(cycles ~ sequence +
                       congruency.1 + 
                       sequence:congruency.1,
                       data = data.pos_1)
anova(model.pos_1)

# Diagnostic 2 - 2x2x2 ANOVA, sequence x congruency_1 x congruency_2 on RT_2 - ie. analysing position 2 trials only.
data.pos_2 <- subset (data, trial_pos == 1)
model.pos_2 <- aov(cycles ~
                   sequence + congruency.1 + congruency.2 + 
                   sequence:congruency.1 + sequence:congruency.2 + congruency.1:congruency.2 +
                   sequence:congruency.1:congruency.2,              
                   data = data.pos_2)
anova(model.pos_2)

# Main test - 2x2x2x2 ANOVA, sequence x congruency_1 x congruency_2 x congruency_3 on RT_3 - ie. analysing position 3 trials only.
# already got data for pos_3
model.pos_3 <- aov(cycles ~
                   sequence + congruency.1 + congruency.2 + congruency.3 +
                   sequence:congruency.1 + sequence:congruency.2 + sequence:congruency.3 +
                   congruency.1:congruency.2 + congruency.1:congruency.3 + congruency.2:congruency.3 +
                   sequence:congruency.1 + sequence:congruency.2 + sequence:congruency.3 +
                   sequence:congruency.1:congruency.2 +
                   sequence:congruency.1:congruency.3 +
                   sequence:congruency.2:congruency.3 +
                   sequence:congruency.1:congruency.2:congruency.3,
                   data = data.pos_3)
anova(model.pos_3)
