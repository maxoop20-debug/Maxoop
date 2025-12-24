#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    const double v0 = 50.0;
    const double g = 9.8;
    const double dt = 1.0;
    
    vector<double> times;
    vector<double> heights;
    
    if (v0 <= 0) {
        cout << "Начальная скорость должна быть положительной!" << endl;
        return 1;
    }
    
    for (double t = 0.0; t <= 1000; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;
        
        times.push_back(t);
        heights.push_back(h);
        
        if (h < -100) {
            break;
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "==============================" << endl;
    cout << "   Время (с)   Высота (м)" << endl;
    cout << "==============================" << endl;
    
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << times[i] << setw(15) << heights[i];
        if (heights[i]<=heights[i-1]  && i > 0) {
            cout << "  <-- ПАДЕНИЕ";
        }
        cout << endl;
    }
    cout << "==============================" << endl;
    
    if (heights.empty()) {
        cout << "Нет данных о высоте" << endl;
        return 0;
    }
    
    
    size_t idx = 0;
    bool found = false;
    
    for (size_t i = 1; i < heights.size(); i++) {
        if (heights[i] <= 0.0) {
            idx = i;
            found = true;
            break;
        }
    }
    
    if (found) {
        cout << "\nРЕЗУЛЬТАТЫ:" << endl;
        cout << "Момент падения: t = " << times[idx] << " с" << endl;
        cout << "Высота: " << heights[idx] << " м" << endl;
        
        double analytical_t_fall = 2.0 * v0 / g;
        cout << "Аналитическое время: " << analytical_t_fall << " с" << endl;
        
        if (idx > 0) {
            double t_before = times[idx-1];
            double h_before = heights[idx-1];
            double t_after = times[idx];
            double h_after = heights[idx];
            
            cout << "\nИнтерполяция:" << endl;
            cout << "t = " << t_before << " с, h = " << h_before << " м" << endl;
            cout << "t = " << t_after << " с, h = " << h_after << " м" << endl;
            
            if (h_before > 0 && h_after <= 0) {
                double interpolated_t = t_before + (0 - h_before) * (t_after - t_before) / (h_after - h_before);
                cout << "Интерполированное время: " << interpolated_t << " с" << endl;
            }
        }
    } else {
        cout << "Падения не было" << endl;
        cout << "Последняя высота: " << heights.back() << " м при t = " << times.back() << " с" << endl;
    }
    
    return 0;
}