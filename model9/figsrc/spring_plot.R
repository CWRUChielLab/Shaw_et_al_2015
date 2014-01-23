#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
d2 <- read.csv(args[2]);
pdf(args[3], width=5, height=5);
par(mar=c(5,5,1,1), mex=0.5)
frequency = 1/(d1$duration_a_I2 + d1$duration_a_h + d1$duration_a_I3);
#frequency1a = 1/(d1a$duration_a_I2 + d1a$duration_a_h + d1a$duration_a_I3);
frequency2 = 1/(d2$duration_a_I2 + d2$duration_a_h + d2$duration_a_I3);
frequency[is.na(frequency)] = 0
k_spring = d1$k_spring;
#k_spring1a = d1a$k_spring;
k_spring2 = d2$k_spring;
plot(k_spring, frequency, col="black", type="l", ylim=c(0,max(frequency)), xlab="spring coefficient", ylab="frequency (Hz)");
#lines(k_spring1a, frequency1a, col="red", type="l");
lines(k_spring2, frequency2, col="blue", type="l", lty=2);
legend("bottomleft", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
