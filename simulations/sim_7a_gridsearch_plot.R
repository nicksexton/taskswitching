rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
library (ggplot2)



### Averaging across multiple runs, mung the data sets together
#cols.to.avg = c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", "mean.1SW", "mean.2SW", "mean.ALT")

# CLIP Low noise version 3
data.clip <- read.delim("sim_7a_gridsearch_results_highnoise_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# CLIP Low noise version 3
data.rescale <- read.delim("sim_7a_gridsearch_results_highnoise_rescale.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)


# Sim 8 (scaling conflict td weights by TD control strength)
data.sim8 <- read.delim("sim_8a_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# temp test
data.temp <- read.delim("temp_sim_7a_results.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

# transformation function for switch/n2-rep costs with small range
compress <- function (x) 2*((1/(1+exp(-0.5 * x)) - 0.5))
 
# transformation function for switch/n2-rep costs with big range
compress1 <- function (x) 2*((1/(1+exp(-0.1 * x)) - 0.5))

# transformation function for error switch costs
compress.err <- function (x) 2*((1/(1+exp(-100 * x)) - 0.5))


intersect <- function (sc, n2rc)  { max (sc, 0) * max (n2rc, 0)}
intersectErr <- function (error.sc, error.n2rc) {max (error.sc, 0) * max (error.n2rc, 0)}

twotailed <- function (p, effect) {  return (ifelse (effect > 0, p, 1-p))  }


# Code could be vectorised!
data.clip$intersect <- rep(0, nrow(data.clip))
data.clip$intersectErr <- rep(0, nrow(data.clip))
progress <- txtProgressBar (min=0, max=nrow(data.clip), style=3)
for (i in 1:nrow(data.clip)) {
  data.clip[i,]$intersect <- intersect (data.clip[i,]$sc, data.clip[i,]$n2rc)
  data.clip[i,]$intersectErr <- intersectErr ((data.clip[i,]$err.3.1SW - data.clip[i,]$err.3.0SW), (data.clip[i,]$err.3.ALT - data.clip[i,]$err.3.2SW))
  setTxtProgressBar(progress, i)  
}

# Code could be vectorised!
data.temp$intersect <- rep(0, nrow(data.temp))
data.temp$intersectErr <- rep(0, nrow(data.temp))
progress <- txtProgressBar (min=0, max=nrow(data.temp), style=3)
for (i in 1:nrow(data.temp)) {
  data.temp[i,]$intersect <- intersect (data.temp[i,]$sc, data.temp[i,]$n2rc)
  data.temp[i,]$intersectErr <- intersectErr ((data.temp[i,]$err.3.1SW - data.temp[i,]$err.3.0SW), (data.temp[i,]$err.3.ALT - data.temp[i,]$err.3.2SW))
  setTxtProgressBar(progress, i)  
}

# Code could be vectorised!
data.sim8$intersect <- rep(0, nrow(data.sim8))
data.sim8$intersectErr <- rep(0, nrow(data.sim8))
progress <- txtProgressBar (min=0, max=nrow(data.sim8), style=3)
for (i in 1:nrow(data.sim8)) {
  data.sim8[i,]$intersect <- intersect (data.sim8[i,]$sc, data.sim8[i,]$n2rc)
  data.sim8[i,]$intersectErr <- intersectErr ((data.sim8[i,]$err.3.1SW - data.sim8[i,]$err.3.0SW), (data.sim8[i,]$err.3.ALT - data.sim8[i,]$err.3.2SW))
  setTxtProgressBar(progress, i)  
}




plot.heatmap.sc <- function (data, condition.title) {

#  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc))
  sc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=sc))
  sc + geom_raster() +
#    facet_wrap( ~ conflict.bias) +
    facet_wrap( ~ conflict.tdwt) +
    scale_fill_gradient2(midpoint=0,  mid="grey70", limits=c(-30,40)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n Switch Costs")) +
    labs(fill="Switch Costs\n(cycles)") +
    theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.sc <- function (data, condition.title) {
  
  sc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress1(sc)))
  sc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n Switch costs") ) +
    labs(fill="Switch Costs\ntransform(cycles)") +
    theme (legend.position=c(0.87,0.1))
}



plot.heatmap.sc.p <- function (data, condition.title) {

  labs <- c(0.05, 0.95)
  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")
# scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
  
  sc.p <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=twotailed (sc.p, sc)))
  sc.p + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=colrs, na.value="black", limits=c(0,1),
                           labels=labs, breaks=labs) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of Switch costs") ) +
  labs(fill="significance (p)") +
  theme (legend.position=c(0.87,0.1))
  
}


