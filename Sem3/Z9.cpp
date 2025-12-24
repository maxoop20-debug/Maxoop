#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    
    const int n = 20;
    vector<double> h(n);
    
    for (int i = 0; i < n; i++) {
        double ideal = 50.0 * i - 0.5 * 9.8 * i * i;
        if (ideal < 0) ideal = 0;
        double noise = (rand() % 200 - 100) / 10.0;
        h[i] = ideal + noise;
    }
    
    vector<double> smoothed(n);
    
    if (n > 0) {
        smoothed[0] = h[0];
    }
    
    if (n > 1) {
        smoothed[n-1] = h[n-1];
    }
    
    for (int i = 1; i < n - 1; i++) {
        smoothed[i] = (h[i-1] + h[i] + h[i+1]) / 3.0;
    }
    
    cout << fixed << setprecision(1);
    cout << "==============================================" << endl;
    cout << "  i  |  Исходная  |  Сглаженная  |  Разница  " << endl;
    cout << "==============================================" << endl;
    
    for (int i = 0; i < n; i++) {
        double diff = smoothed[i] - h[i];
        cout << setw(3) << i << " | " 
             << setw(10) << h[i] << " | " 
             << setw(12) << smoothed[i] << " | " 
             << setw(9) << diff << endl;
    }
    
    double sum_diff = 0;
    for (int i = 0; i < n; i++) {
        double diff = smoothed[i] - h[i];
        sum_diff += diff * diff;
    }
    
    cout << "==============================================" << endl;
    cout << "Среднеквадратичная ошибка: " << sqrt(sum_diff / n) << endl;
    
    return 0;
}