#include <iostream>
#include <string>

using namespace std;

int main() {
    double thrust, lift, drag, mass;
    
    cout << "Введите тягу (Н): ";
    cin >> thrust;
    cout << "Введите подъемную силу (Н): ";
    cin >> lift;
    cout << "Введите сопротивление (Н): ";
    cin >> drag;
    cout << "Введите массу самолета (кг): ";
    cin >> mass;
    
    double acceleration = (thrust - drag) / mass;
    string mode;
    
    if (acceleration > 0.5) {
        mode = "набор высоты";
    } else if (acceleration >= 0 && acceleration <= 0.5) {
        mode = "горизонтальный полет";
    } else if (acceleration < 0) {
        mode = "снижение";
    }
    
    cout << "\nУскорение: " << acceleration << " м/с²" << endl;
    cout << "Режим полета: " << mode << endl;
    
    return 0;
}