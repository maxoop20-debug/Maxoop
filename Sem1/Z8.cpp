#include <iostream>
#include <algorithm>

using namespace std;

struct Aircraft {
    string name;
    double mass;
    double thrust;
    double CL;
    double CD;
    double acceleration;
    double timeToClimb;
};

int main() {
    const int n = 5;
    const double rho = 1.225;
    const double g = 9.81;
    const double S = 30.0;
    const double h = 1000.0;
    
    Aircraft planes[n] = {
        {"Самолет-1", 10000, 50000, 0.8, 0.05},
        {"Самолет-2", 12000, 60000, 0.9, 0.04},
        {"Самолет-3", 9000, 55000, 0.85, 0.06},
        {"Самолет-4", 11000, 52000, 0.82, 0.045},
        {"Самолет-5", 9500, 48000, 0.78, 0.055}
    };
    
    for (int i = 0; i < n; i++) {
        double V = sqrt((2 * planes[i].mass * g) / (rho * S * planes[i].CL));
        double drag = 0.5 * rho * V * V * S * planes[i].CD;
        
        planes[i].acceleration = (planes[i].thrust - drag) / planes[i].mass;
        
        if (planes[i].acceleration > 0) {
            double climbRate = ((planes[i].thrust - drag) * V) / (planes[i].mass * g);
            planes[i].timeToClimb = h / climbRate;
        } else {
            planes[i].timeToClimb = 999999;
        }
    }
    
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (planes[j].timeToClimb > planes[j+1].timeToClimb) {
                Aircraft temp = planes[j];
                planes[j] = planes[j+1];
                planes[j+1] = temp;
            }
        }
    }
    
    cout << "Результаты (отсортировано по времени набора высоты):\n";
    cout << "===================================================\n";
    for (int i = 0; i < n; i++) {
        cout << planes[i].name << ":\n";
        cout << "  Вертикальное ускорение: " << planes[i].acceleration << " м/с²\n";
        cout << "  Время набора " << h << " м: " << planes[i].timeToClimb << " с\n";
        cout << "-----------------------------------\n";
    }
    
    return 0;
}