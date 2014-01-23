#!/usr/bin/Rscript
args <- commandArgs(T);
d_shc <- read.csv(args[1]);
d_mu <- read.csv(args[2]);
d_mu_beta <- read.csv(args[3]);
d_mu_beta_nu <- read.csv(args[4]);
d_lc <- read.csv(args[5]);
outfile_name=args[6];

datalist = list(d_shc=d_shc, d_lc=d_lc, d_mu=d_mu, d_mu_beta=d_mu_beta, d_mu_beta_nu=d_mu_beta_nu);

t_I2a = c();
t_I2b = c();
t_I3 = c();
cycle_delta_x_sw = c();
# for each model, extract the measurements from the last period in the simulation
for (i in 1:length(datalist)) {
    t_I2a = c(t_I2a, tail(datalist[[i]]$period[datalist[[i]]$phase == 0],1));
    t_I2b = c(t_I2b, tail(datalist[[i]]$period[datalist[[i]]$phase == 1],1));
    t_I3  = c(t_I3,  tail(datalist[[i]]$period[datalist[[i]]$phase == 2],1));
    cycle_delta_x_sw  = c(cycle_delta_x_sw,  tail(datalist[[i]]$cycle_delta_x_sw[datalist[[i]]$phase == 0],1));
}
t_total = t_I2a + t_I2b + t_I3;
v_sw = cycle_delta_x_sw / t_total;

write.csv(data.frame(simulation=names(datalist),
    t_I2a=t_I2a, t_I2b=t_I2b, t_I3=t_I3, t_total=t_total,
    cycle_delta_x_sw=cycle_delta_x_sw, v_sw=v_sw
    ), row.names=F, outfile_name)

