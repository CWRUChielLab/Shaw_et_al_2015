#!/usr/bin/Rscript
args <- commandArgs(T);
d_shc <- read.csv(args[1]);
#d_shca <- read.csv(args[2]);
d_u_1 <- read.csv(args[2]);
d_u_2 <- read.csv(args[3]);
d_u_3 <- read.csv(args[4]);
d_u_4 <- read.csv(args[5]);
d_lc <- read.csv(args[6]);
outfile = args[7];

datalist = list(d_shc=d_shc, d_u_1=d_u_1, d_u_2=d_u_2, d_u_3=d_u_3, d_u_4=d_u_4);#, d_lc=d_lc);
datacolors = list("black", "darkred", "goldenrod", "darkgreen", "blue", "gray")

F_sw = NULL;
U_per_length = NULL
successful = NULL
# for each model, extract the measurements from the last period in the simulation
for (i in 1:length(datalist)) {
    successful = rbind(successful, 0 > datalist[[i]]$cycle_delta_x_sw);
    U_per_length = rbind(U_per_length, -((datalist[[i]]$cycle_delta_U_I3 + datalist[[i]]$cycle_delta_U_I2) / datalist[[i]]$cycle_delta_x_sw))
    F_sw = rbind(F_sw, datalist[[i]]$F_sw);
}

#require(tikzDevice);
extension = strsplit(outfile, "\\.")[[1]][-1]
print(extension)
switch(extension,
       pdf=pdf(outfile, width=5, height=5),
       #tex=tikz("test.tex", standAlone=F, width=6, height=3)
       png=png(outfile, width=16, height=4),
       )
par(mar=c(5,5,1,1), mex=0.5)
#par(mar=c(4,4,4,4))
plot(F_sw[1,], U_per_length[1,], col=datacolors[[1]], type="l",
     ylim=c(0,max(U_per_length[1,])),
     xlab="Force on seaweed",
     ylab="Integrated muscle activity per unit length of seaweed");
for (i in 2:length(datalist)) {
    lines(F_sw[i,][successful[i,]], U_per_length[i,][successful[i,]], col=datacolors[[i]], type="l", lty=1);
}
dev.off()
