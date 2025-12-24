#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    const double v0 = 100.0;
    const double g = 9.8;
    const double dt = 0.1;
    
    vector<double> times;
    vector<double> heights;
    
    double t = 0.0;
    while (true) {
        double h = v0 * t - 0.5 * g * t * t;
        
        if (h < 0) {
            break;
        }
        
        times.push_back(t);
        heights.push_back(h);
        
        t += dt;
    }
    
    if (heights.empty()) {
        cout << "Нет данных о полете ракеты" << endl;
        return 0;
    }
    
    auto max_it = max_element(heights.begin(), heights.end());
    size_t idx = distance(heights.begin(), max_it);
    
    cout << fixed << setprecision(2);
    cout << "==================================" << endl;
    cout << "Параметры полета:" << endl;
    cout << "Начальная скорость: " << v0 << " м/с" << endl;
    cout << "Ускорение свободного падения: " << g << " м/с²" << endl;
    cout << "Шаг моделирования: " << dt << " с" << endl;
    cout << "==================================" << endl;
    
    cout << "Максимальная высота: " << *max_it << " м" << endl;
    cout << "Время достижения: " << times[idx] << " с" << endl;
    
    double theoretical_t_max = v0 / g;
    double theoretical_h_max = v0 * v0 / (2 * g);
    cout << "Теоретическая макс. высота: " << theoretical_h_max << " м" << endl;
    cout << "Теоретическое время: " << theoretical_t_max << " с" << endl;
    
    return 0;
}