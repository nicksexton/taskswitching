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
data.inhib = subset(data.raw.inhib, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight", "PATH.block"))
data.noinhib = subset(data.raw.noinhib, select = c("trialid", "PATH.block", "PATH.trial", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct", "correct.response", "conflict.0", "conflict.1", "conflict.2", "weightchange", "weight", "PATH.block"))

data.inhib$inhibition = "Backward Inhibition"
data.noinhib$inhibition = "No Backward Inhibition"



# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence", "cue_sequence", "trial.pos", "reps.allowed", "congruency_trial")
 data.lookuptable = read.delim("sim_18_lookup.txt", header = FALSE, col.names=labels.lookup)

data.inhib <- merge(data.lookuptable, data.inhib, by="trialid", sort=FALSE)
data.noinhib <- merge(data.lookuptable, data.noinhib, by="trialid", sort=FALSE)
data <- rbind (data.inhib, data.noinhib)

                                        # trim 100 - 199: generated in error
#data <- subset(data, data$PATH.block.1 < 100)

process <- function (my.data) {

##### Calculate error rates ######
    my.data.errorrates.inhib <- with (subset (my.data, my.data$inhibition == "Backward Inhibition"), table (sequence, correct))
    if (length (my.data.errorrates.inhib) == 5) { # to manage situation where no errors made 
        my.data.errorrates.inhib <- cbind (c(0, 0, 0, 0, 0), my.data.errorrates.inhib)
    }


#    browser()
    
    errorrates.inhib <- data.frame(my.data.errorrates.inhib[,1] / (my.data.errorrates.inhib[,1] + my.data.errorrates.inhib[,2]))
    errorrates.inhib <- data.frame(errorrates.inhib, c("0SW", "1SW", "2SW", "ALT", "BLK"))
    names(errorrates.inhib) <- c("errors", "sequence")

    my.data.errorrates.noinhib <- with (subset (my.data, my.data$inhibition == "No Backward Inhibition"), table (sequence, correct))

    if (length (my.data.errorrates.noinhib) == 5) { # to manage situation where no errors made 
        my.data.errorrates.noinhib <- cbind (c(0, 0, 0, 0, 0), my.data.errorrates.noinhib)
    }
    errorrates.noinhib <- data.frame(my.data.errorrates.noinhib[,1] / (my.data.errorrates.noinhib[,1] + my.data.errorrates.noinhib[,2]))

    errorrates.noinhib <- data.frame(errorrates.noinhib, c("0SW", "1SW", "2SW", "ALT", "BLK"))
    names(errorrates.noinhib) <- c("errors", "sequence")

    errorrates.inhib$inhibition <- "Backward Inhibition"
    errorrates.noinhib$inhibition <- "No Backward Inhibition"


    errorrates <- data.frame(cbind (t(errorrates.inhib$errors), t(errorrates.noinhib$errors)))
    names (errorrates) <- c("err.0SW.inhib", "err.1SW.inhib", "err.2SW.inhib", "err.ALT.inhib", "err.BLK.inhib",
                            "err.0SW.noinhib", "err.1SW.noinhib", "err.2SW.noinhib", "err.ALT.noinhib", "err.BLK.noinhib")
    
   # inhib.errorrates <- data.frame(rbind ("inhib" = errorrates.inhib$errors, "noinhib" = errorrates.noinhib$errors))
    # names(errorrates) <- c("err.0SW", "err.1SW", "err.2SW", "err.ALT", "err.BLK")

##### Now filter trials for correct only
                                        # should filter for correct sequences only!
    my.data = subset (my.data, correct == TRUE)

    inhib <- subset (my.data, my.data$inhibition == "Backward Inhibition")
    noinhib <- subset (my.data, my.data$inhibition == "No Backward Inhibition")
    
    rt <- rbind ("inhib" = by (inhib$cycles, inhib$sequence, mean)[1:5],
           "noinhib" = by (noinhib$cycles, noinhib$sequence, mean)[1:5])

    rt <- cbind (data.frame(t(rt[1,]), t(rt[2,])))
    names (rt) <- c("0SW.inhib", "1SW.inhib", "2SW.inhib", "ALT.inhib", "BLK.inhib",
                            "0SW.noinhib", "1SW.noinhib", "2SW.noinhib", "ALT.noinhib", "BLK.noinhib")

#browser()
    
    return (cbind (rt[,1:5], errorrates[,1:5], rt[,6:10], errorrates[,6:10])) # split inhib and noinhib for easier reshaping later
    
} 

big.data <- data

big.data.aggr.1 <- process (subset(big.data, big.data$PATH.block.1 == 1))

data.aggr <- ddply (big.data, "PATH.block.1", process)



    

data.shaped.inhib <- data.aggr[,1:11] 
data.shaped.inhib$inhibition <- "Backward Inhibition"
names (data.shaped.inhib) <- c("model", "rt.0SW", "rt.1SW", "rt.2SW", "rt.ALT", "rt.BLK", "err.0SW", "err.1SW", "err.2SW", "err.ALT", "err.BLK", "inhibition")
data.shaped.noinhib <- cbind (data.aggr[,1], data.aggr[,12:21])
data.shaped.noinhib$inhibition <- "No Backward Inhibition"
names(data.shaped.noinhib) <- c("model", "rt.0SW", "rt.1SW", "rt.2SW", "rt.ALT", "rt.BLK", "err.0SW", "err.1SW", "err.2SW", "err.ALT", "err.BLK", "inhibition")
data.shaped <- rbind (data.shaped.inhib, data.shaped.noinhib)



data.shaped$rt.switch.cost <- data.shaped$rt.1SW - data.shaped$rt.0SW
data.shaped$err.switch.cost <- data.shaped$err.1SW - data.shaped$err.0SW
data.shaped$rt.n2rep.cost <- data.shaped$rt.ALT - data.shaped$rt.2SW
data.shaped$err.n2rep.cost <- data.shaped$err.ALT - data.shaped$err.2SW

## data.long <- rbind (
##     cbind(data.shaped$model, data.shaped$rt.0SW, data.shaped$inhibition, rep("rt.0SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.1SW, data.shaped$inhibition, rep("rt.1SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.1SW, data.shaped$inhibition, rep("rt.2SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.1SW, data.shaped$inhibition, rep("rt.ALT", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.1SW, data.shaped$inhibition, rep("rt.BLK", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.0SW, data.shaped$inhibition, rep("err.0SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.1SW, data.shaped$inhibition, rep("err.1SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.1SW, data.shaped$inhibition, rep("err.2SW", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.1SW, data.shaped$inhibition, rep("err.ALT", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.1SW, data.shaped$inhibition, rep("err.BLK", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.switch.cost, data.shaped$inhibition, rep("rt.switch.cost", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.switch.cost, data.shaped$inhibition, rep("err.switch.cost", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$rt.n2rep.cost, data.shaped$inhibition, rep("rt.n2rep.cost", nrow(data.shaped))),
##     cbind(data.shaped$model, data.shaped$err.n2rep.cost, data.shaped$inhibition, rep("err.n2rep.cost", nrow(data.shaped)))
## )

data.long <- rbind (
    cbind(data.shaped$model, data.shaped$rt.0SW, data.shaped$err.0SW, data.shaped$inhibition, rep("0SW", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.1SW, data.shaped$err.1SW, data.shaped$inhibition, rep("1SW", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.2SW, data.shaped$err.2SW, data.shaped$inhibition, rep("2SW", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.ALT, data.shaped$err.ALT, data.shaped$inhibition, rep("ALT", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.BLK, data.shaped$err.BLK, data.shaped$inhibition, rep("BLK", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.switch.cost, data.shaped$err.switch.cost, data.shaped$inhibition, rep("switch.cost", nrow(data.shaped))),
    cbind(data.shaped$model, data.shaped$rt.n2rep.cost, data.shaped$err.n2rep.cost, data.shaped$inhibition, rep("n2rep.cost", nrow(data.shaped)))
)

data.long <- data.frame(data.long)
names(data.long) <- c("model", "rt", "error.rate", "inhibition", "sequence")
data.long$error.rate <- as.numeric(as.character(data.long$error.rate))
data.long$rt <- as.numeric(as.character(data.long$rt))




# RTs for 1SW condition


order <- c("BLK", "0SW", "1SW", "2SW", "ALT", "switch.cost","n2rep.cost")

bargraph <- ggplot (data.long, aes(x=sequence, y=rt, group=inhibition, fill=inhibition))
bargraph +
    stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
    stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) +
    scale_x_discrete (limits=order, labels=c("BLK", "0SW", "1SW", "2SW", "ALT", "switch cost", "n-2 repetition\n cost")) +
        ggtitle("Simulation 1: RTs") + theme(legend.position="bottom") +
            scale_fill_grey(start = 0.3, end = 0.7)

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_18", "n2rep_paper_sim0_rt.png")
ggsave(filename=imageFile, width = 120, height = 150, units = "mm")




                                        #todo
                                        # re-order so BLK is left-most on x-axis



error.rate.graph <- ggplot (data.long, aes(x=sequence, y=error.rate, group=inhibition, fill=inhibition))
error.rate.graph +
    stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
    stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) +
        scale_x_discrete (limits=order, labels=c("BLK", "0SW", "1SW", "2SW", "ALT", "switch cost", "n-2 repetition\n cost")) +
            scale_y_continuous (breaks=c(0.0, 0.002, 0.004, 0.006, 0.008), labels=c("0.0%", "0.2%", "0.4%", "0.6%", "0.8%")) +
        ggtitle("Simulation 1: Error Rates") + theme(legend.position="bottom") +
            scale_fill_grey(start = 0.3, end = 0.7)


imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_18", "n2rep_paper_sim0_errors.png")
ggsave(filename=imageFile, width = 120, height = 150, units = "mm")




