# Apple doesn't seem to be updating the version of GCC in XCode, so we need to
# use LLVM in XCode 5 to use C++ 2011 features.  For simplicity, we use this
# by default on all platforms (although newer versions of GCC also work)
#CXX=clang++
#CC=clang

# settings
TARGET=model9
OBJECTS=$(TARGET).o settings.o

YAMLCPP= \
	lib/yaml-cpp-0.5.0/src/emitterstate.o \
	lib/yaml-cpp-0.5.0/src/scantag.o \
	lib/yaml-cpp-0.5.0/src/directives.o \
	lib/yaml-cpp-0.5.0/src/scanner.o \
	lib/yaml-cpp-0.5.0/src/scantoken.o \
	lib/yaml-cpp-0.5.0/src/singledocparser.o \
	lib/yaml-cpp-0.5.0/src/memory.o \
	lib/yaml-cpp-0.5.0/src/simplekey.o \
	lib/yaml-cpp-0.5.0/src/node.o \
	lib/yaml-cpp-0.5.0/src/node_data.o \
	lib/yaml-cpp-0.5.0/src/emitterutils.o \
	lib/yaml-cpp-0.5.0/src/nodeevents.o \
	lib/yaml-cpp-0.5.0/src/null.o \
	lib/yaml-cpp-0.5.0/src/emit.o \
	lib/yaml-cpp-0.5.0/src/contrib/graphbuilderadapter.o \
	lib/yaml-cpp-0.5.0/src/contrib/graphbuilder.o \
	lib/yaml-cpp-0.5.0/src/exp.o \
	lib/yaml-cpp-0.5.0/src/emitfromevents.o \
	lib/yaml-cpp-0.5.0/src/scanscalar.o \
	lib/yaml-cpp-0.5.0/src/parser.o \
	lib/yaml-cpp-0.5.0/src/emitter.o \
	lib/yaml-cpp-0.5.0/src/regex.o \
	lib/yaml-cpp-0.5.0/src/binary.o \
	lib/yaml-cpp-0.5.0/src/nodebuilder.o \
	lib/yaml-cpp-0.5.0/src/stream.o \
	lib/yaml-cpp-0.5.0/src/convert.o \
	lib/yaml-cpp-0.5.0/src/parse.o \
	lib/yaml-cpp-0.5.0/src/ostream_wrapper.o \
	lib/yaml-cpp-0.5.0/src/tag.o \

LC_YAML=figsrc/lc_u_max.yaml figsrc/lc_mu.yaml figsrc/lc_tau_a.yaml figsrc/lc_tau_l.yaml

VPATH=figsrc:figbuild

CXXFLAGS=--std=c++11 -O3 -Ilib/yaml-cpp-0.5.0/include

all : $(TARGET) figures params.tex fig/lc_incremental_params.csv

$(TARGET) : $(OBJECTS) $(YAMLCPP)
	g++ $^ $(CXXFLAGS) -o $@

$(TARGET).o : settings.h

%.cpp : %.yaml code-generator
	./code-generator $< $@

%.h : %.yaml code-generator
	./code-generator $< $@

.PRECIOUS : %.cpp %.csv

code-generator : code-generator.cpp $(YAMLCPP)
	g++ $^ $(CXXFLAGS) -o $@

yaml2xpp : yaml2xpp.cpp $(YAMLCPP)
	g++ $^ $(CXXFLAGS) -o $@

yaml2tex : yaml2tex.cpp $(YAMLCPP)
	g++ $^ $(CXXFLAGS) -o $@

params.tex : yaml2tex settings.yaml
	./$^ $@

