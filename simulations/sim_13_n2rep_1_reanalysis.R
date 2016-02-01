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

# block size 200 
data.raw = read.delim("sim_13_modelv1_gridsearch.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
# large version block size 500
# data.raw = read.delim("sim_13_modelv1_gridsearch_500.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


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


#data.sim13b.comparerepetitions.gain79 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -8.571 & data.raw$conflict.gain== 78.571)
data.sim13b.comparerepetitions.gain77 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -9 & data.raw$conflict.gain== 77)

############### Switch costs vs. n2rep costs ############

data.sim13b.gain77.bias9 <- subset (data.raw, data.raw$conflict.gain == 77 & data.raw$conflict.bias == -9)

# Data set for looking at sc and n2rc varying with confl-td weight
data.sim13b.sc <- subset (data.sim13b.gain77.bias9, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "sc"))
names (data.sim13b.sc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias", "cost")
data.sim13b.sc$dv <- rep("sc", nrow(data.sim13b.sc))
data.sim13b.n2rc <- subset (data.sim13b.gain77.bias9, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "n2rc"))
names (data.sim13b.n2rc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias",  "cost")
data.sim13b.n2rc$dv <- rep("n2rc", nrow(data.sim13b.n2rc))
data.sim13b <- rbind (data.sim13b.sc, data.sim13b.n2rc)

# Now plot switch costs and n-2 rep costs
bargraph <- ggplot (data.sim13b, aes(x=conflict.tdwt, y=cost, group=dv, fill=dv))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "Cost", group = "DV") +
  ggtitle("Sim13b, default bias & gain, effect of TDWT on switch & n2rep costs")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_varyweight_costs.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")






############### OVERALL RTS ##############################
# RTs overall for repeats (1SW, 0SW) and nonrepeats (2SW, ALT)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain77, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 77 bias 9")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_rts_aggregate.png") 
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")

############## RT BY CONDITION (single gain value) #########
linegraph <- ggplot (data.sim13b.comparerepetitions.gain77, aes(x=conflict.tdwt, y=RT, group=dv, colour=dv))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  labs (x = "Inhibitory Weight", y = "RT", group = "DV") +
  ggtitle("RTs individually by condition, gain 77 bias 9")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_rts_all.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")



#### NOW INVESTIGATE MULTIPLE GAIN VALUES ####
data.sim13b.comparerepetitions.gain84 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -9 & data.raw$conflict.gain== 84)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain84, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 84")


data.sim13b.comparerepetitions.gain70 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -9 & data.raw$conflict.gain== 70)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain70, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 70")


