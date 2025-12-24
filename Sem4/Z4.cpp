#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

// Структура для точки маршрута
struct Waypoint {
    int id;
    double x, y, z;      // координаты в метрах
    double speed;        // рекомендуемая скорость м/с
    string description;  // описание точки
    
    Waypoint() : id(0), x(0), y(0), z(0), speed(0) {}
    
    Waypoint(int i, double px, double py, double pz, double s, const string& desc)
        : id(i), x(px), y(py), z(pz), speed(s), description(desc) {}
};

// Класс для управления точками маршрута
class WaypointManager {
private:
    vector<Waypoint> waypoints;
    int currentIndex;    // индекс текущей точки маршрута
    string filename;
    
public:
    WaypointManager() : currentIndex(0), filename("waypoints.txt") {}
    
    // Добавление новой точки маршрута
    void addWaypoint(int id, double x, double y, double z, double speed, const string& desc) {
        Waypoint wp(id, x, y, z, speed, desc);
        waypoints.push_back(wp);
        cout << "Точка маршрута добавлена: ID " << id << ", " << desc << endl;
    }
    
    // Сохранение маршрута в файл
    bool saveRoute() {
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << filename << endl;
            return false;
        }
        
        for (size_t i = 0; i < waypoints.size(); i++) {
            const Waypoint& wp = waypoints[i];
            file << wp.id << ","
                 << fixed << setprecision(1)
                 << wp.x << ","
                 << wp.y << ","
                 << wp.z << ","
                 << wp.speed << ","
                 << wp.description << endl;
        }
        
