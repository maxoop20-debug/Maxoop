#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Структура для хранения информации о цели
struct Target {
    int id;
    string name;
    double x, y, z;
    double priority;
    double distance;
};

// Класс для управления целями
class TargetManager {
private:
    vector<Target> targets;
    string filename = "targets.txt";
    
public:
    // Добавление новой цели
    void addTarget(int id, const string& name, double x, double y, double z,
                  double priority, double distance) {
        Target newTarget;
        newTarget.id = id;
        newTarget.name = name;
        newTarget.x = x;
        newTarget.y = y;
        newTarget.z = z;
        newTarget.priority = priority;
        newTarget.distance = distance;
        
        targets.push_back(newTarget);
        cout << "Цель добавлена: ID " << id << ", " << name << endl;
    }
    
    // Удаление цели по ID
    bool removeTarget(int target_id) {
        for (size_t i = 0; i < targets.size(); i++) {
            if (targets[i].id == target_id) {
                cout << "Цель удалена: ID " << target_id << ", " << targets[i].name << endl;
                targets.erase(targets.begin() + i);
                return true;
            }
        }
        cout << "Цель с ID " << target_id << " не найдена" << endl;
        return false;
    }
    
    // Сохранение целей в файл
    void saveTargetsToFile() {
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << filename << endl;
            return;
        }
        
        for (size_t i = 0; i < targets.size(); i++) {
            const Target& t = targets[i];
            file << t.id << ","
                 << t.name << ","
                 << fixed << setprecision(1)
                 << t.x << ","
                 << t.y << ","
                 << t.z << ","
                 << t.priority << ","
                 << t.distance << endl;
        }
        
        file.close();
        cout << "Цели сохранены в файл: " << filename << endl;
    }
    
    // Загрузка целей из файла
    void loadTargetsFromFile() {
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Файл " << filename << " не найден или не может быть открыт" << endl;
            return;
        }
        
        targets.clear();
        string line;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Target t;
            stringstream ss(line);
            string token;
            
            // Читаем ID
            getline(ss, token, ',');
            t.id = atoi(token.c_str());
            
            // Читаем имя
            getline(ss, t.name, ',');
            
            // Читаем координаты X
            getline(ss, token, ',');
            t.x = atof(token.c_str());
            
            // Читаем координаты Y
            getline(ss, token, ',');
            t.y = atof(token.c_str());
            
            // Читаем координаты Z
            getline(ss, token, ',');
            t.z = atof(token.c_str());
            
            // Читаем приоритет
            getline(ss, token, ',');
            t.priority = atof(token.c_str());
            
            // Читаем дистанцию
            getline(ss, token, ',');
            t.distance = atof(token.c_str());
            
            targets.push_back(t);
        }
        
        file.close();
        cout << "Загружено " << targets.size() << " целей из файла: " << filename << endl;
    }
    
    // Получение целей с высоким приоритетом
    vector<Target> getHighPriorityTargets(double min_priority) {
        vector<Target> highPriorityTargets;
        
        for (size_t i = 0; i < targets.size(); i++) {
            if (targets[i].priority >= min_priority) {
                highPriorityTargets.push_back(targets[i]);
            }
        }
        
        return highPriorityTargets;
    }
    
    // Вывод всех целей
    void printAllTargets() {
        cout << "\n=== ЦЕЛИ В СИСТЕМЕ ===" << endl;
        
        if (targets.empty()) {
            cout << "Нет целей в системе" << endl;
            return;
        }
        
        for (size_t i = 0; i < targets.size(); i++) {
            const Target& t = targets[i];
            cout << "ID: " << t.id << ", "
                 << t.name << ", "
                 << "Position: (" << t.x << ", " << t.y << ", " << t.z << "), "
                 << "Priority: " << t.priority << ", "
                 << "Distance: " << t.distance << " м" << endl;
        }
        cout << "Всего целей: " << targets.size() << endl;
    }
    
    // Найти цель по ID
    Target* findTargetById(int id) {
        for (size_t i = 0; i < targets.size(); i++) {
            if (targets[i].id == id) {
                return &targets[i];
            }
        }
        return NULL;
    }
    
    // Получить ближайшую цель
    Target getClosestTarget() {
        if (targets.empty()) {
            Target empty;
            empty.id = -1;
            return empty;
        }
        
        Target closest = targets[0];
        for (size_t i = 1; i < targets.size(); i++) {
            if (targets[i].distance < closest.distance) {
                closest = targets[i];
            }
        }
        return closest;
    }
    
    // Получить самую приоритетную цель
    Target getHighestPriorityTarget() {
        if (targets.empty()) {
            Target empty;
            empty.id = -1;
            return empty;
        }
        
        Target highest = targets[0];
        for (size_t i = 1; i < targets.size(); i++) {
            if (targets[i].priority > highest.priority) {
                highest = targets[i];
            }
        }
        return highest;
    }
};

