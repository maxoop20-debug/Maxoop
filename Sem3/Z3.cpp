#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    const double v0 = 50.0;
    const double g = 9.8;
    const double dt = 1.0;
    const double t_max = 20.0;
    
    if (v0 <= 0) {
        cout << "Предупреждение: начальная скорость должна быть положительной!" << endl;
        return 1;
    }
    
    vector<double> times;
    vector<double> velocities;
    
    int estimated_size = static_cast<int>(t_max / dt) + 2;
    times.reserve(estimated_size);
    velocities.reserve(estimated_size);
    
    for (double t = 0.0; t <= t_max; t += dt) {
        double v = v0 - g * t;
        
        times.push_back(t);
        velocities.push_back(v);
        
        if (v < -v0) {
            break;
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "==============================" << endl;
    cout << "   Время (с)   Скорость (м/с)" << endl;
    cout << "==============================" << endl;
    
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << times[i] << setw(15) << velocities[i] << endl;
    }
    
    double t_zero = v0 / g;
    cout << "==============================" << endl;
    cout << "Момент изменения знака скорости: t = " << t_zero << " с" << endl;
    cout << "Всего точек: " << times.size() << endl;
    
    return 0;
}