rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
# setwd("~/Thesis/data/simulation_6e")
library (ggplot2)



### Averaging across multiple runs, mung the data sets together
#cols.to.avg = c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", "mean.1SW", "mean.2SW", "mean.ALT")

# CLIP Low noise version 3
#data.clip.lownoise.0 = read.delim("sim_6e_gridsearch_results_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

data.clip.highnoise.0 = read.delim("sim_6e_gridsearch_results_highnoise_clip.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)

                                        # trim data to only 4 weight levels (for fitting on single grid row)
#data.clip.highnoise.0 <- subset (data.clip.highnoise.0, (conflict.tdwt == 0 | conflict.tdwt == -10.714 | conflict.tdwt == -19.286 | conflict.tdwt == -27.857))
data.clip.highnoise.0 <- subset (data.clip.highnoise.0, (conflict.tdwt == 0 | conflict.tdwt == -10 | conflict.tdwt == -20 | conflict.tdwt == -30))


# transformation function for switch/n2-rep costs with small range
compress <- function (x) 2*((1/(1+exp(-0.5 * x)) - 0.5))
 
# transformation function for switch/n2-rep costs with big range
compress1 <- function (x) 2*((1/(1+exp(-0.1 * x)) - 0.5))

# transformation function for error switch costs
compress.err <- function (x) 2*((1/(1+exp(-100 * x)) - 0.5))


intersect <- function (sc, n2rc)  { max (sc, 0) * max (n2rc, 0)}
intersectErr <- function (error.sc, error.n2rc) {max (error.sc, 0) * max (error.n2rc, 0)}
twotailed <- function (p, effect) {  return (ifelse (effect > 0, p, 1-p))  }

calculate.r <- function (t, df) sqrt(t^2 / (t^2 + df))



### Highnoise
data.clip.highnoise.0$intersect <- rep(0, nrow(data.clip.highnoise.0))
data.clip.highnoise.0$intersectErr <- rep(0, nrow(data.clip.highnoise.0))
data.clip.highnoise.0$intersect.effsize <- rep(0, nrow(data.clip.highnoise.0))
for (i in 1:nrow(data.clip.highnoise.0)) {
  data.clip.highnoise.0[i,]$intersect <- intersect (data.clip.highnoise.0[i,]$sc, data.clip.highnoise.0[i,]$n2rc)
#  data.clip.highnoise.0[i,]$intersect.effsize <- intersect (calculate.r (data.clip.highnoise.0[i,]$sc.t, data.clip.highnoise.0[i,]$sc.df), calculate.r ((data.clip.highnoise.0[i,]$n2rc.t, data.clip.highnoise.0[i,]$n2rc.df) * ifelse (data.clip.highnoise.0[i,]$n2rc.t > 0, -1, 1)))
  data.clip.highnoise.0[i,]$intersect.effsize <- with (data.clip.highnoise.0[i,],
                                                       sqrt(intersect (calculate.r (sc.t, sc.df),
                                                                  calculate.r (n2rc.t, n2rc.df) * ifelse (n2rc.t > 0, -1, 1))))
  data.clip.highnoise.0[i,]$intersectErr <- intersectErr ((data.clip.highnoise.0[i,]$err.3.1SW -
                                                           data.clip.highnoise.0[i,]$err.3.0SW),
                                                          (data.clip.highnoise.0[i,]$err.3.ALT -
                                                           data.clip.highnoise.0[i,]$err.3.2SW))
}

#data.allow.highnoise.0$intersect <- rep(0, nrow(data.allow.highnoise.0))
#data.allow.highnoise.0$intersectErr <- rep(0, nrow(data.allow.highnoise.0))
#for (i in 1:nrow(data.allow.highnoise.0)) {
#  data.allow.highnoise.0[i,]$intersect <- intersect (data.allow.highnoise.0[i,]$sc, data.allow.highnoise.0[i,]$n2rc)
#  data.allow.highnoise.0[i,]$intersectErr <- intersectErr ((data.allow.highnoise.0[i,]$err.3.1SW - data.allow.highnoise.0[i,]$err.3.0SW), (data.allow.highnoise.0[i,]$err.3.ALT - data.allow.highnoise.0[i,]$err.3.2SW))
#}

#data.rescale.highnoise.0$intersect <- rep(0, nrow(data.rescale.highnoise.0))
#data.rescale.highnoise.0$intersectErr <- rep(0, nrow(data.rescale.highnoise.0))
#for (i in 1:nrow(data.rescale.highnoise.0)) {
#  data.rescale.highnoise.0[i,]$intersect <- intersect (data.rescale.highnoise.0[i,]$sc, data.rescale.highnoise.0[i,]#$n2rc)
#  data.rescale.highnoise.0[i,]$intersectErr <- intersectErr ((data.rescale.highnoise.0[i,]$err.3.1SW - data.rescale.#highnoise.0[i,]$err.3.0SW), (data.rescale.highnoise.0[i,]$err.3.ALT - data.rescale.highnoise.0[i,]$err.3.2SW))
#}


plot.heatmap.sc <- function (data, condition.title) {

#  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.tdwt, fill=sc))
  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=sc))
  sc + geom_raster() +
