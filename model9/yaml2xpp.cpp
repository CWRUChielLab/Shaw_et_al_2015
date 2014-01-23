#include <iostream>
#include <fstream>
#include <ctime>
#include <yaml-cpp/yaml.h>

using namespace std;

const char* dimNames[] = {
    "I2",
    "h",
    "I3",
    "s"
};


int main(int argc, const char** argv) {

    if (argc != 3) {
        cerr << "Usage:" << endl;
        cerr << "    " << argv[0] << " <input file> <output file>" << endl;
        cerr << endl;
        cerr << "<input file> should be a yaml file" << endl;
        cerr << "<output file> should be a .ode file" << endl;
        cerr << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];

    YAML::Node defaults = YAML::LoadFile(inputFilename);
    ofstream fout(outputFilename);

    fout << "# " << outputFilename << endl;
    fout << "# generated from " << inputFilename << " using " << argv[0] << endl;
    time_t now = time(NULL);
    fout << "# on " << ctime(&now) << endl;
    fout << endl;
    fout << endl;

    fout << "# governing equations:" << endl;
    fout << "a_I2' = f(a_I2, a_I3, a_h) + (s - s_I2) * sigma_I2" << endl;
    fout << "a_h'  = f(a_h,  a_I2, a_I3) + (s - s_h)  * sigma_h" << endl;
    fout << "a_I3' = f(a_I3, a_h,  a_I2) + (s - s_I3) * sigma_I3" << endl;
    fout << "s'    = (";
    fout << " k_I2 * g((s - c_I2) / w_I2) * a_I2 +";
    fout << " k_h  * g((s - c_h)  / w_h)  * a_h  +";
    fout << " k_I3 * g((s - c_I3) / w_I3) * a_I3 +";
    fout << " k_spring * (s0_spring - s)";
    fout << ") / k_d" << endl;

    fout << endl;
    fout << endl;

    fout << "# functions:" << endl;
    fout << "f(me,prev,next) =  " <<
        "me * (alpha - beta*me - delta * prev - gamma * next) + mu" << endl;
    fout << "g_k=2.598076211353316" << endl;
    //fout << "par g_k = 3*3^0.5/2" << endl;
    fout << "g(x) = -g_k * x * (x - 1) * (x + 1)" << endl;

    fout << endl;
    fout << endl;

    fout << "# parameters:" << endl;
    YAML::Node modelParameters = defaults["ModelParameters"];
    for (auto iterParam = modelParameters.begin();
            iterParam != modelParameters.end(); ++iterParam) {

        if ((*iterParam).second.IsSequence()) {
            for (int i = 0; i < (*iterParam).second.size(); ++i) {
                fout << "par " << (*iterParam).first.as<string>() << "_" << dimNames[i] <<
                    "=" << (*iterParam).second[i].as<string>() << endl;
            }
        } else {
            fout << "par " << (*iterParam).first.as<string>() << "=" <<
                (*iterParam).second.as<string>() << endl;
        }
    }

    fout << "# initial conditions:" << endl;
    YAML::Node initialConditions = defaults["InitialConditions"];
    for (auto iterVar = initialConditions.begin();
            iterVar != initialConditions.end(); ++iterVar) {

        fout << "init " << (*iterVar).first.as<string>() << "=" <<
            (*iterVar).second.as<string>() << endl;
    }

    fout << endl;
    fout << "done" << endl;
}

