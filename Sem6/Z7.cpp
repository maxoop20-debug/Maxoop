#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class WaypointSorter {
private:
    struct Waypoint {
        int id;
        double x, y, z;
        std::string name;
        double distance;
    };
    std::vector<Waypoint> waypoints;

public:
    bool loadWaypoints(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        int id;
        double x, y, z;
        std::string name;
        char comma;
        while (file >> id >> comma >> x >> comma >> y >> comma >> z >> comma && std::getline(file, name)) {
            waypoints.push_back({id, x, y, z, name, 0.0});
        }
        file.close();
        return true;
    }

    void calculateDistances(double current_x, double current_y, double current_z) {
        for (auto& w : waypoints) {
            double dx = w.x - current_x;
            double dy = w.y - current_y;
            double dz = w.z - current_z;
            w.distance = sqrt(dx*dx + dy*dy + dz*dz);
        }
    }

    void sortByDistance() {
        std::sort(waypoints.begin(), waypoints.end(),
                  [](const Waypoint& a, const Waypoint& b) { return a.distance < b.distance; });
    }

    void saveSortedWaypoints(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& w : waypoints) {
            file << w.id << "," << w.x << "," << w.y << "," << w.z << "," << w.name << "," << w.distance << "\n";
        }
        file.close();
    }
};