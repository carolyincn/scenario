
library(ggplot2)
library(scales)
library(doBy)
library(reshape2)


# rate = read.table("rate-trace.txt", header=T)
# rate$Node = factor(rate$Node)
# # rate$PacketRaw <- rate$PacketRaw
# rate$Type = factor(rate$Type)

# rate.combined = summaryBy(. ~ Time + Node + Type, data=rate, FUN=sum)
# root1 = subset(rate.combined, Type=="OutInterests")
# root2 = subset(rate.combined, Type=="InData")
# df = data.frame(Time = root1$Time, Node = root1$Node, Rate = root2$PacketRaw.sum / root1$PacketRaw.sum)
# df.test = subset(df, grepl("^good-leaf-", Node))
# df.avg = summaryBy(. ~ Time, data=df.test, FUN=mean)

data = read.table("index.txt", header=T)
data$Node = factor(data$Node)
data.test = subset(data, grepl("^gw", Node))
data = droplevels(data.test)
fa = levels(data$Node)
data$id = seq_len(nrow(data))
df <- melt(data, id.var = c("Time", "Node"))
names(df) <- c('Time', 'Node', 'Type', 'Rate')
df = subset(df, Type %in% c('enrate', 'ginirate', 't05rate'))
df = subset(df, Time != 1)

pdf("incrate.pdf")
for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(df, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=Rate, color=Type, shape=Type), size=1) +
    geom_line(aes (x=Time, y=Rate, color=Type), size=0.5) +
    ylab("Packets") +
    facet_wrap(~ Node)
  print(g.root)
}

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(df, Node %in% sub_class & Type=='enrate')) +
    geom_point(aes (x=Time, y=Rate), size=1) +
    geom_line(aes (x=Time, y=Rate), size=0.5) +
    ylab("q=1") +
    facet_wrap(~ Node)
  print(g.root)
}

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(df, Node %in% sub_class & Type=='ginirate')) +
    geom_point(aes (x=Time, y=Rate), size=1) +
    geom_line(aes (x=Time, y=Rate), size=0.5) +
    ylab("q=2") +
    facet_wrap(~ Node)
  print(g.root)
}

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(df, Node %in% sub_class & Type=='t05rate')) +
    geom_point(aes (x=Time, y=Rate), size=1) +
    geom_line(aes (x=Time, y=Rate), size=0.5) +
    ylab("q=0.5") +
    facet_wrap(~ Node)
  print(g.root)
}




retval <- dev.off()