#    facet_wrap( ~ conflict.bias) +
    facet_grid( ~ conflict.tdwt) +
    scale_fill_gradient2(midpoint=0,  mid="grey70", limits=c(-30,40)) +
    ggtitle(paste (condition.title,
                   ", Switch Costs")) +
    labs(fill="Switch Costs\n(cycles)") +
#   theme (legend.position=c(0.87,0.1))
    theme (legend.position="right")  
}

plot.heatmapCompress.sc <- function (data, condition.title) {
  
  sc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress1(sc)))
  sc + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
        ggtitle(paste (condition.title,
                       ", Switch costs") ) +
    labs(fill="Switch Costs\ntransform(cycles)") +
#   theme (legend.position=c(0.87,0.1))
    theme (legend.position="top")
}



# two tailed version
plot.heatmap.sc.p <- function (data, condition.title) {

  labs <- c(0.05, 0.95)
  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")

  sc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=twotailed (sc.p, sc)))  
  sc.p + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=colrs, na.value="black", limits=c(0,1),
                           labels=labs, breaks=labs) +
        ## ggtitle(paste (condition.title,
        ##                ", p value of Switch costs") ) +
  labs(fill="significance (p)") +
  # theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}


plot.effectsize.sc <- function (data, condition.title) {
  

  
#  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")

  sc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=calculate.r(sc.t, sc.df)))  
  sc.p + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradient2(high="red", low="blue", mid="white", limits=c(-0.5,0.5)) +
        ## ggtitle(paste (condition.title,
        ##                ", p value of Switch costs") ) +
  labs(fill="effect size (r)") +
  # theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}



plot.heatmap.n2rc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=n2rc))
  n2rc + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-30,40)) +
        ggtitle(paste (condition.title,
                       ", N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\n(cycles)") +
#  theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}

plot.heatmapCompress.n2rc <- function (data, condition.title) {
# use a suitable squashing function as transf

#  FUN = match.fun(FUN)  
  n2rc <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress1(n2rc)))
  n2rc + geom_raster() +
    facet_grid( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
    ggtitle(paste (condition.title,
                   ", N-2 Repetition costs") ) +
  labs(fill="N-2 Repetition Costs\ntransform(cycles)") +
#  theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}



# two tailed version
plot.heatmap.n2rc.p <- function (data, condition.title) {

  labs <- c(0.05, 0.95)
  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")

  n2rc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=twotailed (n2rc.p, n2rc)))
  n2rc.p + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=colrs, na.value="black", limits=c(0,1),
                           labels=labs, breaks=labs) +
        ## ggtitle(paste (condition.title,
        ##                ", p value of N-2 Repetition costs") ) +
  labs(fill="significance (p)") +
# theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}


calculate.r <- function (t, df) sqrt(t^2 / (t^2 + df))

plot.effectsize.n2rc <- function (data, condition.title) {



#  colrs <- c("red", "white", "white", "white", "white", "white", "white", "white", "white", "green")
  # nb direction of effect size is flipped because t values are inverted
  sc.p <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=(calculate.r(n2rc.t, n2rc.df)* ifelse(n2rc.t>0, -1, 1) )))  
  sc.p + geom_raster() +
    facet_grid( ~ conflict.tdwt) +
      scale_fill_gradient2(high="red", low="blue", mid="white", limits=c(-0.25,0.25)) +
        ## ggtitle(paste (condition.title,
        ##                ", p value of Switch costs") ) +
  labs(fill="effect size (r)") +
  # theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}




