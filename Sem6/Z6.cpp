#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class TelemetryFilter {
private:
    std::vector<std::vector<std::string>> data;

public:
    bool loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<std::string> row;
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();
        return true;
    }

    void filterData() {
        auto isValidAltitude = [](double alt) { return alt >= 0 && alt <= 20000; };
        auto isValidSpeed = [](double speed) { return speed >= 0 && speed <= 500; };

        std::vector<std::vector<std::string>> filtered;
        filtered.push_back(data[0]); // header
        for (size_t i = 1; i < data.size(); i++) {
            double alt = std::stod(data[i][1]);
            double speed = std::stod(data[i][2]);
            if (isValidAltitude(alt) && isValidSpeed(speed)) {
                filtered.push_back(data[i]);
            }
        }
        data = filtered;
    }

    bool saveToCSV(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& row : data) {
            for (size_t j = 0; j < row.size(); j++) {
                file << row[j];
                if (j < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
        return true;
    }

    void printFilteredStats() {
        std::cout << "Filtered rows: " << data.size() - 1 << "\n";
    }
};