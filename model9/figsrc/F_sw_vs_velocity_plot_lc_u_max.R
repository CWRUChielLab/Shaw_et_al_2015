#!/usr/bin/Rscript
args <- commandArgs(T);
d_shc <- read.csv(args[1]);
#d_shca <- read.csv(args[2]);
d_k_1 <- read.csv(args[2]);
d_k_2 <- read.csv(args[3]);
d_k_3 <- read.csv(args[4]);
d_k_4 <- read.csv(args[5]);
d_lc <- read.csv(args[6]);
outfile = args[7];

frequency_shc = 1/(d_shc$duration_a_I2 + d_shc$duration_a_h + d_shc$duration_a_I3);
frequency_k_1 = 1/(d_k_1$duration_a_I2 + d_k_1$duration_a_h + d_k_1$duration_a_I3);
frequency_k_2 = 1/(d_k_2$duration_a_I2 + d_k_2$duration_a_h + d_k_2$duration_a_I3);
frequency_k_3 = 1/(d_k_3$duration_a_I2 + d_k_3$duration_a_h + d_k_3$duration_a_I3);
frequency_k_4 = 1/(d_k_4$duration_a_I2 + d_k_4$duration_a_h + d_k_4$duration_a_I3);
frequency_lc = 1/(d_lc$duration_a_I2 + d_lc$duration_a_h + d_lc$duration_a_I3);
v_sw_shc = -d_shc$cycle_delta_x_sw * frequency_shc; # todo: scale by time
v_sw_k_1 = -d_k_1$cycle_delta_x_sw * frequency_k_1;
v_sw_k_2 = -d_k_2$cycle_delta_x_sw * frequency_k_2;
v_sw_k_3 = -d_k_3$cycle_delta_x_sw * frequency_k_3;
v_sw_k_4 = -d_k_4$cycle_delta_x_sw * frequency_k_4;
v_sw_lc = -d_lc$cycle_delta_x_sw * frequency_lc;
F_sw_shc = d_shc$F_sw;
F_sw_k_1 = d_k_1$F_sw;
F_sw_k_2 = d_k_2$F_sw;
F_sw_k_3 = d_k_3$F_sw;
F_sw_k_4 = d_k_4$F_sw;
F_sw_lc = d_lc$F_sw;

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
plot(F_sw_shc, v_sw_shc, col="black", type="l",
     xlab="Force on seaweed",
     ylim=c(-0.1,0.1),
     #xlab="fraction of maximal proprioception ($\\sigma/\\sigma_\\mathrm{max}$)",
     ylab="Average seaweed velocity");#, lwd=3);
#lines(f_sigma1a, frequency_shca, col="red", type="l");#, lwd=3);
lines(F_sw_k_1, v_sw_k_1, col=rgb(0.0,0.6,0.5), type="l", lty=1);#, lwd=3);
lines(F_sw_k_2, v_sw_k_2, col=rgb(0.0,0.45,0.7), type="l", lty=1);#, lwd=3);
lines(F_sw_k_3, v_sw_k_3, col=rgb(0.8,0.6,0.7), type="l", lty=1);#, lwd=3);
lines(F_sw_k_4, v_sw_k_4, col=rgb(0.3,0.7,0.9), type="l", lty=1);#, lwd=3);
#lines(F_sw_lc, v_sw_lc, col="blue", type="l", lty=2);#, lwd=3);
#legend("topright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
dev.off()
