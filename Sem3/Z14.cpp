#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

int main() {
    std::vector<double> y_coords = {0, 10, 25, 45, 70, 100};
    std::vector<double> displacements(y_coords.size());

    // Разности соседних координат
    std::adjacent_difference(y_coords.begin(), y_coords.end(), displacements.begin());

    // Суммируем абсолютные значения (игнорируем первый элемент, т.к. он = y_coords[0])
    double total_path = std::accumulate(displacements.begin() + 1, displacements.end(), 0.0,
                                        [](double sum, double d) { return sum + std::abs(d); });

    std::cout << "Суммарный путь: " << total_path << " м" << std::endl;

    return 0;
}