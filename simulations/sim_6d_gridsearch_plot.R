rm (list = ls())

library (ggplot2)

data.clip = read.delim("sim_6d_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

data.allow = read.delim("sim_6d_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


# NB 1SW > 0SW = +ve switch costs (empirical), ALT > 2SW = +ve n-2rc (empirical)

plot.heatmaps <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {

  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc))
  sc + geom_raster() +
    facet_wrap( ~ conflict.bias) +
      scale_fill_gradient2(midpoint=0,  mid="grey70", limits=c(-10,10)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n Switch Costs"))
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_sc.png", sep=""))
    ggsave(image.file)
  }
  
  
  sc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc.p))
  sc.p + geom_raster() +
    facet_wrap( ~ conflict.bias) +
      scale_fill_gradient2(midpoint=.05, low="red", mid="grey70", high="grey70", limits=c(0,0.1)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of Switch costs") )
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_sc_p.png", sep="")) 
    ggsave(image.file)
  }

  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=n2rc))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.bias) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-10,10)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n N-2 Repetition costs") )
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_n2rc.png", sep="")) 
    ggsave(image.file)
  }
  
  n2rc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=n2rc.p))
  n2rc.p + geom_raster() +
    facet_wrap( ~ conflict.bias) +
      scale_fill_gradient2(midpoint=.05, low="red", mid="grey70", high="grey70", limits=c(0,0.1)) +
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of N-2 Repetition costs") )
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_n2rc_p.png", sep="") )
    ggsave(image.file)
  }
}

plot.heatmaps (data.clip, condition.title="Conflict Clipped", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6d", filename.stem="simulation_6d_gridsearch_clip", save=TRUE)
