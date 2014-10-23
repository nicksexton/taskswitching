# Activation plots for simulation 6
# Probably only want to use with more compact data sets

rm (list = ls())
library (plyr) #for ddply
library (reshape2) # for melt

activations.file = "sim_6_act_small.txt"
data.file = "sim_6_data_small.txt"
lookup.file = "sim_6_lookup_small.txt"

#activations
labels.data = c(
  "path", "cycle",
  "topdown.0", "topdown.1", "topdown.2",
  "input.0.0", "input.0.1", "input.1.0", "input.1.1", "input.2.0", "input.2.1",
  "output.0.0", "output.0.1", "output.1.0", "output.1.1", "output.2.0", "output.2.1",
  "taskdemand.0", "taskdemand.1", "taskdemand.2",
  "conflict.01", "conflict.12", "conflict.02",
  "conflictinput.01", "conflictinput.12", "conflictinput.0"
  )
activations.raw <- read.delim(activations.file, header=FALSE, sep=c("", ":"), col.names=labels.data)
# activations = transform (activations.raw, path = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))

#data
labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "RT",
           "response")
data.raw <- read.delim(data.file, header=FALSE, sep=c("", ":"), col.names=labels.data)
data.raw$correct <- with (data.raw, ifelse (cue == 0, stim_0 == response %% 2,
                                    ifelse (cue == 1, stim_1 == response %% 2,
                                    ifelse (cue == 2, stim_2 == response %% 2, NA))))
#

activations <- merge (activations.raw, data.raw[, c("trialpath", "trialid", "RT", "response", "correct")], by.x = c("path"), by.y = c("trialpath"), sort=FALSE)


# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
lookuptable = read.delim(lookup.file, header = FALSE, col.names=labels.lookup)

activations <- merge(activations, lookuptable, by="trialid", sort=FALSE)




# ========================== segment trials ========================================

plotdata.subset <- subset(activations,
                          correct == TRUE &
                          sequence == "0/1/1" &
                          trial_pos == 0 &
                          response == 0
                          )


# summarise data
plotdata.summary.mean <- ddply (plotdata.subset,
                           c("cycle"),
                           summarise,
                           taskdemand.0 = mean(taskdemand.0),
                           taskdemand.1 = mean(taskdemand.1),
                           taskdemand.2 = mean(taskdemand.2),
                           layer = "Task Demand") 
plotdata.summary.sd <- ddply (plotdata.subset,
                           c("cycle"),
                           summarise,
                           taskdemand.0 = sd(taskdemand.0),
                           taskdemand.1 = sd(taskdemand.1),
                           taskdemand.2 = sd(taskdemand.2),
                           layer = "Task Demand")

plotdata.long.mean <- melt (plotdata.summary.mean,
                       id.vars = c("cycle", "layer"),
                       measure.vars = c("taskdemand.0", "taskdemand.1", "taskdemand.2"),
                       variable.name = "unit",
                       value.name = "activation")
plotdata.long.sd <- melt (plotdata.summary.sd,
                       id.vars = c("cycle", "layer"),
                       measure.vars = c("taskdemand.0", "taskdemand.1", "taskdemand.2"),
                       variable.name = "unit",
                       value.name = "sd")

plotdata <- merge (plotdata.long.mean, plotdata.long.sd, 
                   by=c("cycle", "unit", "layer"), sort=FALSE)


act.plot <- ggplot(plotdata,
                   aes(x=cycle, y=activation, colour=unit)) + geom_ribbon(aes(ymin=activation - sd, ymax = activation + sd, alpha = 0.1, fill=unit)) + geom_line()  
act.plot



