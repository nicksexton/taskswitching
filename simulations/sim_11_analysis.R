# R data analysis for thesis simulation 11
# Varies the RSI of n-1 and n trials independently



rm (list = ls())
library(ggplot2) # for graphs
library(pastecs) # for descriptive statistics
library(reshape2) # for transform
library(ez) # for ezANOVA
imageDirectory <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_11") # path to save images to
                                        #(eg ~/Dropbox/PhD/Thesis/simulations/etc

labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "cycles",
           "response")


data.raw <- read.delim("sim_11_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

                                        # now split trial path into block and trial ID
data.raw$trialpath <- as.character(data.raw$trialpath)
data.raw = transform (data.raw, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))
data.raw$correct.trial <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                                  ifelse (cue == 1, stim_1 == response %% 2,
                                                          ifelse (cue == 2, stim_2 == response %% 2, NA))))

block.is.correct <- function (x) {

                                        # fastest version
                                        # see sim_6_analysis_functions.R for alternative (slower, but less hacky) versions
                                        # same as version 2 but disregards safety checks that data frame
                                        # is correctly sorted (in order of trialid)
  
  correct.block <- vector (mode="logical", length=nrow(x))
  for (i in 1:nrow(x)) {
   
    if (x[i,]$PATH.trial == 0) {
      ifelse ((x[i,]$correct.trial & x[i+1,]$correct.trial & x[i+2,]$correct.trial), correct.block[i:(i+2)] <- TRUE, correct.block[i:(i+2)] <- FALSE)
    }
  }
  x <- cbind (x, correct.block)
  return (x)
}


data.raw <- block.is.correct (data.raw)

# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "rsi_n1", "rsi_n", "blank")
data.lookuptable = read.delim("sim_11_lookup.txt", header = FALSE, col.names=labels.lookup)
data.raw <- merge(data.lookuptable, data.raw, by.x = "trialid", by.y = "trialid")
data.raw = subset(data.raw, select = c("trialid", "sequence_cond", "sequence", "PATH.block", "PATH.trial", "rsi_n1", "rsi_n", "cue", "stim_0", "stim_1", "stim_2", "response", "cycles", "correct.block", "correct.trial"))

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
data = subset (data.raw, correct.block == TRUE)

                                        #
# exclude outliers (RTs +/- 3 SDs) for each task (irrespective of trial position or switch condition)
# exclude outliers (RTs +/- 3 SDs)
descriptives <- by(data$cycles, data$cue, stat.desc)
data <- subset (data,
        !((cue == 0 ) & cycles > descriptives$"0"[9] + 3 * descriptives$"0"[13]))
data <- subset (data,
        !((PATH.trial == 1 ) & cycles > descriptives$"1"[9] + 3 * descriptives$"1"[13]))
data <- subset (data,
        !((PATH.trial == 2 ) & cycles > descriptives$"2"[9] + 3 * descriptives$"2"[13]))

data$seq.3 <- factor(data$seq.3)
data$seq.2 <- factor(data$seq.2)
data$seq.1 <- factor(data$seq.1)
#
#
#
######################### DEBUG STUFF ####################################


data$rsi <- paste (data$rsi_n1, ":", data$rsi_n)

## #================ RTs for 2SW condition
## data.2SW <- subset (data, sequence_cond == "2SW")
## by (data.2SW$cycles, data.2SW$seq.3, stat.desc)
## bargraph <- ggplot (subset(data.2SW, PATH.trial ==1), aes(x=seq.3, y=cycles, group=seq.2, fill=seq.2))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
##   ggtitle("Simulation 4: 2SW only")

## #look at middle trial
## bargraph <- ggplot (subset(data.2SW, PATH.trial == 1), aes(x=seq.2, y=cycles, group=seq.1, fill=seq.1))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Task (trial 2)", y = "RT", group = "task (trial 1)") +
##   ggtitle("Simulation 4: 2SW only, middle trial")

