
# was: sim_7a_grid_gridsearch_plot_cogsci2015sim2.R

rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
library (ggplot2)


source ("gridsearch_plot_code.R")
source ("gridsearch_plot_relative_cogsci2015sim2.R")

### Averaging across multiple runs, mung the data sets together
#cols.to.avg = c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", "mean.1SW", "mean.2SW", "mean.ALT")

# Sim 8 (scaling conflict td weights by TD control strength)
                                        # data.sim8 <- read.delim("sim_8a_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.sim8 <- read.delim("n2rep_paper_results_sim2.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


data.sim8.ref <- data.sim8[data.sim8$conflict.tdwt==0,] # get reference data
                                        # trim data to only 4 weight levels (for fitting on single grid row)
#data.sim8 <- subset (data.sim8, (conflict.tdwt == -0.1786 | conflict.tdwt == -0.8929 | conflict.tdwt == -1.6071 | conflict.tdwt == -2.3214))


# Sim 13 - do we even need to do this??
# Code could be vectorised!
data.sim8$intersect <- rep(0, nrow(data.sim8))
data.sim8$intersectErr <- rep(0, nrow(data.sim8))
progress <- txtProgressBar (min=0, max=nrow(data.sim8), style=3)
for (i in 1:nrow(data.sim8)) {
  data.sim8[i,]$intersect <- intersect (data.sim8[i,]$sc, data.sim8[i,]$n2rc)
  data.sim8[i,]$intersectErr <- intersectErr ((data.sim8[i,]$err.3.1SW - data.sim8[i,]$err.3.0SW), (data.sim8[i,]$err.3.ALT - data.sim8[i,]$err.3.2SW))
  setTxtProgressBar(progress, i)  
}


# Filter for just symmetric switches
data.sim13 <- subset (data.sim8, data.sim8$task.input.str == 3 & data.sim8$task.topdown.str == 12)

# Data set for looking at sc and n2rc varying with confl-td weight
data.sim13.sc <- subset (data.sim13, select=c("conflict.tdwt", "alternation", "sc"))
names (data.sim13.sc) <- c("conflict.tdwt", "alternation", "cost")
data.sim13.sc$dv <- rep("sc", nrow(data.sim13.sc))
data.sim13.n2rc <- subset (data.sim13, select=c("conflict.tdwt", "alternation", "n2rc"))
names (data.sim13.n2rc) <- c("conflict.tdwt", "alternation", "cost")
data.sim13.n2rc$dv <- rep("n2rc", nrow(data.sim13.sc))
data.sim13 <- rbind (data.sim13.sc, data.sim13.n2rc)

