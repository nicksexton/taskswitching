rm(list = ls())

congruency.iscorrect <- function(cue, stim0, stim1, stim2, congruency) {

  return (
     switch(congruency,
            "II" = switch (toString(cue),
                   "0" = ((stim0 != stim1) & (stim1 == stim2)), 
                   "1" = ((stim1 != stim0) & (stim0 == stim2)),
                   "2" = ((stim2 != stim0) & (stim0 == stim1))),
            "IC" = switch (toString(cue),
              "0" = (stim1 != stim2),
              "1" = (stim0 != stim2),
              "2" = (stim0 != stim1))
           
            )
          )
}
  


labels.trialconf = c("blockid", "trialid", "cue", "stim.0", "stim.1", "stim.2", "param1", "param2" )
labels.lookup = c("trialid", "task.condition", "task.sequence", "position", "congruency.run", "congruency.trial") 


trialconf <- read.delim("sim_6_trials.conf", header=FALSE, sep=c("", ":"), col.names=labels.trialconf) # symmetric
lookup <- read.delim("sim_6_lookup.txt", header=FALSE, sep=c("", ":"), col.names=labels.lookup) # symmetric

full <- merge(trialconf, lookup, by.x = "trialid", by.y = "trialid")

rows.total <- nrow (full)

# proportion of all trials as specific congruency condition
nrow(subset(full, congruency.run == "II/II/II")) / rows.total

full$congruency.correct <- Vectorize (congruency.iscorrect)(full$cue, full$stim.0, full$stim.1, full$stim.2, full$congruency.trial)


table (full$congruency.correct)

table (full$task.sequence)
table (subset(full, task.condition == "0SW")$task.sequence)

table (subset(full, task.condition == "1SW")$task.sequence)

# look at one switch condition only
full.1SW.2 = subset(full,
                    ((task.condition == "1SW") & (position == "2")
                     ))

# stimulus counts
table (full.1SW.2$stim.0)
table (full.1SW.2$stim.1)
table (full.1SW.2$stim.2)

# task counts
table (full.1SW.2$cue)



# look at one task sequence only
full.taskseq.221 = subset(full, task.sequence == "2/2/1")

table (full.taskseq.221$cue)
