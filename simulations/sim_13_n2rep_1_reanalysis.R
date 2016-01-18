                                        # was: sim_6e_gridsearch_plot_cogsci2015sim1.R
rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
#setwd("~/Thesis/data/simulation_6e")
library (ggplot2)



### Averaging across multiple runs, mung the data sets together
#cols.to.avg = c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", "mean.1SW", "mean.2SW", "mean.ALT")


my.cols <- c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", 
"mean.1SW", "scsc.t", "sc.df", "sc.p", "mean.2SW", "mean.ALT", 
"n2rc", "n2rc.t", "n2rc.df", "n2rc.p", "err.12.0SW", "err.12.1SW", "err.12.2SW", 
"err.12.ALT", "timeout.12.0SW", "timeout.12.1SW", "timeout.12.2SW", "timeout.12.ALT",
"err.3.0SW", "err.3.1SW", "err.3.2SW", "err.3.ALT", "timeout.3.0SW", 
"timeout.3.1SW", "timeout.3.2SW", "timeout.3.ALT")

# data.raw = read.delim("n2rep_paper_simulation_1_results.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE) 
#### WHY IS THIS DATA SCREWY?????
# answer- this data file was created by the bugged (!) version of simulation1, use this instead:

# CLIP Low noise version 3
data.raw = read.delim("sim_13_modelv1_gridsearch.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


# Default settings: bias -10.0, gain 75.0


# Just look at overall RT graph for now

## data.sim13b.rt <- subset (data.raw, data.raw$conflict.bias== -11.429 & (data.raw$conflict.gain== 71.429 | data.raw$conflict.gain== 64.286 | data.raw$conflict.gain== 78.571)) 
data.sim13b.rt <- data.raw

                                        #
data.sim13b.0SW <- subset (data.sim13b.rt, select=c("conflict.tdwt", "conflict.bias", "conflict.gain", "mean.0SW"))
names (data.sim13b.0SW) <- c("conflict.tdwt", "conflict.bias", "conflict.gain", "RT")
data.sim13b.0SW$dv <- rep("0SW", nrow(data.sim13b.0SW))
#
data.sim13b.1SW <- subset (data.sim13b.rt, select=c("conflict.tdwt", "conflict.bias", "conflict.gain", "mean.1SW"))
names (data.sim13b.1SW) <- c("conflict.tdwt", "conflict.bias", "conflict.gain", "RT")
data.sim13b.1SW$dv <- rep("1SW", nrow(data.sim13b.1SW))
#
data.sim13b.2SW <- subset (data.sim13b.rt, select=c("conflict.tdwt", "conflict.bias", "conflict.gain", "mean.2SW"))
names (data.sim13b.2SW) <- c("conflict.tdwt", "conflict.bias", "conflict.gain", "RT")
data.sim13b.2SW$dv <- rep("2SW", nrow(data.sim13b.2SW))
#
data.sim13b.ALT <- subset (data.sim13b.rt, select=c("conflict.tdwt", "conflict.bias", "conflict.gain", "mean.ALT"))
names (data.sim13b.ALT) <- c("conflict.tdwt", "conflict.bias", "conflict.gain", "RT")
data.sim13b.ALT$dv <- rep("ALT", nrow(data.sim13b.ALT))
                                        #

data.sim13b.rt.repetitions <- rbind (data.sim13b.0SW, data.sim13b.1SW)
data.sim13b.rt.repetitions$repetitions <- rep("repetitions", nrow(data.sim13b.rt.repetitions))

data.sim13b.rt.norepetitions <- rbind (data.sim13b.2SW, data.sim13b.ALT)
data.sim13b.rt.norepetitions$repetitions<- rep("norepetitions", nrow(data.sim13b.rt.norepetitions))


# Now plot RTs per sequence
# Just for single bias & gain values:
                                        #data.sim13b.rt.repetitions.single.gain <- subset (data.sim13b.rt.repetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 78.571)

data.sim13b.comparerepetitions <- rbind (data.sim13b.rt.repetitions, data.sim13b.rt.norepetitions)


data.sim13b.comparerepetitions.gain79 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 78.571)


############### OVERALL RTS ##############################
# RTs overall for repeats (1SW, 0SW) and nonrepeats (2SW, ALT)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain79, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 79")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13_rts_aggregate.png") 
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")

############## RT BY CONDITION (single gain value) #########
linegraph <- ggplot (data.sim13b.comparerepetitions.gain79, aes(x=conflict.tdwt, y=RT, group=dv, colour=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  labs (x = "Inhibitory Weight", y = "RT", group = "DV") +
  ggtitle("basic RT, Data from simulation 7, n2rep paper simulation 1")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_rts_comparerepetitions.png")
ggsave(filename=imageFile, width = 400, height = 100, units = "mm")



#### NOW INVESTIGATE MULTIPLE GAIN VALUES ####
data.sim13b.comparerepetitions.gain86 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 85.714)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain86, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 86")



data.sim13b.comparerepetitions.gain71 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 71.429)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain71, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 71")

data.sim13b.comparerepetitions.gain64 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 64.286)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain64, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 64")



################# Overall RTs for three different gain values ##############

# Now plot RTs per sequence
                                        # Just for single bias & gain values:
data.sim13b.comparegain <- rbind (data.sim13b.0SW, data.sim13b.1SW, data.sim13b.2SW, data.sim13b.ALT)

data.sim13b.comparegain   <- subset (data.sim13b.comparegain, data.sim13b.comparegain$conflict.bias== -11.429& data.sim13b.comparegain$conflict.gain > 50) 


linegraph <- ggplot (data.sim13b.comparegain, aes(x=conflict.tdwt, y=RT, group=conflict.gain, colour=conflict.gain))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  labs (x = "Inhibitory Weight", y = "RT", group = "Gain") +
  ggtitle("basic RT, Data from simulation 7, n2rep paper simulation 1")
