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

data.raw <- read.delim("sim_14_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)



# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw$correct.response <- with (data.raw, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw$correct <- with (data.raw, correct.response == response %% 2)

# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial_pos", "congruency_seq", "congruency_trial")
 data.lookuptable = read.delim("sim_14_lookup.txt", header = FALSE, col.names=labels.lookup)

data <- merge(data.lookuptable, data, by="trialid", sort=FALSE)

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

bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=cue, y=cycles, group=cue, fill=cue))
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


# Trial 3, outliers excluded, neg conflict clipped
data.clip.trial3 <- subset (conflict.clip(exclude.outliers(data)), trial_pos == 2)
scatter.clip.excl <- ggplot (data.clip.trial3, aes(conflict.0, cycles))
scatter.clip.excl + geom_point(aes(colour=sequence)) +
    ggtitle ("neg conflict clipped, outliers excluded") +
        geom_text (x=0.3, y=200, label=corr.annotation(data.clip.trial3$conflict.0, data.clip.trial3$cycles), parse=TRUE)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_excl_clip.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



# Trial 3, outliers excluded, neg conflict rescaled
data.rescale.trial3 <- subset (conflict.rescale(exclude.outliers(data)), trial_pos == 2)
scatter.rescale.excl <- ggplot (data.rescale.trial3, aes(conflict.0, cycles))
scatter.rescale.excl + geom_point(aes(colour=sequence)) + ggtitle ("neg conflict rescaled, outliers excluded") +  geom_text (x=0.6, y=220, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_excl_rescale.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


# Trial 3, outliers included, neg conflict clipped
data.clip.trial3 <- subset (conflict.clip(data), trial_pos == 2)
scatter.clip.incl <- ggplot (data.clip.trial3,
                             aes(conflict.0, cycles))
scatter.clip.incl + geom_point(aes(colour=sequence)) + ggtitle ("neg conflict clipped, outliers included") +  geom_text (x=0.3, y=400, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_clip.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

# Trial 3, outliers included, neg conflict rescaled
data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
scatter.rescale.incl <- ggplot (data.rescale.trial3,
                                aes(conflict.0, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("neg conflict rescaled, outliers included") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_rescale.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



## Looking at repeating and non-repeating sequences separately
# 
data.rescale.by.rep <- subset (conflict.rescale(data),
                               trial_pos == 2 )
data.rescale.by.rep$rep <- rep("repeat", nrow(data.rescale.by.rep))
data.rescale.by.rep$rep[data.rescale.by.rep$sequence=="2SW" |
                        data.rescale.by.rep$sequence == "ALT"] <- "no_repeat"

scatter.rescale.incl <- ggplot (data.rescale.by.rep,
                                aes(conflict.0, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) +
    ggtitle ("neg conflict rescaled, outliers included") +
        facet_grid(. ~ rep)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_rescale.png")
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
with (subset (conflict.rescale(data), trial_pos == 2), by(conflict.total, sequence, stat.desc))



bargraph <- ggplot (subset(conflict.rescale(exclude.outliers(data)), trial_pos == 2),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Total Conflict") +
  ggtitle("Simulation 14: Total Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14_total_conflict_by_sequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")




###### FOR LAB NOTEBOOK - compare 3 graphs, conflict.0, conflict.1 and conflict.2

# Trial 3, outliers included, neg conflict rescaled
data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
scatter.rescale.incl <- ggplot (data.rescale.trial3,
                                aes(conflict.0, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("AB conflict, (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE) + labs (x="Conflict 0", y = "trial 3 RT")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_rescale_conflict0.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

# Trial 3, outliers included, neg conflict rescaled
data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
scatter.rescale.incl <- ggplot (data.rescale.trial3,
                                aes(conflict.1, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("BC conflict (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE)+ labs (x="Conflict 1", y = "trial 3 RT")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_rescale_conflict1.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

# Trial 3, outliers included, neg conflict rescaled
data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
scatter.rescale.incl <- ggplot (data.rescale.trial3,
                                aes(conflict.2, cycles))
scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("CA conflict, (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE) + labs (x="Conflict 2", y = "trial 3 RT")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_incl_rescale_conflict2.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")





bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.0, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 0") +
  ggtitle("Simulation 14: AB Conflict")
imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict0_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.1, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 1") +
  ggtitle("Simulation 14: BC Conflict")
imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict1_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.2, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 2") +
  ggtitle("Simulation 14: CA Conflict")
imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict2_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (subset(conflict.rescale(data), trial_pos == 2),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Total Conflict") +
  ggtitle("Simulation 14: Total Conflict")
imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflicttotal_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=cycles, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "cycles") +
  ggtitle("Simulation 14: RT")
imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_RT_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


# Rescale, exclude outliers
model <- aov(conflict.0 ~ sequence,
                       data = subset (data, trial_pos == 2))
anova(model)
with (subset (conflict.rescale(exclude.outliers(data)), trial_pos == 2), by(conflict.0, sequence, stat.desc))



#### Look at conflict from all trials, not just trial 3


bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.0, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 0") +
  ggtitle("Simulation 14: AB Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict0_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.1, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 1") +
  ggtitle("Simulation 14: BC Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict1_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.2, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Conflict 2") +
  ggtitle("Simulation 14: CA Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflict2_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

bargraph <- ggplot (conflict.rescale(data),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "Total Conflict") +
  ggtitle("Simulation 14: Total Conflict")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_conflicttotal_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=cycles, group=sequence, fill=sequence))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Sequence", y = "cycles") +
  ggtitle("Simulation 14: RT")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_14", "sim_14b_RT_bysequence.png")
ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


# Rescale, exclude outliers
model <- aov(conflict.total ~ sequence,
                       data = subset (data))
anova(model)
with (conflict.rescale(data), by(conflict.total, sequence, stat.desc))

