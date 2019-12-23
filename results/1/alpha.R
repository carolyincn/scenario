library(ggplot2)
library(scales)
library(doBy)
library(grid)
library(gridExtra)
library(reshape2)

data = read.table("alpha.txt", header=T)
df <- melt(data, id.var = c("alpha"))
names(df) <- c('alpha', 'Type', 'rate')

pdf("alpha.pdf", width=8, height=4)
g.root <- ggplot(df) +
  geom_point(aes (x=alpha, y=rate, color=Type, shape = Type), size=2) +
  geom_line(aes (x=alpha, y=rate, color=Type), size=0.5) +
  # scale_x_continuous(breaks=seq(0, 180, 30)) +
  ylab("Rate") +
  scale_colour_discrete(labels=c("FP", "FN")) +
  scale_shape_discrete(labels=c("FP", "FN")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)
