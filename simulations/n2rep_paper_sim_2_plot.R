
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
data.sim8 <- subset (data.sim8, (conflict.tdwt == -0.1786 | conflict.tdwt == -0.8929 | conflict.tdwt == -1.6071 | conflict.tdwt == -2.3214))



# Code could be vectorised!
data.sim8$intersect <- rep(0, nrow(data.sim8))
data.sim8$intersectErr <- rep(0, nrow(data.sim8))
progress <- txtProgressBar (min=0, max=nrow(data.sim8), style=3)
for (i in 1:nrow(data.sim8)) {
  data.sim8[i,]$intersect <- intersect (data.sim8[i,]$sc, data.sim8[i,]$n2rc)
  data.sim8[i,]$intersectErr <- intersectErr ((data.sim8[i,]$err.3.1SW - data.sim8[i,]$err.3.0SW), (data.sim8[i,]$err.3.ALT - data.sim8[i,]$err.3.2SW))
  setTxtProgressBar(progress, i)  
}






## Re-run of 15x, clip lownoise
#plot.heatmaps.bytaskseq (data.clip, cond.title="Conflict Clipped\nNoise=.006", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_7a/clip", file.stem="simulation_7a_gridsearch_clip", save=TRUE)

## temp debug 
#plot.heatmaps.bytaskseq (data.sim8, cond.title="Simulation 8\nNoise=.006", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_8a", file.stem="simulation_8a_gridsearch_clip", save=TRUE)






### Relative plots (all DVs relative to conflict.tdwt == 0, answering question of what conflict units do)

data.sim8.relative <- merge (x=data.sim8, y=data.sim8.ref, by=c("task.input.str", "task.topdown.str", "alternation"))

plot.relative.bytaskseq (data.sim8.relative, cond.title="Simulation 8", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/cogsci_2015/simulation_2", file.stem="cogsci_sim_2", save=TRUE)
