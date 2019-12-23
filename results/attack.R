
library(ggplot2)
library(scales)
library(doBy)
library(reshape2)


data = read.table("index.txt", header=T)
data$Node = factor(data$Node)
data.test = subset(data, grepl("^gw", Node))
data = droplevels(data.test)
fa = levels(data$Node)
data = subset(data, select=c('Time', 'Node', 'apa', 'cusum'))
df <- melt(data, id.var = c("Time", "Node"))
names(df) <- c('Time', 'Node', 'Type', 'Rate')
# df = subset(df, Type %in% c('enrate', 'ginirate', 't05rate'))
df = subset(df, Time != 1)

pdf("attack.pdf")

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(df, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=Rate, color=Type, shape=Type), size=1) +
    geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
    ylab("attack") +
    scale_x_continuous(breaks=seq(0, 300, 30)) +
    facet_wrap(~ Node)
  print(g.root)
}

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=apa), size=1) +
    geom_line(aes (x=Time, y=apa), size=0.5) +
    ylab("attack") +
    scale_x_continuous(breaks=seq(0, 300, 30)) +
    facet_wrap(~ Node)
  print(g.root)
}

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=cusum), size=1) +
    geom_line(aes (x=Time, y=cusum), size=0.5) +
    ylab("attack") +
    scale_x_continuous(breaks=seq(0, 300, 30)) +
    facet_wrap(~ Node)
  print(g.root)
}