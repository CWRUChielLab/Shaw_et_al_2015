#!/usr/bin/Rscript

library(fBasics, quietly=T) # used for dagoTest

d <- read.csv("data_with_failed_sbm_ig_noRNLF_fixed.csv")

s <- d$StartB6B9;
e <- d$EndB3;
e[is.na(e) | e < s] = d$EndB6B9[is.na(e) | e < s];
sel = d$Behavior == "thin swallow" & !is.na(s) & !is.na(e)
retraction = (e - s)[sel];
#protraction = na.omit((d$EndI2 - d$StartI2)[d$Behavior == "thin swallow"]);

# save the figure as a pdf
pdf("kde-retraction.pdf", 5, 5);

plot(density(retraction), main="", xlab="Retraction duration (s)", ylab="density")
plot(rug(retraction))

# close the pdf file
dev.off();

# print out key statistics
print(dagoTest(retraction))
print(paste("skewness: ", skewness(retraction)))
print(paste("number of patterns:", length(retraction)))
print(paste("number of patterns:", length(retraction)))
print(paste("number of animals:", length(unique(d$Animal[sel]))))
