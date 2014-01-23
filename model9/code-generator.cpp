#include <iostream>
#include <fstream>
#include <ctime>
#include <yaml-cpp/yaml.h>

using namespace std;

int main(int argc, const char** argv) {

    if (argc != 3) {
        cerr << "Usage:" << endl;
        cerr << "    " << argv[0] << " <input file> <output file>" << endl;
        cerr << endl;
        cerr << "<input file> should be a yaml file" << endl;
        cerr << "<output file> should be a .h or .cpp file" << endl;
        cerr << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];

    YAML::Node defaults = YAML::LoadFile(inputFilename);
    ofstream fout(outputFilename);
    fout.precision(16);

    bool isHeader = ((outputFilename[outputFilename.length() - 2] == '.') &&
        (outputFilename[outputFilename.length() - 1] == 'h'));

    fout << "// " << outputFilename << endl;
    fout << "// generated from " << inputFilename << " using " << argv[0] << endl;
    time_t now = time(NULL);
    fout << "// on " << ctime(&now) << endl;
    fout << endl;
    fout << "#include <yaml-cpp/yaml.h>" << endl;
    fout << "#include <limits>" << endl;
    fout << endl;

    // generate class declarations for each section
    for (auto iterClass = defaults.begin(); iterClass != defaults.end();
            ++iterClass) {

        fout << "struct " << (*iterClass).first.as<string>() << " {" << endl;

        for (auto iterParam = (*iterClass).second.begin();
                iterParam != (*iterClass).second.end(); ++iterParam) {

            fout << "    double " << (*iterParam).first.as<string>();
            if ((*iterParam).second.IsSequence()) {
                fout << "[" << (*iterParam).second.size() << "]";
            }
            fout << ";" << endl;
        }
        fout << endl;
        fout << "    " << (*iterClass).first.as<string>() << "();" << endl;
        fout << "    void load(const YAML::Node& node);" << endl;
        fout << "    void save(YAML::Node* node) const;" << endl;
        fout << "};" << endl << endl;
    }

    // generate sweep class declarations for each section
    for (auto iterClass = defaults.begin(); iterClass != defaults.end();
            ++iterClass) {

        fout << "struct Sweep" << (*iterClass).first.as<string>() << " {" <<
            endl << "    bool hasSweeps;" << endl;

        for (auto iterParam = (*iterClass).second.begin();
                iterParam != (*iterClass).second.end(); ++iterParam) {

            fout << "    double " << (*iterParam).first.as<string>() <<
                "_begin";
            if ((*iterParam).second.IsSequence()) {
                fout << "[" << (*iterParam).second.size() << "]";
            }
            fout << ";" << endl;
            fout << "    double " << (*iterParam).first.as<string>() <<
                "_end";
            if ((*iterParam).second.IsSequence()) {
                fout << "[" << (*iterParam).second.size() << "]";
            }
            fout << ";" << endl;
        }
        fout << endl;
        fout << "    Sweep" << (*iterClass).first.as<string>() << "();"
            << endl;
        fout << "    " << (*iterClass).first.as<string>() <<
            " generate(double u) const;" << endl;
        fout << "    void load(const YAML::Node& node);" << endl;
        fout << "    void save(YAML::Node* node) const;" << endl;
        fout << "    std::vector<std::string> sweptNames() const;" << endl;
        fout << "    std::vector<double> sweptValues(double u) const;" << endl;
        fout << "};" << endl << endl;
    }

    // generate a class containing all of the other settings
    fout << "struct Settings {" << endl;
    for (auto iterClass = defaults.begin(); iterClass != defaults.end();
            ++iterClass) {

        fout << "    struct " << (*iterClass).first.as<string>() << " "  <<
            (*iterClass).first.as<string>() << ";" << endl;
        fout << "    struct Sweep" << (*iterClass).first.as<string>() << " sweep" <<
            (*iterClass).first.as<string>() << ";" << endl;
    }
    fout << endl;
    fout << "    void load(const YAML::Node& node);" << endl;
    fout << "    void save(YAML::Node* node) const;" << endl;
    fout << "};" << endl << endl;


    if (!isHeader) {
        // generate member function definitions for each class
        for (auto iterClass = defaults.begin(); iterClass != defaults.end();
                ++iterClass) {

            // generate the constructor
            fout << (*iterClass).first.as<string>() <<
                "::" << (*iterClass).first.as<string>() << "() {" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    for (int i = 0; i < (*iterParam).second.size(); ++i) {
                        double val = (*iterParam).second[i].as<double>();
                        fout << "    " << param << "[" << i << "] = ";
                        if (val == val) {
                            fout << val;
                        } else {
                            fout << "std::numeric_limits<double>::quiet_NaN()";
                        }
                        fout << ";" << endl;
                    }
                } else {
                    double val = (*iterParam).second.as<double>();
                    fout << "    " << param << " = ";
                    if (val == val) {
                        fout << val;
                    } else {
                        fout << "std::numeric_limits<double>::quiet_NaN()";
                    }
                    fout << ";" << endl;
                }
            }
            fout << "}" << endl << endl;

            // generate the load function
            fout << "void " << (*iterClass).first.as<string>() <<
                "::load(const YAML::Node& root) {" << endl;
            fout << "const YAML::Node& node = root[\"" <<
                (*iterClass).first.as<string>() << "\"];" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                fout << "    if (node[\"" << param << "\"]) {" << endl;
                if ((*iterParam).second.IsSequence()) {
                    fout << "        for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "            if (node[\"" << param << "\"][i].IsSequence()) {" << endl;
                    fout << "                " << param <<
                        "[i] = node[\"" << param << "\"][i][0].as<double>();" << endl;
                    fout << "            } else {" << endl;
                    fout << "                " << param <<
                        "[i] = node[\"" << param << "\"][i].as<double>();" << endl;
                    fout << "            }" << endl;
                    fout << "        }" << endl;
                } else {
                    fout << "        if (node[\"" << param << "\"].IsSequence()) {" << endl;
                    fout << "            " << param <<
                        " = node[\"" << param << "\"][0].as<double>();" << endl;
                    fout << "        } else {" << endl;
                    fout << "            " << param <<
                        " = node[\"" << param << "\"].as<double>();" << endl;
                    fout << "        }" << endl;
                }
                fout << "    }" << endl;
            }
            fout << "}" << endl << endl;

            // generate the save function
            fout << "void " << (*iterClass).first.as<string>() <<
                "::save(YAML::Node* root) const {" << endl;
            fout << "YAML::Node node = (*root)[\"" <<
                (*iterClass).first.as<string>() << "\"];" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    fout << "    for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "        node[\"" << param << "\"].push_back(" <<
                        param << "[i]);" << endl;
                    fout << "    }" << endl;
                } else {
                    fout << "    node[\"" << param << "\"] = " << param << ";"
                        << endl;
                }
            }
            fout << "}" << endl << endl;

            // generate the constructor for sweeps
            fout << "Sweep" << (*iterClass).first.as<string>() <<
                "::Sweep" << (*iterClass).first.as<string>() << "() {" << endl;

            fout << "    hasSweeps = false;" << endl;
            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    for (int i = 0; i < (*iterParam).second.size(); ++i) {
                        double val = (*iterParam).second[i].as<double>();
                        fout << "    " << param << "_begin[" << i << "] = " <<
                            param << "_end[" << i << "] = ";
                        if (val == val) {
                            fout << val;
                        } else {
                            fout << "std::numeric_limits<double>::quiet_NaN()";
                        }
                        fout << ";" << endl;
                    }
                } else {
                    double val = (*iterParam).second.as<double>();
                    fout << "    " << param << "_begin = " <<
                        param << "_end = ";
                    if (val == val) {
                        fout << val;
                    } else {
                        fout << "std::numeric_limits<double>::quiet_NaN()";
                    }
                    fout << ";" << endl;
                }
            }
            fout << "}" << endl << endl;

            // generate the load function for sweeps
            fout << "void Sweep" << (*iterClass).first.as<string>() <<
                "::load(const YAML::Node& root) {" << endl;
            fout << "const YAML::Node& node = root[\"" <<
                (*iterClass).first.as<string>() << "\"];" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                fout << "    if (node[\"" << param << "\"]) {" << endl;
                if ((*iterParam).second.IsSequence()) {
                    fout << "        for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "            if (node[\"" << param << "\"][i].IsSequence()) {" << endl;
                    fout << "                hasSweeps = true;" << endl;
                    fout << "                " << param <<
                        "_begin[i] = node[\"" << param << "\"][i][0].as<double>();" << endl;
                    fout << "                " << param <<
                        "_end[i] = node[\"" << param << "\"][i][1].as<double>();" << endl;
                    fout << "            } else {" << endl;
                    fout << "                " << param << "_begin[i] = " << param <<
                        "_end[i] = node[\"" << param << "\"][i].as<double>();" << endl;
                    fout << "            }" << endl;
                    fout << "        }" << endl;
                } else {
                    fout << "        if (node[\"" << param << "\"].IsSequence()) {" << endl;
                    fout << "            hasSweeps = true;" << endl;
                    fout << "            " << param <<
                        "_begin = node[\"" << param << "\"][0].as<double>();" << endl;
                    fout << "            " << param <<
                        "_end = node[\"" << param << "\"][1].as<double>();" << endl;
                    fout << "        } else {" << endl;
                    fout << "            " << param << "_begin = " << param <<
                        "_end = node[\"" << param << "\"].as<double>();" << endl;
                    fout << "        }" << endl;
                }
                fout << "    }" << endl;
            }
            fout << "}" << endl << endl;

            // generate the save function for sweeps
            fout << "void Sweep" << (*iterClass).first.as<string>() <<
                "::save(YAML::Node* root) const {" << endl;
            fout << "YAML::Node node = (*root)[\"" <<
                (*iterClass).first.as<string>() << "\"];" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    fout << "    for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "        if (" << param << "_begin[i] != " <<
                        param << "_end[i]) {" << endl;
                    fout << "            node[\"" << param <<
                        "\"].push_back(YAML::Node());" << endl;
                    fout << "            node[\"" << param <<
                        "\"][i].push_back(" << param << "_begin[i]);" << endl;
                    fout << "            node[\"" << param <<
                        "\"][i].push_back(" << param << "_end[i]);" << endl;
                    fout << "        } else {" << endl;
                    fout << "        node[\"" << param << "\"].push_back(" <<
                        param << "_begin[i]);" << endl;
                    fout << "        }" << endl;
                    fout << "    }" << endl;
                } else {
                    fout << "    if (" << param << "_begin != " << param <<
                        "_end) {" << endl;
                    fout << "        node[\"" << param <<
                        "\"] = YAML::Node();" << endl;
                    fout << "        node[\"" << param <<
                        "\"].push_back(" << param << "_begin);" << endl;
                    fout << "        node[\"" << param <<
                        "\"].push_back(" << param << "_end);" << endl;
                    fout << "    } else {" << endl;
                    fout << "        node[\"" << param << "\"] = " << param <<
                        "_begin;" << endl;
                    fout << "    }" << endl;
                }
            }
            fout << "}" << endl << endl;

            // generate the generate function for sweeps
            fout << (*iterClass).first.as<string>() << " Sweep" <<
                (*iterClass).first.as<string>() <<
                "::generate(double u) const {" << endl;

            fout << "    " << (*iterClass).first.as<string>() <<
                " result;" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    fout << "    for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "    result." << param << "[i] = " << param << "_begin[i]" <<
                        "+ (" << param << "_end[i] - " << param << "_begin[i]) * u;" <<
                        endl;
                    fout << "    }" << endl;
                } else {
                    fout << "    result." << param << " = " << param << "_begin" <<
                        "+ (" << param << "_end - " << param << "_begin) * u;" <<
                        endl;
                }
            }
            fout << "    return result;" << endl << endl;
            fout << "}" << endl << endl;


            // generate the sweptNames function for sweeps
            fout << "std::vector<std::string> Sweep" <<
                (*iterClass).first.as<string>() <<
                "::sweptNames() const {" << endl;

            fout << "    std::vector<std::string> result;" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    fout << "    for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "        if (" << param << "_begin[i] != " <<
                        param << "_end[i]) {" << endl;
                    fout << "            result.push_back(\"" << param <<
                        "\" + std::to_string(i));" << endl;
                    fout << "        }" << endl;
                    fout << "    }" << endl;
                } else {
                    fout << "    if (" << param << "_begin != " <<
                        param << "_end) {" << endl;
                    fout << "        result.push_back(\"" << param <<
                        "\");" << endl;
                    fout << "    }" << endl;
                }
            }
            fout << "    return result;" << endl << endl;
            fout << "}" << endl << endl;


            // generate the sweptValues function for sweeps
            fout << "std::vector<double> Sweep" <<
                (*iterClass).first.as<string>() <<
                "::sweptValues(double u) const {" << endl;

            fout << "    std::vector<double> result;" << endl;

            for (auto iterParam = (*iterClass).second.begin();
                    iterParam != (*iterClass).second.end(); ++iterParam) {

                const string& param = (*iterParam).first.as<string>();

                if ((*iterParam).second.IsSequence()) {
                    fout << "    for (int i = 0; i < " <<
                        (*iterParam).second.size() << "; ++i) {" << endl;
                    fout << "        if (" << param << "_begin[i] != " <<
                        param << "_end[i]) {" << endl;
                    fout << "            result.push_back(" << param <<
                        "_begin[i]" << "+ (" << param << "_end[i] - " <<
                        param << "_begin[i]) * u);" << endl;
                    fout << "        }" << endl;
                    fout << "    }" << endl;
                } else {
                    fout << "    if (" << param << "_begin != " <<
                        param << "_end) {" << endl;
                    fout << "        result.push_back(" << param <<
                        "_begin" << "+ (" << param << "_end - " <<
                        param << "_begin) * u);" << endl;
                    fout << "    }" << endl;
                }
            }
            fout << "    return result;" << endl << endl;
            fout << "}" << endl << endl;
        }


        // generate a load for all settings
        fout << "void Settings::load(const YAML::Node& node) {" << endl;
        for (auto iterClass = defaults.begin(); iterClass != defaults.end();
                ++iterClass) {

            fout << "    this->" << (*iterClass).first.as<string>() <<
                ".load(node);"  << endl;
            fout << "    this->sweep" << (*iterClass).first.as<string>() <<
                ".load(node);"  << endl;
        }
        fout << "};" << endl << endl;

        // generate a save for all settings
        fout << "void Settings::save(YAML::Node* node) const {" << endl;
        for (auto iterClass = defaults.begin(); iterClass != defaults.end();
                ++iterClass) {

            fout << "    this->sweep" << (*iterClass).first.as<string>() <<
                ".save(node);"  << endl;
        }
        fout << "};" << endl << endl;
    }

    return 0;
}

