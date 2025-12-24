#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

// Структура для хранения точки траектории
struct Point {
    double time;
    double x, y, z;
    double speed;
};

// Функция для расчета расстояния между двумя точками
double calculateDistance(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

int main() {
    // Массив точек траектории
    const int NUM_POINTS = 3;
    Point trajectory[NUM_POINTS] = {
        {0.0, 10.5, 20.3, 100.0, 25.0},
        {1.5, 15.2, 25.1, 105.0, 27.5},
        {3.0, 20.8, 30.7, 110.0, 30.0}
    };
    
    // Открываем файл для записи
    ofstream csvFile("trajectory.csv");
    
    if (!csvFile.is_open()) {
        cerr << "Ошибка создания файла!" << endl;
        return 1;
    }
    
    // Записываем заголовок
    csvFile << "time,x,y,z,speed" << endl;
    
    // Записываем данные с фиксированной точностью
    csvFile << fixed << setprecision(1);
    
    // Записываем все точки
    for (int i = 0; i < NUM_POINTS; i++) {
        csvFile << trajectory[i].time << ","
                << trajectory[i].x << ","
                << trajectory[i].y << ","
                << trajectory[i].z << ","
                << trajectory[i].speed << endl;
    }
    
    // Закрываем файл
    csvFile.close();
    
    // Рассчитываем и выводим статистику
    if (NUM_POINTS >= 2) {
        double totalDistance = 0;
        double maxSpeed = trajectory[0].speed;
        
        for (int i = 0; i < NUM_POINTS; i++) {
            // Находим максимальную скорость
            if (trajectory[i].speed > maxSpeed) {
                maxSpeed = trajectory[i].speed;
            }
            
            // Суммируем расстояния между точками
            if (i > 0) {
                totalDistance += calculateDistance(trajectory[i-1], trajectory[i]);
            }
        }
        
        cout << "=== СТАТИСТИКА ТРАЕКТОРИИ ===" << endl;
        cout << fixed << setprecision(2);
        cout << "Общее пройденное расстояние: " << totalDistance << " м" << endl;
        cout << "Максимальная скорость: " << maxSpeed << " м/с (" 
             << maxSpeed * 3.6 << " км/ч)" << endl;
        cout << "Общее время: " << trajectory[NUM_POINTS-1].time << " с" << endl;
    }
    
    cout << "\nДанные сохранены в файл: trajectory.csv" << endl;
    
    return 0;
}