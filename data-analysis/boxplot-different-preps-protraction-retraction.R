#!/usr/bin/Rscript

d <- read.csv("data_with_failed_sbm_ig_noRNLF_fixed.csv")

s <- d$StartB6B9;
e <- d$EndB3;
e[is.na(e) | e < s] = d$EndB6B9[is.na(e) | e < s];
retraction = e - s;
protraction = d$EndI2 - d$StartI2

sel=(
     (d$Behavior == "SBMbite" | d$Behavior == "IGcarb" | d$Behavior == "bite")
     #(d$Behavior == "SBMswallow" | d$Behavior == "IGcarb" | d$Behavior == "thin swallow")
     & !is.na(protraction) & !is.na(retraction)
     );
behavior = factor(d$Behavior[sel, drop=T], c("bite", "SBMbite", "IGcarb"));
#behavior = factor(d$Behavior[sel, drop=T], c("thin swallow", "SBMswallow", "IGcarb"));

# save the figure as a pdf
pdf("boxplot-different-preps-protraction-retraction.pdf", 5, 5);

# human-readable labels for each of the groups
group_labels=c("in vivo", "SBM", "ganglia")

boxplot(
    protraction[sel] ~ behavior,
    names=rep("", length(group_labels)),
    ylab="Duration (s)",
    border="gray", #boxwex=0.5,
    xlim=c(0,8), ylim=c(0, 17)
);
stripchart(
    protraction[sel] ~ behavior,
    vertical=T, add=T, pch="+", cex=0.5
);
boxplot(
    retraction[sel] ~ behavior,
    names=rep("", length(group_labels)),
    border="darkgray", col="gray", #boxwex=0.5,
    add=T, at=1:3 + 4
);
stripchart(
    retraction[sel] ~ behavior,
    vertical=T, add=T, pch="+", cex=0.5,
    at=1:3 + 4
);

# R doesn't handle multi-line tick labels well, so we'll draw them manually
for (i in 1:length(group_labels)) {
    mtext(group_labels[i], at=i, side=1, line=1, las=2);
    mtext(group_labels[i], at=i+4, side=1, line=1, las=2);
}
mtext("Protraction", at=2, side=3, line=1, las=0);
mtext("Retraction", at=2 + 4, side=3, line=1, las=0);

# close the pdf file
dev.off();

# print out key statistics
print("Protraction:");
print(kruskal.test(protraction[sel] ~ behavior))
#print(summary(aov(protraction[sel] ~ behavior + d$Animal[sel, drop=T])))
print("Retraction:");
#print(summary(aov(retraction[sel] ~ behavior + d$Animal[sel, drop=T])))
print(kruskal.test(retraction[sel] ~ behavior))
for (b in unique(behavior)) {
    print(paste(b, ":   animals: ",
        length(unique(d$Animal[sel][behavior == b, drop=T])),
        " patterns: ", sum(behavior == b))
    );
}

#print(paste("number of patterns:", length(retraction)))
#print(paste("number of animals:", length(unique(d$Animal[sel]))))
