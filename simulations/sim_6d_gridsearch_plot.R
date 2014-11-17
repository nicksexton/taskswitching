rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
library (ggplot2)


# 10x10x10, wide region
#data.allow = read.delim("sim_6d_gridsearch_results_allow.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# 10x10x10, wide region
#data.clip = read.delim("sim_6d_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

#10x10x10, wide region
#data.rescale = read.delim("sim_6d_gridsearch_results_rescale.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


# Low noise version, 15x15x15, wide region
#data.clip.lownoise.0 = read.delim("sim_6d_gridsearch_results_clip_lownoise.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
#data.clip.lownoise.1 = read.delim("sim_6d_gridsearch_results_clip_lownoise_1.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

### Averaging across multiple runs, mung the data sets together
cols.to.avg = c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", "mean.1SW", "mean.2SW", "mean.ALT")

# CLIP Low noise version 3
data.clip.lownoise.0 = read.delim("sim_6d_gridsearch_results_15x_clip_0.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

data.clip.lownoise.1 = read.delim("sim_6d_gridsearch_results_15x_clip_1.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.clip.lownoise.2 = read.delim("sim_6d_gridsearch_results_15x_clip_2.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.clip.lownoise.3 = read.delim("sim_6d_gridsearch_results_15x_clip_3.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.clip.lownoise.4 = read.delim("sim_6d_gridsearch_results_15x_clip_4.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)



# ALLOW low noise version 3
data.allow.lownoise.0 = read.delim("sim_6d_gridsearch_results_15x_allow_0.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

data.allow.lownoise.1 = read.delim("sim_6d_gridsearch_results_15x_allow_1.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.allow.lownoise.2 = read.delim("sim_6d_gridsearch_results_15x_allow_2.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# RESCALE low noise version
data.rescale.lownoise.0 = read.delim("sim_6d_gridsearch_results_15x_rescale_0.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

data.rescale.lownoise.1 = read.delim("sim_6d_gridsearch_results_15x_rescale_1.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.rescale.lownoise.2 = read.delim("sim_6d_gridsearch_results_15x_rescale_2.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.rescale.lownoise.3 = read.delim("sim_6d_gridsearch_results_15x_rescale_3.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)






recalc.means <- function (x1, x2) apply(X=cbind(x1, x2), MARGIN=1, FUN=mean) 

data.clip.lownoise.merge$mean.0SW <-recalc.means (data.clip.lownoise.merge$mean.0SW.x,
                                                  data.clip.lownoise.merge$mean.0SW.y)
data.clip.lownoise.merge$mean.1SW <-recalc.means (data.clip.lownoise.merge$mean.1SW.x,
                                                  data.clip.lownoise.merge$mean.1SW.y)
data.clip.lownoise.merge$mean.2SW <-recalc.means (data.clip.lownoise.merge$mean.2SW.x,
                                                  data.clip.lownoise.merge$mean.2SW.y)
data.clip.lownoise.merge$mean.ALT <-recalc.means (data.clip.lownoise.merge$mean.ALT.x,
                                                  data.clip.lownoise.merge$mean.ALT.y)
data.clip.lownoise.merge <- subset (data.clip.lownoise.merge, select=c(cols.to.avg))
data.clip.lownoise.merge$sc <- data.clip.lownoise.merge$mean.1SW - data.clip.lownoise.merge$mean.0SW
data.clip.lownoise.merge$n2rc <- data.clip.lownoise.merge$mean.ALT - data.clip.lownoise.merge$mean.2SW

# NB 1SW > 0SW = +ve switch costs (empirical), ALT > 2SW = +ve n-2rc (empirical)

# transformation function for switch/n2-rep costs with small range
compress <- function (x) 2*((1/(1+exp(-0.5 * x)) - 0.5))
 
# transformation function for switch/n2-rep costs with big range
compress1 <- function (x) 2*((1/(1+exp(-0.1 * x)) - 0.5))

intersect <- function (sc, n2rc) compress1 (max (sc, 0) * max (n2rc, 0))

# Code could be vectorised!
data.clip.lownoise.0$intersect <- rep(0, nrow(data.clip.lownoise.0))
for (i in 1:nrow(data.clip.lownoise.0)) {
  data.clip.lownoise.0[i,]$intersect <- intersect (data.clip.lownoise.0[i,]$sc, data.clip.lownoise.0[i,]$n2rc)
}

data.allow.lownoise.0$intersect <- rep(0, nrow(data.allow.lownoise.0))
for (i in 1:nrow(data.allow.lownoise.0)) {
  data.allow.lownoise.0[i,]$intersect <- intersect (data.allow.lownoise.0[i,]$sc, data.clip.lownoise.0[i,]$n2rc)
}

data.rescale.lownoise.0$intersect <- rep(0, nrow(data.rescale.lownoise.0))
for (i in 1:nrow(data.rescale.lownoise.0)) {
  data.rescale.lownoise.0[i,]$intersect <- intersect (data.rescale.lownoise.0[i,]$sc, data.clip.lownoise.0[i,]$n2rc)
}


plot.heatmap.sc <- function (data, condition.title) {

#  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc))
  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=sc))
  sc + geom_raster() +
#    facet_wrap( ~ conflict.bias) +
    facet_wrap( ~ conflict.tdwt) +
    scale_fill_gradient2(midpoint=0,  mid="grey70", limits=c(-30,40)) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n Switch Costs")) +
    labs(fill="Switch Costs\n(cycles)") +
    theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.sc <- function (data, condition.title) {
  
#  FUN = match.fun(FUN)
  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, compress1(sc)))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
#      scale_fill_gradient2(midpoint=0, limits=c(-10,20)) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n Switch costs") ) +
    labs(fill="Switch Costs\ntransform(cycles)") +
    theme (legend.position=c(0.87,0.1))
}



plot.heatmap.sc.p <- function (data, condition.title) {

  sc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=sc.p))
  sc.p + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=.05, low="red", mid="grey70", high="grey70", limits=c(0,0.1)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of Switch costs") ) +
  labs(fill="significance (p)") +
  theme (legend.position=c(0.87,0.1))
  
}


plot.heatmap.n2rc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=n2rc))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-30,40)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\n(cycles)") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.n2rc <- function (data, condition.title) {
# use a suitable squashing function as transf

