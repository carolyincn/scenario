library(ggplot2)
library(scales)
library(doBy)
library(grid)
library(gridExtra)

data = read.table("tsallis.txt", header=T)
data$Node = factor(data$Node)
data.test = subset(data, Node=='gw-1' & Time != 1)


pdf("tsallis.pdf", width=8, height=4)

g1.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=t05), size=1) +
  geom_line(aes (x=Time, y=t05), size=0.5) +
  ggtitle("q = 0.5") +
  ylab("Tq") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 
# print(g1.root)

g2.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=entropy), size=1) +
  geom_line(aes (x=Time, y=entropy), size=0.5) +
  ggtitle("q -> 1") +
  ylab("Tq") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 
# print(g2.root)

g3.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=gini), size=1) +
  geom_line(aes (x=Time, y=gini), size=0.5) +
  ggtitle("q = 2") +
  ylab("Tq") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 
# print(g3.root)

g4.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=t05rate), size=1) +
  geom_line(aes (x=Time, y=t05rate), size=0.5) +
  ggtitle("q = 0.5") +
  ylab("Rate") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 

g5.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=enrate), size=1) +
  geom_line(aes (x=Time, y=enrate), size=0.5) +
  ggtitle("q -> 1") +
  ylab("Rate") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 


g6.root <- ggplot(data.test) +
  geom_point(aes (x=Time, y=ginirate), size=1) +
  geom_line(aes (x=Time, y=ginirate), size=0.5) +
  ggtitle("q = 2") +
  ylab("Rate") +
  scale_x_continuous(breaks=seq(0, 180, 60)) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) 

multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)

  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)

  numPlots = length(plots)

  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                    ncol = cols, nrow = ceiling(numPlots/cols))
  }

 if (numPlots==1) {
    print(plots[[1]])

  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))

    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))

      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}

multiplot(g1.root, g2.root, g3.root, cols=3)
multiplot(g4.root, g5.root, g6.root, cols=3)
