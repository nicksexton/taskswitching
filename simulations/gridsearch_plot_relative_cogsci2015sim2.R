### Code for plotting gridsearch relative to one level of a variable
### in this case, conflict.tdwt == 0, for answering the question of what benefit conflict units offer

compress.relative <- function (x) 2*((1/(1+exp(-0.05 * x)) - 0.5))
compress.relerr <- function (x) 2*((1/(1+exp(-25 * x)) - 0.5))

plot.relative.sc <- function (data, condition.title) {
  
  sc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=(sc.x - sc.y)))
  sc + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +
    facet_grid( ~ conflict.tdwt.x) +
#      scale_fill_gradient2(midpoint=0, mid="grey70", limits=c(-1,1)) +
      scale_fill_gradient2(midpoint=0, mid="grey70") +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
      ggtitle(paste (condition.title,
                       "Relative Switch costs") ) +
    labs(fill="cycles") +
#    theme (legend.position=c(0.87,0.1))
    theme (legend.position="right")      
}



plot.relative.n2rc <- function (data, condition.title) {

  n2rc <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=(n2rc.x - n2rc.y)))
  n2rc + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +
    facet_grid( ~ conflict.tdwt.x) +
    scale_fill_gradient2(midpoint=0, mid="grey70") +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                       "Relative N-2 Repetition costs") ) +
  labs(fill="cycles") +
  theme (legend.position="right")
}

plot.relative.rt.0SW <- function (data, condition.title) {

labs <- c(-50, -20, -10, 0, 10, 20, 50)
bre <- compress.relative(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.relative(mean.0SW.x - mean.0SW.y)))
  rt.0SW + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +                                        
    facet_grid( ~ conflict.tdwt.x) +
      scale_fill_gradient2(low="blue", high="red", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "Relative RTs, 0SW") ) +
  labs(fill="cycles") +
  theme (legend.position="right")

}

plot.relative.rt.1SW <- function (data, condition.title) {

labs <- c(-50, -20, -10, 0, 10, 20, 50)
bre <- compress.relative(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.relative(mean.1SW.x - mean.1SW.y)))
  rt.0SW + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +
    facet_grid( ~ conflict.tdwt.x) +      
#      scale_fill_gradientn(colours=rt.colour.scale, na.value="black", labels=labs,
#                           breaks=bre, limits=rt.limits) +
      scale_fill_gradient2(low="blue", high="red", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "Relative RTs, 1SW") ) +
  labs(fill="cycles") +
  theme (legend.position="right")

}


plot.relative.rt.2SW <- function (data, condition.title) {

labs <- c(-50, -20, -10, 0, 10, 20, 50)
bre <- compress.relative(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.relative(mean.2SW.x - mean.2SW.y)))
  rt.0SW + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +                                        
    facet_grid( ~ conflict.tdwt.x) +
      scale_fill_gradient2(low="blue", high="red", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +
      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "Relative RTs, 2SW") ) +
  labs(fill="cycles") +
  theme (legend.position="right")

}

plot.relative.rt.ALT <- function (data, condition.title) {

labs <- c(-50, -20, -10, 0, 10, 20, 50)
bre <- compress.relative(labs)
  
  rt.0SW <- ggplot(data, aes(x=task.topdown.str, y=task.input.str, fill=compress.relative(mean.ALT.x - mean.ALT.y)))
  rt.0SW + geom_raster() +
#    facet_wrap( ~ conflict.tdwt.x) +
    facet_grid( ~ conflict.tdwt.x) +      
      scale_fill_gradient2(low="blue", high="red", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +

        geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "Relative RTs, ALT") ) +
  labs(fill="cycles") +
  theme (legend.position="right")

}


plot.relative.logErrors.3.ALT <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.3.ALT.x - err.3.ALT.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(3) relative to conflict.tdwt of 0\n ALT condition") ) +
  #labs(fill="compress (sqrt(n2rc x sc))") +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}

