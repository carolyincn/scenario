library(ggplot2)
library(scales)
library(doBy)
library(grid)
library(gridExtra)
library(reshape2)

data1 = read.table("tsallis-uni.txt", header=T)
data1.test = subset(data1, Node=='gw-1' & Time != 1, select=c('Time', 't05rate'))
data2 = read.table("tsallis-exp.txt", header=T)
data2.test = subset(data2, Node=='gw-1' & Time != 1, select=c('Time', 't05rate'))
test = merge(data1.test, data2.test, by = c("Time"), sort=F)

names(test) <- c('Time', 'Uniform', 'Exponential')
df <- melt(test, id.var = c("Time"))
names(df) <- c('Time', 'Type', 'Rate')

pdf("rate-compare.pdf", width=8, height=4)

g1.root <- ggplot(df) +
  geom_point(aes (x=Time, y=Rate, color=Type, shape=Type), size=2) +
  geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
  # ggtitle("q = 0.5") +
  ylab("Rate") +
  scale_x_continuous(breaks=seq(0, 300, 30)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 
print(g1.root)

