#!/usr/bin/Rscript
args <- commandArgs(T);
d <- read.csv(args[1]);
pdf(args[2], width=6, height=3);
par(mar=c(2,2,1,1))
x_o_closed = mapply(function(x_o, a_h, a_I3) { if (a_h + a_I3 >= 0.5) return(x_o) else return(NaN); }, d$x_o, d$a_h, d$a_I3)
plot(d$t, d$x_o, col="black", type="l", ylim=c(0,1), xlim=c(270,300));
lines(d$t, x_o_closed, col="darkgreen", type="l", lw=5);
dev.off()
