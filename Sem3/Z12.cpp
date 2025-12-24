#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<double> acceleration_data;
    
    acceleration_data.push_back(1.2);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(3.4);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(5.6);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(7.8);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(9.1);
    acceleration_data.push_back(2.3);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(4.5);
    acceleration_data.push_back(6.7);
    acceleration_data.push_back(0.0);
    acceleration_data.push_back(8.9);
    
    int zero_count = 0;
    for (size_t i = 0; i < acceleration_data.size(); i++) {
        if (acceleration_data[i] == 0.0) {
            zero_count++;
        }
    }
    
    cout << "Данные датчика ускорения:" << endl;
    cout << "=========================" << endl;
    
    for (size_t i = 0; i < acceleration_data.size(); i++) {
        cout << "[" << i << "] " << acceleration_data[i] << " м/с²";
        if (acceleration_data[i] == 0.0) {
            cout << " (НУЛЬ)";
        }
        cout << endl;
    }
    
    cout << "=========================" << endl;
    cout << "Общее количество измерений: " << acceleration_data.size() << endl;
    cout << "Количество нулевых показаний: " << zero_count << endl;
    
    double percentage = (static_cast<double>(zero_count) / acceleration_data.size()) * 100.0;
    cout << "Процент нулевых показаний: " << percentage << "%" << endl;
    
    if (zero_count > acceleration_data.size() / 2) {
        cout << "ВНИМАНИЕ: Больше половины показаний нулевые!" << endl;
    }
    
    return 0;
}