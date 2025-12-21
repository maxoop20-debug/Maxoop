#include <iostream>
#include <cmath>
#include <string>

using namespace std;

struct Airplane {
    string name;
    double mass;
    double wingArea;
    double thrust;
    double CL;
    double CD;
    double velocity;
    double lift;
    double drag;
    double climbRate;
    double timeToClimb;
};

int main() {
    const double rho = 1.225;
    const double g = 9.81;
    const double h = 1000.0;

    Airplane planes[3] = {
        {"Самолет 1", 10000, 30, 50000, 0.8, 0.05},
        {"Самолет 2", 12000, 35, 60000, 0.9, 0.04},
        {"Самолет 3", 9000, 25, 55000, 0.85, 0.06}
    };

    for (int i = 0; i < 3; i++) {
        Airplane& p = planes[i];
        
        p.velocity = sqrt((2 * p.mass * g) / (rho * p.wingArea * p.CL));
        
        p.lift = 0.5 * rho * p.velocity * p.velocity * p.wingArea * p.CL;
        
        p.drag = 0.5 * rho * p.velocity * p.velocity * p.wingArea * p.CD;
        
        p.climbRate = ((p.thrust - p.drag) * p.velocity) / (p.mass * g);
        
        if (p.climbRate > 0) {
            p.timeToClimb = h / p.climbRate;
        } else {
            p.timeToClimb = 1e9;
        }
        
        cout << "\n" << p.name << endl;
        cout << "Скорость: " << p.velocity << " м/с" << endl;
        cout << "Подъемная сила: " << p.lift << " Н" << endl;
        cout << "Сопротивление: " << p.drag << " Н" << endl;
        cout << "Скороподъемность: " << p.climbRate << " м/с" << endl;
        cout << "Время набора " << h << " м: " << p.timeToClimb << " с" << endl;
    }

    int bestIndex = 0;
    double bestTime = planes[0].timeToClimb;
    
    for (int i = 1; i < 3; i++) {
        if (planes[i].timeToClimb < bestTime) {
            bestTime = planes[i].timeToClimb;
            bestIndex = i;
        }
    }
    
    cout << "\n=====================================" << endl;
    cout << "Лучший: " << planes[bestIndex].name << endl;
    cout << "Время набора высоты: " << bestTime << " с" << endl;

    return 0;
}