#!/usr/bin/Rscript
args <- commandArgs(T);
d1 <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
d2 <- read.csv(args[2]);
pdf(args[3], width=5, height=5);
par(mar=c(5,5,1,1), mex=0.5)
protvsret = d1$duration_a_I2/(d1$duration_a_h + d1$duration_a_I3);
#frequency1a = 1/(d1a$duration_a_I2 + d1a$duration_a_h + d1a$duration_a_I3);
protvsret2 = d2$duration_a_I2/(d2$duration_a_h + d2$duration_a_I3);
protvsret[is.na(protvsret)] = 0
protvsret2[is.na(protvsret2)] = 0
k_spring = d1$k_spring;
#k_spring1a = d1a$k_spring;
k_spring2 = d2$k_spring;
plot(k_spring, protvsret, col="black", type="l", ylim=c(0,max(protvsret)), xlab="spring coefficient", ylab="ratio of protraction to retraction");
#lines(k_spring1a, frequency1a, col="red", type="l");
lines(k_spring2, protvsret2, col="blue", type="l", lty=2);
legend("bottomleft", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
