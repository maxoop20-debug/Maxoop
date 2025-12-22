#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    const double v0 = 50.0;
    const double g = 9.8;
    const double dt = 1.0;
    
    if (v0 <= 0) {
        cout << "Предупреждение: начальная скорость должна быть положительной!" << endl;
        return 1;
    }
    
    vector<double> times;
    vector<double> heights;
    
    int estimated_size = static_cast<int>(v0 * 2 / g / dt) + 1;
    times.reserve(estimated_size);
    heights.reserve(estimated_size);
    
    for (double t = 0.0; t <= 1000; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;
        
        if (h < 0) {
            break;
        }
        
        times.push_back(t);
        heights.push_back(h);
    }
    
    cout << fixed << setprecision(2);
    cout << "==============================" << endl;
    cout << "   Время (с)   Высота (м)" << endl;
    cout << "==============================" << endl;
    
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << times[i] << setw(15) << heights[i] << endl;
    }
    
    cout << "==============================" << endl;
    cout << "Всего точек: " << times.size() << endl;
    
    return 0;
}