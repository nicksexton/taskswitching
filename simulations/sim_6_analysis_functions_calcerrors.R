# Change algorithms to calculate error rates 

##################################### Preprocess Data ##################################
########################################################################################



split.trialpath <- function (x) {
  x$trialpath <- as.character(x$trialpath)
  transform (x, PATH = colsplit(trialpath, pattern = "\\:", names=c('block', 'trial')))
}


# evaluates to TRUE of FALSE
trial.is.correct <- function (x, max.cycles) 
  with (x, cycles < max.cycles & (ifelse (cue == 0, stim_0 == response %% 2,
                                          ifelse (cue == 1, stim_1 == response %% 2,
                                                  ifelse (cue == 2, stim_2 == response %% 2, NA)))))


# returns TRUE if all trials with same PATH.block are correct
# note trialpath needs to be unique for each row
block.is.correct <- function (x) {

  num.blocks <- length(unique(x$PATH.block))
  block <- vector (mode="numeric", length=num.blocks) 
  correct.block <- vector (mode="logical", length=num.blocks)
  
  for (i in unique(x$PATH.block)) {
    this.block <- subset (x, PATH.block == i)
    block[i+1] <- i
    correct.block[i+1] <- (all (this.block$correct.trial == TRUE))
  }
       
  x <- merge (x, data.frame (block, correct.block), by.x="PATH.block", by.y="block")
  # return (data.frame (block, correct.block))
  return (x)
}


# faster
block.is.correct1 <- function (x) {

  num.blocks <- length(unique(x$PATH.block))
  block <- vector (mode="numeric", length=num.blocks) 
  correct.block <- vector (mode="logical", length=num.blocks)

  reduce <- subset (x, select=c("PATH.block", "correct.trial"))
  
  for (i in unique(x$PATH.block)) {
    this.block <- subset (reduce, PATH.block == i)
    block[i+1] <- i
    correct.block[i+1] <- (all (this.block$correct.trial == TRUE))
  }
       
  x <- merge (x, data.frame (block, correct.block), by.x="PATH.block", by.y="block")
  # return (data.frame (block, correct.block))
  return (x)
}


block.is.correct2 <- function (x) {

  correct.block <- vector (mode="logical", length=nrow(x))
  for (i in 1:nrow(x)) {
    
    if ((x[i,]$PATH.trial == 0) & (x[i+1,]$PATH.trial == 1) & (x[i+2,]$PATH.trial == 2)) { # check optional if data frame sorted
      if ((x[i,]$PATH.block == x[i+1,]$PATH.block) & (x[i,]$PATH.block == x[i+2,]$PATH.trial)) { # check optional if data frame sorted
        ifelse ((x[i,]$correct.trial & x[i+1,]$correct.trial & x[i+2,]$correct.trial), correct.block[i:i+2] <- TRUE, correct.block[i:i+2] <- FALSE)
      }
    }

  }
  cbind (x, correct.block)
  # return (data.frame (block, correct.block))
  return (x)
}


block.is.correct3 <- function (x) {
                                        # fastest version
                                        # same as version 2 but disregards safety checks that data frame
                                        # is correctly sorted (in order of trialid)
  
  correct.block <- vector (mode="logical", length=nrow(x))
  for (i in 1:nrow(x)) {
   
    if (x[i,]$PATH.trial == 0) {
      ifelse ((x[i,]$correct.trial & x[i+1,]$correct.trial & x[i+2,]$correct.trial), correct.block[i:(i+2)] <- TRUE, correct.block[i:(i+2)] <- FALSE)
    }
  }
  x <- cbind (x, correct.block)
  return (x)
}


block.is.trials12.correct <- function (x) {
# only checks whether trials 1 & 2 are correct.
# Useful if you want to assess whether trial 3 is correct independently

  correct.block12 <- vector (mode="logical", length=nrow(x))
  for (i in 1:nrow(x)) {
   
    if (x[i,]$PATH.trial == 0) {
      ifelse ((x[i,]$correct.trial & x[i+1,]$correct.trial),
              correct.block12[i:(i+2)] <- TRUE,
              correct.block12[i:(i+2)] <- FALSE)
    }
  }
  x <- cbind (x, correct.block12)
  return (x)
  
}

# combine in a process.data function
process.data <- function (x, max.cycles = 500) {
  x <- split.trialpath (x)
  x$correct.trial <- trial.is.correct (x, max.cycles)
  x <- block.is.trials12.correct (x)


# select PATH.trial == 3 only?!
  # Don't filter data here as it's needed to calc errors,
  # this needs to be done by whatever calling function calculates DVs
#  x <- subset (x, PATH.trial == 2) # make sure this gets done before RT is calculated!
  
  return (x)
}

