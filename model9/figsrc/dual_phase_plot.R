#!/usr/bin/Rscript
args <- commandArgs(T);

# parse the command line arguments
i = 1;
zoombox = 1;
zoomshift = 0
trajectoryFilename_lc = args[i]; i = i + 1;
trajectoryFilename_shc = args[i]; i = i + 1;
outfileName = args[i]; i = i + 1;
if (length(args) >= i + 1 && args[i] == "--zoombox") {
    zoombox = as.numeric(args[i + 1]);
    zoomshift = as.numeric(args[i + 2]);
    i = i + 3;
}
cycleFileName_lc = sub("trajectory", "cycle", trajectoryFilename_lc);
cycleFileName_shc = sub("trajectory", "cycle", trajectoryFilename_shc);

# read the files
d_lc <- read.csv(trajectoryFilename_lc);
dCycle_lc <- read.csv(cycleFileName_lc);
d_shc <- read.csv(trajectoryFilename_shc);
dCycle_shc <- read.csv(cycleFileName_shc);

# pull out the last full cycle
tStart_lc = dCycle_lc$t[length(dCycle_lc$t) - 3] - d_lc$t[2]
tEnd_lc   = dCycle_lc$t[length(dCycle_lc$t)]
dSingleCycle_lc = d_lc[d_lc$t >= tStart_lc & d_lc$t <= tEnd_lc,]
tStart_shc = dCycle_shc$t[length(dCycle_shc$t) - 3] - d_shc$t[2]
tEnd_shc   = dCycle_shc$t[length(dCycle_shc$t)]
dSingleCycle_shc = d_shc[d_shc$t >= tStart_shc & d_shc$t <= tEnd_shc,]

#oversampling=50
#n=length(dSingleCycle$t) * oversampling
#dSingleCycle = data.frame(t=approx(dSingleCycle$t, n=n)$y, a_I2=approx(dSingleCycle$a_I2, n=n)$y, a_h=approx(dSingleCycle$a_h, n=n)$y, a_I3=approx(dSingleCycle$a_I3, n=n)$y)

# trim to the zoom box
dTrimmed_lc = dSingleCycle_lc[dSingleCycle_lc$a_h <= zoombox & dSingleCycle_lc$a_I2 <= zoombox & dSingleCycle_lc$a_I3 >= 1 - zoombox + zoomshift,]
dTrimmed_shc = dSingleCycle_shc[dSingleCycle_shc$a_h <= zoombox & dSingleCycle_shc$a_I2 <= zoombox & dSingleCycle_shc$a_I3 >= 1 - zoombox + zoomshift,]

require(lattice);
require(latticeExtra);
#panelWithSpacedPoints <- function(x,y,z,dt,dotSpacing,...) {
#    stride = floor(dotSpacing/dt);
#    panel.cloud(x,y,z,type="l",...);
#    subsamp=1:floor(length(x)/stride)*stride;
#    panel.cloud(x[subsamp],y[subsamp],z[subsamp],...);
#}


require(tikzDevice);
extension = strsplit(outfileName, "\\.")[[1]][-1]
switch(extension,
       pdf=pdf(outfileName, width=6, height=6),
       tex=tikz(outfileName, standAlone=T, width=4, height=4),
       png=png(outfileName, width=6, height=6, dpi=120),
       )
cloud(a_h ~ a_I2 + a_I3, dTrimmed_shc, screen=list(x=30, y=-45, z=-22),
      scales=list(draw=F), #list(arrows=F, tick.number=3, distance=1.2),
      type="l", col="black",#panel=panelWithSpacedPoints, pch=1,
      #xlab="$a_\\mathrm{I2}$", zlab="$a_\\mathrm{h}$", ylab="$a_\\mathrm{I3}$",
      xlim=c(0,zoombox), ylim=c(1 - zoombox + zoomshift, 1 + zoomshift), zlim=c(0,zoombox),
      dt=(dTrimmed_shc$t[2] - dTrimmed_shc$t[1]), dotSpacing=0.1, cex=2,
      par.settings = list(axis.line = list(col = "transparent"))
      ) + as.layer(
        cloud(a_h ~ a_I2 + a_I3, dTrimmed_lc, screen=list(x=30, y=-45, z=-22),
              scales=list(draw=F), #list(arrows=F, tick.number=3, distance=1.2),
              type="l", col=rgb(0.3,0.7,0.9),#panel=panelWithSpacedPoints, pch=1,
              #xlab="$a_\\mathrm{I2}$", zlab="$a_\\mathrm{h}$", ylab="$a_\\mathrm{I3}$",
              xlim=c(0,zoombox), ylim=c(1 - zoombox + zoomshift, 1 + zoomshift), zlim=c(0,zoombox),
              dt=(dTrimmed_lc$t[2] - dTrimmed_lc$t[1]), dotSpacing=0.1, cex=2,
              par.settings = list(axis.line = list(col = "transparent"))
              )
      )

layer(panel.cloud(a_h ~ a_I2 + a_I3, dTrimmed_shc))
#cloud(a_I3 ~ a_I2 + a_h, d, screen=list(x=30, y=-45, z=-22), scales=list(arrows=T), panel=panelWithSpacedPoints, pch=1, xlim=c(0,0.1), ylim=c(0,0.1), zlim=c(0.9,1))
#cloud(a_I3 ~ a_I2 + a_h, d, screen=list(x=30, y=-45, z=-22), scales=list(arrows=T), panel=panelWithSpacedPoints, pch=1, xlim=c(0,0.01), ylim=c(0,0.01), zlim=c(0.99,1))
#par(mar=c(4,4,4,4))
dev.off()
