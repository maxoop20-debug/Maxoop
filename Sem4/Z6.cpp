#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Функции для проверки данных (вместо лямбда-функций)
bool isValidAltitude(double alt) {
    return alt >= 0 && alt <= 20000;
}

bool isValidSpeed(double speed) {
    return speed >= 0 && speed <= 500;
}

bool isValidHeading(double heading) {
    return heading >= 0 && heading <= 360;
}

bool isValidFuel(double fuel) {
    return fuel >= 0 && fuel <= 100;
}

// Класс для фильтрации телеметрии
class TelemetryFilter {
private:
    vector<vector<string> > data;  // Исправлено: пробел между > >
    int removedCount;
    
public:
    TelemetryFilter() : removedCount(0) {}
    
    // Загрузка данных из CSV файла
    bool loadFromCSV(const string& filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }
        
        data.clear();
        string line;
        
        // Читаем первую строку (заголовок)
        if (getline(file, line)) {
            vector<string> header = splitCSVLine(line);
            data.push_back(header);
        }
        
        // Читаем остальные строки
        int lineNumber = 1;
        while (getline(file, line)) {
            lineNumber++;
            vector<string> row = splitCSVLine(line);
            
            // Проверяем, что строка имеет правильное количество полей
            if (row.size() != 5) {
                cerr << "Предупреждение: строка " << lineNumber 
                     << " имеет " << row.size() << " полей вместо 5" << endl;
                continue;
            }
            
            data.push_back(row);
        }
        
        file.close();
        cout << "Загружено " << (int)data.size() - 1 << " строк из файла " << filename << endl;
        return true;
    }
    
    // Фильтрация данных
    void filterData() {
        if (data.size() <= 1) return; // Только заголовок или пусто
        
        vector<vector<string> > filteredData;  // Исправлено: пробел между > >
        removedCount = 0;
        
        // Добавляем заголовок
        filteredData.push_back(data[0]);
        
        // Фильтруем строки данных
        for (size_t i = 1; i < data.size(); i++) {
            const vector<string>& row = data[i];
            
            // Пытаемся преобразовать строки в числа
            double time, altitude, speed, heading, fuel;
            
            if (!parseRow(row, time, altitude, speed, heading, fuel)) {
                removedCount++;
                continue;
            }
            
            // Проверяем данные с помощью функций
            if (isValidAltitude(altitude) && isValidSpeed(speed) && 
                isValidHeading(heading) && isValidFuel(fuel)) {
                filteredData.push_back(row);
            } else {
                removedCount++;
                
                // Выводим информацию об ошибке
                if (!isValidAltitude(altitude)) {
                    cout << "  Удалена строка " << i << ": некорректная высота " << altitude << endl;
                } else if (!isValidSpeed(speed)) {
                    cout << "  Удалена строка " << i << ": некорректная скорость " << speed << endl;
                } else if (!isValidHeading(heading)) {
                    cout << "  Удалена строка " << i << ": некорректный курс " << heading << endl;
                } else if (!isValidFuel(fuel)) {
                    cout << "  Удалена строка " << i << ": некорректное топливо " << fuel << endl;
                }
            }
        }
        
        // Заменяем данные отфильтрованными
        data = filteredData;
    }
    
    // Сохранение отфильтрованных данных в CSV файл
    bool saveToCSV(const string& filename) {
        ofstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось создать файл " << filename << endl;
            return false;
        }
        
        // Записываем данные
        for (size_t i = 0; i < data.size(); i++) {
            const vector<string>& row = data[i];
            
            for (size_t j = 0; j < row.size(); j++) {
                file << row[j];
                if (j < row.size() - 1) {
                    file << ",";
                }
            }
            file << endl;
        }
        
        file.close();
        cout << "Отфильтрованные данные сохранены в файл: " << filename << endl;
        return true;
    }
    
    // Вывод статистики фильтрации
    void printFilteredStats() {
        cout << "\n=== СТАТИСТИКА ФИЛЬТРАЦИИ ===" << endl;
        
        if (data.size() <= 1) {
            cout << "Нет данных для анализа." << endl;
            return;
        }
        
        int totalRows = (int)data.size() - 1; // без заголовка
        int originalRows = totalRows + removedCount;
        
        cout << "Всего строк загружено: " << originalRows << endl;
        cout << "Удалено строк: " << removedCount << endl;
        cout << "Осталось строк: " << totalRows << endl;
        
        if (removedCount > 0 && originalRows > 0) {
            double percentage = (double)totalRows / originalRows * 100;
            cout << fixed << setprecision(1);
            cout << "Процент сохраненных данных: " << percentage << "%" << endl;
        } else {
            cout << "Все данные корректны." << endl;
        }
        
        // Дополнительная статистика
        cout << "\nПроверяемые диапазоны:" << endl;
        cout << "  Высота: 0 - 20000 м" << endl;
        cout << "  Скорость: 0 - 500 м/с" << endl;
        cout << "  Курс: 0 - 360 градусов" << endl;
        cout << "  Топливо: 0 - 100 %" << endl;
    }
    
    // Создание тестового файла
    static void createTestFile() {
        ofstream file("telemetry.csv");
        
        file << "time,altitude,speed,heading,fuel" << endl;
        file << "0,1000,250,45,80" << endl;
        file << "1,1050,255,46,79" << endl;
        file << "2,-50,260,47,78" << endl;
        file << "3,1100,1000,48,77" << endl;
        file << "4,1150,265,49,76" << endl;
        file << "5,1200,270,370,75" << endl;  // Некорректный курс
        file << "6,1250,275,51,120" << endl;  // Некорректное топливо
        
        file.close();
        cout << "Тестовый файл создан: telemetry.csv" << endl;
    }
    
