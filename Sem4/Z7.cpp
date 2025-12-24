#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Структура для точки маршрута
struct Waypoint {
    int id;
    double x, y, z;
    string name;
    double distance;
    
    Waypoint() : id(0), x(0), y(0), z(0), distance(0) {}
    
    Waypoint(int i, double px, double py, double pz, const string& n) 
        : id(i), x(px), y(py), z(pz), name(n), distance(0) {}
};

// Функция сравнения для сортировки по расстоянию
bool compareByDistance(const Waypoint& a, const Waypoint& b) {
    return a.distance < b.distance;
}

// Класс для сортировки точек маршрута
class WaypointSorter {
private:
    vector<Waypoint> waypoints;
    
public:
    // Загрузка точек маршрута из файла
    bool loadWaypoints(const string& filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }
        
        waypoints.clear();
        string line;
        int lineNumber = 0;
        
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            // Разбиваем строку на поля
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }
            
            // Должно быть 5 полей: id, x, y, z, name
            if (fields.size() >= 5) {
                int id = atoi(fields[0].c_str());
                double x = atof(fields[1].c_str());
                double y = atof(fields[2].c_str());
                double z = atof(fields[3].c_str());
                string name = fields[4];
                
                // Удаляем возможные пробелы в имени
                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);
                
                Waypoint wp(id, x, y, z, name);
                waypoints.push_back(wp);
            } else {
                cerr << "Предупреждение: строка " << lineNumber 
                     << " имеет неверный формат" << endl;
            }
        }
        
        file.close();
        cout << "Загружено " << waypoints.size() << " точек маршрута" << endl;
        return !waypoints.empty();
    }
    
    // Расчет расстояний от текущей позиции
    void calculateDistances(double current_x, double current_y, double current_z) {
        for (size_t i = 0; i < waypoints.size(); i++) {
            Waypoint& wp = waypoints[i];
            
            double dx = wp.x - current_x;
            double dy = wp.y - current_y;
            double dz = wp.z - current_z;
            
            wp.distance = sqrt(dx*dx + dy*dy + dz*dz);
        }
    }
    
    // Сортировка точек по расстоянию
    void sortByDistance() {
        sort(waypoints.begin(), waypoints.end(), compareByDistance);
        cout << "Точки маршрута отсортированы по расстоянию" << endl;
    }
    
    // Сохранение отсортированных точек в файл
    void saveSortedWaypoints(const string& filename) {
        ofstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось создать файл " << filename << endl;
            return;
        }
        
        // Записываем отсортированные данные
        file << "id,x,y,z,name,distance" << endl;
        
        for (size_t i = 0; i < waypoints.size(); i++) {
            const Waypoint& wp = waypoints[i];
            file << fixed << setprecision(2);
            file << wp.id << ","
                 << wp.x << ","
                 << wp.y << ","
                 << wp.z << ","
                 << wp.name << ","
                 << wp.distance << endl;
        }
        
        file.close();
        cout << "Отсортированные точки сохранены в файл: " << filename << endl;
    }
    
    // Вывод отсортированного списка на экран
    void printSortedWaypoints() {
        cout << "\n=== ОТСОРТИРОВАННЫЕ ТОЧКИ МАРШРУТА ===" << endl;
        cout << left << setw(5) << "ID" 
             << setw(15) << "Название" 
             << setw(15) << "Координаты (x,y,z)" 
             << setw(12) << "Расстояние" << endl;
        cout << string(50, '-') << endl;
        
        for (size_t i = 0; i < waypoints.size(); i++) {
            const Waypoint& wp = waypoints[i];
            cout << left << setw(5) << wp.id
                 << setw(15) << wp.name
                 << fixed << setprecision(1);
            
            string coords = "(" + toString(wp.x) + ", " + 
                            toString(wp.y) + ", " + 
                            toString(wp.z) + ")";
            cout << setw(15) << coords
                 << setw(12) << setprecision(2) << wp.distance << " м" << endl;
        }
    }
    
    // Создание тестового файла
    static void createTestFile() {
        ofstream file("waypoints.txt");
        
        file << "1,1000,500,2000,CheckpointA" << endl;
        file << "2,2000,1500,2500,CheckpointB" << endl;
        file << "3,500,1000,1500,CheckpointC" << endl;
        file << "4,3000,2000,3000,CheckpointD" << endl;
        
        file.close();
        cout << "Тестовый файл создан: waypoints.txt" << endl;
    }
    
    // Получение ближайшей точки
    Waypoint getNearestWaypoint() {
        if (waypoints.empty()) {
            return Waypoint();
        }
        
        // Поскольку список отсортирован, первая точка - ближайшая
        return waypoints[0];
    }
    
    // Получение дальней точки
    Waypoint getFarthestWaypoint() {
        if (waypoints.empty()) {
            return Waypoint();
        }
        
        // Последняя точка - самая дальняя
        return waypoints[waypoints.size() - 1];
    }
    
    // Вывод статистики
    void printStatistics() {
        if (waypoints.empty()) {
            cout << "Нет данных для статистики" << endl;
            return;
        }
        
        cout << "\n=== СТАТИСТИКА РАССТОЯНИЙ ===" << endl;
        
        double minDist = waypoints[0].distance;
        double maxDist = waypoints[waypoints.size() - 1].distance;
        double avgDist = 0;
        
        for (size_t i = 0; i < waypoints.size(); i++) {
            avgDist += waypoints[i].distance;
        }
        avgDist /= waypoints.size();
        
        cout << fixed << setprecision(2);
        cout << "Ближайшая точка: " << waypoints[0].name 
             << " (" << minDist << " м)" << endl;
        cout << "Дальняя точка: " << waypoints[waypoints.size() - 1].name 
             << " (" << maxDist << " м)" << endl;
        cout << "Среднее расстояние: " << avgDist << " м" << endl;
        cout << "Разброс расстояний: " << maxDist - minDist << " м" << endl;
    }
    