clean :
	rm -rf $(TARGET) code-generator $(OBJECTS) $(YAMLCPP) settings.h settings.cpp figbuild/* fig/* yaml2tex yaml2xpp params.tex

figures : fig/spring_plot.pdf fig/proprioception_plot.pdf \
	fig/spring_protraction_vs_retraction_plot.pdf \
	fig/proprioception_protraction_vs_retraction_plot.pdf \
	fig/F_sw_vs_velocity_plot.pdf \
	fig/F_sw_vs_velocity_plot_lc_params.pdf \
	fig/F_sw_vs_velocity_plot_lc_u_max.pdf \
	fig/F_sw_vs_U_total_plot.pdf \
	fig/F_sw_vs_E_total_plot.pdf \
	fig/tau_a_vs_velocity_plot.pdf \
	fig/tau_a_vs_frequency_plot.pdf \
	fig/lc_timeplot.pdf fig/shc_timeplot.pdf \
	fig/lc_neural_timeplot.pdf fig/shc_neural_timeplot.pdf \
	fig/lc_mechanical_timeplot.pdf fig/shc_mechanical_timeplot.pdf \
	fig/lc_F_sw_high_timeplot.pdf fig/shc_F_sw_high_timeplot.pdf \
	fig/lc_F_sw_high_neural_timeplot.pdf fig/shc_F_sw_high_neural_timeplot.pdf \
	fig/lc_F_sw_high_mechanical_timeplot.pdf fig/shc_F_sw_high_mechanical_timeplot.pdf \
	fig/lc_spring_high_timeplot.pdf fig/shc_spring_high_timeplot.pdf \
	fig/lc_spring_high_neural_timeplot.pdf fig/shc_spring_high_neural_timeplot.pdf \
	fig/lc_spring_high_mechanical_timeplot.pdf fig/shc_spring_high_mechanical_timeplot.pdf \
	fig/lc_shc_sigma0_phaseplot.pdf \
	fig/shc_phaseplot.pdf fig/shc_phaseplot_zoomed.pdf \
	fig/shc_eta_bio_neural_timeplot.pdf \
	fig/lc_phaseplot.pdf fig/lc_phaseplot_zoomed.pdf \
	fig/shc_phaseplot_sigma0.pdf fig/lc_phaseplot_sigma0.pdf \
	fig/shc_phaseplot_sigma0_zoomed.pdf fig/lc_phaseplot_sigma0_zoomed.pdf \
	fig/shc_phaseplot_sigma_low.pdf fig/lc_phaseplot_sigma_low.pdf \
	fig/shc_phaseplot_sigma_low_zoomed.pdf fig/lc_phaseplot_sigma_low_zoomed.pdf \
	fig/lc_sigma_low_neural_timeplot.pdf fig/shc_sigma_low_neural_timeplot.pdf \
	fig/lc_sigma_low_mechanical_timeplot.pdf fig/shc_sigma_low_mechanical_timeplot.pdf \
	fig/lc_sigma0_neural_timeplot.pdf fig/shc_sigma0_neural_timeplot.pdf \
	fig/lc_sigma0_mechanical_timeplot.pdf fig/shc_sigma0_mechanical_timeplot.pdf \
	fig/shc_phaseplot_spring_mid.pdf fig/lc_phaseplot_spring_mid.pdf \
	fig/lc_phaseplot_F_sw_high.pdf fig/lc_phaseplot_F_sw_high_zoomed.pdf \
	fig/shc_phaseplot_F_sw_high.pdf fig/shc_phaseplot_F_sw_high_zoomed.pdf \
	fig/lc_phaseplot_spring_high.pdf fig/lc_phaseplot_spring_high_zoomed.pdf \
	fig/shc_phaseplot_spring_high.pdf fig/shc_phaseplot_spring_high_zoomed.pdf \
	fig/lc_prc_s_kick_small_plot.pdf fig/shc_prc_s_kick_small_plot.pdf \
	fig/lc_prc_a_I2_reset_to_0_plot.pdf fig/lc_prc_a_h_reset_to_0_plot.pdf fig/lc_prc_a_I3_reset_to_0_plot.pdf \
	fig/shc_prc_a_I2_reset_to_0_plot.pdf fig/shc_prc_a_h_reset_to_0_plot.pdf fig/shc_prc_a_I3_reset_to_0_plot.pdf \
	fig/lc_prc_a_I2_kick_small_plot.pdf fig/lc_prc_a_h_kick_small_plot.pdf fig/lc_prc_a_I3_kick_small_plot.pdf \
	fig/shc_prc_a_I2_kick_small_plot.pdf fig/shc_prc_a_h_kick_small_plot.pdf fig/shc_prc_a_I3_kick_small_plot.pdf \
	fig/shc_prc_a_I3_kick_medium_plot.pdf fig/lc_prc_a_I3_kick_medium_plot.pdf \
	fig/kde_retraction_plot.pdf

figbuild/shc_trajectory.csv : $(TARGET)
	./$^ figbuild/shc

figbuild/lc_trajectory.csv : $(TARGET) $(LC_YAML)
	./$^ figbuild/lc

fig/shc_timeplot.pdf : trajectory_plot.R shc_trajectory.csv
	Rscript $^ $@

fig/lc_timeplot.pdf : trajectory_plot.R lc_trajectory.csv
	Rscript $^ $@

fig/shc_neural_timeplot.pdf : neural_trajectory_plot.R shc_trajectory.csv
	Rscript $^ $@

fig/lc_neural_timeplot.pdf : neural_trajectory_plot.R lc_trajectory.csv
	Rscript $^ $@

fig/shc_mechanical_timeplot.pdf : mechanical_trajectory_plot.R shc_trajectory.csv
	Rscript $^ $@

fig/lc_mechanical_timeplot.pdf : mechanical_trajectory_plot.R lc_trajectory.csv
	Rscript $^ $@

fig/shc_spring_high_timeplot.pdf : trajectory_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@

fig/lc_spring_high_timeplot.pdf : trajectory_plot.R lc_spring_high_trajectory.csv
	Rscript $^ $@

fig/shc_spring_high_neural_timeplot.pdf : neural_trajectory_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@

fig/lc_spring_high_neural_timeplot.pdf : neural_trajectory_plot.R lc_spring_high_trajectory.csv
	Rscript $^ $@

fig/shc_spring_high_mechanical_timeplot.pdf : mechanical_trajectory_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@

fig/lc_spring_high_mechanical_timeplot.pdf : mechanical_trajectory_plot.R lc_spring_high_trajectory.csv
	Rscript $^ $@

fig/shc_F_sw_high_timeplot.pdf : trajectory_plot.R shc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/lc_F_sw_high_timeplot.pdf : trajectory_plot.R lc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/shc_F_sw_high_neural_timeplot.pdf : neural_trajectory_plot.R shc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/lc_F_sw_high_neural_timeplot.pdf : neural_trajectory_plot.R lc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/shc_F_sw_high_mechanical_timeplot.pdf : mechanical_trajectory_plot.R shc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/lc_F_sw_high_mechanical_timeplot.pdf : mechanical_trajectory_plot.R lc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/lc_shc_sigma0_phaseplot.pdf : dual_phase_plot.R lc_sigma0_trajectory.csv shc_sigma0_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot.pdf : phase_plot.R shc_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot.tex : phase_plot.R shc_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_zoomed.pdf : phase_plot.R shc_trajectory.csv
	Rscript $^ $@ --zoombox 0.0001 0.0001

fig/lc_phaseplot.pdf : phase_plot.R lc_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_zoomed.pdf : phase_plot.R lc_trajectory.csv
	Rscript $^ $@ --zoombox 0.2 0

figbuild/shc_sigma0_trajectory.csv : $(TARGET) sigma_to_0.yaml eta_low.yaml
	./$^ figbuild/shc_sigma0

figbuild/shc_eta_bio_trajectory.csv : $(TARGET) eta_bio.yaml
	./$^ figbuild/shc_eta_bio

figbuild/lc_sigma0_trajectory.csv : $(TARGET) $(LC_YAML) sigma_to_0.yaml
	./$^ figbuild/lc_sigma0

fig/shc_eta_bio_neural_timeplot.pdf : neural_trajectory_plot.R shc_eta_bio_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_sigma0.pdf : phase_plot.R shc_sigma0_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_sigma0.pdf : phase_plot.R lc_sigma0_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_sigma0_zoomed.pdf : phase_plot.R shc_sigma0_trajectory.csv
	Rscript $^ $@ --zoombox 0.0001 0.0001

fig/lc_phaseplot_sigma0_zoomed.pdf : phase_plot.R lc_sigma0_trajectory.csv
	Rscript $^ $@ --zoombox 0.2 0

figbuild/shc_sigma_low_trajectory.csv : $(TARGET) sigma_low.yaml
	./$^ figbuild/shc_sigma_low

figbuild/lc_sigma_low_trajectory.csv : $(TARGET) $(LC_YAML) sigma_low.yaml
	./$^ figbuild/lc_sigma_low

fig/shc_phaseplot_sigma_low_zoomed.pdf : phase_plot.R shc_sigma_low_trajectory.csv
	Rscript $^ $@ --zoombox 0.0001 0.0001

fig/lc_phaseplot_sigma_low_zoomed.pdf : phase_plot.R lc_sigma_low_trajectory.csv
	Rscript $^ $@ --zoombox 0.2 0

fig/shc_phaseplot_sigma_low.pdf : phase_plot.R shc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_sigma_low.pdf : phase_plot.R lc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/shc_sigma_low_neural_timeplot.pdf : neural_trajectory_plot.R shc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/lc_sigma_low_neural_timeplot.pdf : neural_trajectory_plot.R lc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/shc_sigma_low_mechanical_timeplot.pdf : mechanical_trajectory_plot.R shc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/lc_sigma_low_mechanical_timeplot.pdf : mechanical_trajectory_plot.R lc_sigma_low_trajectory.csv
	Rscript $^ $@

fig/shc_sigma0_neural_timeplot.pdf : neural_trajectory_plot.R shc_sigma0_trajectory.csv
	Rscript $^ $@

fig/lc_sigma0_neural_timeplot.pdf : neural_trajectory_plot.R lc_sigma0_trajectory.csv
	Rscript $^ $@

fig/shc_sigma0_mechanical_timeplot.pdf : mechanical_trajectory_plot.R shc_sigma0_trajectory.csv
	Rscript $^ $@

fig/lc_sigma0_mechanical_timeplot.pdf : mechanical_trajectory_plot.R lc_sigma0_trajectory.csv
	Rscript $^ $@

figbuild/shc_F_sw_high_trajectory.csv : $(TARGET) F_sw_high.yaml
	./$^ figbuild/shc_F_sw_high

figbuild/lc_F_sw_high_trajectory.csv : $(TARGET) $(LC_YAML) F_sw_high.yaml
	./$^ figbuild/lc_F_sw_high

figbuild/shc_spring_mid_trajectory.csv : $(TARGET) F_sw_0_b_sw_unchanged.yaml spring_mid.yaml
	./$^ figbuild/shc_spring_mid

figbuild/lc_spring_mid_trajectory.csv : $(TARGET) $(LC_YAML) F_sw_0_b_sw_unchanged.yaml spring_mid.yaml
	./$^ figbuild/lc_spring_mid

fig/shc_phaseplot_spring_mid.pdf : phase_plot.R shc_spring_mid_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_spring_mid.pdf : phase_plot.R lc_spring_mid_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_F_sw_high.pdf : phase_plot.R shc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_F_sw_high_zoomed.pdf : phase_plot.R shc_F_sw_high_trajectory.csv
	Rscript $^ $@ --zoombox 0.0001 0.0001

fig/lc_phaseplot_F_sw_high.pdf : phase_plot.R lc_F_sw_high_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_F_sw_high_zoomed.pdf : phase_plot.R lc_F_sw_high_trajectory.csv
	Rscript $^ $@ --zoombox 0.2 0

figbuild/shc_spring_high_trajectory.csv : $(TARGET) F_sw_0_b_sw_unchanged.yaml spring_high.yaml
	./$^ figbuild/shc_spring_high

figbuild/lc_spring_high_trajectory.csv : $(TARGET) $(LC_YAML) F_sw_0_b_sw_unchanged.yaml spring_high.yaml
	./$^ figbuild/lc_spring_high

fig/shc_phaseplot_spring_high.pdf : phase_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@

fig/shc_phaseplot_spring_high_zoomed.pdf : phase_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@ --zoombox 0.0001 0.0001

fig/lc_phaseplot_spring_high.pdf : phase_plot.R lc_spring_high_trajectory.csv
	Rscript $^ $@

fig/lc_phaseplot_spring_high_zoomed.pdf : phase_plot.R lc_spring_high_trajectory.csv
	Rscript $^ $@ --zoombox 0.2 0

fig/shc_timeplot_spring_high.pdf : trajectory_plot.R shc_spring_high_trajectory.csv
	Rscript $^ $@

figbuild/lc_proprioception_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml sigma_sweep.yaml
	./$^ figbuild/lc_proprioception

figbuild/shc_proprioception_sweep.csv : $(TARGET) eta_to_0.yaml sigma_sweep.yaml
	./$^ figbuild/shc_proprioception

#figbuild/shc_proprioception_focused_sweep.csv : $(TARGET) eta_to_0.yaml sigma_focused_sweep.yaml
#	./$^ figbuild/shc_proprioception_focused

fig/proprioception_plot.pdf : proprioception_plot.R shc_proprioception_sweep.csv lc_proprioception_sweep.csv
	Rscript $^ $@

fig/proprioception_protraction_vs_retraction_plot.pdf : proprioception_protraction_vs_retraction_plot.R shc_proprioception_sweep.csv lc_proprioception_sweep.csv
	Rscript $^ $@

figbuild/lc_spring_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml F_sw_0_b_sw_unchanged.yaml spring_sweep.yaml
	./$^ figbuild/lc_spring

figbuild/shc_spring_sweep.csv : $(TARGET) eta_to_0.yaml F_sw_0_b_sw_unchanged.yaml spring_sweep.yaml
	./$^ figbuild/shc_spring

#figbuild/shc_spring_focused_sweep.csv : $(TARGET) eta_to_0.yaml spring_focused_sweep.yaml
#	./$^ figbuild/shc_spring_focused

fig/spring_plot.pdf : spring_plot.R shc_spring_sweep.csv lc_spring_sweep.csv
	Rscript $^ $@

fig/spring_protraction_vs_retraction_plot.pdf : spring_protraction_vs_retraction_plot.R shc_spring_sweep.csv lc_spring_sweep.csv
	Rscript $^ $@

#figbuild/lc_F_sw_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml b_sw_high.yaml F_sw_sweep.yaml
figbuild/lc_F_sw_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_F_sw

figbuild/lc_mu_F_sw_sweep.csv : $(TARGET) lc_mu.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_F_sw

figbuild/lc_mu_beta_F_sw_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_beta_F_sw

figbuild/lc_mu_beta_nu_F_sw_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_beta_nu_F_sw

figbuild/lc_mu_beta_nu_u_max_2_F_sw_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml u_max_2.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_beta_nu_u_max_2_F_sw

figbuild/lc_mu_beta_nu_u_max_3_F_sw_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml u_max_3.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_beta_nu_u_max_3_F_sw

figbuild/lc_mu_beta_nu_u_max_4_F_sw_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml u_max_4.yaml F_sw_sweep.yaml
	./$^ figbuild/lc_mu_beta_nu_u_max_4_F_sw

#figbuild/shc_F_sw_sweep.csv : $(TARGET) eta_to_0.yaml b_sw_high.yaml F_sw_sweep.yaml
figbuild/shc_F_sw_sweep.csv : $(TARGET) eta_to_0.yaml F_sw_sweep.yaml
	./$^ figbuild/shc_F_sw

fig/F_sw_vs_velocity_plot.pdf : F_sw_vs_velocity_plot.R shc_F_sw_sweep.csv lc_F_sw_sweep.csv
	Rscript $^ $@

fig/F_sw_vs_velocity_plot_lc_params.pdf : F_sw_vs_velocity_plot_lc_params.R shc_F_sw_sweep.csv lc_mu_F_sw_sweep.csv  lc_mu_beta_F_sw_sweep.csv lc_mu_beta_nu_F_sw_sweep.csv lc_F_sw_sweep.csv
	Rscript $^ $@

fig/F_sw_vs_velocity_plot_lc_u_max.pdf : F_sw_vs_velocity_plot_lc_u_max.R shc_F_sw_sweep.csv lc_mu_beta_nu_F_sw_sweep.csv lc_mu_beta_nu_u_max_2_F_sw_sweep.csv lc_mu_beta_nu_u_max_3_F_sw_sweep.csv lc_mu_beta_nu_u_max_4_F_sw_sweep.csv lc_F_sw_sweep.csv
	Rscript $^ $@

fig/F_sw_vs_U_total_plot.pdf : F_sw_vs_U_total_plot.R shc_F_sw_sweep.csv lc_mu_beta_nu_F_sw_sweep.csv lc_mu_beta_nu_u_max_2_F_sw_sweep.csv lc_mu_beta_nu_u_max_3_F_sw_sweep.csv lc_mu_beta_nu_u_max_4_F_sw_sweep.csv lc_F_sw_sweep.csv
	Rscript $^ $@

fig/F_sw_vs_E_total_plot.pdf : F_sw_vs_E_total_plot.R shc_F_sw_sweep.csv lc_mu_beta_nu_F_sw_sweep.csv lc_mu_beta_nu_u_max_2_F_sw_sweep.csv lc_mu_beta_nu_u_max_3_F_sw_sweep.csv lc_mu_beta_nu_u_max_4_F_sw_sweep.csv lc_F_sw_sweep.csv
	Rscript $^ $@

figbuild/shc_cycle.csv : $(TARGET)
	./$^ figbuild/shc

figbuild/lc_mu_cycle.csv : $(TARGET) lc_mu.yaml
	./$^ figbuild/lc_mu

figbuild/lc_mu_tau_a_cycle.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml
	./$^ figbuild/lc_mu_tau_a

figbuild/lc_mu_tau_a_tau_l_cycle.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml
	./$^ figbuild/lc_mu_tau_a_tau_l

figbuild/lc_cycle.csv : $(TARGET) $(LC_YAML)
	./$^ figbuild/lc

fig/lc_incremental_params.csv : lc_incremental_params.R shc_cycle.csv lc_mu_cycle.csv lc_mu_tau_a_cycle.csv lc_mu_tau_a_tau_l_cycle.csv lc_cycle.csv
	Rscript $^ $@

figbuild/lc_mu_tau_a_sweep.csv : $(TARGET) lc_mu.yaml tau_a_sweep.yaml
	./$^ figbuild/lc_mu_tau_a

figbuild/lc_mu_tau_a_tau_a_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml tau_a_sweep.yaml
	./$^ figbuild/lc_mu_tau_a_tau_a

figbuild/lc_mu_tau_a_tau_l_tau_a_sweep.csv : $(TARGET) lc_mu.yaml lc_tau_a.yaml lc_tau_l.yaml tau_a_sweep.yaml
	./$^ figbuild/lc_mu_tau_a_tau_l_tau_a

figbuild/lc_tau_a_sweep.csv : $(TARGET) $(LC_YAML) tau_a_sweep.yaml
	./$^ figbuild/lc_tau_a

figbuild/shc_tau_a_sweep.csv : $(TARGET) tau_a_sweep.yaml
	./$^ figbuild/shc_tau_a

figbuild/cr_mu_tau_a_sweep.csv : $(TARGET) cr_mu.yaml tau_a_sweep.yaml
	./$^ figbuild/cr_mu_tau_a

figbuild/cr_mu_low_tau_a_sweep.csv : $(TARGET) cr_mu_low.yaml tau_a_sweep.yaml
	./$^ figbuild/cr_mu_low_tau_a

figbuild/cr_mu_mid_tau_a_sweep.csv : $(TARGET) cr_mu_mid.yaml tau_a_sweep.yaml
	./$^ figbuild/cr_mu_mid_tau_a

fig/tau_a_vs_velocity_plot.pdf : tau_a_vs_velocity_plot.R shc_tau_a_sweep.csv lc_tau_a_sweep.csv lc_mu_tau_a_sweep.csv lc_mu_tau_a_tau_a_sweep.csv lc_mu_tau_a_tau_l_tau_a_sweep.csv cr_mu_tau_a_sweep.csv cr_mu_low_tau_a_sweep.csv cr_mu_mid_tau_a_sweep.csv
	Rscript $^ $@

fig/tau_a_vs_frequency_plot.pdf : tau_a_vs_frequency_plot.R shc_tau_a_sweep.csv lc_tau_a_sweep.csv
	Rscript $^ $@

figbuild/shc_eta0_trajectory.csv : $(TARGET) eta_to_0.yaml sigma_to_0.yaml
	./$^ figbuild/shc_eta0

figbuild/shc_eta0_dummy_sweep.csv : $(TARGET) eta_to_0.yaml dummy_sweep.yaml
	./$^ figbuild/shc_eta0_dummy

figbuild/shc_prc_s_kick_small_sweep.csv : $(TARGET) eta_to_0.yaml s_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_s_kick_small

fig/shc_prc_s_kick_small_plot.pdf : prc_plot.R shc_prc_s_kick_small_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_eta0_trajectory.csv : $(TARGET) $(LC_YAML) sigma_to_0.yaml
	./$^ figbuild/lc_eta0

figbuild/lc_prc_s_kick_small_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml s_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_s_kick_small

fig/lc_prc_s_kick_small_plot.pdf : prc_plot.R lc_prc_s_kick_small_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_eta0_dummy_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml dummy_sweep.yaml
	./$^ figbuild/lc_eta0_dummy

figbuild/lc_prc_a_I2_reset_to_0_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_I2_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_I2_reset_to_0

fig/lc_prc_a_I2_reset_to_0_plot.pdf : prc_plot.R lc_prc_a_I2_reset_to_0_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_h_reset_to_0_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_h_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_h_reset_to_0

fig/lc_prc_a_h_reset_to_0_plot.pdf : prc_plot.R lc_prc_a_h_reset_to_0_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_I3_reset_to_0_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_I3_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_I3_reset_to_0

fig/lc_prc_a_I3_reset_to_0_plot.pdf : prc_plot.R lc_prc_a_I3_reset_to_0_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_I2_reset_to_0_sweep.csv : $(TARGET) eta_to_0.yaml a_I2_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_I2_reset_to_0

fig/shc_prc_a_I2_reset_to_0_plot.pdf : prc_plot.R shc_prc_a_I2_reset_to_0_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_h_reset_to_0_sweep.csv : $(TARGET) eta_to_0.yaml a_h_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_h_reset_to_0

fig/shc_prc_a_h_reset_to_0_plot.pdf : prc_plot.R shc_prc_a_h_reset_to_0_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_I3_reset_to_0_sweep.csv : $(TARGET) eta_to_0.yaml a_I3_reset_to_0.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_I3_reset_to_0

fig/shc_prc_a_I3_reset_to_0_plot.pdf : prc_plot.R shc_prc_a_I3_reset_to_0_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_I2_kick_small_sweep.csv : $(TARGET) eta_to_0.yaml a_I2_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_I2_kick_small

fig/shc_prc_a_I2_kick_small_plot.pdf : prc_plot.R shc_prc_a_I2_kick_small_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_h_kick_small_sweep.csv : $(TARGET) eta_to_0.yaml a_h_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_h_kick_small

fig/shc_prc_a_h_kick_small_plot.pdf : prc_plot.R shc_prc_a_h_kick_small_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_I3_kick_small_sweep.csv : $(TARGET) eta_to_0.yaml a_I3_kick_small.yaml dt_small.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_I3_kick_small

fig/shc_prc_a_I3_kick_small_plot.pdf : prc_plot.R shc_prc_a_I3_kick_small_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/shc_prc_a_I3_kick_medium_sweep.csv : $(TARGET) eta_to_0.yaml a_I3_kick_medium.yaml t_kick_sweep.yaml
	./$^ figbuild/shc_prc_a_I3_kick_medium

fig/shc_prc_a_I3_kick_medium_plot.pdf : prc_plot.R shc_prc_a_I3_kick_medium_sweep.csv shc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_I2_kick_small_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_I2_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_I2_kick_small

fig/lc_prc_a_I2_kick_small_plot.pdf : prc_plot.R lc_prc_a_I2_kick_small_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_h_kick_small_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_h_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_h_kick_small

fig/lc_prc_a_h_kick_small_plot.pdf : prc_plot.R lc_prc_a_h_kick_small_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_I3_kick_small_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_I3_kick_small.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_I3_kick_small

fig/lc_prc_a_I3_kick_small_plot.pdf : prc_plot.R lc_prc_a_I3_kick_small_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_prc_a_I3_kick_medium_sweep.csv : $(TARGET) $(LC_YAML) eta_to_0.yaml a_I3_kick_medium.yaml t_kick_sweep.yaml
	./$^ figbuild/lc_prc_a_I3_kick_medium

fig/lc_prc_a_I3_kick_medium_plot.pdf : prc_plot.R lc_prc_a_I3_kick_medium_sweep.csv lc_eta0_dummy_sweep.csv
	Rscript $^ $@

figbuild/lc_noise_pseudo_sweep.csv : $(TARGET) $(LC_YAML) noise_pseudosweep.yaml
	./$^ figbuild/lc_noise_pseudo

figbuild/shc_noise_pseudo_sweep.csv : $(TARGET) noise_pseudosweep.yaml
	./$^ figbuild/shc_noise_pseudo

fig/kde_retraction_plot.pdf : kde_retraction_plot.R shc_noise_pseudo_sweep.csv lc_noise_pseudo_sweep.csv
	Rscript $^ $@

