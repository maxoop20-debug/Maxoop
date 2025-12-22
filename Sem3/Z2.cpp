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
    
    if (heights.empty()) {
        cout << "Нет данных о высоте" << endl;
        return 0;
    }
    
    size_t idx_max = 0;
    for (size_t i = 1; i < heights.size(); ++i) {
        if (heights[i] > heights[idx_max]) {
            idx_max = i;
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "==============================" << endl;
    cout << "   Время (с)   Высота (м)" << endl;
    cout << "==============================" << endl;
    
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << times[i] << setw(15) << heights[i];
        if (i == idx_max) {
            cout << "  <-- максимальная";
        }
        cout << endl;
    }
    
    cout << "==============================" << endl;
    cout << "Максимальная высота:" << endl;
    cout << "Время: " << times[idx_max] << " с" << endl;
    cout << "Высота: " << heights[idx_max] << " м" << endl;
    
    return 0;
}