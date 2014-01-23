#include <iostream>
#include <fstream>
#include <random>
#include <functional>
#include <ctime>
#include <limits>
#include <yaml-cpp/yaml.h>
#include "settings.h"

using namespace std;

enum StateVariableId {
    a_I2,
    a_h,
    a_I3,
    numNeurons,
    x_o = numNeurons,
    x_sw,
    u_I2,
    u_I3,
    E_I2,
    E_I3,
    U_I2,
    U_I3,
    numStateVars
};

const char* stateVariableNames[] = {
    "a_I2",
    "a_h",
    "a_I3",
    "x_o",
    "x_sw",
    "u_I2",
    "u_I3",
    "E_I2",
    "E_I3",
    "U_I2",
    "U_I3",
};

// Calculates the drift for a neuron given the activity of itself, the next
// neuron, and the previous neuron.
inline double f(double me, double prev, double next, double tau, const ModelParameters& p){
    return (me*(p.alpha - p.beta*me - p.delta*prev - p.gamma*next) + p.mu) / tau;
}

// Calculates the height of a normalized length-tension curve, with
// a maximum height of 1 and zeros at -1, 0, and 1.
inline double g(double x) {
    const double k = 2.598076211353316; // = 3 * sqrt(3) / 2
    return -k * x * (x - 1) * (x + 1);
}

// calculates the deterministic drift for the model (i.e. the multiplier
// of dt in the SDE)
inline void drift(double (*dy)[numStateVars],
        const double y[numStateVars], double t, const ModelParameters& p) {
    double tau = p.tau_a * (1 + p.tau_l[a_I2] * y[a_I2] + p.tau_l[a_h] * y[a_h] + p.tau_l[a_I3] * y[a_I3]);
    (*dy)[a_I2] = f(y[a_I2], y[a_I3], y[a_h],  tau, p) +
        (y[x_o] - p.S[a_I2]) * p.sigma[a_I2];
    (*dy)[a_h]  = f(y[a_h],  y[a_I2], y[a_I3], tau, p) +
        (y[x_o] - p.S[a_h])  * p.sigma[a_h];
    (*dy)[a_I3] = f(y[a_I3], y[a_h],  y[a_I2], tau, p) +
        (y[x_o] - p.S[a_I3]) * p.sigma[a_I3];
    (*dy)[u_I2] = ((y[a_I2] + y[a_h]) * p.u_max - y[u_I2]) / p.tau;
    (*dy)[u_I3] = (y[a_I3] * p.u_max - y[u_I3]) / p.tau;

    double F_I2 = p.k[a_I2] * g((y[x_o] - p.c[a_I2]) / p.w[a_I2]) * y[u_I2];
    double F_I3 = p.k[a_I3] * g((y[x_o] - p.c[a_I3]) / p.w[a_I3]) * y[u_I3];
    double F_musc = F_I2 + F_I3;
    double F_spring = p.k_spring * (p.x_spring - y[x_o]);

    // hold onto the seaweed if closing activity is high enough
    if (y[a_h] + y[a_I3] >= 0.5) {
        (*dy)[x_o]  = (*dy)[x_sw] =
            (F_musc + F_spring + p.F_sw) / (p.b_o + p.b_sw);
    } else {
        // let go
        (*dy)[x_o]  = F_musc / p.b_o;
        if (p.b_sw == 0) {  // avoid infinite speed when damping is off
            (*dy)[x_sw] = 0;
        } else {
            (*dy)[x_sw] = (p.F_sw + F_spring) / p.b_sw;
        }
    }

    (*dy)[U_I2] = y[u_I2];
    (*dy)[U_I2] = y[u_I3];
    (*dy)[E_I2] = (*dy)[x_o] * F_I2;
    (*dy)[E_I3] = (*dy)[x_o] * F_I3;
}


// calculates diffusion rate for the model (i.e. the multiplier of dW in
// the SDE)
inline void diffusion(double (*db)[numStateVars],
        const double y[numStateVars], double t, const ModelParameters& p) {
    (*db)[a_I2] = p.eta;
    (*db)[a_h]  = p.eta;
    (*db)[a_I3] = p.eta;
    (*db)[x_o]  = 0;
    (*db)[x_sw] = 0;
    (*db)[u_I2]  = 0;
    (*db)[u_I3]  = 0;
    (*db)[U_I2]  = 0;
    (*db)[U_I3]  = 0;
    (*db)[E_I2]  = 0;
    (*db)[E_I3]  = 0;
}


// a function that is negative before an event occurs and positive afterwards
// (e.g. time since the event, or distance from a poincare section)
inline double eventTest(const double y[numStateVars], double t,
        const ModelParameters& p) {
    // return the time since the kick
    return t - p.t_kick;
}


