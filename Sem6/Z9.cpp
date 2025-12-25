#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class Aircraft {
private:
    std::map<std::string, double> params;

public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            size_t eq = line.find('=');
            if (eq != std::string::npos) {
                std::string key = line.substr(0, eq);
                double value = std::stod(line.substr(eq + 1));
                params[key] = value;
            }
        }
        file.close();
        return true;
    }
};

class Environment {
private:
    std::vector<std::tuple<double, double, double>> atmosphereTable; // alt, density, pressure

public:
    bool loadAtmosphereTable(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string header;
        std::getline(file, header);
        double alt, dens, press;
        char comma;
        while (file >> alt >> comma >> dens >> comma >> press) {
            atmosphereTable.push_back({alt, dens, press});
        }
        file.close();
        return true;
    }

    double getDensity(double altitude) {
        for (const auto& row : atmosphereTable) {
            if (std::get<0>(row) >= altitude) return std::get<1>(row);
        }
        return 1.225; // default sea level
    }
};