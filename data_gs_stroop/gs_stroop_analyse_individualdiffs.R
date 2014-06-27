# R data analysis of mixed trials data
# plots conditions (Word reading vs. colour naming,
# for Neutral, Congruent and Incongruent trials


library(ggplot2) # for graphs
library(Hmisc)

indiffData<- read.delim("gs_stroop_data.txt")

#graphMeans + stat_summary(fun.y = mean, geom = "bar", position="dodge") + stat_summary (fun.data = mean_cl_normal, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + labs(x = "Trial type", y = "Reaction time", fill = "Task")



# SCATTER PLOTS
scatter.tdwt.ri <- ggplot(indiffData, aes(TDwt_i, RI, colour=result))
scatter.tdwt.ri +
  geom_point() +
  geom_smooth(method=lm, aes(fill=result), alpha=0.1) +
  labs(x = "Task Demand -> Output units inhib. weight", y = "Stroop Interference (RTi - RTc)") +
  ylim(-10, 100)

scatter.tdwt.tswc <- ggplot(indiffData, aes(TDwt_i, TSwc, colour=result))
scatter.tdwt.tswc +
  geom_point() +
  geom_smooth(method=lm, aes(fill=result), alpha=0.1) +
  labs(x = "Task Demand -> Output units inhib. weight", y = "Task Switch Cost (word -> colour)") +
  ylim(-20, 20)


scatter.tdwt.tscw <- ggplot(indiffData, aes(TDwt_i, TScw, colour=result))
scatter.tdwt.tscw +
  geom_point() +
  geom_smooth(method=lm, aes(fill=result), alpha=0.1) +
  labs(x = "Task Demand -> Output units inhib. weight", y = "Task Switch Cost (colour -> word)") 


scatter.ri.tswc <- ggplot(indiffData, aes(RI, TSwc, colour=result))
scatter.ri.tswc +
  geom_point() + geom_smooth(method=lm, aes(fill=result), alpha=0.1) +
  labs(x = "Stroop Interference (RTi - RTc)", y = "Task Switch Cost (word -> colour)") +
  xlim (-10, 100) +
  ylim(-20, 20)


scatter.ri.tscw <- ggplot(indiffData, aes(RI, TScw, colour=result))
scatter.ri.tscw +
  geom_point() + geom_smooth(method=lm, aes(fill=result), alpha=0.1) +
  labs(x = "Stroop Interference (RTi - RTc)", y = "Task Switch Cost (colour -> word)") 


# CORRELATIONS
indiffDataCorrect <- subset (indiffData, result=="Cor")
cor(indiffDataCorrect[, c("TDwt_i", "RI", "TSwc", "TScw")])
