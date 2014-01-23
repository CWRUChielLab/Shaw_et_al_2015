#!/usr/bin/Rscript
args <- commandArgs(T);
d__shc <- read.csv(args[1]);
d__lc <- read.csv(args[2]);
d__lc_mu <- read.csv(args[3]);
d__lc_mu_tau_a <- read.csv(args[4]);
d__lc_mu_tau_a_tau_l <- read.csv(args[5]);
d__cr_mu <- read.csv(args[6]);
d__cr_low_mu <- read.csv(args[7]);
d__cr_mid_mu <- read.csv(args[8]);
outfile = args[9];

period__shc = (d__shc$duration_a_I2 + d__shc$duration_a_h + d__shc$duration_a_I3);
period__lc = (d__lc$duration_a_I2 + d__lc$duration_a_h + d__lc$duration_a_I3);
period__lc_mu = (d__lc_mu$duration_a_I2 + d__lc_mu$duration_a_h + d__lc_mu$duration_a_I3);
period__lc_mu_tau_a = (d__lc_mu_tau_a$duration_a_I2 + d__lc_mu_tau_a$duration_a_h + d__lc_mu_tau_a$duration_a_I3);
period__lc_mu_tau_a_tau_l = (d__lc_mu_tau_a_tau_l$duration_a_I2 + d__lc_mu_tau_a_tau_l$duration_a_h + d__lc_mu_tau_a_tau_l$duration_a_I3);
period__cr_mu = (d__cr_mu$duration_a_I2 + d__cr_mu$duration_a_h + d__cr_mu$duration_a_I3);
period__cr_mid_mu = (d__cr_mid_mu$duration_a_I2 + d__cr_mid_mu$duration_a_h + d__cr_mid_mu$duration_a_I3);
period__cr_low_mu = (d__cr_low_mu$duration_a_I2 + d__cr_low_mu$duration_a_h + d__cr_low_mu$duration_a_I3);
v_sw__shc = -d__shc$cycle_delta_x_sw / period__shc;
v_sw__lc = -d__lc$cycle_delta_x_sw / period__lc;
v_sw__lc_mu = -d__lc_mu$cycle_delta_x_sw / period__lc_mu;
v_sw__lc_mu_tau_a = -d__lc_mu_tau_a$cycle_delta_x_sw / period__lc_mu_tau_a;
v_sw__lc_mu_tau_a_tau_l = -d__lc_mu_tau_a_tau_l$cycle_delta_x_sw / period__lc_mu_tau_a_tau_l;
v_sw__cr_mu = -d__cr_mu$cycle_delta_x_sw / period__cr_mu;
v_sw__cr_mid_mu = -d__cr_mid_mu$cycle_delta_x_sw / period__cr_mid_mu;
v_sw__cr_low_mu = -d__cr_low_mu$cycle_delta_x_sw / period__cr_low_mu;
tau_a__shc = d__shc$tau_a;#/0.05;
tau_a__lc = d__lc$tau_a;#/0.6;
tau_a__lc_mu = d__lc_mu$tau_a;#/0.6;
tau_a__lc_mu_tau_a = d__lc_mu_tau_a$tau_a;#/0.6;
tau_a__lc_mu_tau_a_tau_l = d__lc_mu_tau_a_tau_l$tau_a;#/0.6;
tau_a__cr_mu = d__cr_mu$tau_a;#/0.6;
tau_a__cr_mid_mu = d__cr_mid_mu$tau_a;#/0.6;
tau_a__cr_low_mu = d__cr_low_mu$tau_a;#/0.6;

v_sw__shc[abs(v_sw__shc) > 1e6] = NA;
v_sw__lc[abs(v_sw__lc) > 1e6] = NA;
v_sw__lc_mu[abs(v_sw__lc_mu) > 1e6] = NA;
v_sw__lc_mu_tau_a[abs(v_sw__lc_mu_tau_a) > 1e6] = NA;
v_sw__lc_mu_tau_a_tau_l[abs(v_sw__lc_mu_tau_a_tau_l) > 1e6] = NA;
v_sw__cr_mu[abs(v_sw__cr_mu) > 1e6] = NA;
v_sw__cr_mid_mu[abs(v_sw__cr_mid_mu) > 1e6] = NA;
v_sw__cr_low_mu[abs(v_sw__cr_low_mu) > 1e6] = NA;

