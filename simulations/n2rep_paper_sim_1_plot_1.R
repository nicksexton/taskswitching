rm (list = ls())
setwd("~/Programming/c_pdp_models/simulations")
#setwd("~/Thesis/data/simulation_6e")
library (ggplot2)




my.cols <- c("conflict.gain", "conflict.tdwt", "conflict.bias", "mean.0SW", 
"mean.1SW", "scsc.t", "sc.df", "sc.p", "mean.2SW", "mean.ALT", 
"n2rc", "n2rc.t", "n2rc.df", "n2rc.p", "err.12.0SW", "err.12.1SW", "err.12.2SW", 
"err.12.ALT", "timeout.12.0SW", "timeout.12.1SW", "timeout.12.2SW", "timeout.12.ALT",
"err.3.0SW", "err.3.1SW", "err.3.2SW", "err.3.ALT", "timeout.3.0SW", 
"timeout.3.1SW", "timeout.3.2SW", "timeout.3.ALT")

#data.clip.highnoise.0 = read.delim("n2rep_paper_simulation_1_results.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE)
data.clip.highnoise.0 = read.delim("n2rep_paper_simulation_1_debugresults.txt", sep=c("\t"), strip.white=TRUE, header=TRUE, stringsAsFactors=FALSE) # small version to look for bug

                                        # trim data to only 4 weight levels (for fitting on single grid row)
#data.clip.highnoise.0 <- subset (data.clip.highnoise.0, (conflict.tdwt == 0 | conflict.tdwt == -10 | conflict.tdwt == -20 | conflict.tdwt == -30))



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
        #      scale_fill_gradient2(high="red", low="blue", mid="white", limits=c(-0.25,0.25)) +
        scale_fill_gradient2(high="red", low="blue", mid="white", limits=c(-0.4,0.4)) +
  labs(fill="effect size (r)") +
  theme (legend.position="right")  
}




## plot.heatmap.sctimesn2rc <- function (data, condition.title) {
## # plots the intersection of SCs and N2RCs by calculating the product of both DVs

##   SCtimesN2RC <- ggplot(data, aes(x=conflict.gain, y=conflict.bias, fill=compress1(intersect)))
##   SCtimesN2RC + geom_raster() +
##     facet_grid( ~ conflict.tdwt) +                                        
##     scale_fill_gradient2(low="green", high="red", na.value="black", limits=c(0,1)) +
##   labs(fill="compress (sqrt(n2rc x sc))") +
## #  theme (legend.position=c(0.87,0.1))
##   theme (legend.position="right")  
## }


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


    plot.effectsize.sctimesn2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_scxn2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }


  plot.effectsize.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_sc.png", sep="")) 
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

    

  plot.effectsize.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "_effsize_n2rc.png", sep="")) 
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

    
}



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





########## FOR COGSCI PAPER #########
plot.heatmaps (data.clip.highnoise.0, condition.title="Simulation 1", image.directory="/home/nickdbn/Dropbox/PhD/Thesis/simulation_results/cogsci_2015/simulation_1", filename.stem="n2rep_paper_sim_1_debug", save=TRUE)