# Now plot switch costs and n-2 rep costs
linegraph <- ggplot (data.sim13, aes(x=conflict.tdwt, y=cost, group=dv, fill=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("Data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_reanalysis.png") 
ggsave(imageFile)



# Question - what else is happening when confl-td weight is altered? is there a general effect e.g. on response times?

data.sim13.rt <- subset (data.sim8, data.sim8$task.input.str == 3 & data.sim8$task.topdown.str == 12)
#
data.sim13.0SW <- subset (data.sim13.rt, select=c("conflict.tdwt", "alternation", "mean.0SW"))
names (data.sim13.0SW) <- c("conflict.tdwt", "alternation", "RT")
data.sim13.0SW$dv <- rep("0SW", nrow(data.sim13.0SW))
#
data.sim13.1SW <- subset (data.sim13.rt, select=c("conflict.tdwt", "alternation", "mean.1SW"))
names (data.sim13.1SW) <- c("conflict.tdwt", "alternation", "RT")
data.sim13.1SW$dv <- rep("1SW", nrow(data.sim13.1SW))
#
data.sim13.2SW <- subset (data.sim13.rt, select=c("conflict.tdwt", "alternation", "mean.2SW"))
names (data.sim13.2SW) <- c("conflict.tdwt", "alternation", "RT")
data.sim13.2SW$dv <- rep("2SW", nrow(data.sim13.2SW))
#
data.sim13.ALT <- subset (data.sim13.rt, select=c("conflict.tdwt", "alternation", "mean.ALT"))
names (data.sim13.ALT) <- c("conflict.tdwt", "alternation", "RT")
data.sim13.ALT$dv <- rep("ALT", nrow(data.sim13.ALT))
#
data.sim13.rt <- rbind (data.sim13.0SW, data.sim13.1SW, data.sim13.2SW, data.sim13.ALT)


# Now plot RTs per sequence
linegraph <- ggplot (data.sim13.rt, aes(x=conflict.tdwt, y=RT, group=dv, colour=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("basic RT, Data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_rts_all.png")
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")


############### OVERALL RTS ##############################
# RTs overall (all 4 conditions)
linegraph <- ggplot (data.sim13.rt, aes(x=conflict.tdwt, y=RT))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("aggregate RT, Data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_rts_aggregate.png") 
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")


############### NO REPEATS ##############################
# gross simulation of data - RTs when 0SW, 1SW are ommitted 
data.sim13.norepeats <- rbind (data.sim13.2SW, data.sim13.ALT)
                                        # RTs no repeats
linegraph <- ggplot (data.sim13.norepeats, aes(x=conflict.tdwt, y=RT))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("aggregate RT, no repeats (2SW and ALT only) Data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_rts_aggregate_norepeats.png") 
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")


################ DOUBLE REPEATS ##########################
# gross simulation of data - RTs when 0SW, 1SW are doubled 
data.sim13.doublerepeats <- rbind (data.sim13.0SW, data.sim13.1SW, data.sim13.0SW, data.sim13.1SW, data.sim13.2SW, data.sim13.ALT)
                                        # RTs double repeats
linegraph <- ggplot (data.sim13.doublerepeats, aes(x=conflict.tdwt, y=RT))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("aggregate RT, double repeats (2SW and ALT only) Data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_rts_aggregate_doublerepeats.png") 
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")


####################### ANALYSIS OF ERRORS #####################

# Filter for just symmetric switches
err.sim13 <- subset (data.sim8, data.sim8$task.input.str == 3 & data.sim8$task.topdown.str == 12)
err.sim13$error.sc <- err.sim13$err.3.1SW - err.sim13$err.3.0SW
err.sim13$error.n2rc <- err.sim13$err.3.ALT - err.sim13$err.3.2SW


# Err set for looking at sc and n2rc varying with confl-td weight
err.sim13.sc <- subset (err.sim13, select=c("conflict.tdwt", "alternation", "error.sc"))
names (err.sim13.sc) <- c("conflict.tdwt", "alternation", "cost")
err.sim13.sc$dv <- rep("error sc", nrow(err.sim13.sc))
err.sim13.n2rc <- subset (err.sim13, select=c("conflict.tdwt", "alternation", "error.n2rc"))
names (err.sim13.n2rc) <- c("conflict.tdwt", "alternation", "cost")
err.sim13.n2rc$dv <- rep("error n2rc", nrow(err.sim13.sc))
err.sim13 <- rbind (err.sim13.sc, err.sim13.n2rc)


# Now plot switch costs and n-2 rep costs
linegraph <- ggplot (err.sim13, aes(x=conflict.tdwt, y=cost, group=dv, fill=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
#  stat_summary(fun.err = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("Err from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_errors.png") 
ggsave(imageFile)


# Error rates per sequence
# (Question - what else is happening when confl-td weight is altered? is there a general effect e.g. on response times?)

err.sim13.rt <- subset (data.sim8, data.sim8$task.input.str == 3 & data.sim8$task.topdown.str == 12)
#
err.sim13.0SW <- subset (err.sim13.rt, select=c("conflict.tdwt", "alternation", "err.3.0SW"))
names (err.sim13.0SW) <- c("conflict.tdwt", "alternation", "RT")
err.sim13.0SW$dv <- rep("0SW", nrow(err.sim13.0SW))
#
err.sim13.1SW <- subset (err.sim13.rt, select=c("conflict.tdwt", "alternation", "err.3.1SW"))
names (err.sim13.1SW) <- c("conflict.tdwt", "alternation", "RT")
err.sim13.1SW$dv <- rep("1SW", nrow(err.sim13.1SW))
#
err.sim13.2SW <- subset (err.sim13.rt, select=c("conflict.tdwt", "alternation", "err.3.2SW"))
names (err.sim13.2SW) <- c("conflict.tdwt", "alternation", "RT")
err.sim13.2SW$dv <- rep("2SW", nrow(err.sim13.2SW))
#
err.sim13.ALT <- subset (err.sim13.rt, select=c("conflict.tdwt", "alternation", "err.3.ALT"))
names (err.sim13.ALT) <- c("conflict.tdwt", "alternation", "RT")
err.sim13.ALT$dv <- rep("ALT", nrow(err.sim13.ALT))
#
err.sim13.rt <- rbind (err.sim13.0SW, err.sim13.1SW, err.sim13.2SW, err.sim13.ALT)



linegraph <- ggplot (err.sim13.rt, aes(x=conflict.tdwt, y=RT, group=dv, colour=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.err = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost (Error rate)", group = "DV") +
  ggtitle("Error rates, data from simulation 8, n2rep paper simulation 2")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_err_all.png")
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")
