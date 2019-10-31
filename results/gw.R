library(ggplot2)
library(scales)
library(doBy)


data = read.table("rate-trace.txt", header=T)

data = subset(data, Type %in% c("OutInterests", "InData"))
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

root1 = subset(data.combined, Type=="OutInterests")
root2 = subset(data.combined, Type=="InData")

df = data.frame(Time = root1$Time, Node = root1$Node, Rate = root2$PacketRaw.sum / root1$PacketRaw.sum)
df.test = subset(df, grepl("^gw-", Node))

data1 = read.table("gini.txt", header=T)
test = merge(df.test, data1, by = c("Time", "Node"), sort=F)

fa = levels(data1$Node)

pdf("gw.pdf")

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(test, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=Rate), size=1, color = "blue") +
    geom_line(aes (x=Time, y=Rate), size=0.5) +
    geom_point(aes (x=Time, y=gini), size=1, color = "green") +
    geom_line(aes (x=Time, y=gini), size=0.5) +
    ylab("Packets") +
    facet_wrap(~ Node)
  print(g.root)
}

retval <- dev.off()