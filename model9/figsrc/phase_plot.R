#!/usr/bin/Rscript
args <- commandArgs(T);

# parse the command line arguments
i = 1;
zoombox = 1;
zoomshift = 0
trajectoryFilename = args[i]; i = i + 1;
outfileName = args[i]; i = i + 1;
if (length(args) >= i + 1 && args[i] == "--zoombox") {
    zoombox = as.numeric(args[i + 1]);
    zoomshift = as.numeric(args[i + 2]);
    i = i + 3;
}
cycleFileName = sub("trajectory", "cycle", trajectoryFilename);

# read the files
d <- read.csv(trajectoryFilename);
dCycle <- read.csv(cycleFileName);

# pull out the last full cycle
tStart = dCycle$t[length(dCycle$t) - 3] - d$t[2]
tEnd   = dCycle$t[length(dCycle$t)]

dSingleCycle = d[d$t >= tStart & d$t <= tEnd,]

oversampling=50
n=length(dSingleCycle$t) * oversampling
dSingleCycle = data.frame(t=approx(dSingleCycle$t, n=n)$y, a_I2=approx(dSingleCycle$a_I2, n=n)$y, a_h=approx(dSingleCycle$a_h, n=n)$y, a_I3=approx(dSingleCycle$a_I3, n=n)$y)

# trim to the zoom box
dTrimmed = dSingleCycle[dSingleCycle$a_h <= zoombox & dSingleCycle$a_I2 <= zoombox & dSingleCycle$a_I3 >= 1 - zoombox + zoomshift,]

require(lattice);
panelWithSpacedPoints <- function(x,y,z,dt,dotSpacing,...) {
    stride = floor(dotSpacing/dt);
    panel.cloud(x,y,z,type="l",...);
    subsamp=1:floor(length(x)/stride)*stride;
    panel.cloud(x[subsamp],y[subsamp],z[subsamp],...);
}


require(tikzDevice);
extension = strsplit(outfileName, "\\.")[[1]][-1]
switch(extension,
       pdf=pdf(outfileName, width=6, height=6),
       tex=tikz(outfileName, standAlone=T, width=4, height=4),
       png=png(outfileName, width=6, height=6, dpi=120),
       )
cloud(a_h ~ a_I2 + a_I3, dTrimmed, screen=list(x=30, y=-45, z=-22),
      scales=list(arrows=F, tick.number=3, distance=1.2), panel=panelWithSpacedPoints, pch=1,
      #xlab="$a_\\mathrm{I2}$", zlab="$a_\\mathrm{h}$", ylab="$a_\\mathrm{I3}$",
      xlim=c(0,zoombox), ylim=c(1 - zoombox + zoomshift, 1 + zoomshift), zlim=c(0,zoombox),
      dt=(dTrimmed$t[2] - dTrimmed$t[1]), dotSpacing=0.1, cex=2,
      par.settings = list(axis.line = list(col = "transparent")))
#cloud(a_I3 ~ a_I2 + a_h, d, screen=list(x=30, y=-45, z=-22), scales=list(arrows=T), panel=panelWithSpacedPoints, pch=1, xlim=c(0,0.1), ylim=c(0,0.1), zlim=c(0.9,1))
#cloud(a_I3 ~ a_I2 + a_h, d, screen=list(x=30, y=-45, z=-22), scales=list(arrows=T), panel=panelWithSpacedPoints, pch=1, xlim=c(0,0.01), ylim=c(0,0.01), zlim=c(0.99,1))
#par(mar=c(4,4,4,4))
dev.off()