#  FUN = match.fun(FUN)  
  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress1(n2rc)))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\ntransform(cycles)") +
  theme (legend.position=c(0.87,0.1))
}


plot.heatmap.n2rc.p <- function (data, condition.title) {
  n2rc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=n2rc.p))
  n2rc.p + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=.05, low="red", high="grey70", limits=c(0,0.1)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of N-2 Repetition costs") ) +
  labs(fill="significance (p)") +
    theme (legend.position=c(0.87,0.1))
}


# Temp function - plotting heatmap of base RTs in 1SW condition.
# Should really average eg. mean.0SW and mean.1SW properly (ie., weighted for number of correct trials) or,
# even better, modify gridsearch code to output error rate (or number of correct vs. incorrect trials) to data

plot.heatmap.rt.0SW <- function (data, condition.title) {

  rt.ALT <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=mean.2SW))
  rt.ALT + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(60,300)) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (ALT condition)") ) +
  labs(fill="RT (cycles)") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.sctimesn2rc <- function (data, condition.title) {


  SCtimesN2RC <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=intersect))
  SCtimesN2RC + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
    ggtitle(paste (condition.title,
                   "\nParameter space for intersection of SCs and N2RCs") ) +
  labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))

}
  
plot.heatmaps <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {

  plot.heatmap.sctimesn2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_sctimesn2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }
  
  plot.heatmap.rt.0SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_rt0SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }
  
  plot.heatmap.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_sc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }  

  plot.heatmapCompress.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_compr_sc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }  
  
  plot.heatmap.sc.p (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_sc_p.png", sep="")) 
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_n2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")    
  }

  plot.heatmapCompress.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_compr_n2rc.png", sep="")) 
    ggsave(filename=image.file, width=200, height=250, units="mm")    
  }
  
  plot.heatmap.n2rc.p (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_n2rc_p.png", sep="") )
    ggsave(filename=image.file, width=200, height=250, units="mm")    
  }

}




## Re-run of 15x, clip lownoise
plot.heatmaps (data.clip.lownoise.0, condition.title="Conflict Clipped run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_15x_clip_lownoise", save=TRUE)

#plot.heatmaps (data.clip.lownoise.1, condition.title="Conflict Clipped run 1\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_clip_lownoise_1", save=TRUE)

#plot.heatmaps (data.clip.lownoise.merge, condition.title="Conflict Clipped Merge\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_clip_lownoise_merge", save=TRUE)



# 15x, allow lownoise
plot.heatmaps (data.allow.lownoise.0, condition.title="Neg Conflict Allowed run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_15x_allow_lownoise", save=TRUE)


# 15x rescale lownoise
plot.heatmaps (data.rescale.lownoise.0, condition.title="Conflict Rescaled run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_15x_rescale_lownoise", save=TRUE)
