#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

class TargetManager {
private:
    struct Target {
        int id;
        std::string name;
        double x, y, z;
        double priority;
        double distance;
    };
    std::vector<Target> targets;
    
public:
    void addTarget(int id, const std::string& name, double x, double y, double z, double priority, double distance) {
        Target t;
        t.id = id;
        t.name = name;
        t.x = x;
        t.y = y;
        t.z = z;
        t.priority = priority;
        t.distance = distance;
        targets.push_back(t);
    }
    
    bool removeTarget(int target_id) {
        for (size_t i = 0; i < targets.size(); i++) {
            if (targets[i].id == target_id) {
                targets.erase(targets.begin() + i);
                return true;
            }
        }
        std::cout << "ID: " << target_id << " не найден!";
        return false;
    }
    
    void saveTargetsToFile() {
        std::ofstream fout("targets.txt");
        if (!fout.is_open()) {
            throw std::runtime_error("Невозможно открыть файл!");
        }
        fout << std::fixed << std::setw(5) << std::right << "ID "
             << std::setw(10) << "Name " << std::setw(5) << "X " << std::setw(5) << "Y " << std::setw(5) << "Z "
             << std::setw(10) << "Priority " << std::setw(10) << "Distance " << "\n";
        
        // Using iterator instead of range-based for loop
        for (std::vector<Target>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
            fout << it->id << "," << it->name << "," << it->x << ","
                 << it->y << "," << it->z << "," << it->priority << "," << it->distance << "\n";
        }
        fout.close();
    }
    
    void loadTargetsFromFile() {
        std::ifstream fin("targets.txt");
        if (!fin.is_open()) {
            throw std::runtime_error("Ошибка открытия файла!");
        }
        
        targets.clear();
        std::string line;
        std::getline(fin, line); // Skip header
        
        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string value;
            Target t;
            
            std::getline(ss, value, ',');
            t.id = atoi(value.c_str());
            
            std::getline(ss, t.name, ',');
            
            std::getline(ss, value, ',');
            t.x = atof(value.c_str());
            
            std::getline(ss, value, ',');
            t.y = atof(value.c_str());
            
            std::getline(ss, value, ',');
            t.z = atof(value.c_str());
            
            std::getline(ss, value, ',');
            t.priority = atof(value.c_str());
            
            std::getline(ss, value, ',');
            t.distance = atof(value.c_str());
            
            targets.push_back(t);
        }
        fin.close();
    }
    
    std::vector<Target> getHighPriorityTargets(double min_priority) {
        std::vector<Target> high_targets;
        // Using iterator instead of range-based for loop
        for (std::vector<Target>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
            if (it->priority >= min_priority)
                high_targets.push_back(*it);
        }
        return high_targets;
    }
    
    void SortByDistance() {
        for (size_t i = 0; i < targets.size(); i++) {
            for (size_t j = 0; j < targets.size() - i - 1; j++) {
                if (targets[j].distance > targets[j + 1].distance) {
                    std::swap(targets[j], targets[j + 1]);
                }
            }
        }
    }
};

int main() {
    TargetManager man;
    man.addTarget(1, "Point1", 2.34, 4.34, 6.56, 0.8, 1000);
    man.addTarget(2, "Point2", 44, 1.34, 5.6, 0.9, 3000);
    man.addTarget(3, "Point3", 2.3324324, 65.323, 75.12, 1, 2000);
    man.saveTargetsToFile();
    man.loadTargetsFromFile();
    man.SortByDistance();
    man.saveTargetsToFile();
    return 0;
}