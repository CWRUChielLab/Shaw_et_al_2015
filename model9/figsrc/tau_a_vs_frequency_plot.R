#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
d2 <- read.csv(args[2]);
outfile = args[3];

frequency1 = 1/(d1$duration_a_I2 + d1$duration_a_h + d1$duration_a_I3);
frequency2 = 1/(d2$duration_a_I2 + d2$duration_a_h + d2$duration_a_I3);
tau_a1 = d1$tau_a;#/0.05;
tau_a2 = d2$tau_a;#/0.6;

frequency1[is.na(frequency1)] = 0
frequency2[is.na(frequency2)] = 0

#require(tikzDevice);
extension = strsplit(outfile, "\\.")[[1]][-1]
print(extension)
switch(extension,
       pdf=pdf(outfile, width=5, height=5),
       #tex=tikz("test.tex", standAlone=F, width=6, height=3)
       png=png(outfile, width=16, height=4),
       )
par(mar=c(5,5,1,1), mex=0.5)

if (T) {
    # x scaled, log-linear plot
    plot(log(tau_a1/0.05), log(frequency1), col="black", type="l",
         xlim=c(-2,2),
         xlab="Log of scaled neural time constant",
         ylab="Log of frequency",
         ylim=c(-3,1)
         );#, lwd=3);
    lines(log(tau_a2/0.15), log(frequency2), col="blue", type="l", lty=2);#, lwd=3);
    abline(v = 0, col="gray")
    #legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
}
dev.off()
