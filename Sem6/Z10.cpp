#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct TrajectoryPoint {
    double time, velocity, altitude, distance, fuel;
};

class Trajectory {
private:
    std::vector<TrajectoryPoint> points;

public:
    bool saveToCSV(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << "time,velocity,altitude,distance,fuel\n";
        for (const auto& p : points) {
            file << p.time << "," << p.velocity << "," << p.altitude << ","
                 << p.distance << "," << p.fuel << "\n";
        }
        file.close();
        return true;
    }

    void generatePlotScript(const std::string& filename) {
        std::ofstream file(filename);
        file << "set terminal png\n";
        file << "set output 'trajectory_plot.png'\n";
        file << "plot 'trajectory.csv' using 1:3 with lines title 'Altitude'\n";
        file.close();
    }

    void saveReport(const std::string& filename) {
        double totalTime = 0, totalFuel = 0, avgVel = 0;
        if (!points.empty()) {
            totalTime = points.back().time;
            totalFuel = points.back().fuel;
            for (const auto& p : points) avgVel += p.velocity;
            avgVel /= points.size();
        }
        std::ofstream file(filename);
        file << "Total flight time: " << totalTime << " s\n";
        file << "Total fuel consumed: " << totalFuel << " kg\n";
        file << "Average velocity: " << avgVel << " m/s\n";
        file.close();
    }
};