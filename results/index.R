
library(ggplot2)
library(scales)
library(doBy)


data = read.table("index.txt", header=T)
data$Node = factor(data$Node)
data.test = subset(data, grepl("^gw", Node))
data = droplevels(data.test)
fa = levels(data$Node)

pdf("index.pdf")
# pdf("gini.pdf")

for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=entropy), size=1) +
    geom_line(aes (x=Time, y=entropy), size=0.5) +
    ylab("q=1") +
    facet_wrap(~ Node)
  print(g.root)
}

# pdf("en.pdf")
for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=gini), size=1) +
    geom_line(aes (x=Time, y=gini), size=0.5) +
    ylab("q=2") +
    facet_wrap(~ Node)
  print(g.root)
}

# pdf("t05.pdf")
for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=t05), size=1) +
    geom_line(aes (x=Time, y=t05), size=0.5) +
    ylab("q=0.5") +
    facet_wrap(~ Node)
  print(g.root)
}

# pdf("enrate.pdf")
# for(i in 1:ceiling(length(fa)/16)){
#   sub_class=fa[c(1:16)+(i-1)*16]
#   g.root <- ggplot(subset(data, Node %in% sub_class)) +
#     geom_point(aes (x=Time, y=enrate), size=1) +
#     geom_line(aes (x=Time, y=enrate), size=0.5) +
#     ylab("Packets") +
#     facet_wrap(~ Node)
#   print(g.root)
# }

# pdf("ginirate.pdf")
# for(i in 1:ceiling(length(fa)/16)){
#   sub_class=fa[c(1:16)+(i-1)*16]
#   g.root <- ggplot(subset(data, Node %in% sub_class)) +
#     geom_point(aes (x=Time, y=ginirate), size=1) +
#     geom_line(aes (x=Time, y=ginirate), size=0.5) +
#     ylab("Packets") +
#     facet_wrap(~ Node)
#   print(g.root)
# }


# pdf("attack.pdf")
# for(i in 1:ceiling(length(fa)/16)){
#   sub_class=fa[c(1:16)+(i-1)*16]
#   g.root <- ggplot(subset(data, Node %in% sub_class)) +
#     geom_point(aes (x=Time, y=attack), size=1) +
#     geom_line(aes (x=Time, y=attack), size=0.5) +
#     ylab("attack") +
#     facet_wrap(~ Node)
#   print(g.root)
# }


retval <- dev.off()