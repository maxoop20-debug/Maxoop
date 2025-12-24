#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

bool isLessThan10(double h) {
    return h < 10.0;
}

int main() {
    vector<double> heights;
    heights.push_back(45.0);
    heights.push_back(8.5);
    heights.push_back(32.0);
    heights.push_back(5.2);
    heights.push_back(15.7);
    heights.push_back(2.1);
    heights.push_back(25.3);
    heights.push_back(9.9);
    heights.push_back(50.0);
    heights.push_back(10.1);
    
    cout << "Исходный список высот:" << endl;
    cout << "======================" << endl;
    for (size_t i = 0; i < heights.size(); i++) {
        cout << "[" << i << "] " << fixed << setprecision(1) << heights[i] << " м" << endl;
    }
    cout << "Всего элементов: " << heights.size() << endl << endl;
    
    vector<double>::iterator new_end = remove_if(heights.begin(), heights.end(), isLessThan10);
    
    heights.erase(new_end, heights.end());
    
    cout << "Список высот (h >= 10 м):" << endl;
    cout << "========================" << endl;
    if (heights.empty()) {
        cout << "Все значения были удалены (все высоты < 10 м)" << endl;
    } else {
        for (size_t i = 0; i < heights.size(); i++) {
            cout << "[" << i << "] " << fixed << setprecision(1) << heights[i] << " м" << endl;
        }
    }
    cout << "Осталось элементов: " << heights.size() << endl;
    
    return 0;
}