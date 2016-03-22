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
library (plyr)
                                        # library(lsr) # for etaSquared

imageDirectory <- file.path(Sys.getenv("HOME"), "Dropbox", "PhD", "Thesis", "simulation_results", "simulation_16")

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight")

data.raw <- read.delim("sim_16_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)



# now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw$correct.response <- with (data.raw, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw$correct <- with (data.raw, correct.response == response %% 2)

# trim unwanted columns
data = subset(data.raw, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight"))


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial.pos", "reps.allowed", "congruency_trial")
 data.lookuptable = read.delim("sim_16_lookup.txt", header = FALSE, col.names=labels.lookup)

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




                                        # epochs <- c(0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000)
n.trials <- max (data$trial.pos)
epochs <- round(n.trials * c(0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
                             0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0), 0)

data$epoch <- .bincode (data$trial.pos, breaks = epochs, FALSE) 

data.aggr <- aggregate (data, list (block = data$PATH.block, seq = data$sequence, epoch=data$epoch, rep=data$reps.allowed), mean)

## rt.by.epoch <- ggplot (data.aggr,  aes(x=epoch, y=cycles, group=seq, colour=seq))
## rt.by.epoch +
##     stat_summary(fun.y = mean, geom = "line", position = "dodge") +
##     stat_summary(fun.y = mean, geom = "point") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) + 
##       facet_grid(. ~ rep) +
##       labs (x = "Epoch (100 trials)", y = "Cycles") +
##       ggtitle("Simulation 16: strategic adaptation model, RT by epoch") 


data.w <- subset (data.aggr, select=c("block", "seq", "epoch", "rep", "cycles"))
data.w <- reshape(data.w,
                  timevar = "seq",
                  idvar=c("block", "rep", "epoch"), 
                  direction="wide")

data.w$cost.switch <- data.w$cycles.1SW - data.w$cycles.0SW
data.w$cost.n2rep <- data.w$cycles.ALT - data.w$cycles.2SW

data.cost <- reshape (data.w,
                      v.names="cycles",
                      varying=c("cycles.2SW","cycles.ALT", "cycles.0SW", "cycles.1SW", "cost.switch", "cost.n2rep"),
                      timevar="DV",
                      times=c("cycles.2SW","cycles.ALT", "cycles.0SW", "cycles.1SW", "cost.switch", "cost.n2rep"),
                      direction="long")

cost.by.epoch <- ggplot (data.cost,  aes(x=epoch, y=cycles, group=DV, colour=DV))
cost.by.epoch +
    stat_summary(fun.y = mean, geom = "line", position = "dodge") +
    stat_summary(fun.y = mean, geom = "point") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) + 
      facet_grid(. ~ rep) +
      labs (x = paste("Epoch (", epochs[1], ") trials"), y = "Cycles") +
      ggtitle("Simulation 16: strategic adaptation model, RT by epoch") 

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_16", "sim_16e_cost_by_epoch.png")
##  ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



weight.by.epoch <- ggplot (data.aggr,  aes(x=epoch, y=weight, group=rep, colour=rep))
weight.by.epoch +
    stat_summary(fun.y = mean, geom = "line", position = "dodge") +
    stat_summary(fun.y = mean, geom = "point") +
    stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) + 
      labs (x = paste("Epoch (", epochs[1], ") trials"), y = "Weight") +
      ggtitle("Simulation 16: strategic adaptation model, Weight by epoch") 

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_16", "sim_16e_weight_by_epoch.png")
##  ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


weight.by.epoch <- ggplot (data.aggr,  aes(x=epoch, y=weight, group=block, colour=block))
weight.by.epoch +
    stat_summary(fun.y = mean, geom = "line", position = "dodge") +
    stat_summary(fun.y = mean, geom = "point") +
        stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) +
            facet_grid(. ~ rep) +
      labs (x = paste("Epoch (", epochs[1], ") trials"), y = "Cycles") +
          ggtitle("Simulation 16: strategic adaptation model, Weight by epoch")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_16", "sim_16e_weight_by_epoch_individual.png")
##  ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



