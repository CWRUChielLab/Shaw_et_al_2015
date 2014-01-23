#!/usr/bin/Rscript

library(fBasics, quietly=T) # used for dagoTest

args <- commandArgs(T);
dshc <- read.csv(args[1]);
#d1a <- read.csv(args[2]);
dlc <- read.csv(args[2]);
pdf(args[3], width=5, height=5);
par(mfrow=c(2,1), mar=c(5,5,1,1), mex=0.5)

retraction_shc = dshc$duration_a_I3;
retraction_lc = dlc$duration_a_I3;
#protvsret[is.na(protvsret)] = 0
#protvsret2[is.na(protvsret2)] = 0

plot(density(retraction_shc), main="", xlab="SHC retraction duration (s)", ylab="density")
rug(retraction_shc)
plot(density(retraction_lc), main="", xlab="LC retraction duration (s)", ylab="density", col="blue")
rug(retraction_lc, col="blue")


print("SHC: ")
print(dagoTest(retraction_shc))
print(paste("skewness: ", skewness(retraction_shc)))
print(paste("number of patterns:", length(retraction_shc)))
print(paste("number of patterns:", length(retraction_shc)))

print("SLC: ")
print(dagoTest(retraction_lc))
print(paste("skewness: ", skewness(retraction_lc)))
print(paste("number of patterns:", length(retraction_lc)))
print(paste("number of patterns:", length(retraction_lc)))

# close the pdf file
dev.off();
