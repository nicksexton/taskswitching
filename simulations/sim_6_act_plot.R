# Activation plots for simulation 6
# Probably only want to use with more compact data sets

rm (list = ls())

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
activations.raw <- read.delim("sim_6_act.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)
# activations = transform (activations.raw, path = colsplit(sequence, pattern = "/", names=c('1', '2', '3')))

#data
labels.data = c("trialpath", "trialid", "cue", "stim_0", "stim_1", "stim_2", "RT",
           "response")
data.raw <- read.delim("sim_6_data.txt", header=FALSE, sep=c("", ":"), col.names=labels.data)

activations <- merge (activations.raw, data.raw[, c("trialpath", "trialid", "RT", "response")], by.x = c("path"), by.y = c("trialpath"), sort=FALSE)




# Join lookup table with simulated data
labels.lookup = c("trialid", "sequence_cond", "sequence", "trial_pos", "congruency_seq", "congruency_trial", "blank")
lookuptable = read.delim("sim_6_lookup.txt", header = FALSE, col.names=labels.lookup)

activations <- merge(activations, lookuptable, by="trialid", sort=FALSE)


