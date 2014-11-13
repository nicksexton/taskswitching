rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
library (ggplot2)

# 10x10x10, wide region
data.allow = read.delim("sim_6d_gridsearch_results_allow.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# 10x10x10, wide region
data.clip = read.delim("sim_6d_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# Low noise version, 15x15x15, wide region
data.clip.lownoise = read.delim("sim_6d_gridsearch_results_clip_lownoise.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

#10x10x10, wide region
data.rescale = read.delim("sim_6d_gridsearch_results_rescale.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


# NB 1SW > 0SW = +ve switch costs (empirical), ALT > 2SW = +ve n-2rc (empirical)

compress <- function (x) 2*((1/(1+exp(-0.5 * x)) - 0.5))


plot.heatmap.sc <- function (data, condition.title) {

#  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc))
  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=sc))
  sc + geom_raster() +
#    facet_wrap( ~ conflict.bias) +
    facet_wrap( ~ conflict.tdwt) +
    scale_fill_gradient2(midpoint=0,  mid="grey70", limits=c(-10,10)) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n Switch Costs")) +
    labs(fill="Switch Costs\n(cycles)") +
    theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.sc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress(sc)))
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
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-10,10)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\n(cycles)") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.n2rc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress(n2rc)))
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


plot.heatmaps <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {

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

plot.heatmaps (data.clip, condition.title="Conflict Clipped", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_clip", save=TRUE)

plot.heatmaps (data.rescale, condition.title="Conflict Rescaled", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_rescale", save=TRUE)

plot.heatmaps (data.allow, condition.title="Negative Conflict Allowed", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_allow", save=TRUE)

plot.heatmaps (data.clip.lownoise, condition.title="Conflict Clipped\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_clip_lownoise", save=TRUE)

