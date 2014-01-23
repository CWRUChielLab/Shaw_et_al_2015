#!/usr/bin/Rscript

# read in the data for the non-held case
d <- read.csv("data_with_failed_sbm_ig_noRNLF_fixed.csv")
s <- d$StartB6B9;
e <- d$EndB3;
e[is.na(e) | e < d$EndB6B9] = d$EndB6B9[is.na(e) | e < d$EndB6B9];
sel=(d$Behavior == "thin swallow" & (d$Animal == "Q30" | d$Animal == "Q19")
     & !is.na(e-s));
unheldRetractionTimes = (e - s)[sel];
unheldProtractionTimes = (d$EndI2 - d$StartI2)[sel]


# read in the data for the held case
dh <- read.csv("HeldTimings.csv");
heldRetractionTimes = (dh$secondBlock + dh$thirdBlock)[dh$held == "True"];
heldProtractionTimes = na.omit((dh$I2End - dh$I2FirstSpike)[dh$held == "True"])

# save the figure as a pdf
pdf("boxplot-held-protraction-retraction.pdf", 5, 5);

# human-readable labels for each of the groups
group_labels=c("not held", "held")

boxplot(
    unheldProtractionTimes, heldProtractionTimes,
    #names=c("free seaweed", "held seaweed"),
    ylab="Duration (s)",
    border="gray", #boxwex=0.5,
    xlim=c(0,6), ylim=c(0, 4)
);
stripchart(
    list(unheldProtractionTimes, heldProtractionTimes),
    vertical=T, add=T, pch="+", cex=0.5
);
boxplot(
    unheldRetractionTimes, heldRetractionTimes,
    names=c("", ""),
    border="darkgray", col="gray", #boxwex=0.5,
    add=T, at=1:2 + 3
);
stripchart(
    list(unheldRetractionTimes, heldRetractionTimes),
    vertical=T, add=T, pch="+", cex=0.5,
    at=1:2 + 3
);

# R doesn't handle multi-line tick labels well, so we'll draw them manually
for (i in 1:length(group_labels)) {
    mtext(group_labels[i], at=i, side=1, line=1, las=2);
    mtext(group_labels[i], at=i + 3, side=1, line=1, las=2);
}
mtext("Protraction", at=1.5, side=3, line=1, las=0);
mtext("Retraction", at=1.5 + 3, side=3, line=1, las=0);

# print out key statistics
print("Protraction:");
print(wilcox.test(unheldProtractionTimes, heldProtractionTimes))
print("Retraction:");
print(wilcox.test(unheldRetractionTimes, heldRetractionTimes))
print(paste("Held:    animals: ", length(unique(dh$animal[dh$held == "True"])),
    ", patterns: ", length(heldProtractionTimes)));
print(paste("Unheld:    animals: ", length(unique(d$Animal[sel])),
    ", patterns: ", length(unheldProtractionTimes)));

# close the pdf file
dev.off();