plot.heatmap.sctimesn2rc <- function (data, condition.title) {
# plots the intersection of SCs and N2RCs by calculating the product of both DVs

  SCtimesN2RC <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress1(intersect)))
  SCtimesN2RC + geom_raster() +
    facet_grid( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
    ## ggtitle(paste (condition.title,
    ##                ", intersection of SCs and N2RCs") ) +
  labs(fill="compress (sqrt(n2rc x sc))") +
#  theme (legend.position=c(0.87,0.1))
  theme (legend.position="right")  
}


plot.effectsize.sctimesn2rc <- function (data, condition.title) {
# plots the intersection of SCs and N2RCs by calculating the product of both DVs

  SCtimesN2RC <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=intersect.effsize))
  SCtimesN2RC + geom_raster() +
    facet_grid( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="yellow", high="grey20", na.value="white",
                         limits=c(0,0.301),  breaks=c(0, 0.1, 0.2, 0.3)) +
    ## ggtitle(paste (condition.title,
    ##                ", intersection of SCs and N2RCs") ) +
  labs(fill="co-occurrance\ngeom. mean (r)") +
  theme (legend.position="right")  
}

                                     
plot.heatmaps <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {

  ## plot.heatmap.sctimesn2rc (data, condition.title)
  ## if (save==TRUE) {
  ##   image.file <- file.path(image.directory, paste(filename.stem, "_sctimesn2rc.png", sep=""))
  ##   ggsave(filename=image.file, width=200, height=50, units="mm")
  ## }

    plot.effectsize.sctimesn2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_scxn2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

#  plot.heatmap.rt.0SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_rt0SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }
  
#  plot.heatmap.sc (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_sc.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }  

#  plot.heatmapCompress.sc (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_compr_sc.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }  
  
  ## plot.heatmap.sc.p (data, condition.title)
  ## if (save==TRUE) {
  ##   image.file <- file.path(image.directory, paste(filename.stem, "_sc_p.png", sep="")) 
  ##   ggsave(filename=image.file, width=200, height=50, units="mm")
  ## }

  plot.effectsize.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_sc.png", sep="")) 
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

    
#  plot.heatmap.n2rc (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_n2rc.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")    
#  }

#  plot.heatmapCompress.n2rc (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "_compr_n2rc.png", sep="")) 
#    ggsave(filename=image.file, width=200, height=250, units="mm")    
#  }
  
  ## plot.heatmap.n2rc.p (data, condition.title)
  ## if (save==TRUE) {
  ##   image.file <- file.path(image.directory, paste(filename.stem, "_n2rc_p.png", sep="") )
  ##   ggsave(filename=image.file, width=200, height=50, units="mm")    
  ## }

  plot.effectsize.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_n2rc.png", sep="")) 
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

    
}


# Temp function - plotting heatmap of base RTs in 1SW condition.
# Should really average eg. mean.0SW and mean.1SW properly (ie., weighted for number of correct trials) or,
# even better, modify gridsearch code to output error rate (or number of correct vs. incorrect trials) to data

rt.colour.scale <- c("skyblue1", "springgreen4", "yellow", "red", "darkred")

plot.heatmap.rt.0SW <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.0SW <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(mean.0SW)))
  rt.0SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs, breaks=bre) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (0SW condition)") ) +
  labs(fill="log RT") +
  theme (legend.position=c(0.87,0.1))

}

plot.heatmap.rt.1SW <- function (data, condition.title) {

  labs=c(50, 75, 100, 150, 200, 400)
  bre = log(labs)
  
  rt.1SW <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(mean.1SW)))
  rt.1SW + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +
      scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs, breaks=bre) +
    ggtitle(paste (condition.title,
                   "\nParameter space for conflict parameters\n RT (1SW condition)") ) +
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
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.12.0SW)))
  error.rates + geom_raster() +
   facet_wrap( ~ conflict.tdwt) +                                        
   scale_fill_gradientn(name="Error Rate", colours=colrs,
                        na.value="white", breaks=bre, labels=lab) +
#scale_fill_gradientn(colours=c("black", "red4", "red3", "red2", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
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
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.12.1SW)))
  error.rates + geom_raster() +
   facet_wrap( ~ conflict.tdwt) +                                        
   scale_fill_gradientn(name="Error Rate", colours=colrs,
                        na.value="white", breaks=bre, labels=lab) +
  ggtitle(paste (condition.title,
                   "\nParameter space for Errors(1,2) in 1SW condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.errors.1SW <- function (data, condition.title) {
#Errors(1,2) in 1SW condition plotted on a linear scale
  
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=err.12.1SW))
#  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.12.1SW)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
 # scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white") +
scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
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
  
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.3.ALT)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
  scale_fill_gradientn(name="Error Rate", colours=colrs,
                           na.value="white", breaks=bre, labels=lab) +
# scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
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
  
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=log(err.3.2SW)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt) +
  scale_fill_gradientn(name="Error Rate", colours=colrs,
                           na.value="white", breaks=bre, labels=lab) +
# scale_fill_gradientn(colours=c("black", "darkred", "red", "orange", "yellow"), na.value="white", limits=c(0,0.3)) +
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
  
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress.err(err.3.ALT - err.3.2SW)))
  error.rates + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(-1,1)) + 
    ggtitle(paste (condition.title,
                   "\nParameter space for Error n-2RC: contrast between ALT and 2SW conditions") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

plot.heatmap.errors.sc <- function (data, condition.title) {
#Errors switch cost: Difference between Errors(3) in ALT and 2SW conditions
  
  error.rates <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress.err(err.3.1SW - err.3.0SW)))
  error.rates + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(-1,1)) + 
    ggtitle(paste (condition.title,
                   "\nParameter space for Error switch cost: contrast between 1SW and 0SW conditions") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  theme (legend.position=c(0.87,0.1))
}

  compress.intersectErr <- function (x) 2*((1/(1+exp(-500000 * x)) - 0.5))

plot.errormap.sctimesn2rc <- function (data, condition.title) {
# plots the intersection of SCs and N2RCs by calculating the product of both DVs
  SCtimesN2RC <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress.intersectErr(intersectErr)))
  SCtimesN2RC + geom_raster() +
    facet_wrap( ~ conflict.tdwt) +                                        
    scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
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


########## FOR COGSCI PAPER #########
plot.heatmaps (data.clip.highnoise.0, condition.title="Simulation 1", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/cogsci_2015/simulation_1", filename.stem="remake_sim_1", save=TRUE)



## Re-run of 15x, clip lownoise
#plot.heatmaps (data.clip.lownoise.0, condition.title="Conflict Clipped\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/clip", filename.stem="simulation_6e_gridsearch_clip", save=TRUE)

#plot.errormaps (data.clip.lownoise.0, condition.title="Conflict Clipped\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/clip", filename.stem="simulation_6e_gridsearch_clip", save=TRUE)


                                        # Clip highnoise
#plot.heatmaps (data.clip.highnoise.0, condition.title="Conflict Clipped\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_clip", filename.stem="simulation_6e_gridsearch_clip", save=TRUE)

#plot.errormaps (data.clip.highnoise.0, condition.title="Conflict Clipped\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_clip", filename.stem="simulation_6e_gridsearch_clip", save=TRUE)



                                        # 15x, allow lownoise
#plot.heatmaps (data.allow.lownoise.0, condition.title="Neg Conflict Allowed run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/allow", filename.stem="simulation_6e_gridsearch_allow", save=TRUE)

#plot.errormaps (data.allow.lownoise.0, condition.title="Neg Conflict Allowed run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/allow", filename.stem="simulation_6e_gridsearch_allow", save=TRUE)


                                        # 15x, allow highnoise
#plot.heatmaps (data.allow.highnoise.0, condition.title="Neg Conflict Allowed run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_allow", filename.stem="simulation_6e_gridsearch_allow", save=TRUE)
#plot.errormaps (data.allow.highnoise.0, condition.title="Neg Conflict Allowed run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_allow", filename.stem="simulation_6e_gridsearch_allow", save=TRUE)



                                        # 15x rescale lownoise
#plot.heatmaps (data.rescale.lownoise.0, condition.title="Conflict Rescaled run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/rescale", filename.stem="simulation_6e_gridsearch_rescale", save=TRUE)
#plot.errormaps (data.rescale.lownoise.0, condition.title="Conflict Rescaled run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/rescale", filename.stem="simulation_6e_gridsearch_rescale", save=TRUE)


                                        # 15x rescale highnoise
#plot.heatmaps (data.rescale.highnoise.0, condition.title="Conflict Rescaled run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_rescale", filename.stem="simulation_6e_gridsearch_rescale", save=TRUE)
#plot.errormaps (data.rescale.highnoise.0, condition.title="Conflict Rescaled run 0\nNoise=.004", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/simulation_6e/highnoise_rescale", filename.stem="simulation_6e_gridsearch_rescale", save=TRUE)
