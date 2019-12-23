
library(ggplot2)
library(scales)
library(doBy)

pdf("q.pdf", width=5, height=4)


fun.1 <- function(x) -(1 - x^(-0.5) - (1 - x)^(-0.5)) / 1.5
fun.0 <- function(x) -(1 - x^(-0.1) - (1 - x)^(-0.1)) / 1.1
fun.2 <- function(x) -(1 - sqrt(x) - sqrt(1 - x)) / 0.5
fun.4 <- function(x) (1 - x*x - (1 - x)*(1-x))
fun.3 <- function(x) -(x * log(x) + (1-x)*log(1-x))

g <- ggplot(data.frame(x=c(0,1)), aes(x)) +
  stat_function(fun=fun.0, geom="line", aes(linetype="q = -0.1", colour="q = -0.1")) +
  stat_function(fun=fun.1, geom="line", aes(linetype="q = -0.5", colour="q = -0.5")) +
  stat_function(fun=fun.2, geom="line", aes(linetype="q = 0.5", colour="q = 0.5")) +
  stat_function(fun=fun.3, geom="line", aes(linetype="q -> 1", colour="q -> 1")) +
  stat_function(fun=fun.4, geom="line", aes(linetype="q = 2", colour="q = 2")) +
  xlab("p") +
  ylab("Tq") +
  scale_linetype_manual("Function", values=c(2, 1, 4, 5, 3), breaks=c("q = -0.5","q = -0.1", "q = 0.5","q -> 1","q = 2")) +
  scale_colour_manual("Function", values=c(2, 3, 4, 6, 1), breaks=c("q = -0.5","q = -0.1", "q = 0.5","q -> 1","q = 2")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank())
  


print(g)