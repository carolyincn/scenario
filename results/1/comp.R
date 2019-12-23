library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)
library(reshape2)



data = read.table("comp.txt", header=T)
df <- melt(data, id.var = c("Time"))
names(df) <- c('Time', 'Type', 'Rate')

pdf("comp.pdf", width=8, height=4)
g.root <- ggplot(subset(df, Type %in% c('normal', 'yingdui200'))) +
  geom_point(aes (x=Time, y=Rate, color=Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
  scale_x_continuous(breaks=seq(0, 300, 30)) +
  ylab("Rate") +
  scale_colour_discrete(labels=c("Without mitigation", "With mitigation")) +
  scale_shape_discrete(labels=c("Without mitigation", "With mitigation")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)

g.root <- ggplot(subset(df, Type %in% c('yingdui200', 'yingdui50'))) +
  geom_point(aes (x=Time, y=Rate, color=Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
  scale_x_continuous(breaks=seq(0, 300, 30)) +
  ylab("Rate") +
  scale_colour_discrete(labels=c("Attack rate 200p/s", "Attack rate 30p/s")) +
  scale_shape_discrete(labels=c("Attack rate 200p/s", "Attack rate 30p/s")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)

g.root <- ggplot(subset(df, Type %in% c('yingdui200', 'lianggeqianzhui'))) +
  geom_point(aes (x=Time, y=Rate, color=Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
  scale_x_continuous(breaks=seq(0, 300, 30)) +
  ylab("Rate") +
  scale_colour_discrete(labels=c("one evil prefix", "two evil prefixes")) +
  scale_shape_discrete(labels=c("one evil prefix", "two evil prefixes")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)