data.sim13b.comparerepetitions.gain63 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -9 & data.raw$conflict.gain== 63)
linegraph <- ggplot (data.sim13b.comparerepetitions.gain63, aes(x=conflict.tdwt, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Inhibitory Weight", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), gain 63")






################# Overall RTs for three different gain values ##############

# Now plot RTs per sequence
                                        # Just for single bias & gain values:
#data.sim13b.comparegain <- rbind (data.sim13b.0SW, data.sim13b.1SW, data.sim13b.2SW, data.sim13b.ALT)
data.sim13b.comparegain <- rbind (data.sim13b.2SW, data.sim13b.ALT)
data.sim13b.comparegain   <- subset (data.sim13b.comparegain, data.sim13b.comparegain$conflict.bias== -12 & data.sim13b.comparegain$conflict.gain > 50) 


linegraph <- ggplot (data.sim13b.comparegain, aes(x=conflict.tdwt, y=RT, group=conflict.gain, colour=conflict.gain))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  labs (x = "Inhibitory Weight", y = "RT", group = "Gain") +
  ggtitle("Bias -12, comparing different gain values")




################### --------- Look at varying gain, bias on X axis ------- ############

data.sim13b.varygain <- rbind (data.sim13b.0SW, data.sim13b.1SW, data.sim13b.2SW, data.sim13b.ALT)
data.sim13b.varygain   <- subset (data.sim13b.varygain, data.sim13b.varygain$conflict.bias== -12 & data.sim13b.comparegain$conflict.tdwt > -10)

linegraph <- ggplot (data.sim13b.comparegain, aes(x=conflict.gain, y=RT, group=conflict.tdwt, colour=conflict.tdwt))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
  labs (x = "Gain", y = "RT", group = "Weight") +
  ggtitle("Bias -12, comparing different Weight values values")


data.sim13b.comparerepetitions.weight11 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.bias== -9 & data.raw$conflict.tdwt== -10.5)
linegraph <- ggplot (data.sim13b.comparerepetitions.weight11, aes(x=conflict.gain, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Gain", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), weight -10.5, Bias -9")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_varygain_aggregate.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")




############### Switch costs vs. n2rep costs ############

data.sim13b.weight11.bias9 <- subset (data.raw, data.raw$conflict.tdwt == -10.5 & data.raw$conflict.bias == -9)

# Data set for looking at sc and n2rc varying with confl-td weight
data.sim13b.sc <- subset (data.sim13b.weight11.bias9, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "sc"))
names (data.sim13b.sc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias", "cost")
data.sim13b.sc$dv <- rep("sc", nrow(data.sim13b.sc))
data.sim13b.n2rc <- subset (data.sim13b.weight11.bias9, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "n2rc"))
names (data.sim13b.n2rc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias",  "cost")
data.sim13b.n2rc$dv <- rep("n2rc", nrow(data.sim13b.n2rc))
data.sim13b <- rbind (data.sim13b.sc, data.sim13b.n2rc)

# Now plot switch costs and n-2 rep costs
bargraph <- ggplot (data.sim13b, aes(x=conflict.gain, y=cost, group=dv, fill=dv))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Gain", y = "Cost", group = "DV") +
  ggtitle("Varying Gain, Bias -9, Weight -10.5")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_varygain_costs.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")

############## -------------- Vary bias on X asix ------------- ############

data.sim13b.weight11.gain77 <- subset (data.raw, data.raw$conflict.tdwt == -10.5 & data.raw$conflict.gain == 77)
#
# Data set for looking at sc and n2rc varying with confl-td weight
data.sim13b.sc <- subset (data.sim13b.weight11.gain77, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "sc"))
names (data.sim13b.sc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias", "cost")
data.sim13b.sc$dv <- rep("sc", nrow(data.sim13b.sc))
data.sim13b.n2rc <- subset (data.sim13b.weight11.gain77, select=c("conflict.tdwt", "conflict.gain", "conflict.bias", "n2rc"))
names (data.sim13b.n2rc) <- c("conflict.tdwt", "conflict.gain", "conflict.bias",  "cost")
data.sim13b.n2rc$dv <- rep("n2rc", nrow(data.sim13b.n2rc))
data.sim13b <- rbind (data.sim13b.sc, data.sim13b.n2rc)
#
# Now plot switch costs and n-2 rep costs
bargraph <- ggplot (data.sim13b, aes(x=conflict.bias, y=cost, group=dv, fill=dv))
bargraph +
  stat_summary(fun.y = mean, geom = "bar", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Bias", y = "Cost", group = "DV") +
  ggtitle("Varying Bias, Gain 77 Weight -10.5")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_varybias_costs.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")


#### Compare repetitions vs. no repetitions

data.sim13b.comparerepetitions.weight11 <- subset (data.sim13b.comparerepetitions, data.raw$conflict.gain== 77 & data.raw$conflict.tdwt== -10.5)
linegraph <- ggplot (data.sim13b.comparerepetitions.weight11, aes(x=conflict.bias, y=RT, group=repetitions, colour=repetitions))
linegraph +
  stat_summary(fun.y = mean, geom = "line", position = "dodge") +
#  stat_summary(fun.data = mean_cl_boot, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + 
  labs (x = "Gain", y = "RT", group = "repetitions") +
  ggtitle("RT for repetitions (1SW, 0SW) vs. nonrepetitions (2SW, ALT), weight -10.5, Gain 77")

imageFile <- file.path("~/Dropbox/PhD/Thesis/simulation_results/simulation_13", "sim_13b_defaults_varybias_aggregate.png")
ggsave(filename=imageFile, width = 300, height = 300, units = "mm")