private:
    // Разделение строки CSV на поля
    vector<string> splitCSVLine(const string& line) {
        vector<string> result;
        stringstream ss(line);
        string token;
        
        while (getline(ss, token, ',')) {
            // Удаляем лишние пробелы
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            result.push_back(token);
        }
        
        return result;
    }
    
    // Парсинг строки в числа
    bool parseRow(const vector<string>& row, double& time, double& altitude, 
                  double& speed, double& heading, double& fuel) {
        // Проверяем пустые значения
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i].empty()) {
                return false;
            }
        }
        
        try {
            time = atof(row[0].c_str());
            altitude = atof(row[1].c_str());
            speed = atof(row[2].c_str());
            heading = atof(row[3].c_str());
            fuel = atof(row[4].c_str());
            return true;
        } catch (...) {
            return false;
        }
    }
};

// Главная функция
int main() {
    cout << "=== ФИЛЬТРАЦИЯ ДАННЫХ ТЕЛЕМЕТРИИ ===" << endl;
    
    // Создаем тестовый файл
    TelemetryFilter::createTestFile();
    
    // Создаем фильтр
    TelemetryFilter filter;
    
    // Загружаем данные
    if (!filter.loadFromCSV("telemetry.csv")) {
        return 1;
    }
    
    // Фильтруем данные
    cout << "\nНачало фильтрации данных..." << endl;
    filter.filterData();
    
    // Сохраняем отфильтрованные данные
    filter.saveToCSV("telemetry_filtered.csv");
    
    // Выводим статистику
    filter.printFilteredStats();
    
    // Дополнительный тест
    cout << "\n=== ДОПОЛНИТЕЛЬНЫЙ ТЕСТ ===" << endl;
    
    // Создаем еще один файл с разными ошибками
    ofstream testFile("test_data.csv");
    testFile << "time,altitude,speed,heading,fuel" << endl;
    testFile << "0,500,200,90,50" << endl;
    testFile << "1,-100,300,180,60" << endl;
    testFile << "2,30000,400,270,70" << endl;
    testFile << "3,2000,-50,0,80" << endl;
    testFile << "4,1000,250,400,90" << endl;
    testFile << "5,1500,300,45,-10" << endl;
    testFile << "6,2000,350,90,110" << endl;
    testFile.close();
    
    cout << "Тестовый файл создан: test_data.csv" << endl;
    
    TelemetryFilter filter2;
    if (filter2.loadFromCSV("test_data.csv")) {
        filter2.filterData();
        filter2.saveToCSV("test_filtered.csv");
        filter2.printFilteredStats();
    }
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    
    return 0;
}