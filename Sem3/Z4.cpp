#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>

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
    
    if (velocities.empty()) {
        cout << "Нет данных о скорости" << endl;
        return 0;
    }
    
    double sum = accumulate(velocities.begin(), velocities.end(), 0.0);
    double avg = sum / velocities.size();
    
    cout << fixed << setprecision(2);
    cout << "==============================" << endl;
    cout << "   Время (с)   Скорость (м/с)" << endl;
    cout << "==============================" << endl;
    
    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(10) << times[i] << setw(15) << velocities[i] << endl;
    }
    
    cout << "==============================" << endl;
    cout << "Статистика:" << endl;
    cout << "Количество измерений: " << velocities.size() << endl;
    cout << "Сумма скоростей: " << sum << " м/с" << endl;
    cout << "Средняя скорость (арифметическая): " << avg << " м/с" << endl;
    
    double t_zero = v0 / g;
    double T = times.back();
    double integral_avg = (1.0 / T) * (v0 * T - 0.5 * g * T * T);
    
    cout << "Средняя скорость (интегральная): " << integral_avg << " м/с" << endl;
    cout << "Момент изменения знака скорости: t = " << t_zero << " с" << endl;
    
    return 0;
}