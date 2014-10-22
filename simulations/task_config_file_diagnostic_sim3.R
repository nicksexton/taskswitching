rm (list = ls())


congruency.iscorrect <- function(cue, stim0, stim1, stim2, congruency) {

  return (
     switch(toString(congruency),
            "II" = {
#              print ("II matched")
              switch (toString(cue),
                      "0" = ((stim0 != stim1) & (stim1 == stim2)), 
                      "1" = ((stim1 != stim0) & (stim0 == stim2)),
                      "2" = ((stim2 != stim0) & (stim0 == stim1)))
                     },
            "IC" = {
#              print ("IC matched")
              switch (toString(cue),
                      "0" = (stim1 != stim2),
                      "1" = (stim0 != stim2),
                      "2" = (stim0 != stim1))
                   }
            )
          )
}
  


labels.trialconf = c("blockid", "trialid", "cue", "stim.0", "stim.1", "stim.2", "param1", "param2" )
# labels.lookup = c("trialid", "task.condition", "task.sequence", "position", "congruency.run", "congruency.trial")
labels.lookup = c("trialid", "task.sequence", "position", "congruency.run", "congruency.trial", "blank") 

trialconf <- read.delim("sim_3_trials.conf", header=FALSE, sep=c("", ":"), col.names=labels.trialconf) # symmetric
lookup <- read.delim("sim_3_lookup.txt", header=FALSE, sep=c("", ":"), col.names=labels.lookup) # symmetric

full <- merge(trialconf, lookup, by.x = "trialid", by.y = "trialid")

rows.total <- nrow (full)


# Check duplicate trialids
trialconf$duplicated = duplicated(subset(trialconf, select="trialid"))
table (trialconf$duplicated)
# trialconf.dupe <- subset(trialconf, duplicated == TRUE)
lookup$duplicated = duplicated(subset(lookup, select="trialid"))
table (lookup$duplicated)
#
full$duplicated = duplicated(subset(full, select="trialid"))
table (full$duplicated)


# proportion of all trials as specific congruency condition, should all be 0.125
nrow(subset(full, congruency.run == "II/II/II")) / rows.total 
nrow(subset(full, congruency.run == "IC/II/II")) / rows.total
nrow(subset(full, congruency.run == "II/IC/II")) / rows.total
nrow(subset(full, congruency.run == "IC/II/IC")) / rows.total

nrow(subset(full, congruency.run == "IC/IC/II")) / rows.total
nrow(subset(full, congruency.run == "IC/II/IC")) / rows.total
nrow(subset(full, congruency.run == "II/IC/IC")) / rows.total
nrow(subset(full, congruency.run == "IC/IC/IC")) / rows.total

full$congruency.correct <- Vectorize (congruency.iscorrect)(full$cue, full$stim.0, full$stim.1, full$stim.2, full$congruency.trial)

table (full$congruency.correct)

# just look at correct=FALSE trials
full.false <- subset (full, congruency.correct == FALSE)
full.true <- subset (full, congruency.correct == TRUE)

table (full$task.sequence)
table (full$congruency.run)

table (full.false$congruency.run, full.false$congruency.trial)
table (full.false$congruency.run, full.false$position)
# vs. total:
table (full$congruency.run, full$position)
