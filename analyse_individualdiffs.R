# R data analysis of mixed trials data
# plots conditions (Word reading vs. colour naming,
# for Neutral, Congruent and Incongruent trials


library(ggplot2) # for graphs
library(Hmisc)

indiffData<- read.delim("gs_stroop_data.txt")

#graphMeans + stat_summary(fun.y = mean, geom = "bar", position="dodge") + stat_summary (fun.data = mean_cl_normal, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + labs(x = "Trial type", y = "Reaction time", fill = "Task")



# SCATTER PLOTS
scatter.tdwt.ri <- ggplot(indiffData, aes(TDwt_i, RI))
scatter.tdwt.ri + geom_point() + geom_smooth(method=lm) + labs(x = "Task Demand -> Output units inhib. weight", y = "Response Inhibition (RTi - RTc)") 

scatter.tdwt.tswc <- ggplot(indiffData, aes(TDwt_i, TSwc))
scatter.tdwt.tswc + geom_point() + geom_smooth(method=lm) + labs(x = "Task Demand -> Output units inhib. weight", y = "Task Switching (word -> colour)") 


scatter.tdwt.tscw <- ggplot(indiffData, aes(TDwt_i, TScw))
scatter.tdwt.tscw + geom_point() + geom_smooth(method=lm) + labs(x = "Task Demand -> Output units inhib. weight", y = "Task Switching (colour -> word)")


scatter.ri.tswc <- ggplot(indiffData, aes(RI, TSwc))
scatter.ri.tswc + geom_point() + geom_smooth(method=lm) + labs(x = "Response Inhibition (RTi - RTc)", y = "Task Switching (word -> colour)") 


scatter.ri.tscw <- ggplot(indiffData, aes(RI, TScw))
scatter.ri.tscw + geom_point() + geom_smooth(method=lm) + labs(x = "Response Inhibition (RTi - RTc)", y = "Task Switching (colour -> word)") 


