#include <iostream>
#include <cmath>

using namespace std;

int main() {
    const double mass = 10000;
    const double area = 30;
    const double CL = 0.8;
    const double CD = 0.05;
    const double rho = 1.225;
    const double g = 9.81;
    const double h = 1000;
    
    double Tmin, Tmax, dT;
    
    cout << "Введите минимальную тягу (Н): ";
    cin >> Tmin;
    cout << "Введите максимальную тягу (Н): ";
    cin >> Tmax;
    cout << "Введите шаг изменения тяги (Н): ";
    cin >> dT;
    
    double minTime = 1e9;
    double optimalThrust = Tmin;
    
    int count = static_cast<int>((Tmax - Tmin) / dT) + 1;
    
    cout << "\nТяга (Н)\tВремя набора (с)" << endl;
    cout << "------------------------------" << endl;
    
    for (int i = 0; i <= count; i++) {
        double thrust = Tmin + i * dT;
        if (thrust > Tmax) break;
        
        double V = sqrt((2 * mass * g) / (rho * area * CL));
        double drag = 0.5 * rho * V * V * area * CD;
        
        if (thrust > drag) {
            double climbRate = ((thrust - drag) * V) / (mass * g);
            double time = h / climbRate;
            
            cout << thrust << "\t\t" << time << endl;
            
            if (time < minTime) {
                minTime = time;
                optimalThrust = thrust;
            }
        } else {
            cout << thrust << "\t\tне может набирать высоту" << endl;
        }
    }
    
    cout << "\nРезультат:" << endl;
    cout << "Оптимальная тяга: " << optimalThrust << " Н" << endl;
    cout << "Минимальное время набора " << h << " м: " << minTime << " с" << endl;
    
    return 0;
}