conflict.by.epoch <- ggplot (data.aggr,  aes(x=epoch, y=(conflict.0 + conflict.1 + conflict.2)))
conflict.by.epoch +
    stat_summary(fun.y = mean, geom = "line", position = "dodge") +
    stat_summary(fun.y = mean, geom = "point") +
        stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) +
            facet_grid(. ~ rep) +
      labs (x = paste("Epoch (", epochs[1], ") trials"), y = "Conflict") +
          ggtitle("Simulation 16: strategic adaptation model, Conflict by epoch")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_16", "sim_16e_conflict_by_epoch.png")
##  ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



conflict.by.epoch <- ggplot (data.aggr,  aes(x=epoch, y=(conflict.0 + conflict.1 + conflict.2), group=block, colour=block))
conflict.by.epoch +
    stat_summary(fun.y = mean, geom = "line", position = "dodge") +
    stat_summary(fun.y = mean, geom = "point") +
        stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.2), width = 0.2) +
            facet_grid(. ~ rep) +
      labs (x = paste("Epoch (", epochs[1], ") trials"), y = "Conflict") +
          ggtitle("Simulation 16: strategic adaptation model, Conflict by epoch")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_16", "sim_16e_conflict_by_epoch_individual.png")
##  ggsave(filename=imageFile, width = 200, height = 200, units = "mm")



scatter.excl <- ggplot (data, aes(conflict.0 + conflict.1 + conflict.2, cycles))
scatter.excl + geom_point(aes(colour=sequence)) +
            facet_grid(. ~ reps.allowed) +
                ggtitle ("RT vs. cumulative conflict") 


### function for testing learning algorithm

squashing.function <- function (x) {  (5/(1+exp(-0.5 * (x) - 1)) - 4) }

weight.increment <- function (cum.conflict.last, cum.conflict.this, weight.change.last, learning.rate) {


    magn.weight.change <- learning.rate * squashing.function (cum.conflict.this - cum.conflict.last)
                                        # returns small positive if conflict goes up
                                        # returns large negative if conflict goes down
    if (weight.change.last > 0) {  weight.change <- magn.weight.change * -1 }
    else {weight.change <- magn.weight.change}
        
    return (weight.change)

}

x <- seq (-10, 10, 0.2)
y <- squashing.function (x)
plot (x, y)


##                                         # Filter negative conflict (replace with zero)

## conflict.calc.total <- function (data) {
##     data$conflict.total <- data$conflict.0 + data$conflict.1 + data$conflict.2
##     return (data)
## }


## # debug graph: for symmetric task difficulty, expect RTs to be equal

## bargraph <- ggplot (subset(conflict.calc.total(data), trial_pos == 2),  aes(x=cue, y=cycles, group=cue, fill=cue))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Task (trial 3)", y = "RT") +
##   ggtitle("Simulation 15: overall trial 3 RTs (debug)")



## corr.annotation <- function(x,y, digits = 2) {
##     c <- round(cor(x, y), digits = digits)
##     paste("italic(r) == ", c)
## }


##    # For simulation 14b, only look at conflict affecting task 0


## # Trial 3, outliers excluded, neg conflict clipped
## data.trial3 <- subset (exclude.outliers(data), trial_pos == 2)
## scatter.excl <- ggplot (data.trial3, aes(conflict.0, cycles))
## scatter.excl + geom_point(aes(colour=sequence)) +
##     ggtitle ("outliers excluded") +
##         geom_text (x=0.3, y=200, label=corr.annotation(data.trial3$conflict.0, data.trial3$cycles), parse=TRUE)

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_scatter_excl.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


## # Trial 3, outliers excluded, neg conflict clipped
## data.trial3.incl <- subset (data, trial_pos == 2)
## scatter.incl <- ggplot (data.trial3.incl, aes(conflict.0, cycles))
## scatter.incl + geom_point(aes(colour=sequence)) +
##     ggtitle ("outliers included") +
##         geom_text (x=0.3, y=200, label=corr.annotation(data.trial3$conflict.0, data.trial3$cycles), parse=TRUE)

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_scatter_incl.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")





## ## Looking at repeating and non-repeating sequences separately
## # 
## data.by.rep <- subset (conflict.calc.total(data),
##                        trial_pos == 2 )
## data.by.rep$rep <- rep("repeat", nrow(data.by.rep))
## data.by.rep$rep[data.by.rep$sequence=="2SW" |
##                     data.by.rep$sequence == "ALT"] <- "no_repeat"