plot.relative.logErrors.3.2SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.3.2SW.x - err.3.2SW.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(3) relative to conflict.tdwt of 0\n 2SW condition") ) +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}


plot.relative.logErrors.3.1SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.3.1SW.x - err.3.1SW.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(3) relative to conflict.tdwt of 0\n 1SW condition") ) +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}

plot.relative.logErrors.3.0SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.3.0SW.x - err.3.0SW.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(3) relative to conflict.tdwt of 0\n 0SW condition") ) +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}


plot.relative.logErrors.12.0SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.12.0SW.x - err.12.0SW.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(1,2) relative to conflict.tdwt of 0\n 0SW condition") ) +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}

plot.relative.logErrors.12.1SW <- function (data, condition.title) {
#Errors(3) in ALT condition

  labs=c(-0.5, -0.2, -0.1, 0, 0.1, 0.2, 0.5)
  bre = compress.relerr(labs)

  
  error.rates <- ggplot(data, aes(x=task.topdown.str, y=task.input.str,
                                  fill=compress.relerr(err.12.1SW.x - err.12.1SW.y)))
  error.rates + geom_raster() +
  facet_wrap( ~ conflict.tdwt.x) +
    scale_fill_gradient2(low="green", high="orange", mid="white", midpoint=0, na.value="black",
                           labels=labs, breaks=bre) +    

      geom_segment (aes(x=5, xend=20, y=3, yend=3)) + # task input str for B,C tasks
      geom_segment (aes(x=12, xend=12, y=1.5, yend=5.0)) + # TD ctrl str for B,C tasks
        ggtitle(paste (condition.title,
                   "\nErrors(1,2) relative to conflict.tdwt of 0\n 1SW condition") ) +
  labs(fill="Error rate difference") +                     
  theme (legend.position=c(0.87,0.1))
}


plot.relative <- function (data, condition.title, image.directory, filename.stem, save=FALSE) {
  plot.relative.sc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_sc.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

  plot.relative.n2rc (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_n2rc.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

  plot.relative.rt.0SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_rt_0SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

  plot.relative.rt.1SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_rt_1SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

  plot.relative.rt.2SW (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_rt_2SW.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

  plot.relative.rt.ALT (data, condition.title)
  if (save==TRUE) {
    image.file <- file.path(image.directory, paste(filename.stem, "rel_rt_ALT.png", sep=""))
    ggsave(filename=image.file, width=200, height=50, units="mm")
  }

#  plot.relative.logErrors.3.0SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors3_0SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }

#  plot.relative.logErrors.3.1SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors3_1SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }

#  plot.relative.logErrors.3.2SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors3_2SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }

#  plot.relative.logErrors.3.ALT (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors3_ALT.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }

#  plot.relative.logErrors.12.0SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors12_0SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }

#  plot.relative.logErrors.12.1SW (data, condition.title)
#  if (save==TRUE) {
#    image.file <- file.path(image.directory, paste(filename.stem, "rel_errors12_1SW.png", sep=""))
#    ggsave(filename=image.file, width=200, height=250, units="mm")
#  }
  

  
}


plot.relative.bytaskseq <- function (data, cond.title, image.directory, file.stem, save=FALSE) {


  plot.relative (subset(data, data$alternation=="AB"),
#                        condition.title=paste("Task sequence AB\n",cond.title, sep=""),
                 condition.title=paste("",cond.title, sep=""),
                 image.directory=image.directory,
                 filename.stem=paste(file.stem, "AB_", sep=""),
                 save=save)

#  plot.relative (subset(data, data$alternation=="BA"),
#                  condition.title=paste("Task sequence BA\n",cond.title, sep=""),
#                  image.directory=image.directory,
#                  filename.stem=paste(file.stem, "BA_", sep=""),
#                  save=save)

#  plot.errormaps (subset(data, data$alternation=="BA"),
#                        condition.title=paste("Task sequence BA\n",cond.title, sep=""),
#                        image.directory=image.directory,
#                        filename.stem=paste(file.stem, "BA_", sep=""),
#                        save=save)

 
  
}
