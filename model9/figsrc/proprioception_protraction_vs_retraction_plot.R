#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
d2 <- read.csv(args[2]);
pdf(args[3], width=5, height=5);
par(mar=c(5,5,1,1), mex=0.5)
protvsret = d1$duration_a_I2/(d1$duration_a_h + d1$duration_a_I3);
protvsret2 = d2$duration_a_I2/(d2$duration_a_h + d2$duration_a_I3);
protvsret[is.na(protvsret)] = 0
protvsret2[is.na(protvsret2)] = 0

maxsigma = max(d1$sigma1)
f_sigma = d1$sigma1/maxsigma;
f_sigma2 = d2$sigma1/maxsigma;

plot(f_sigma, protvsret, col="black", type="l", ylim=c(0,max(protvsret)), xlab="fraction of maximal proprioception", ylab="ratio of protraction to retraction");
lines(f_sigma2, protvsret2, col="blue", type="l", lty=2);
legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
