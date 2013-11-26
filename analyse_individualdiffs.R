# R data analysis of mixed trials data
# plots conditions (Word reading vs. colour naming,
# for Neutral, Congruent and Incongruent trials


library(ggplot2) # for graphs
library(Hmisc)

indiffData<- read.delim("gs_stroop_data.txt")

#graphMeans + stat_summary(fun.y = mean, geom = "bar", position="dodge") + stat_summary (fun.data = mean_cl_normal, geom = "errorbar", position = position_dodge(width = 0.90), width = 0.2) + labs(x = "Trial type", y = "Reaction time", fill = "Task")


# scatter plot
# SAMPLE SCATTER PLOT
scatter <- ggplot(indiffData, aes(RT.Cn, RT.Cc))
scatter + geom_point() + geom_smooth(method=lm) + labs(x = "Reaction time (neutral trials)", y = "Reaction time (congruent trials)")
