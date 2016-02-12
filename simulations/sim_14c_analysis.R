# R data analysis for thesis simulation 3/9
# analysing n-2 repetition cost given varying congruency conditions

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
library(lsr) # for etaSquared

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_9")

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response", "conflict.0", "conflict.1", "conflict.2")

data.raw.low<- read.delim("sim_14c_data_low.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)




# now split trial path into block and trial ID
data.raw.low$trialpath <- as.character(data.raw.low$trialpath)
data.raw.low = transform (data.raw.low, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw.low$correct.response <- with (data.raw.low, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw.low$correct <- with (data.raw.low, correct.response == response %% 2)

# trim unwanted columns
data.low = subset(data.raw.low, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial_pos", "congruency_seq", "congruency_trial")
 data.lookuptable = read.delim("sim_14_lookup.txt", header = FALSE, col.names=labels.lookup)

data.low <- merge(data.lookuptable, data.low, by="trialid", sort=FALSE)


### Repeat for high

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_9")

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response", "conflict.0", "conflict.1", "conflict.2")

data.raw.high<- read.delim("sim_14c_data_high.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)




# now split trial path into block and trial ID
data.raw.high$trialpath <- as.character(data.raw.high$trialpath)
data.raw.high = transform (data.raw.high, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw.high$correct.response <- with (data.raw.high, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw.high$correct <- with (data.raw.high, correct.response == response %% 2)

# trim unwanted columns
data.high = subset(data.raw.high, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial_pos", "congruency_seq", "congruency_trial")
 data.lookuptable = read.delim("sim_14_lookup.txt", header = FALSE, col.names=labels.lookup)

data.high <- merge(data.lookuptable, data.high, by="trialid", sort=FALSE)


#### Merge
data.low$weight <- -9.0
data.high$weight <- -3.5

data <- rbind (data.low, data.high)

##### Now filter trials for correct only
                                        # should filter for correct sequences only!
data = subset (data, correct == TRUE)


exclude.outliers <- function (data) {
    
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
return (data)
}
########################


data.rescale <- data
data.clip <- data

                                        # Filter negative conflict (replace with zero)
conflict.clip <- function (data){
data$conflict.0[data$conflict.0<0] <- 0
data$conflict.1[data$conflict.1<0] <- 0
data$conflict.2[data$conflict.2<0] <- 0
data$conflict.total <- data$conflict.0 + data$conflict.1 + data$conflict.2
return (data)
}


conflict.rescale <- function (data){
data$conflict.0 <- (data$conflict.0 + 1) / 2
data$conflict.1 <- (data$conflict.1 + 1) / 2
data$conflict.2 <- (data$conflict.2 + 1) / 2
data$conflict.total <- data$conflict.0 +
                               data$conflict.1 +
                               data$conflict.2
return (data)
}


# debug graph: for symmetric task difficulty, expect RTs to be equal

bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=weight, y=cycles, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Task (trial 3)", y = "RT") +
  ggtitle("Simulation 9: overall trial 3 RTs (debug)")



corr.annotation <- function(x,y, digits = 2) {
    c <- round(cor(x, y), digits = digits)
    paste("italic(r) == ", c)
}


   # For simulation 14b, only look at conflict affecting task 0




## Looking at repeating and non-repeating sequences separately for high vs low weight
# 
data.rescale.by.rep <- subset (conflict.rescale(data),
                               trial_pos == 2 )
data.rescale.by.rep$rep <- rep("repeat", nrow(data.rescale.by.rep))
data.rescale.by.rep$rep[data.rescale.by.rep$sequence=="2SW" |
                        data.rescale.by.rep$sequence == "ALT"] <- "no_repeat"

scatter.rescale.incl <- ggplot (data.rescale.by.rep,
                                aes(conflict.0, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) +
    ggtitle ("total (final trial) conflict by RT, strong vs weak weights") +
        facet_grid(weight ~ rep)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_incl_rescale.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



### Look at total conflict instead

scatter.rescale.incl <- ggplot (data.rescale.by.rep,
                                aes(conflict.total, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) +
    ggtitle ("total (final trial) conflict by RT, strong vs weak weights") +
        facet_grid(weight ~ rep)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_totalc_incl_rescale.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



### Repeating vs. non-repeating sequences, for high vs. low weight

# 
data.rescale.by.rep <- conflict.rescale(data)
data.rescale.by.rep$rep <- rep("repeat", nrow(data.rescale.by.rep))
data.rescale.by.rep$rep[data.rescale.by.rep$sequence=="2SW" |
                        data.rescale.by.rep$sequence == "ALT"] <- "no_repeat"

scatter.rescale.incl <- ggplot (data.rescale.by.rep,
                                aes(conflict.0, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) +
    ggtitle ("AB conflict by RT, all trials") +
        facet_grid(weight ~ rep)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_incl_rescale_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



### Look at total conflict instead

scatter.rescale.incl <- ggplot (data.rescale.by.rep,
                                aes(conflict.total, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) +
    ggtitle ("total conflict by RT, all trials") +
        facet_grid(weight ~ rep)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_totalc_incl_rescale_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")






                                        # Is there an association between RT and conflict?


## # Clip, exclude outliers
## model <- aov(conflict.total~ sequence,
##                        data = subset (conflict.clip(exclude.outliers(data)), trial_pos == 2))
## anova(model)
## with (subset (conflict.clip(exclude.outliers(data)), trial_pos == 2), by(conflict.0, sequence, stat.desc))


## # Clip, include outliers
## model <- aov(conflict.total ~ sequence,
##                        data = subset (conflict.clip(data), trial_pos == 2))
## anova(model)
## with (subset (conflict.clip(data), trial_pos == 2), by(conflict.0, sequence, stat.desc))


## # Rescale, exclude outliers
## model <- aov(conflict.total ~ sequence,
##                        data = subset (conflict.rescale(exclude.outliers(data)), trial_pos == 2))
## anova(model)
## with (subset (conflict.rescale(exclude.outliers(data)), trial_pos == 2), by(conflict.0, sequence, stat.desc))


# Rescale, include outliers
model <- aov(conflict.total ~ sequence,
                       data = subset (conflict.rescale(data), trial_pos == 2))
anova(model)
with (subset (conflict.rescale(data), trial_pos == 2), by(conflict.0, sequence, stat.desc))






bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.0, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 0") +
  ggtitle("Simulation 14c: AB Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflict0_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.1, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 1") +
  ggtitle("Simulation 14c: BC Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14cconflict1_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.2, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 2") +
  ggtitle("Simulation 14: CA Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflict2_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.total, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Total Conflict") +
  ggtitle("Simulation 14: Total Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflicttotal_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=cycles, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "cycles") +
  ggtitle("Simulation 14: RT")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_RT_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


# Rescale, exclude outliers
model <- aov(conflict.0 ~ sequence,
                       data = subset (data, trial_pos == 2))
anova(model)
with (subset (conflict.rescale(exclude.outliers(data)), trial_pos == 2), by(conflict.0, sequence, stat.desc))





#### Conflict on all trials, not just trial 2 ###

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.0, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 0") +
  ggtitle("Simulation 14c: AB Conflict, ALL TRIALS")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflict0_bysequence_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.1, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 1") +
  ggtitle("Simulation 14c: BC Conflict, ALL TRIALS")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflict1_bysequence_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.2, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 2") +
  ggtitle("Simulation 14c: CA Conflict, ALL TRIALS")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflict2_bysequence_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.total, group=weight, fill=weight))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Total Conflict") +
  ggtitle("Simulation 14c: Total Conflict, ALL TRIALS")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14c_conflicttotal_bysequence_alltrials.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



# Rescale, include outliers, all trials
model <- aov(conflict.total ~ sequence,
                       data = data)
anova(model)
with (conflict.rescale(data), by(conflict.total, sequence, stat.desc))
