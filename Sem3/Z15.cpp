#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<double> x_coords = {30, 10, 20, 10, 30, 40, 20};

    // Сортировка
    std::sort(x_coords.begin(), x_coords.end());

    // Удаление дубликатов
    auto last = std::unique(x_coords.begin(), x_coords.end());
    x_coords.erase(last, x_coords.end());

    std::cout << "Уникальные отсортированные координаты X: ";
    for (double x : x_coords) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}