## # Look at initial trial (broken out by task.sequence
## bargraph <- ggplot (subset(data.2SW, PATH.trial == 0), aes(x=seq.1, y=cycles, group=sequence, fill=sequence))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Task (trial 0)", y = "RT", group = "sequence") +
##   ggtitle("Simulation 4: 2SW only, initial trial")



## #=============== RTs for ALT condition
## data.ALT <- subset (data, sequence_cond == "ALT")
## by (data.ALT$cycles, data.ALT$seq.3, stat.desc)
## bargraph <- ggplot (data.ALT, aes(x=seq.3, y=cycles, group=seq.2, fill=seq.2))
## bargraph +
##   stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
##   stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
##   labs (x = "Task (trial 3)", y = "RT", group = "task (trial 2)") +
##   ggtitle("Simulation 4: Alt Switch only")


## boxplot.task0 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 0), aes(x=sequence_cond, y=cycles))
## boxplot.task0 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 0")
## #imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task0_boxplot.png") 
## #ggsave(imageFile)

## boxplot.task1 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 1), aes(x=sequence_cond, y=cycles))
## boxplot.task1 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 1")
## #imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task1_boxplot.png") 
## #ggsave(imageFile)

## boxplot.task2 <- ggplot (subset(data, PATH.trial == 2 & seq.3 == 2), aes(x=sequence_cond, y=cycles))
## boxplot.task2 + geom_boxplot() + labs (x = "sequence", y = "RT (cycles)") + ggtitle ("Simulation 6, task 2")
## #imageFile <- file.path(imageDirectory, "sim_6d_diagnostic_task2_boxplot.png") 
## #ggsave(imageFile)


## ##########################################################################


                                        # look only at trial 3
data.trial3 <- subset (data, PATH.trial == 2)


# Plot graph for switches between tasks 0 and 1

linegraph <- ggplot (data.trial3, aes(x=sequence_cond, y=cycles, group=rsi, colour=rsi))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  stat_summary(fun.data = mean_cl_boot, geom = "errorbar") +
  labs (x = "Sequence", y = "RT", group = "RSI (preceding trial n-1:n)") +
      ggtitle("Simulation 11, varied intertrial intervals")

                                        #imageFile <- file.path(imageDirectory, "sim_11_07-15.png") 
                                        #imageFile <- file.path(imageDirectory, "sim_11_03-06.png")
                                        #imageFile <- file.path(imageDirectory, "sim_11_03-15.png")
imageFile <- file.path(imageDirectory, "sim_11_12-25.png") 

ggsave(imageFile)





######################################### Stat Tests ######################################


## ############## FOR ASYMMETRIC SWITCHING ##################


############## 0 and 1 ##################

# Switch cost - assessing whether switch costs are reversed
# 2X2 ANOVA:
# Factors: sequence_cond (levels: 0SW, 1SW only), seq.3 (levels: 0, 2)
# Hypothesis:
#    main effect of sequence cond (ie., switch cost)
#    main effect of seq.3 (ie., different RTs for different tasks)
#    seq.3 x sequence_cond interaction: signf. asymmetric switch cost


# switch cost

#n2 repetition cost
model.n2rc <- aov(cycles ~ sequence_cond +
                      rsi_n1 + rsi_n +
                      sequence_cond:rsi_n1 + sequence_cond:rsi_n +
                      sequence_cond:rsi_n1:rsi_n,                  
                  data = data.trial3)
anova(model.n2rc)

## # post-hoc test: is there a significant n-2 repetition cost in each condition
## # task 0
## model.task01.n2rc.0 <- t.test (cycles ~ sequence_cond,
##                                data = subset (data.task01, seq.3 == 0),
##                                sequence_cond == "2SW" | sequence_cond == "ALT")
## model.task01.n2rc.0

## # task 1
## model.task01.n2rc.1 <- t.test (cycles ~ sequence_cond,
##                                data = subset (data.task01, seq.3 == 1),
##                                sequence_cond == "2SW" | sequence_cond == "ALT")
## model.task01.n2rc.1



## # Test for reversed switch cost asymmetry???


