# Activation plots for simulation 6
# Probably only want to use with more compact data sets

rm (list = ls())
library (plyr) #for ddply
library (reshape2) # for melt
library (ggplot2)

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
lookuptable$trial_pos <- factor (lookuptable$trial_pos)

activations <- merge(activations, lookuptable, by="trialid", sort=FALSE)

# ========================== Function to organise data for plot ================================

prepare.data <- function (plotdata.subset) {
# subset data to the trial(s) of interest only


                                        # summarise data
  plotdata.taskdemand.summary.mean <- ddply (plotdata.subset,
                                             c("cycle", "trial_pos"),
                                             summarise,
                                             taskdemand.0 = mean(taskdemand.0),
                                             taskdemand.1 = mean(taskdemand.1),
                                             taskdemand.2 = mean(taskdemand.2),
                                             layer = "2. Task Demand") 
  plotdata.taskdemand.summary.sd <- ddply (plotdata.subset,
                                           c("cycle", "trial_pos"),
                                           summarise,
                                           taskdemand.0 = sd(taskdemand.0),
                                           taskdemand.1 = sd(taskdemand.1),
                                           taskdemand.2 = sd(taskdemand.2),
                                           layer = "2. Task Demand")


  plotdata.conflict.summary.mean <- ddply (plotdata.subset,
                                           c("cycle", "trial_pos"),
                                           summarise,
                                           conflict.01 = mean(conflict.01),
                                           conflict.12 = mean(conflict.12),
                                           conflict.02 = mean(conflict.02),
                                           layer = "1. Conflict") 
  plotdata.conflict.summary.sd <- ddply (plotdata.subset,
                                         c("cycle", "trial_pos"),
                                         summarise,
                                         conflict.01 = sd(conflict.01),
                                         conflict.12 = sd(conflict.12),
                                         conflict.02 = sd(conflict.02),
                                         layer = "1. Conflict")
  
  plotdata.outputs.summary.mean <- ddply (plotdata.subset,
                                          c("cycle", "trial_pos"),
                                          summarise,
                                          output.0.0 = mean(output.0.0),
                                          output.0.1 = mean(output.0.1),
                                          output.1.0 = mean(output.1.0),
                                          output.1.1 = mean(output.1.1),
                                          output.2.0 = mean(output.2.0),
                                          output.2.1 = mean(output.2.1),
                                          layer = "3. Output") 
  plotdata.outputs.summary.sd <- ddply (plotdata.subset,
                                        c("cycle", "trial_pos"),
                                        summarise,
                                        output.0.0 = sd(output.0.0),
                                        output.0.1 = sd(output.0.1),
                                        output.1.0 = sd(output.1.0),
                                        output.1.1 = sd(output.1.1),
                                        output.2.0 = sd(output.2.0),
                                        output.2.1 = sd(output.2.1),
                                        layer = "3. Output")

  plotdata.taskdemand.long.mean <- melt (plotdata.taskdemand.summary.mean,
                                         id.vars = c("cycle", "layer", "trial_pos"),
                                         measure.vars = c("taskdemand.0", "taskdemand.1", "taskdemand.2"),
                                         variable.name = "unit",
                                         value.name = "activation")
  plotdata.taskdemand.long.sd <- melt (plotdata.taskdemand.summary.sd,
                                       id.vars = c("cycle", "layer", "trial_pos"),
                                       measure.vars = c("taskdemand.0", "taskdemand.1", "taskdemand.2"),
                                       variable.name = "unit",
                                       value.name = "sd")
  
  plotdata.conflict.long.mean <- melt (plotdata.conflict.summary.mean,
                                       id.vars = c("cycle", "layer", "trial_pos"),
                                       measure.vars = c("conflict.01", "conflict.12", "conflict.02"),
                                       variable.name = "unit",
                                       value.name = "activation")
  plotdata.conflict.long.sd <- melt (plotdata.conflict.summary.sd,
                                     id.vars = c("cycle", "layer", "trial_pos"),
                                     measure.vars = c("conflict.01", "conflict.12", "conflict.02"),                  
                                     variable.name = "unit",
                                     value.name = "sd")

  plotdata.outputs.long.mean <- melt (plotdata.outputs.summary.mean,
                                      id.vars = c("cycle", "layer", "trial_pos"),
                                      measure.vars = c("output.0.0", "output.0.1",
                                        "output.1.0", "output.1.1",
                                        "output.2.0", "output.2.1"),
                                      variable.name = "unit",
                                      value.name = "activation")
  plotdata.outputs.long.sd <- melt (plotdata.outputs.summary.sd,
                                    id.vars = c("cycle", "layer", "trial_pos"),
                                    measure.vars = c("output.0.0", "output.0.1",
                                      "output.1.0", "output.1.1",
                                      "output.2.0", "output.2.1"),
                                    variable.name = "unit",
                                    value.name = "sd")


  plotdata.long.mean <- rbind (plotdata.taskdemand.long.mean,
                               plotdata.conflict.long.mean,
                               plotdata.outputs.long.mean)
  plotdata.long.sd <- rbind (plotdata.taskdemand.long.sd,
                             plotdata.conflict.long.sd,
                             plotdata.outputs.long.sd)


  plotdata <- merge (plotdata.long.mean, plotdata.long.sd, 
                     by=c("cycle", "unit", "layer", "trial_pos"), sort=FALSE)

#  rbind (df1, df2, df3, df4) #??
  
  return (plotdata)

}


# ========================== segment trials ========================================


  unit <- c("output.0.0", "output.1.0", "output.2.0")
  task <- as.factor(c("0.0", "1.0", "2.0"))
  df1 <- data.frame(unit, task)
  #
  unit <- c("output.0.1", "output.1.1", "output.2.1")
task <- as.factor(c("0.1", "1.1", "2.1"))
df2 <- data.frame(unit, task)

#
  unit <- c("taskdemand.0", "taskdemand.1", "taskdemand.2")
  task <- as.factor(c(0, 1, 2))
df3 <- data.frame(unit, task)
#
  unit <- c("conflict.01", "conflict.12", "conflict.02")
  task <- as.factor(c("0-1", "1-2", "0-2"))
colours <- c("yellow", "cyan", "magenta")
  df4 <- data.frame(unit, task)
  
unit.colours <- rbind (df1, df2, df3, df4)

colours.scale <- c("#CC6666", # light red
                   "#66BB66", # light green
                   "#6666CC", # light blue
                   "#770000", # dark red
                   "#007700", # dark green
                   "#000077", # dark blue
                   "#FF0000", # red
                   "#00BB00", # green
                   "#0000FF", # blue
                   "#BBBB00", # yellow
                   "#00BBBB", # cyan
                   "#BB00BB") # magenta


plotdata.raw <- subset(activations,
                   correct == TRUE &
                   sequence == "2/0/2" &
                   # trial_pos == 0 &
                   (response %% 2) == 0
                   )

plotdata <- prepare.data (plotdata.raw)
plotdata <- merge (plotdata, unit.colours, by = "unit")

act.plot <- ggplot(plotdata,
                   aes(x=cycle, y=activation, colour=task)) +
#  geom_ribbon(aes(ymin=activation - sd, ymax = activation + sd, alpha = 0.01, fill=task)) +
  geom_line() +
  scale_fill_manual(values=colours.scale) +
  scale_colour_manual(values=colours.scale)
#  scale_fill_manual(values=c("red", "green", "blue", "yellow", "cyan", "magenta")) +
#  scale_colour_manual(values=c("red", "green", "blue", "yellow", "cyan", "magenta"))
#
act.plot + facet_grid (layer ~ trial_pos) 


                                                                                                                                          

