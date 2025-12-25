#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<double> temperatures = {50, -150, 100, 250, 30, 300, -50, 80};

    auto new_end = std::remove_if(temperatures.begin(), temperatures.end(),
                                  [](double temp) { return temp < -100 || temp > 200; });
    temperatures.erase(new_end, temperatures.end());

    std::cout << "Очищенные данные: ";
    for (double t : temperatures) std::cout << t << " ";
    std::cout << std::endl;

    return 0;
}