#require(tikzDevice);
extension = strsplit(outfile, "\\.")[[1]][-1]
print(extension)
switch(extension,
       pdf=pdf(outfile, width=5, height=5),
       #tex=tikz("test.tex", standAlone=F, width=6, height=3)
       png=png(outfile, width=16, height=4),
       )
par(mar=c(5,5,1,1), mex=0.5)
if (F) {
    # non-scaled, linear plot
    #par(mar=c(4,4,4,4))
    plot(tau_a__shc, v_sw__shc, col="black", type="l",
         ylim=c(min(v_sw__shc,v_sw__lc, na.rm=T), max(v_sw__shc,v_sw__lc, na.rm=T)),
         xlab="Neural time constant",
         #xlab="fraction of maximal proprioception ($\\sigma/\\sigma_\\mathrm{max}$)",
         ylab="Average seaweed velocity");#, lwd=3);
    #lines(f_sigma__shca, frequency__shca, col="red", type="l");#, lwd=3);
    lines(tau_a__lc, v_sw__lc, col="blue", type="l", lty=2);#, lwd=3);
    #legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
}

if (F) {
    # x and y scaled, linear plot
    plot(tau_a__shc/0.05, v_sw__shc/max(v_sw__shc,na.rm=T), col="black", type="l",
         ylim=c(min(v_sw__shc,v_sw__lc, na.rm=T)/max(v_sw__shc,na.rm=T), max(v_sw__shc,v_sw__lc, na.rm=T)/max(v_sw__shc, na.rm=T)),
         xlim=c(0,10),
         xlab="Neural time constant, scaled",
         ylab="Average seaweed velocity, scaled");#, lwd=3);
    lines(tau_a__lc/0.15, v_sw__lc/max(v_sw__lc,na.rm=T), col="blue", type="l", lty=2);#, lwd=3);
    abline(v = 1)
    #legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
}

if (T) {
    # x scaled, log-linear plot
    #plot(tau_a__shc/0.05, v_sw__shc/max(v_sw__shc,na.rm=T), col="black", type="l",
    plot(log(tau_a__shc/0.05), v_sw__shc, col="black", type="l",
    #     ylim=c(min(v_sw__shc,v_sw__lc, na.rm=T)/max(v_sw__shc,na.rm=T), max(v_sw__shc,v_sw__lc, na.rm=T)/max(v_sw__shc, na.rm=T)),
         xlim=c(-2,2),
         xlab="Log of scaled neural time constant",
         #xlab="fraction of maximal proprioception ($\\sigma/\\sigma_\\mathrm{max}$)",
         #ylab="Average seaweed velocity, scaled");#, lwd=3);
         ylab="Average seaweed velocity");#, lwd=3);
    #lines(f_sigma__shca, frequency__shca, col="red", type="l");#, lwd=3);
    #lines(tau_a__lc/0.2, v_sw__lc/max(v_sw__lc,na.rm=T), col="blue", type="l", lty=2);#, lwd=3);
    abline(v = 0, col="gray")
    lines(log(tau_a__lc/0.2), v_sw__lc, col="blue", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__lc_mu/0.05), v_sw__lc_mu, col="orange", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__lc_mu_tau_a/0.2), v_sw__lc_mu_tau_a, col="darkgreen", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__lc_mu_tau_a_tau_l/0.2), v_sw__lc_mu_tau_a_tau_l, col="darkred", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__cr_mu/0.05), v_sw__cr_mu, col="pink", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__cr_mid_mu/0.05), v_sw__cr_mid_mu, col="violet", type="l", lty=1);#, lwd=3);
    lines(log(tau_a__cr_low_mu/0.05), v_sw__cr_low_mu, col="purple", type="l", lty=1);#, lwd=3);
    #legend("bottomright", legend=c("µ = 0", "µ » 0"), col=c("black", "blue"), lty=c(1,2), bg="white")
}
dev.off()
