# Initialisation file for specific run of a simulation

# rm (list = ls())

filename.output.genetic.results <- "sim_6d_gridsearch_results_clip_lownoise_2.txt" # where to store results


blocksize <- 100
n <- 15 # resolution of grid to explore param space (ie total observations is n^ncols(min)
max.cycles <- 500
model.conf.leaf.min <- c(conflict.gain = 1.0, conflict.tdwt = -30.0, conflict.bias = -40.0)
model.conf.leaf.max <- c(conflict.gain = 100.0, conflict.tdwt = -1.0, conflict.bias = -1.0)

path.ramdiskfolder <- "clip_lownoise_2"

# source ("sim_6d_gridsearch.R")
