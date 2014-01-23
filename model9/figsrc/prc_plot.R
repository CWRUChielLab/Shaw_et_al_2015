#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
dc <- read.csv(args[2]); # control data (without perturbation)
outfile = args[3];

periodc = dc$duration_a_I2[1] + dc$duration_a_h[1] + dc$duration_a_I3[1];
time0 = dc$start_a_I2[1]

phase1 = (d1$t_kick - d1$t_kick[1] - d1$t_kick_rel_I2) / periodc
delta1 = (d1$start_a_I2[phase1 >= 0 & phase1 <= 1] - time0) / periodc
phase1 = phase1[phase1 >= 0 & phase1 <= 1]

#require(tikzDevice);
extension = strsplit(outfile, "\\.")[[1]][-1]
print(extension)
switch(extension,
       pdf=pdf(outfile, width=12, height=4),
       #tex=tikz("test.tex", standAlone=F, width=6, height=3)
       png=png(outfile, width=12, height=3),
       )
par(mar=c(2,2,1,1))
plot(phase1, delta1, col="black", type="l", xlab="phase when perturbation is applied", ylab="change in asymptotic phase");
dev.off()