        file.close();
        cout << "Маршрут сохранен в файл: " << filename << endl;
        return true;
    }
    
    // Загрузка маршрута из файла
    bool loadRoute() {
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return false;
        }
        
        waypoints.clear();
        currentIndex = 0;
        string line;
        int loadedCount = 0;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            // Разбиваем строку на токены
            vector<string> tokens;
            stringstream ss(line);
            string token;
            
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 6) {
                int id = atoi(tokens[0].c_str());
                double x = atof(tokens[1].c_str());
                double y = atof(tokens[2].c_str());
                double z = atof(tokens[3].c_str());
                double speed = atof(tokens[4].c_str());
                string desc = tokens[5];
                
                Waypoint wp(id, x, y, z, speed, desc);
                waypoints.push_back(wp);
                loadedCount++;
            }
        }
        
        file.close();
        cout << "Маршрут загружен: " << loadedCount << " точек" << endl;
        return loadedCount > 0;
    }
    
    // Расчет общего расстояния маршрута
    double calculateTotalDistance() {
        if (waypoints.size() < 2) return 0.0;
        
        double totalDistance = 0.0;
        
        for (size_t i = 1; i < waypoints.size(); i++) {
            const Waypoint& wp1 = waypoints[i-1];
            const Waypoint& wp2 = waypoints[i];
            
            double dx = wp2.x - wp1.x;
            double dy = wp2.y - wp1.y;
            double dz = wp2.z - wp1.z;
            
            double segmentDistance = sqrt(dx*dx + dy*dy + dz*dz);
            totalDistance += segmentDistance;
        }
        
        return totalDistance;
    }
    
    // Получение следующей точки маршрута
    Waypoint getNextWaypoint() {
        if (waypoints.empty()) {
            return Waypoint(); // возвращаем пустую точку
        }
        
        // Если достигли конца маршрута, возвращаем последнюю точку
        if (currentIndex >= (int)waypoints.size()) {
            return waypoints.back();
        }
        
        return waypoints[currentIndex];
    }
    
    // Переход к следующей точке
    bool moveToNextWaypoint() {
        if (currentIndex < (int)waypoints.size() - 1) {
            currentIndex++;
            return true;
        }
        return false;
    }
    
    // Проверка достижения текущей точки маршрута
    bool checkWaypointReached(double x, double y, double z, double tolerance = 10.0) {
        if (waypoints.empty() || currentIndex >= (int)waypoints.size()) {
            return false;
        }
        
        const Waypoint& current = waypoints[currentIndex];
        
        double dx = current.x - x;
        double dy = current.y - y;
        double dz = current.z - z;
        
        double distance = sqrt(dx*dx + dy*dy + dz*dz);
        
        return distance <= tolerance;
    }
    
    // Установка текущей точки по ID
    bool setCurrentWaypoint(int id) {
        for (size_t i = 0; i < waypoints.size(); i++) {
            if (waypoints[i].id == id) {
                currentIndex = i;
                return true;
            }
        }
        return false;
    }
    
    // Получение текущей точки
    Waypoint getCurrentWaypoint() {
        if (waypoints.empty()) {
            return Waypoint();
        }
        
        if (currentIndex >= (int)waypoints.size()) {
            return waypoints.back();
        }
        
        return waypoints[currentIndex];
    }
    
    // Вывод информации о маршруте
    void printRouteInfo() {
        cout << "\n=== ИНФОРМАЦИЯ О МАРШРУТЕ ===" << endl;
        
        if (waypoints.empty()) {
            cout << "Маршрут пуст." << endl;
            return;
        }
        
        cout << "Количество точек: " << waypoints.size() << endl;
        
        double totalDist = calculateTotalDistance();
        cout << fixed << setprecision(1);
        cout << "Общее расстояние: " << totalDist << " метров" << endl;
        
        cout << "\nТочки маршрута:" << endl;
        for (size_t i = 0; i < waypoints.size(); i++) {
            const Waypoint& wp = waypoints[i];
            cout << "  " << (i+1) << ". ID " << wp.id << ": " << wp.description 
                 << " (" << wp.x << ", " << wp.y << ", " << wp.z << ")"
                 << ", Speed: " << wp.speed << " м/с" << endl;
        }
        
        cout << "\nТекущая точка: ";
        if (currentIndex < (int)waypoints.size()) {
            const Waypoint& current = waypoints[currentIndex];
            cout << current.description << " (" << current.x << ", " 
                 << current.y << ", " << current.z << ")" << endl;
        } else {
            cout << "Не установлена" << endl;
        }
    }
    
    // Создание тестового маршрута
    void createTestRoute() {
        cout << "Создание тестового маршрута..." << endl;
        
        addWaypoint(1, 0.0, 0.0, 100.0, 25.0, "Start");
        addWaypoint(2, 100.0, 50.0, 150.0, 30.0, "CheckpointA");
        addWaypoint(3, 200.0, 100.0, 200.0, 35.0, "CheckpointB");
        addWaypoint(4, 300.0, 200.0, 250.0, 40.0, "Finish");
        
        saveRoute();
    }
    
    // Имитация движения по маршруту
    void simulateMovement() {
        if (waypoints.empty()) {
            cout << "Маршрут не загружен!" << endl;
            return;
        }
        
        cout << "\n=== СИМУЛЯЦИЯ ДВИЖЕНИЯ ПО МАРШРУТУ ===" << endl;
        
        // Начинаем с первой точки
        currentIndex = 0;
        
        // Имитируем движение между точками
        for (size_t i = 0; i < waypoints.size(); i++) {
            Waypoint current = getCurrentWaypoint();
            Waypoint next = getNextWaypoint();
            
            cout << "\nТекущая точка: " << current.description 
                 << " (" << current.x << ", " << current.y << ", " << current.z << ")" << endl;
            
            if (i < waypoints.size() - 1) {
                cout << "Следующая точка: " << waypoints[i+1].description 
                     << " (" << waypoints[i+1].x << ", " 
                     << waypoints[i+1].y << ", " << waypoints[i+1].z << ")" << endl;
                
                // Расчет расстояния до следующей точки
                double dx = waypoints[i+1].x - current.x;
                double dy = waypoints[i+1].y - current.y;
                double dz = waypoints[i+1].z - current.z;
                double distance = sqrt(dx*dx + dy*dy + dz*dz);
                
                cout << "Расстояние до следующей точки: " << fixed << setprecision(1) 
                     << distance << " м" << endl;
                
                // Расчет времени до следующей точки
                double time = distance / current.speed;
                cout << "Примерное время движения: " << time << " с" << endl;
                
                // Переход к следующей точке
                moveToNextWaypoint();
            } else {
                cout << "Конечная точка маршрута достигнута!" << endl;
            }
        }
    }
    
    // Получение статистики маршрута
    void getRouteStatistics() {
        if (waypoints.empty()) {
            cout << "Маршрут пуст." << endl;
            return;
        }
        
        cout << "\n=== СТАТИСТИКА МАРШРУТА ===" << endl;
        
        double totalDist = calculateTotalDistance();
        double totalTime = 0;
        double maxSpeed = waypoints[0].speed;
        double minSpeed = waypoints[0].speed;
        double avgSpeed = 0;
        
        // Расчет общего времени и поиск мин/макс скоростей
        for (size_t i = 1; i < waypoints.size(); i++) {
            const Waypoint& wp1 = waypoints[i-1];
            const Waypoint& wp2 = waypoints[i];
            
            double dx = wp2.x - wp1.x;
            double dy = wp2.y - wp1.y;
            double dz = wp2.z - wp1.z;
            double distance = sqrt(dx*dx + dy*dy + dz*dz);
            
            double segmentTime = distance / wp1.speed;
            totalTime += segmentTime;
            
            if (wp1.speed > maxSpeed) maxSpeed = wp1.speed;
            if (wp1.speed < minSpeed) minSpeed = wp1.speed;
        }
        
        // Проверяем последнюю точку
        const Waypoint& last = waypoints.back();
        if (last.speed > maxSpeed) maxSpeed = last.speed;
        if (last.speed < minSpeed) minSpeed = last.speed;
        
        // Средняя скорость
        if (totalTime > 0) {
            avgSpeed = totalDist / totalTime;
        }
        
        cout << fixed << setprecision(1);
        cout << "Количество точек: " << waypoints.size() << endl;
        cout << "Общее расстояние: " << totalDist << " м" << endl;
        cout << "Общее время: " << totalTime << " с (" << totalTime/60.0 << " мин)" << endl;
        cout << "Максимальная скорость: " << maxSpeed << " м/с (" << maxSpeed*3.6 << " км/ч)" << endl;
        cout << "Минимальная скорость: " << minSpeed << " м/с (" << minSpeed*3.6 << " км/ч)" << endl;
        cout << "Средняя скорость: " << avgSpeed << " м/с (" << avgSpeed*3.6 << " км/ч)" << endl;
    }
};