// Handles the event at the given time and state, updating state variables
// as needed.
void applyEvent(double (*y)[numStateVars], double t,
        const ModelParameters& p) {
    // displace the radula-odontophore
    (*y)[x_o] += p.x_kick;

    // displace or reset the neurons as instructed
    for (int i = 0; i < numNeurons; ++i) {
        (*y)[i] += p.a_kick[i];
        if (p.a_reset[i] == p.a_reset[i]) { // i.e. if not NaN
            (*y)[i] = p.a_reset[i];
        }
    }
}


// perform a single step of size p_sim.dt using the explicit second order
// weak scheme for additive noise from eq 15.1.4, p 287 of:
// Kloeden PE, Platen E. Numerical solution of stocahstic differential
//     equations. Berlin: Springer-Verlag; 1992.
template <class dW_generator_type>
void rk2Step(double (*y)[numStateVars], double* t,
        dW_generator_type& dW_generator, const ModelParameters& p,
        const SimulationParameters p_sim) {
    double db[numStateVars];
    double dW[numStateVars];
    double dy1[numStateVars];
    double dy2[numStateVars];
    double y1[numStateVars];

    // perf: This should be stored as integrator state rather than calculated
    // twice (at the end of the previous step and beginning of the next step).
    // The current even test is fast enough that it's probably not worth the
    // extra code, however.
    double previousEventTest = eventTest(*y, *t, p);

    // calculate the drift and diffusion at the starting point
    drift(&dy1, *y, *t, p);
    diffusion(&db, *y, *t, p);

    // take a trial step
    for (int i = 0; i < numStateVars; ++i) {
        if (db[i] == 0) {
            // generating random variates is slow, so don't do it unnecessarily
            dW[i] = 0;
        } else {
            dW[i] = db[i] * dW_generator();
        }
        y1[i] = (*y)[i] + dy1[i] * p_sim.dt + dW[i];
    }

    // calculate the drift after the trial step
    drift(&dy2, y1, *t + p_sim.dt, p);

    // average the two drifts to get the real step
    for (int i = 0; i < numStateVars; ++i) {
        (*y)[i] += (dy1[i] + dy2[i])/2 * p_sim.dt + dW[i];

        // rectify the neural activity
        if ((*y)[i] < 0 && i < numNeurons) {
            (*y)[i] *= -1;
        }
        if ((*y)[i] > 1 && i == x_o) {
            (*y)[i] = 1;
        }
    }

    (*t) += p_sim.dt;

    // accuracy: We could use bisection of some form to find the exact time of
    // the event and integrate up to that point.  For the moment, however,
    // event timing resolution of one step seems adequate.
    if (previousEventTest < 0 && eventTest(*y, *t, p) > 0) {
        applyEvent(y, *t, p);
    }
}