// Вспомогательная функция для вывода вектора целей
void printTargets(const vector<Target>& targets, const string& title) {
    cout << "\n=== " << title << " ===" << endl;
    
    if (targets.empty()) {
        cout << "Нет целей" << endl;
        return;
    }
    
    for (size_t i = 0; i < targets.size(); i++) {
        const Target& t = targets[i];
        cout << "ID: " << t.id << ", "
             << t.name << ", "
             << "Position: (" << t.x << ", " << t.y << ", " << t.z << "), "
             << "Priority: " << t.priority << ", "
             << "Distance: " << t.distance << " м" << endl;
    }
}

// Создание тестового файла с начальными данными
void createTestFile() {
    ofstream file("targets.txt");
    file << "1,Name1,100.5,200.3,50.0,0.8,1500.0" << endl;
    file << "2,Name2,150.2,180.7,300.0,0.9,2000.0" << endl;
    file << "3,Name3,50.0,50.0,10.0,0.1,500.0" << endl;
    file.close();
    cout << "Тестовый файл создан: targets.txt" << endl;
}

int main() {
    cout << "=== МЕНЕДЖЕР ЦЕЛЕЙ НАВЕДЕНИЯ ===" << endl;
    
    // Создаем тестовый файл, если его нет
    createTestFile();
    
    // Создаем менеджер целей
    TargetManager manager;
    
    // Загружаем цели из файла
    manager.loadTargetsFromFile();
    
    // Выводим все цели
    manager.printAllTargets();
    
    // Добавляем новую цель (как в примере)
    cout << "\n--- Добавление новой цели ---" << endl;
    manager.addTarget(4, "Name4", 200.0, 100.0, 20.0, 0.7, 1800.0);
    
    // Выводим обновленный список
    manager.printAllTargets();
    
    // Демонстрация других методов
    cout << "\n--- Дополнительные операции ---" << endl;
    
    // Поиск высокоприоритетных целей
    vector<Target> highPriority = manager.getHighPriorityTargets(0.7);
    printTargets(highPriority, "ВЫСОКОПРИОРИТЕТНЫЕ ЦЕЛИ (priority >= 0.7)");
    
    // Находим ближайшую цель
    Target closest = manager.getClosestTarget();
    if (closest.id != -1) {
        cout << "\nБлижайшая цель:" << endl;
        cout << "ID: " << closest.id << ", "
             << closest.name << ", Distance: " << closest.distance << " м" << endl;
    }
    
    // Находим самую приоритетную цель
    Target highestPriority = manager.getHighestPriorityTarget();
    if (highestPriority.id != -1) {
        cout << "\nСамая приоритетная цель:" << endl;
        cout << "ID: " << highestPriority.id << ", "
             << highestPriority.name << ", Priority: " << highestPriority.priority << endl;
    }
    
    // Удаляем цель
    cout << "\n--- Удаление цели ID 2 ---" << endl;
    manager.removeTarget(2);
    manager.printAllTargets();
    
    // Сохраняем цели в файл
    cout << "\n--- Сохранение целей в файл ---" << endl;
    manager.saveTargetsToFile();
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    
    return 0;
}