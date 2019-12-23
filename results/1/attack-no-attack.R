# Copyright (c) 2012,2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)
library(reshape2)


#########################
# Rate trace processing #
#########################
data = read.table("攻击vs无攻击.txt", header=T)
data$id = seq_len(nrow(data))
df <- melt(data, id.var = c("id", "Time"))
names(df) <- c('id', 'Time', 'Type', 'entropy')

pdf("攻击vs无攻击.pdf", width=8, height=4)
g.root <- ggplot(subset(df, Type %in% c('entropy1', 'entropy2'))) +
  geom_point(aes (x=Time, y=entropy, color=Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=entropy, color=Type), size=0.5) +
  scale_x_continuous(breaks=seq(0, 180, 30)) +
  ylab("Entropy") +
  scale_colour_discrete(labels=c("Without attack", "With attack")) +
  scale_shape_discrete(labels=c("Without attack", "With attack")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)

g.root <- ggplot(subset(df, Type %in% c('gini1', 'gini2'))) +
  geom_point(aes (x=Time, y=entropy, color=Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=entropy, color=Type), size=0.5) +
  scale_x_continuous(breaks=seq(0, 180, 30)) +
  ylab("Gini") +
  scale_colour_discrete(labels=c("Without attack", "With attack")) +
  scale_shape_discrete(labels=c("Without attack", "With attack")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)

retval <- dev.off()