## scatter.incl <- ggplot (data.by.rep,
##                         aes(conflict.total, cycles))
## scatter.incl + geom_point(aes(colour=sequence)) +
##     ggtitle ("outliers included") +
##         facet_grid(. ~ rep)


## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_scatter_byrep.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")







##                                         # Is there an association between RT and conflict?


## # include outliers
## model <- aov(conflict.total ~ sequence,
##                        data = subset (conflict.calc.total(data), trial_pos == 2))
## anova(model)
## with (subset (conflict.calc.total(data), trial_pos == 2), by(conflict.total, sequence, stat.desc))



## bargraph <- ggplot (subset(conflict.calc.total(data), trial_pos == 2),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Total Conflict") +
##   ggtitle("Simulation 15: Total Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_total_conflict_by_sequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")




## ###### FOR LAB NOTEBOOK - compare 3 graphs, conflict.0, conflict.1 and conflict.2

## ## # Trial 3, outliers included, neg conflict rescaled
## ## data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
## ## scatter.rescale.incl <- ggplot (data.rescale.trial3,
## ##                                 aes(conflict.0, cycles))
## ## scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("AB conflict, (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE) + labs (x="Conflict 0", y = "trial 3 RT")

## ## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_incl_rescale_conflict0.png")
## ## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## ## # Trial 3, outliers included, neg conflict rescaled
## ## data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
## ## scatter.rescale.incl <- ggplot (data.rescale.trial3,
## ##                                 aes(conflict.1, cycles))
## ## scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("BC conflict (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE)+ labs (x="Conflict 1", y = "trial 3 RT")

## ## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_incl_rescale_conflict1.png")
## ## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## ## # Trial 3, outliers included, neg conflict rescaled
## ## data.rescale.trial3 <- subset (conflict.rescale(data), trial_pos == 2)
## ## scatter.rescale.incl <- ggplot (data.rescale.trial3,
## ##                                 aes(conflict.2, cycles))
## ## scatter.rescale.incl + geom_point(aes(colour=sequence)) + ggtitle ("CA conflict, (rescaled, outliers included)") +  geom_text (x=0.6, y=200, label=corr.annotation(data.rescale.trial3$conflict.0, data.rescale.trial3$cycles), parse=TRUE) + labs (x="Conflict 2", y = "trial 3 RT")

## ## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_incl_rescale_conflict2.png")
## ## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")





## bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=conflict.0, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 0") +
##   ggtitle("Simulation 15: AB Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_conflict0_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=conflict.1, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 1") +
##       ggtitle("Simulation 15: BC Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_conflict1_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=conflict.2, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 2") +
##   ggtitle("Simulation 15: CA Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_conflict2_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (subset(conflict.calc.total(data), trial_pos == 2),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Total Conflict") +
##       ggtitle("Simulation 15: Total Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_conflicttotal_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


## bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=cycles, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "cycles") +
##   ggtitle("Simulation 15: RT")
## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_RT_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")




## #### Look at conflict from all trials, not just trial 3


## bargraph <- ggplot (conflict.calc.total(data),  aes(x=sequence, y=conflict.0, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 0") +
##   ggtitle("Simulation 15: AB Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_conflict0_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (conflict.calc.total(data),  aes(x=sequence, y=conflict.1, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 1") +
##   ggtitle("Simulation 15: BC Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_conflict1_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (conflict.calc.total(data),  aes(x=sequence, y=conflict.2, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Conflict 2") +
##   ggtitle("Simulation 15: CA Conflict")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_14b_conflict2_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")

## bargraph <- ggplot (conflict.calc.total(data),  aes(x=sequence, y=conflict.total, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "Total Conflict") +
##   ggtitle("Simulation 15: Total Conflict, all trials")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_conflicttotal_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


## bargraph <- ggplot (subset(data, trial_pos == 2),  aes(x=sequence, y=cycles, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Sequence", y = "cycles") +
##   ggtitle("Simulation 15: RT")

## imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_15", "sim_15_RT_bysequence.png")
## ggsave(filename=imageFile, width = 200, height = 200, units = "mm")


## # Rescale, exclude outliers
## model <- aov(conflict.total ~ sequence,
##                        data = subset (conflict.calc.total(data)))
## anova(model)
## with (conflict.calc.total(data), by(conflict.total, sequence, stat.desc))