################################# Stat Analysis #####################################
#####################################################################################

descriptives <- function (x) by (x$cycles, x$sequence_cond, stat.desc)

calculate.RT.mean <- function (x, condition) {
  ifelse (nrow(x[x$sequence_cond==condition]) == 0,
          NA,
          descriptives(x)[[condition]][["mean"]])
}
calculate.RT.sd <- function (x, condition) {
  ifelse (nrow(x[x$sequence_cond==condition]) == 0,
          NA,
          descriptives(x)[[condition]][["std.dev"]])
}

calculate.RT.mean.unsafe <- function (x, condition) {
                                        # returns an error if length of x is zero (eg if all trials fail)
                                        # use safe version if this needs to be checked
  descriptives(x)[[condition]][["mean"]]

}

calculate.RT.sd.unsafe <- function (x, condition) {
                                        # returns an error if length of x is zero (eg if all trials fail)
                                        # use safe version if this needs to be checked
  descriptives(x)[[condition]][["std.dev"]]
}


tabulate.RT <- function (data) {

  output <- data.frame (sequence = c("0SW", "1SW", "2SW", "ALT"))
  for (i in 1:4) {
    output$mean[i] <- calculate.RT.mean (data, output$sequence[i])
    output$sd[i] <- calculate.RT.sd (data, output$sequence[i])
  }

  return (output)
}






test.switchcost <- function (x)
  t.test (cycles ~ sequence_cond, data = subset(x, sequence_cond == "0SW" | sequence_cond =="1SW"))

test.n2rc <- function (x)
  t.test (cycles ~ sequence_cond, data = subset(x, sequence_cond == "2SW" | sequence_cond =="ALT"))


calculate.switchcost <- function (x){
  ifelse (nrow(subset(x, x$"sequence_cond"=="0SW")) > 5 &
          nrow(subset(x, x$"sequence_cond"=="1SW")) > 5,
          results <- data.frame (           
            mean.0SW = calculate.RT.mean.unsafe (x, "0SW"),
            mean.1SW = calculate.RT.mean.unsafe (x, "1SW"),
            sc = calculate.RT.mean.unsafe (x, "1SW") - calculate.RT.mean.unsafe (x, "0SW"),
            t = test.switchcost(x)[[1]][[1]],
            df = test.switchcost(x)[[2]][[1]],
            p = test.switchcost(x)[[3]][[1]]),
          results <- data.frame (
            mean.0SW = NA,
            mean.1SW = NA,
            sc = NA,
            t = NA,
            df = NA,
            p = NA)
          )


  return (results)

}

calculate.n2rc <- function (x){
  ifelse (nrow(subset(x, x$"sequence_cond"=="2SW")) > 5 &
        nrow(subset(x, x$"sequence_cond"=="ALT")) > 5,

            results <- data.frame (
              mean.2SW = calculate.RT.mean.unsafe (x, "2SW"),
              mean.ALT = calculate.RT.mean.unsafe (x, "ALT"),
              n2rc = calculate.RT.mean.unsafe (x, "ALT") - calculate.RT.mean.unsafe (x, "2SW"),
              t = test.n2rc(x)[[1]][[1]],
              df = test.n2rc(x)[[2]][[1]],
              p = test.n2rc(x)[[3]][[1]]),
            results <- data.frame (
              mean.0SW = NA,
              mean.1SW = NA,
              sc = NA,
              t = NA,
              df = NA,
              p = NA
              )
            )
    return (results)
}

calculate.errors <- function (errorcounts, condition) {

                                        # check data has been passed in, else no trials survived to be checked!
                                        # (eg 100% error rate on trials 12, filtered before checking trial 3)
  if (length(errorcounts) == 0) {
    return (c(err.0SW=NA, err.1SW=NA, err.2SW=NA, err.ALT=NA))
  }
  
  errors.tab <- table (errorcounts, condition)
  if (all(dim (errors.tab) == c(2,4))) {  # if table has two rows
    error.rates <- errors.tab["FALSE",] / (2 * apply (X=errors.tab, MARGIN=2, FUN=sum)) # calc error rates
  } else {
    if (attr(errors.tab, "dimnames")[1] == TRUE) {
      error.rates <- c(err.0SW=0, err.1SW=0, err.2SW=0, err.ALT=0) # else
    }
    else {
      error.rates <- c(err.0SW=1, err.1SW=1, err.2SW=1, err.ALT=1) # else
    }
  } 

  return (error.rates)
}
  
