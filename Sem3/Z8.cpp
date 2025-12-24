#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    const double g = 9.8;
    const double dt = 1.0;
    
    vector<double> times1, heights1;
    vector<double> times2, heights2;
    
    double v01 = 50.0;
    double v02 = 40.0;
    
    for (double t = 0.0; t <= 1000; t += dt) {
        double h1 = v01 * t - 0.5 * g * t * t;
        times1.push_back(t);
        heights1.push_back(h1);
        if (h1 < -100) break;
    }
    
    for (double t = 0.0; t <= 1000; t += dt) {
        double h2 = v02 * t - 0.5 * g * t * t;
        times2.push_back(t);
        heights2.push_back(h2);
        if (h2 < -100) break;
    }
    
    double max1 = heights1[0];
    double max2 = heights2[0];
    size_t idx_max1 = 0;
    size_t idx_max2 = 0;
    
    for (size_t i = 1; i < heights1.size(); i++) {
        if (heights1[i] > max1) {
            max1 = heights1[i];
            idx_max1 = i;
        }
    }
    
    for (size_t i = 1; i < heights2.size(); i++) {
        if (heights2[i] > max2) {
            max2 = heights2[i];
            idx_max2 = i;
        }
    }
    
    double t_fall1 = -1;
    double t_fall2 = -1;
    
    for (size_t i = 1; i < heights1.size(); i++) {
        if (heights1[i] <= 0.0) {
            t_fall1 = times1[i];
            break;
        }
    }
    
    for (size_t i = 1; i < heights2.size(); i++) {
        if (heights2[i] <= 0.0) {
            t_fall2 = times2[i];
            break;
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "==========================================================" << endl;
    cout << "                 Ракета 1        Ракета 2" << endl;
    cout << "==========================================================" << endl;
    cout << "Начальная скорость: " << setw(8) << v01 << " м/с" 
         << setw(15) << v02 << " м/с" << endl;
    cout << "Максимальная высота: " << setw(8) << max1 << " м" 
         << setw(15) << max2 << " м" << endl;
    cout << "Время макс. высоты: " << setw(8) << times1[idx_max1] << " с" 
         << setw(15) << times2[idx_max2] << " с" << endl;
    cout << "Время падения: " << setw(8);
    if (t_fall1 >= 0) cout << t_fall1 << " с";
    else cout << "не упала";
    cout << setw(15);
    if (t_fall2 >= 0) cout << t_fall2 << " с";
    else cout << "не упала";
    cout << endl;
    cout << "==========================================================" << endl;
    
    cout << "\nСРАВНЕНИЕ:" << endl;
    
    if (max1 > max2) {
        cout << "Ракета 1 поднялась выше" << endl;
    } else if (max2 > max1) {
        cout << "Ракета 2 поднялась выше" << endl;
    } else {
        cout << "Ракеты поднялись на одинаковую высоту" << endl;
    }
    
    if (t_fall1 >= 0 && t_fall2 >= 0) {
        if (t_fall1 > t_fall2) {
            cout << "Ракета 1 дольше находилась в воздухе" << endl;
        } else if (t_fall2 > t_fall1) {
            cout << "Ракета 2 дольше находилась в воздухе" << endl;
        } else {
            cout << "Ракеты находились в воздухе одинаковое время" << endl;
        }
    } else if (t_fall1 >= 0) {
        cout << "Ракета 2 еще не упала (время не определено)" << endl;
    } else if (t_fall2 >= 0) {
        cout << "Ракета 1 еще не упала (время не определено)" << endl;
    } else {
        cout << "Обе ракеты еще не упали" << endl;
    }
    
    return 0;
}