#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
d2 <- read.csv(args[2]);
outfile = args[3];

frequency1 = 1/(d1$duration_a_I2 + d1$duration_a_h + d1$duration_a_I3);
frequency2 = 1/(d2$duration_a_I2 + d2$duration_a_h + d2$duration_a_I3);
v_sw1 = -d1$cycle_delta_x_sw * frequency1; # todo: scale by time
v_sw2 = -d2$cycle_delta_x_sw * frequency2;
F_sw1 = d1$F_sw;
F_sw2 = d2$F_sw;

#require(tikzDevice);
extension = strsplit(outfile, "\\.")[[1]][-1]
print(extension)
switch(extension,
       pdf=pdf(outfile, width=5, height=5),
       #tex=tikz("test.tex", standAlone=F, width=6, height=3)
       png=png(outfile, width=16, height=4),
       )
par(mar=c(5,5,1,1), mex=0.5)
#par(mar=c(4,4,4,4))
plot(F_sw1, v_sw1, col="black", type="l",
     xlab="Force on seaweed",
     ylim=c(0,0.1),
     #xlab="fraction of maximal proprioception ($\\sigma/\\sigma_\\mathrm{max}$)",
     ylab="Average seaweed velocity");#, lwd=3);
#lines(f_sigma1a, frequency1a, col="red", type="l");#, lwd=3);
lines(F_sw2, v_sw2, col="blue", type="l", lty=2);#, lwd=3);
#legend("topright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
