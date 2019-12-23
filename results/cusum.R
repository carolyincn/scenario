library(ggplot2)
library(scales)
library(doBy)
library(reshape2)


data = read.table("index.txt", header=T)



pdf("cusum.pdf", width=8, height=4)

g.root <- ggplot(subset(data, Node=='gw-1')) +
  geom_point(aes (x=Time, y=m_cusum), size=1) +
  geom_line(aes (x=Time, y=m_cusum), size=0.5) +
  ylab("attack") +
  scale_x_continuous(breaks=seq(0, 300, 30))+
  # scale_y_continuous(breaks=seq(0, 1, 1))+
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank())
print(g.root)

g.root <- ggplot(subset(data, Node=='gw-9')) +
  geom_point(aes (x=Time, y=m_cusum), size=1) +
  geom_line(aes (x=Time, y=m_cusum), size=0.5) +
  ylab("attack") +
  scale_x_continuous(breaks=seq(0, 300, 30))+
  # scale_y_continuous(breaks=seq(0, 1, 1))+
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank())
print(g.root)

g.root <- ggplot(subset(data, Node=='gw-16')) +
  geom_point(aes (x=Time, y=m_cusum), size=1) +
  geom_line(aes (x=Time, y=m_cusum), size=0.5) +
  ylab("attack") +
  scale_x_continuous(breaks=seq(0, 300, 30))+
  # scale_y_continuous(breaks=seq(0, 1, 1))+
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank())
print(g.root)