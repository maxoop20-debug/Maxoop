#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

class WaypointManager {
private:
    struct Point {
        int id;
        double x, y, z;
        double speed;
        std::string description;
    };
    Point current_waypoint;
    std::vector<Point> points;
    
public:
    void addWaypoint(int id, double x, double y, double z, double speed, const std::string& desc) {
        Point p;
        p.id = id;
        p.x = x;
        p.y = y;
        p.z = z;
        p.speed = speed;
        p.description = desc;
        points.push_back(p);
        
        if (points.size() == 1) {
            current_waypoint = points[0];
        }
        std::cout << "Точка " << id << " добавлена в маршрут." << std::endl;
    }

    bool saveRoute() {
        std::ofstream fout("waypoints.txt");
        if (!fout.is_open()) {
            std::cout << "Ошибка открытия файла!" << std::endl;
            return false;  // Changed from true to false
        }
        for (size_t i = 0; i < points.size(); i++) {
            fout << points[i].id << "," << points[i].x << "," << points[i].y << "," 
                 << points[i].z << "," << points[i].speed << "," << points[i].description << "\n";
        }
        std::cout << "Точки успешно записаны в файл!\n";
        fout.close();
        return true;
    }
    
    bool loadRoute() {
        std::ifstream fin("waypoints.txt");
        if (!fin.is_open()) {
            std::cout << "Ошибка при открытии файла!" << std::endl;
            return false;
        }
        
        points.clear();  // Clear existing points before loading
        std::string line;
        
        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string value;
            Point p;
            
            if (std::getline(ss, value, ',')) p.id = std::stoi(value);
            if (std::getline(ss, value, ',')) p.x = std::stod(value);
            if (std::getline(ss, value, ',')) p.y = std::stod(value);
            if (std::getline(ss, value, ',')) p.z = std::stod(value);
            if (std::getline(ss, value, ',')) p.speed = std::stod(value);
            if (std::getline(ss, value, ',')) p.description = value;
            
            points.push_back(p);
            
            std::cout << "Загруженные данные\n";
            std::cout << "ID: " << p.id << "\tX: " << p.x << "\tY: " << p.y 
                      << "\tZ: " << p.z << "\tSpeed: " << p.speed 
                      << "\tDescription: " << p.description << "\n";
        }
        fin.close();
        std::cout << "Маршрут загружен: " << points.size() << " точек\n";
        return true;
    }
    
    double CalculateTotalDistance() {
        double TotalDistance = 0.0;  // Must initialize to 0
        if (points.size() < 2) {
            std::cout << "Невозможно рассчитать расстояние, не хватает точек!\n";
            return 0.0;
        }
        
        for (size_t i = 0; i < points.size() - 1; i++) {
            double dx = points[i].x - points[i + 1].x;
            double dy = points[i].y - points[i + 1].y;
            double dz = points[i].z - points[i + 1].z;
            TotalDistance += sqrt(dx*dx + dy*dy + dz*dz);
        }
        
        std::cout << "Общее расстояние: " << TotalDistance << " метров\n";
        return TotalDistance;
    }
    
    Point getCurrentWaypoint() {
        return current_waypoint;
    }
    
    Point getNextWaypoint() {
        for (size_t i = 0; i < points.size(); i++) {
            if (current_waypoint.id == points[i].id) {
                std::cout << "Текущая точка: " << current_waypoint.description 
                          << " (" << current_waypoint.x << ", " << current_waypoint.y
                          << ", " << current_waypoint.z << ")\n";
                
                if (i == points.size() - 1) {  // Last point
                    std::cout << "Это последняя точка в маршруте.\n";
                    return current_waypoint;
                } else {
                    std::cout << "Следующая точка: " << points[i + 1].description 
                              << " (" << points[i + 1].x << ", " << points[i + 1].y
                              << ", " << points[i + 1].z << ")\n";
                    return points[i + 1];
                }
            }
        }
        
        std::cout << "Текущая точка не найдена в маршруте.\n";
        return current_waypoint;
    }
};

int main() {
    WaypointManager way_man;
    way_man.addWaypoint(1, 0.0, 0.0, 100.0, 25.0, "Start");
    way_man.addWaypoint(2, 100.0, 50.0, 150.0, 30.0, "CheckpointA");
    way_man.addWaypoint(3, 200.0, 100.0, 200.0, 35.0, "CheckpointB");
    
    bool save = way_man.saveRoute();
    bool load = way_man.loadRoute();
    double total = way_man.CalculateTotalDistance();
    way_man.getNextWaypoint();
    
    return 0;
}