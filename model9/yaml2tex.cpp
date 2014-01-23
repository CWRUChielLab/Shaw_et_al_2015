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


// attempts to convert a valid name in yaml to a valid variable name in TeX
string toTeXName(const string& yamlName) {
    string varName = yamlName;
    size_t pos;

    // remove underscores
    while ((pos = varName.find('_')) != string::npos) {
        varName.erase(pos, 1);
    }

    // replace digits with spelled-out numbers
    while ((pos = varName.find('0')) != string::npos) {
        varName.replace(pos, 1, "Zero");
    }
    while ((pos = varName.find('1')) != string::npos) {
        varName.replace(pos, 1, "One");
    }
    while ((pos = varName.find('2')) != string::npos) {
        varName.replace(pos, 1, "Two");
    }
    while ((pos = varName.find('3')) != string::npos) {
        varName.replace(pos, 1, "Three");
    }

    return varName;
}


// attempts to print a number as attractively as possible
string toTeXVal(const string& yamlVal) {
    string val = yamlVal;

    if (val.substr(0, 3) == "1.e") {
        val = "10^{" + val.erase(0,3) + "}";
    }
    if (val.substr(0, 4) == "-1.e") {
        val = "-10^{" + val.erase(0,4) + "}";
    }

    return "$" + val + "$";
}

int main(int argc, const char** argv) {

    if (argc != 3) {
        cerr << "Usage:" << endl;
        cerr << "    " << argv[0] << " <input file> <output file>" << endl;
        cerr << endl;
        cerr << "<input file> should be a yaml file" << endl;
        cerr << "<output file> should be a .tex file" << endl;
        cerr << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];

    YAML::Node defaults = YAML::LoadFile(inputFilename);
    ofstream fout(outputFilename);

    fout << "% " << outputFilename << endl;
    fout << "% generated from " << inputFilename << " using " << argv[0] << endl;
    time_t now = time(NULL);
    fout << "% on " << ctime(&now) << endl;


    fout << endl;
    fout << "% model parameter values:" << endl;
    YAML::Node modelParameters = defaults["ModelParameters"];
    for (auto iterParam = modelParameters.begin();
            iterParam != modelParameters.end(); ++iterParam) {

        string varName = toTeXName("\\paramVal" + (*iterParam).first.as<string>());

        if ((*iterParam).second.IsSequence()) {
            for (int i = 0; i < (*iterParam).second.size(); ++i) {
                fout << "\\newcommand{" << toTeXName(varName + dimNames[i]) <<
                    "}{" << toTeXVal((*iterParam).second[i].as<string>()) << "}" << endl;
            }
        } else {
            fout << "\\newcommand{" << varName << "}{" <<
                toTeXVal((*iterParam).second.as<string>()) << "}" << endl;
        }
    }

    fout << endl;
    fout << endl;
    fout << "% initial condition values:";
    fout << endl;
    YAML::Node initialConditions = defaults["InitialConditions"];
    for (auto iterVar = initialConditions.begin();
            iterVar != initialConditions.end(); ++iterVar) {

        string varName = toTeXName("\\initVal" + (*iterVar).first.as<string>());

        fout << "\\newcommand{" << varName << "}{" <<
            toTeXVal((*iterVar).second.as<string>()) << "}" << endl;
    }

    fout << endl;
    fout << "% simulation parameter values:" << endl;
    YAML::Node simParameters = defaults["SimulationParameters"];
    for (auto iterParam = simParameters.begin();
            iterParam != simParameters.end(); ++iterParam) {

        string varName = toTeXName("\\simParamVal" + (*iterParam).first.as<string>());

        fout << "\\newcommand{" << varName << "}{" <<
            toTeXVal((*iterParam).second.as<string>()) << "}" << endl;
    }


    fout << endl;
    fout << endl;
    fout << "% parameter table example:" << endl;
    fout << endl;
    fout << "% \\begin{tabular}[htb]{ccl}" << endl;
    fout << "%     parameter & value & description \\\\" << endl;
    fout << "%     \\hline" << endl;
    for (auto iterParam = modelParameters.begin();
            iterParam != modelParameters.end(); ++iterParam) {

        string paramName = (*iterParam).first.as<string>();
        if (paramName.size() > 1 && paramName[1] != '_') {
            // assume it must be a greek letter and escape it
            paramName = "\\" + paramName;
        }

        string varName = toTeXName("\\paramVal" + (*iterParam).first.as<string>());

        if ((*iterParam).second.IsSequence()) {
            for (int i = 0; i < (*iterParam).second.size(); ++i) {
                fout << "%     $" << paramName << "_{" << dimNames[i] <<
                    "}$ & " << toTeXName(varName + dimNames[i]) << " & \\\\" << endl;
            }
        } else {
            fout << "%     $" << paramName << "$ & " <<
                varName << " & \\\\" << endl;
        }
    }
    fout << "% \\end{tabular}" << endl;

    fout << "% initial condition table example:" << endl << endl;
    fout << "% \\begin{tabular}[htb]{ccl}" << endl;
    fout << "% state variable & initial value & description \\\\" << endl;
    fout << "% \\hline" << endl;
    for (auto iterVar = initialConditions.begin();
            iterVar != initialConditions.end(); ++iterVar) {

        string varName = toTeXName("\\initVal" + (*iterVar).first.as<string>());

        fout << "%     $" << (*iterVar).first.as<string>() << "$ & " <<
            varName << " & \\\\" << endl;
    }
    fout << "% \\end{tabular}" << endl;
}

