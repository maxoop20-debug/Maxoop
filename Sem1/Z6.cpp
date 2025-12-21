#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    const int n = 5;
    double V[n] = {100, 110, 105, 115, 120};
    double rho[n] = {1.225, 1.112, 1.006, 0.909, 0.819};
    
    const double S = 30.0;
    const double CL = 0.8;
    
    cout << "| Шаг | Скорость (м/с) | Плотность (кг/м³) | Подъемная сила (Н) |" << endl;
    cout << "|-----|----------------|-------------------|---------------------|" << endl;
    
    for (int i = 0; i < n; i++) {
        double lift = 0.5 * rho[i] * V[i] * V[i] * S * CL;
        
        cout << "| " << setw(3) << i+1 << " | " 
             << setw(13) << fixed << setprecision(2) << V[i] << " | " 
             << setw(17) << setprecision(3) << rho[i] << " | " 
             << setw(19) << setprecision(2) << lift << " |" << endl;
    }
    
    return 0;
}