// Главная функция
int main() {
    cout << "=== ОБРАБОТЧИК WAYPOINTS ===" << endl;
    
    // Создаем менеджер точек маршрута
    WaypointManager manager;
    
    // Загружаем или создаем маршрут
    if (!manager.loadRoute()) {
        cout << "Файл маршрута не найден. Создаю тестовый маршрут..." << endl;
        manager.createTestRoute();
    }
    
    // Выводим информацию о маршруте
    manager.printRouteInfo();
    
    // Выводим статистику
    manager.getRouteStatistics();
    
    // Демонстрация работы методов
    cout << "\n=== ДЕМОНСТРАЦИЯ РАБОТЫ МЕТОДОВ ===" << endl;
    
    // Расчет общего расстояния
    double totalDist = manager.calculateTotalDistance();
    cout << "Общее расстояние маршрута: " << fixed << setprecision(1) << totalDist << " м" << endl;
    
    // Получение текущей и следующей точки
    manager.setCurrentWaypoint(1); // Устанавливаем текущую точку как Start
    Waypoint current = manager.getCurrentWaypoint();
    cout << "Текущая точка: " << current.description 
         << " (" << current.x << ", " << current.y << ", " << current.z << ")" << endl;
    
    Waypoint next = manager.getNextWaypoint();
    cout << "Следующая точка: " << next.description 
         << " (" << next.x << ", " << next.y << ", " << next.z << ")" << endl;
    
    // Проверка достижения точки
    cout << "\nПроверка достижения точки:" << endl;
    
    // Координаты близко к Start (должны быть достигнуты)
    bool reached1 = manager.checkWaypointReached(5.0, 5.0, 100.0, 10.0);
    cout << "Координаты (5, 5, 100): " << (reached1 ? "ДОСТИГНУТА" : "НЕ ДОСТИГНУТА") << endl;
    
    // Координаты далеко от Start (не должны быть достигнуты)
    bool reached2 = manager.checkWaypointReached(50.0, 50.0, 100.0, 10.0);
    cout << "Координаты (50, 50, 100): " << (reached2 ? "ДОСТИГНУТА" : "НЕ ДОСТИГНУТА") << endl;
    
    // Добавление новой точки
    cout << "\nДобавление новой точки маршрута:" << endl;
    manager.addWaypoint(5, 400.0, 300.0, 300.0, 45.0, "BonusPoint");
    
    // Сохранение обновленного маршрута
    manager.saveRoute();
    
    // Перезагрузка маршрута для проверки
    cout << "\nПерезагрузка маршрута из файла:" << endl;
    WaypointManager manager2;
    manager2.loadRoute();
    manager2.printRouteInfo();
    
    // Симуляция движения по маршруту
    cout << "\n=== СИМУЛЯЦИЯ ДВИЖЕНИЯ ===" << endl;
    manager.simulateMovement();
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    
    return 0;
}