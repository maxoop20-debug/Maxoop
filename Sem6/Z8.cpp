#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

class FuelAnalyzer {
private:
    std::vector<double> time_data;
    std::vector<double> fuel_data;
    std::vector<double> rpm_data;

public:
    bool loadData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string header;
        std::getline(file, header);
        double t, f, r;
        char comma;
        while (file >> t >> comma >> f >> comma >> r) {
            time_data.push_back(t);
            fuel_data.push_back(f);
            rpm_data.push_back(r);
        }
        file.close();
        return true;
    }

    void detectAnomalies() {
        double avg = calculateAverageConsumption();
        auto isAnomaly = [avg](double consumption) { return consumption > avg * 1.5; };
        for (size_t i = 0; i < fuel_data.size(); i++) {
            if (isAnomaly(fuel_data[i])) {
                std::cout << "Anomaly at time " << time_data[i] << ": " << fuel_data[i] << "\n";
            }
        }
    }

    double calculateAverageConsumption() {
        if (fuel_data.empty()) return 0.0;
        double sum = std::accumulate(fuel_data.begin(), fuel_data.end(), 0.0);
        return sum / fuel_data.size();
    }

    void generateReport(const std::string& filename) {
        std::ofstream file(filename);
        file << "Average fuel consumption: " << calculateAverageConsumption() << "\n";
        file << "Anomalies detected:\n";
        // логика вывода аномалий
        file.close();
    }
};