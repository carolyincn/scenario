
library(ggplot2)
library(scales)
library(doBy)

data = read.table("gini.txt", header=T)
data$Node = factor(data$Node)

fa = levels(data$Node)
fa
pdf("gini.pdf")

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=gini), size=1) +
    geom_line(aes (x=Time, y=gini), size=0.5) +
    ylab("Packets") +
    facet_wrap(~ Node)
  print(g.root)
}

retval <- dev.off()