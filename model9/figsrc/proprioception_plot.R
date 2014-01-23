#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
d2 <- read.csv(args[2]);
outfile = args[3];

frequency = 1/(d1$duration_a_I2 + d1$duration_a_h + d1$duration_a_I3);
frequency[is.na(frequency)] = 0
#frequency1a = 1/(d1a$duration_a_I2 + d1a$duration_a_h + d1a$duration_a_I3);
frequency2 = 1/(d2$duration_a_I2 + d2$duration_a_h + d2$duration_a_I3);
maxsigma = max(d1$sigma1)
f_sigma = d1$sigma1/maxsigma;
#f_sigma1a = d1a$sigma1/maxsigma;
f_sigma2 = d2$sigma1/maxsigma;

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
plot(f_sigma, frequency, col="black", type="l", ylim=c(0,max(frequency)),
     xlab="fraction of maximal proprioception",
     #xlab="fraction of maximal proprioception ($\\sigma/\\sigma_\\mathrm{max}$)",
     ylab="frequency (Hz)");#, lwd=3);
#lines(f_sigma1a, frequency1a, col="red", type="l");#, lwd=3);
lines(f_sigma2, frequency2, col="blue", type="l", lty=2);#, lwd=3);
legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
