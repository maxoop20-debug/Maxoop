#include <iostream>
#include <string>
#include <vector>

class DataValidator {
private:
    struct Coordinate {
        std::string name;
        double value;
    };
    
    struct ValidInfo {
        bool valid_coordinate;
        std::vector<Coordinate> coordinate;
        bool valid_speed;
        double speed_not_valid;
        bool valid_acc;
        double acc_not_valid;
        
        // Constructor to initialize values
        ValidInfo() : valid_coordinate(true), valid_speed(true), 
                     speed_not_valid(0.0), valid_acc(true), acc_not_valid(0.0) {}
        
        int getCountValid() {
            int i = 0;
            if (valid_coordinate) i++;
            if (valid_speed) i++;
            if (valid_acc) i++;
            return i;
        }
    };
    
    struct Rule {
        std::string field_rule;
        double min_value;
        double max_value;
        
        // Constructor for easier creation
        Rule(const std::string& field, double min, double max) 
            : field_rule(field), min_value(min), max_value(max) {}
    };
    
    std::vector<Rule> rules;
    ValidInfo infor;

public:
    void addValidationRule(const std::string& field, double min, double max) {
        // Use explicit constructor instead of initializer list
        Rule newRule(field, min, max);
        rules.push_back(newRule);
    }
    
    bool validateCoordinates(double x, double y, double z) {
        bool coordinate_check = true;
        // Reset coordinate vector for new validation
        infor.coordinate.clear();
        
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].field_rule == "x") {
                if (x > rules[i].max_value || x < rules[i].min_value) {
                    coordinate_check = false;
                    Coordinate cur_x;
                    cur_x.name = "X";
                    cur_x.value = x;
                    infor.coordinate.push_back(cur_x);
                }
            }
            else if (rules[i].field_rule == "y") {
                if (y > rules[i].max_value || y < rules[i].min_value) {
                    coordinate_check = false;
                    Coordinate cur_y;
                    cur_y.name = "Y";
                    cur_y.value = y;
                    infor.coordinate.push_back(cur_y);
                }
            }
            else if (rules[i].field_rule == "z") {
                if (z > rules[i].max_value || z < rules[i].min_value) {
                    coordinate_check = false;
                    Coordinate cur_z;
                    cur_z.name = "Высота";
                    cur_z.value = z;
                    infor.coordinate.push_back(cur_z);
                }
            }
        }
        infor.valid_coordinate = coordinate_check;
        return coordinate_check;
    }
    
    bool validateSpeed(double speed) {
        bool speed_check = true;
        infor.valid_speed = true;
        
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].field_rule == "speed") {
                if (speed > rules[i].max_value || speed < rules[i].min_value) {
                    speed_check = false;
                    infor.valid_speed = false;
                    infor.speed_not_valid = speed;
                    return false;
                }
            }
        }
        return speed_check;
    }
    
    bool validateAcceleration(double acc) {
        bool acc_check = true;
        infor.valid_acc = true;
        
        for (size_t i = 0; i < rules.size(); i++) {
            if (rules[i].field_rule == "acc") {
                if (acc > rules[i].max_value || acc < rules[i].min_value) {
                    acc_check = false;
                    infor.valid_acc = false;
                    infor.acc_not_valid = acc;
                    return false;
                }
            }
        }
        return acc_check;
    }
    
    void generateValidationReport() {
        std::cout << "===============================================\n";
        std::cout << "Отчет валидации:\n";
        
        // Check coordinate validation using the member variable, not the function
        if (!infor.valid_coordinate) {
            std::cout << "Координаты: Ошибка - ";
            for (size_t i = 0; i < infor.coordinate.size(); i++) {
                std::cout << infor.coordinate[i].name << " = " << infor.coordinate[i].value;
                if (i < infor.coordinate.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
        else {
            std::cout << "Координаты: OK\n";
        }
        
        // Check speed validation
        if (!infor.valid_speed) {
            std::cout << "Скорость: Ошибка - значение " << infor.speed_not_valid << " вне допустимого диапазона\n";
        }
        else {
            std::cout << "Скорость: OK\n";
        }
        
        // Check acceleration validation (fixing the typo in your code)
        if (!infor.valid_acc) {
            std::cout << "Ускорение: Ошибка - значение " << infor.acc_not_valid << " вне допустимого диапазона\n";
        }
        else {
            std::cout << "Ускорение: OK\n";
        }
        
        std::cout << "Итого: " << infor.getCountValid() << "/3 параметров валидны\n";
        std::cout << "===============================================\n";
    }
    
    // Helper function to reset validation info
    void resetValidation() {
        infor = ValidInfo(); // Create new instance to reset all values
    }
};

int main() {
    DataValidator validator;
    
    // Add validation rules
    validator.addValidationRule("x", -100.0, 100.0);
    validator.addValidationRule("y", -100.0, 100.0);
    validator.addValidationRule("z", 0.0, 500.0);
    validator.addValidationRule("speed", 0.0, 100.0);
    validator.addValidationRule("acc", 0.0, 20.0);
    
    // Test 1: All valid
    std::cout << "Тест 1: Все параметры валидны\n";
    validator.validateCoordinates(10.0, 20.0, 150.0);
    validator.validateSpeed(50.0);
    validator.validateAcceleration(10.0);
    validator.generateValidationReport();
    
    validator.resetValidation();
    
    // Test 2: Some invalid
    std::cout << "\nТест 2: Некоторые параметры невалидны\n";
    validator.validateCoordinates(150.0, -120.0, 600.0); // x and z invalid
    validator.validateSpeed(120.0); // Speed invalid
    validator.validateAcceleration(5.0); // Acceleration valid
    validator.generateValidationReport();
    
    return 0;
}