int main(int argc, char* argv[]) {
    Settings settings;
    double y[numStateVars];
    double y_last_step[numStateVars];
    double y_last_cycle[numStateVars];
    double cycle_delta_y[numStateVars];

    time_t startTime = time(NULL);

    if (argc < 2) {
        cerr << "Usage:" << endl;
        cerr << "    " << argv[0] <<
            " [[<settings file> [<settings file> [...]]]  "
            "<output filename prefix>" << endl;
        cerr << endl;
        cerr << "<settings file> should be a yaml configuration file" << endl;
        cerr << "<output filename prefix> is a string that will be " <<
            "prepended to all output file names" << endl;
        cerr << endl;
        return 1;
    }
    string outputPrefix = argv[argc - 1];
    int numSettingsFiles = argc - 2;

    for (int i = 1; i <= numSettingsFiles; ++i) {
        YAML::Node defaults = YAML::LoadFile(argv[i]);
        settings.load(defaults);
    }

    // initial conditions
    y[a_I2] = settings.InitialConditions.a_I2;
    y[a_h] = settings.InitialConditions.a_h;
    y[a_I3] = settings.InitialConditions.a_I3;
    y[x_o] = settings.InitialConditions.x_o;
    y[x_sw] = settings.InitialConditions.x_sw;
    y[u_I2] = settings.InitialConditions.u_I2;
    y[u_I3] = settings.InitialConditions.u_I3;
    y[U_I2] = settings.InitialConditions.U_I2;
    y[U_I3] = settings.InitialConditions.U_I3;
    y[E_I2] = settings.InitialConditions.E_I2;
    y[E_I3] = settings.InitialConditions.E_I3;
    for (int i = 0; i < numStateVars; ++i) {
        y_last_step[i] = y[i];
        y_last_cycle[i] = y[i];
        cycle_delta_y[i] = 0;
    }

    // set up file with trajectory info
    ofstream outfile(outputPrefix + "_trajectory.csv");
    outfile.precision(15);
    outfile << "t";
    for (int i = 0; i < numStateVars; ++i) {
        outfile << "," << stateVariableNames[i];
    }
    outfile << endl;

    // set up file with cycle info
    ofstream cyclefile(outputPrefix + "_cycle.csv");
    cyclefile.precision(15);
    cyclefile << "t,phase,period,amplitude";
    for (int i = 0; i < numStateVars; ++i) {
        cyclefile << ",cycle_delta_" << stateVariableNames[i];
    }
    cyclefile << endl;

    double tStartPhase = 0;
    int phase = 0;
    double maxAmplitude = 0.;
    for (int i = 0; i < numNeurons; ++i) {
        if (y[i] >= maxAmplitude) {
            phase = i;
            maxAmplitude = y[i];
        }
    }
    int nextPhase = (phase + 1) % numNeurons;


    cout << "running..." << endl;

    // initialize the random number generator
    mt19937 random_int_generator(settings.SimulationParameters.seed);
    normal_distribution<double> dW_distribution(0.0, sqrt(settings.SimulationParameters.dt));
    auto dW_generator = bind(dW_distribution, random_int_generator);

    // numerical integration
    double t = 0;
    double lastOutputTime = -settings.SimulationParameters.minOutputInterval; // output first sample
    double lastPhaseDiff = y[nextPhase] - y[phase];
    double tLast = t;
    double tStartPhase1[numNeurons];
    double maxAmplitude1[numNeurons];
    double duration1[numNeurons];
    while (t <= settings.SimulationParameters.duration) {
        if (t - lastOutputTime >= settings.SimulationParameters.minOutputInterval) {
            outfile << t;
            for (int i = 0; i < numStateVars; ++i) {
                outfile << "," << y[i];
            }
            outfile << endl;
            lastOutputTime = t;
        }

        if (y[phase] > maxAmplitude) {
            maxAmplitude = y[phase];
        }

        double phaseDiff = y[nextPhase] - y[phase];
        if (phaseDiff >= 0) {
            // linearly interpolate the time when the amplitudes cross
            double tCross = t -
                (t - tLast) * phaseDiff / (phaseDiff - lastPhaseDiff);
            duration1[phase] = tCross - tStartPhase1[phase];
            tStartPhase1[nextPhase] = tCross;
            maxAmplitude1[phase] = maxAmplitude;

            // update the state variable changes at the end of the cycle
            if (phase == 0) {
                for (int i = 0; i < numStateVars; ++i) {
                    // Approximate the state variable values at the end of the
                    // cycle using linear interpolation
                    double y_this_cycle = (
                        (tCross - tLast) * y[i] + (t - tCross) * y_last_step[i]
                        ) / (t - tLast);
                    cycle_delta_y[i] = y_this_cycle - y_last_cycle[i];
                    y_last_cycle[i] = y_this_cycle;
                }
            }

            cyclefile << tStartPhase1[phase] << "," << phase << "," <<
                duration1[phase] << "," <<  maxAmplitude;
            for (int i = 0; i < numStateVars; ++i) {
                cyclefile << "," << cycle_delta_y[i];
            }
            cyclefile << endl;

            phase = nextPhase;
            nextPhase = (phase + 1) % numNeurons;
            maxAmplitude = 0;
        }
        tLast = t;
        lastPhaseDiff = phaseDiff;
        for (int i = 0; i < numStateVars; ++i) {
            y_last_step[i] = y[i];
        }

        rk2Step(&y, &t, dW_generator, settings.ModelParameters, settings.SimulationParameters);
    }

    if (settings.sweepModelParameters.hasSweeps
            || settings.sweepSimulationParameters.hasSweeps) {
        ofstream sweepfile(outputPrefix + "_sweep.csv");
        sweepfile.precision(15);
        sweepfile << "u,t_kick_rel_I2";
        auto sweepParamNames = settings.sweepModelParameters.sweptNames();
        for (int i = 0; i < numNeurons; ++i) {
            sweepfile << ",start_" << stateVariableNames[i] <<
                ",duration_" << stateVariableNames[i] <<
                ",amplitude_" << stateVariableNames[i];
        }
        for (auto i = sweepParamNames.begin(); i != sweepParamNames.end(); ++i) {
            sweepfile << "," << (*i);
        }
        for (int i = 0; i < numStateVars; ++i) {
            sweepfile << ",cycle_delta_" << stateVariableNames[i];
        }
        sweepfile << endl;

        for (int i = 0; i < settings.SimulationParameters.sweepSteps; ++i) {
            double tStartPhase1[numNeurons];
            double maxAmplitude1[numNeurons];
            double duration1[numNeurons];

            double u = double(i) / (settings.SimulationParameters.sweepSteps - 1);
            ModelParameters p = settings.sweepModelParameters.generate(u);
            SimulationParameters p_sim = settings.sweepSimulationParameters.generate(u);
            for (int i = 0; i < numNeurons; ++i) {
                tStartPhase1[i] = numeric_limits<double>::quiet_NaN();
            }
            double t_kick_rel_I2 = numeric_limits<double>::quiet_NaN();

            // initialize the random number generator
            mt19937 random_int_generator(p_sim.seed);

            // initial conditions
            y[a_I2] = settings.InitialConditions.a_I2;
            y[a_h] = settings.InitialConditions.a_h;
            y[a_I3] = settings.InitialConditions.a_I3;
            y[x_o] = settings.InitialConditions.x_o;
            y[x_sw] = settings.InitialConditions.x_sw;
            y[u_I2] = settings.InitialConditions.u_I2;
            y[u_I3] = settings.InitialConditions.u_I3;
            y[U_I2] = settings.InitialConditions.U_I2;
            y[U_I3] = settings.InitialConditions.U_I3;
            y[E_I2] = settings.InitialConditions.E_I2;
            y[E_I3] = settings.InitialConditions.E_I3;
            for (int i = 0; i < numStateVars; ++i) {
                y_last_step[i] = y[i];
                y_last_cycle[i] = y[i];
                cycle_delta_y[i] = 0;
            }

            // Euler-Maruyama integration
            double t = 0;
            double lastPhaseDiff = y[nextPhase] - y[phase];
            double tLast = t;
            while (t <= p_sim.duration) {

                if (y[phase] > maxAmplitude) {
                    maxAmplitude = y[phase];
                }

                double phaseDiff = y[nextPhase] - y[phase];
                if (phaseDiff >= 0) {
                    // linearly interpolate the time when the amplitudes cross
                    double tCross = t -
                        (t - tLast) * phaseDiff / (phaseDiff - lastPhaseDiff);
                    duration1[phase] = tCross - tStartPhase1[phase];
                    tStartPhase1[nextPhase] = tCross;
                    maxAmplitude1[phase] = maxAmplitude;

                    // update the timing of the onset of I2 relative to the kick
                    if (phase == 0 && t < settings.ModelParameters.t_kick) {
                        t_kick_rel_I2 = settings.ModelParameters.t_kick - tCross;
                    }

                    // update the state variable changes at the end of the cycle
                    if (phase == 0) {
                        for (int i = 0; i < numStateVars; ++i) {
                            // Approximate the state variable values at the end of the
                            // cycle using linear interpolation
                            double y_this_cycle = (
                                (tCross - tLast) * y[i] + (t - tCross) * y_last_step[i]
                                ) / (t - tLast);
                            cycle_delta_y[i] = y_this_cycle - y_last_cycle[i];
                            y_last_cycle[i] = y_this_cycle;
                        }
                    }

                    phase = nextPhase;
                    nextPhase = (phase + 1) % numNeurons;
                    maxAmplitude = 0;
                }
                tLast = t;
                lastPhaseDiff = phaseDiff;
                for (int i = 0; i < numStateVars; ++i) {
                    y_last_step[i] = y[i];
                }

                rk2Step(&y, &t, dW_generator, p, p_sim);
            }

            // check to see if we're stuck or slowing
            if (tStartPhase1[phase] != tStartPhase1[phase] || // i.e. is NaN
                    t - tStartPhase1[phase] > p_sim.duration / 5) {
                duration1[phase] = numeric_limits<double>::quiet_NaN();
            }

            sweepfile << u << "," << t_kick_rel_I2;
            auto sweepParamVals = settings.sweepModelParameters.sweptValues(u);
            for (int j = 0; j < numNeurons; ++j) {
                sweepfile << "," << tStartPhase1[j] << "," << duration1[j] <<
                    "," << maxAmplitude1[j];
            }
            for (auto i = sweepParamVals.begin(); i != sweepParamVals.end(); ++i) {
                sweepfile << "," << (*i);
            }
            for (int i = 0; i < numStateVars; ++i) {
                sweepfile << "," << cycle_delta_y[i];
            }
            sweepfile << endl;
        }
    }

    // save all of the settings used for the simulation
    YAML::Node node;
    settings.save(&node);
    time_t endTime = time(NULL);
    node["Timing"]["StartTime"] = ctime(&startTime);
    node["Timing"]["RunTime"] = endTime - startTime;
    ofstream settings_used(outputPrefix + "_settings.yaml");
    settings_used << node;

    return 0;
}