private:
    // Вспомогательная функция для преобразования double в string
    string toString(double value) {
        stringstream ss;
        ss << fixed << setprecision(1) << value;
        return ss.str();
    }
};

// Главная функция
int main() {
    cout << "=== СОРТИРОВКА WAYPOINTS ПО РАССТОЯНИЮ ===" << endl;
    
    // Создаем тестовый файл
    WaypointSorter::createTestFile();
    
    // Создаем сортировщик
    WaypointSorter sorter;
    
    // Загружаем точки маршрута
    if (!sorter.loadWaypoints("waypoints.txt")) {
        return 1;
    }
    
    // Текущая позиция ЛА
    double current_x = 800;
    double current_y = 600;
    double current_z = 1800;
    
    cout << "\nТекущая позиция ЛА:" << endl;
    cout << "X = " << current_x << " м" << endl;
    cout << "Y = " << current_y << " м" << endl;
    cout << "Z = " << current_z << " м" << endl;
    
    // Рассчитываем расстояния
    cout << "\nРасчет расстояний от текущей позиции..." << endl;
    sorter.calculateDistances(current_x, current_y, current_z);
    
    // Сортируем по расстоянию
    sorter.sortByDistance();
    
    // Выводим отсортированный список
    sorter.printSortedWaypoints();
    
    // Сохраняем в файл
    sorter.saveSortedWaypoints("waypoints_sorted.csv");
    
    // Выводим статистику
    sorter.printStatistics();
    
    // Дополнительный тест с другой позицией
    cout << "\n=== ТЕСТ С ДРУГОЙ ПОЗИЦИЕЙ ===" << endl;
    
    WaypointSorter sorter2;
    sorter2.loadWaypoints("waypoints.txt");
    
    // Новая позиция
    double new_x = 1500;
    double new_y = 1200;
    double new_z = 2200;
    
    cout << "\nНовая позиция ЛА:" << endl;
    cout << "X = " << new_x << " м" << endl;
    cout << "Y = " << new_y << " м" << endl;
    cout << "Z = " << new_z << " м" << endl;
    
    sorter2.calculateDistances(new_x, new_y, new_z);
    sorter2.sortByDistance();
    sorter2.printSortedWaypoints();
    sorter2.saveSortedWaypoints("waypoints_sorted_newpos.csv");
    
    // Демонстрация с пользовательским вводом
    cout << "\n=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===" << endl;
    
    char choice;
    do {
        cout << "\nВведите текущую позицию ЛА:" << endl;
        
        double user_x, user_y, user_z;
        cout << "X: ";
        cin >> user_x;
        cout << "Y: ";
        cin >> user_y;
        cout << "Z: ";
        cin >> user_z;
        
        WaypointSorter userSorter;
        userSorter.loadWaypoints("waypoints.txt");
        userSorter.calculateDistances(user_x, user_y, user_z);
        userSorter.sortByDistance();
        userSorter.printSortedWaypoints();
        
        // Ближайшая точка
        Waypoint nearest = userSorter.getNearestWaypoint();
        cout << "\nБлижайшая точка: " << nearest.name 
             << " (ID: " << nearest.id << ")" << endl;
        cout << "Расстояние: " << nearest.distance << " м" << endl;
        
        // Дальняя точка
        Waypoint farthest = userSorter.getFarthestWaypoint();
        cout << "Дальняя точка: " << farthest.name 
             << " (ID: " << farthest.id << ")" << endl;
        cout << "Расстояние: " << farthest.distance << " м" << endl;
        
        cout << "\nПовторить расчет? (y/n): ";
        cin >> choice;
        
    } while (choice == 'y' || choice == 'Y');
    
    // Создание файла с дополнительными точками для демонстрации
    cout << "\n=== ДОПОЛНИТЕЛЬНЫЙ ТЕСТ ===" << endl;
    
    ofstream extraFile("waypoints_extra.txt");
    extraFile << "1,100,200,300,База" << endl;
    extraFile << "2,500,600,700,Точка1" << endl;
    extraFile << "3,1000,1100,1200,Точка2" << endl;
    extraFile << "4,1500,1600,1700,Точка3" << endl;
    extraFile << "5,2000,2100,2200,Точка4" << endl;
    extraFile << "6,2500,2600,2700,Точка5" << endl;
    extraFile.close();
    
    cout << "Создан файл с дополнительными точками: waypoints_extra.txt" << endl;
    
    WaypointSorter extraSorter;
    extraSorter.loadWaypoints("waypoints_extra.txt");
    extraSorter.calculateDistances(800, 600, 1800);
    extraSorter.sortByDistance();
    extraSorter.printSortedWaypoints();
    extraSorter.saveSortedWaypoints("waypoints_extra_sorted.csv");
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    cout << "Созданные файлы:" << endl;
    cout << "  waypoints.txt - исходные точки" << endl;
    cout << "  waypoints_sorted.csv - отсортированные точки (позиция 1)" << endl;
    cout << "  waypoints_sorted_newpos.csv - отсортированные точки (позиция 2)" << endl;
    cout << "  waypoints_extra.txt - дополнительные точки" << endl;
    cout << "  waypoints_extra_sorted.csv - отсортированные дополнительные точки" << endl;
    
    return 0;
}