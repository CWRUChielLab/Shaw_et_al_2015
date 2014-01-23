#!/usr/bin/Rscript
args <- commandArgs(T);
d <- read.csv(args[1]);
pdf(args[2], width=12, height=4);
plot(d$t, d$a_I2, col="blue", type="l", ylim=c(0,1), xlim=c(270,300));
lines(d$t, d$a_h, col="red");
lines(d$t, d$a_I3, col="goldenrod");
lines(d$t, d$s);
dev.off()
