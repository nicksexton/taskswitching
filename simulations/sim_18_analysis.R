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

data.raw.inhib <- read.delim("sim_18_data_inhib.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)
data.raw.noinhib <- read.delim("sim_18_data_noinhib.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)



# now split trial path into block and trial ID
data.raw.inhib$trialpath <- as.character(data.raw.inhib$trialpath)
data.raw.noinhib$trialpath <- as.character(data.raw.noinhib$trialpath)



data.raw.inhib = transform (data.raw.inhib, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))
data.raw.noinhib = transform (data.raw.noinhib, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))

data.raw.inhib$correct.response <- with (data.raw.inhib, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw.noinhib$correct.response <- with (data.raw.noinhib, ifelse (cue == 0, stim_0,
                                                     ifelse (cue == 1, stim_1,
                                                             ifelse (cue == 2, stim_2, NA))))
data.raw.inhib$correct <- with (data.raw.inhib, correct.response == response %% 2)
data.raw.noinhib$correct <- with (data.raw.noinhib, correct.response == response %% 2)

# trim unwanted columns
data.inhib = subset(data.raw.inhib, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight"))
data.noinhib = subset(data.raw.noinhib, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight"))

data.inhib$inhibition = "Backward Inhibition"
data.noinhib$inhibition = "No Backward Inhibition"


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial.pos", "reps.allowed", "congruency_trial")
 data.lookuptable = read.delim("sim_18_lookup.txt", header = FALSE, col.names=labels.lookup)

data.inhib <- merge(data.lookuptable, data.inhib, by="trialid", sort=FALSE)
data.noinhib <- merge(data.lookuptable, data.noinhib, by="trialid", sort=FALSE)
data <- rbind (data.inhib, data.noinhib)



##### Calculate error rates ######
data.errorrates.inhib <- with (subset (data, data$inhibition == "Backward Inhibition"), table (sequence, correct))
errorrates.inhib <- data.frame(data.errorrates.inhib[,1] / (data.errorrates.inhib[,1] + data.errorrates.inhib[,2]))
errorrates.inhib <- data.frame(errorrates.inhib, c("0SW", "1SW", "2SW", "ALT", "BLK"))
names(errorrates.inhib) <- c("errors", "sequence")

data.errorrates.noinhib <- with (subset (data, data$inhibition == "No Backward Inhibition"), table (sequence, correct))
errorrates.noinhib <- data.frame(data.errorrates.noinhib[,1] / (data.errorrates.noinhib[,1] + data.errorrates.noinhib[,2]))
errorrates.noinhib <- data.frame(errorrates.noinhib, c("0SW", "1SW", "2SW", "ALT", "BLK"))
names(errorrates.noinhib) <- c("errors", "sequence")

errorrates.inhib$inhibition <- "Backward Inhibition"
errorrates.noinhib$inhibition <- "No Backward Inhibition"

errorrates <- rbind (errorrates.inhib, errorrates.noinhib)


names(errorrates) <- c("errors", "sequence", "inhibition")

#errorrates <- rbind(errorrates[5,], errorrates[1:4,])
#levels(errorrates$sequence) <- c("BLK", "0SW", "1SW", "2SW", "ALT")

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




# RTs for 1SW condition


order <- c("BLK", "0SW", "1SW", "2SW", "ALT")

bargraph <- ggplot (data, aes(x=sequence, y=cycles, group=inhibition, fill=inhibition))
bargraph +
    stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
    stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
#  labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
    scale_x_discrete (limits=order) +
        ggtitle("Simulation 1: RTs") + theme(legend.position="bottom") +
            scale_fill_grey(start = 0.3, end = 0.7)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_18", "n2rep_paper_sim0_rt.png")
ggsave(filename=imageFile, width = 120, height = 150, units = "mm")

            
                                        #todo
                                        # re-order so BLK is left-most on x-axis



error.rate.graph <- ggplot (errorrates, aes(x=sequence, y=errors, group=inhibition, fill=inhibition)) +
    scale_x_discrete (limits=order)
error.rate.graph + geom_bar (stat="identity", position="dodge") +
    ggtitle ("Simulation 1: Error rates") + theme(legend.position="bottom") +
        scale_fill_grey(start = 0.3, end = 0.7)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_18", "n2rep_paper_sim0_errors.png")
ggsave(filename=imageFile, width = 120, height = 150, units = "mm")