plot.heatmap.n2rc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=n2rc))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-30,40)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\n(cycles)") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmapCompress.n2rc <- function (data, condition.title) {
# use a suitable squashing function as transf

#  FUN = match.fun(FUN)  
  n2rc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress1(n2rc)))
  n2rc + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\ntransform(cycles)") +
  theme (legend.position=c(0.87,0.1))
}


plot.heatmap.n2rc.p <- function (data, condition.title) {

  labs <- c(0.05, 0.95)
  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")

  n2rc.p <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=twotailed (n2rc.p, n2rc)))
  n2rc.p + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=colrs, na.value="black", limits=c(0,1),
                           labels=labs, breaks=labs) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                       "\nParameter space for conflict parameters\n p value of N-2 Repetition costs") ) +
  labs(fill="significance (p)") +
    theme (legend.position=c(0.87,0.1))
}




plot.heatmap.sctimesn2rc <- function (data, condition.title) {
# plots the intersection of SCs and N2RCs by calculating the product of both DVs

  SCtimesN2RC <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress1(intersect)))
  SCtimesN2RC + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
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
  
#  plot.heatmap.sc (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_sc.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }  

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




# Temp function - plotting heatmap of base RTs in 1SW condition.
# Should really average eg. mean.0SW and mean.1SW properly (ie., weighted for number of correct trials) or,
# even better, modify gridsearch code to output error rate (or number of correct vs. incorrect trials) to data

rt.colour.scale <- c("skyblue1", "springgreen4", "yellow", "red", "darkred")
rt.limits <- c(log(30), log(500))

plot.heatmap.rt.0SW <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(mean.0SW)))
  rt.0SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
      scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs,
                           breaks=bre, limits=rt.limits) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (0SW condition)") ) +
  labs(fill="log RT") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.rt.1SW <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.1SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(mean.1SW)))
  rt.1SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs,
                           breaks=bre, limits=rt.limits) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (1SW condition)") ) +
  labs(fill="log RT") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.rt.2SW <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(mean.2SW)))
  rt.0SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs,
                         breaks=bre, limits=rt.limits) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (2SW condition)") ) +
  labs(fill="log RT") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.rt.ALT <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.1SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(mean.ALT)))
  rt.1SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs,
                           breaks=bre, limits=rt.limits) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (ALT condition)") ) +
  labs(fill="log RT") +
  theme (legend.position=c(0.87,0.1))

}


##### Error Plots

plot.heatmap.logErrors.0SW <- function (data, condition.title) {
#Errors(1,2) in 0SW condition

  lab <- c(0.005, 0.01, 0.025, 0.05, 0.1, 0.2, 0.4) # labels for legend
  bre <- log (lab) # position on legend to plot labels
#  colrs <- c("black", "black", "red4", "red2", "red", "orangered3", "orange", "yellow")
  colrs <- c("black", "black", "darkred", "red", "orange", "yellow")
#  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=err.12.0SW))
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(err.12.0SW)))
  error.rates + geom_raster() +
   facet_wrap( ~ conflict.tdwt) +                                        
   scale_fill_gradientn(name="Error Rate", colours=colrs,
                        na.value="white", breaks=bre, labels=lab) +
#scale_fill_gradientn(colours=c("black", "red4", "red3", "red2", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks                          
   ggtitle(paste (condition.title,
                   "\nParameter space for Errors(1,2) in 0SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.logErrors.1SW <- function (data, condition.title) {
# Heatmap with errors plotted on a log scale

  lab <- c(0.005, 0.01, 0.025, 0.05, 0.1, 0.2, 0.4) # labels for legend
  bre <- log (lab) # position on legend to plot labels
  colrs <- c("black", "black", "red", "orange", "yellow")
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(err.12.1SW)))
  error.rates + geom_raster() +
   facet_wrap( ~ conflict.tdwt) +                                        
   scale_fill_gradientn(name="Error Rate", colours=colrs,
                        na.value="white", breaks=bre, labels=lab) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks                          
  ggtitle(paste (condition.title,
                   "\nParameter space for Errors(1,2) in 1SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.errors.1SW <- function (data, condition.title) {
#Errors(1,2) in 1SW condition plotted on a linear scale
  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=err.12.1SW))
#  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.12.1SW)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
 # scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white") +
scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
  ggtitle(paste (condition.title,
                   "\nParameter space for Errors(1,2) in 1SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.logErrors.3.ALT <- function (data, condition.title) {
#Errors(3) in ALT condition

  lab <- c(0.005, 0.01, 0.025, 0.05, 0.1, 0.2, 0.4) # labels for legend
  bre <- log (lab) # position on legend to plot labels
  colrs <- c("black", "black", "red", "orange", "yellow")
  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(err.3.ALT)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
  scale_fill_gradientn(name="Error Rate", colours=colrs,
                           na.value="white", breaks=bre, labels=lab) +
# scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nParameter space for Errors(3) in ALT condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.logErrors.3.2SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  lab <- c(0.005, 0.01, 0.025, 0.05, 0.1, 0.2, 0.4) # labels for legend
  bre <- log (lab) # position on legend to plot labels
  colrs <- c("black", "black", "red", "orange", "yellow")
  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=log(err.3.2SW)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
  scale_fill_gradientn(name="Error Rate", colours=colrs,
                           na.value="white", breaks=bre, labels=lab) +
# scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nParameter space for Errors(3) in 2SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}


#plot.heatmap.errors.3.2SW <- function (data, condition.title) {
#Errors(3) in 2SW condition
  
#  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=err.3.2SW))
#  error.rates + geom_raster() +
#    facet_wrap( ~ conflict.tdwt) +                                        
#    scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
#    ggtitle(paste (condition.title,
#                   "\nParameter space for Errors(3) in 2SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
#  theme (legend.position=c(0.87,0.1))
#}

plot.heatmap.errors.n2rc <- function (data, condition.title) {
#Errors switch cost: Difference between Errors(3) in ALT and 2SW conditions
  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.err(err.3.ALT - err.3.2SW)))
  error.rates + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(-1,1)) + 
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for Error n-2RC: contrast between ALT and 2SW conditions") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.errors.sc <- function (data, condition.title) {
#Errors switch cost: Difference between Errors(3) in ALT and 2SW conditions
  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.err(err.3.1SW - err.3.0SW)))
  error.rates + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(-1,1)) + 
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for Error switch cost: contrast between 1SW and 0SW conditions") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

  compress.intersectErr <- function (x) 2*((1/(1+exp(-500000 * x)) - 0.5))

plot.errormap.sctimesn2rc <- function (data, condition.title) {
# plots the intersection of SCs and N2RCs by calculating the product of both DVs
  SCtimesN2RC <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.intersectErr(intersectErr)))
  SCtimesN2RC + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                   "\nParameter space for intersection of Error SCs and N2RCs") ) +
  theme (legend.position=c(0.87,0.1))
}


plot.errormaps <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {
  plot.heatmap.rt.0SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_rt_0SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.rt.1SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_rt_1SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.rt.2SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_rt_2SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.rt.ALT (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_rt_ALT.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  
  plot.heatmap.logErrors.0SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_0SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.logErrors.1SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_1SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.logErrors.3.ALT (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_ALT.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.logErrors.3.2SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_2SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.errors.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_n2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }

  plot.heatmap.errors.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_sc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }  

    plot.errormap.sctimesn2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_errors_sctimesn2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=250, units="mm")
  }  

}


plot.heatmaps.bytaskseq <- function (data, cond.title, image.directory, file.stem, save=FALSE) {

  plot.heatmaps (subset(data, data$alternation=="AB"),
                        condition.title=paste("Task sequence AB\n",cond.title, sep=""),
                        image.directory=image.directory,
                        filename.stem=paste(file.stem, "AB_", sep=""),
                        save=save)

  plot.heatmaps (subset(data, data$alternation=="BA"),
                        condition.title=paste("Task sequence BA\n",cond.title, sep=""),
                        image.directory=image.directory,
                        filename.stem=paste(file.stem, "BA_", sep=""),
                        save=save)

  plot.errormaps (subset(data, data$alternation=="AB"),
                        condition.title=paste("Task sequence AB\n",cond.title, sep=""),
                        image.directory=image.directory,
                        filename.stem=paste(file.stem, "AB_", sep=""),
                        save=save)

  plot.errormaps (subset(data, data$alternation=="BA"),
                  condition.title=paste("Task sequence BA\n",cond.title, sep=""),
                  image.directory=image.directory,
                  filename.stem=paste(file.stem, "BA_", sep=""),
                  save=save)

#  plot.errormaps (subset(data, data$alternation=="BA"),
#                        condition.title=paste("Task sequence BA\n",cond.title, sep=""),
#                        image.directory=image.directory,
#                        filename.stem=paste(file.stem, "BA_", sep=""),
#                        save=save)

 
  
}

## Re-run of 15x, clip lownoise
plot.heatmaps.bytaskseq (data.clip, cond.title="Conflict Clipped\nNoise=.006", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_7a/clip", file.stem="simulation_7a_gridsearch_clip", save=TRUE)

## temp debug 
plot.heatmaps.bytaskseq (data.sim8, cond.title="Simulation 8\nNoise=.006", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_8a", file.stem="simulation_8a_gridsearch_clip", save=TRUE)
