#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <ctime>

using namespace std;

// Структура для хранения телеметрических данных
struct TelemetryData {
    double time;        // время, с
    double altitude;    // высота, м
    double speed;       // скорость, м/с
    double heading;     // курс, градусы
    double fuel;        // топливо, %
    
    TelemetryData() : time(0), altitude(0), speed(0), heading(0), fuel(0) {}
    
    TelemetryData(double t, double a, double s, double h, double f) 
        : time(t), altitude(a), speed(s), heading(h), fuel(f) {}
};

// Класс для логирования телеметрии
class TelemetryLogger {
private:
    string currentFilename;
    int fileCounter;
    const int MAX_RECORDS = 100;  // максимальное количество записей в файле
    
public:
    TelemetryLogger() : fileCounter(1) {
        updateFilename();
    }
    
    // Обновление имени файла
    void updateFilename() {
        stringstream ss;
        ss << "telemetry_" << setfill('0') << setw(3) << fileCounter << ".bin";
        currentFilename = ss.str();
    }
    
    // Запись данных в бинарный файл
    bool logData(double time, double altitude, double speed, double heading, double fuel) {
        // Открываем файл в режиме добавления (binary, append)
        ofstream file(currentFilename.c_str(), ios::binary | ios::app);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла для записи: " << currentFilename << endl;
            return false;
        }
        
        // Создаем структуру данных
        TelemetryData data(time, altitude, speed, heading, fuel);
        
        // Записываем структуру в бинарный файл
        file.write(reinterpret_cast<const char*>(&data), sizeof(TelemetryData));
        
        file.close();
        
        // Проверяем, нужно ли ротировать файл
        rotateFileIfNeeded();
        
        return true;
    }
    
    // Ротация файла при необходимости
    void rotateFileIfNeeded() {
        // Проверяем размер файла
        ifstream file(currentFilename.c_str(), ios::binary | ios::ate);
        
        if (!file.is_open()) {
            return;
        }
        
        // Получаем размер файла
        streamsize size = file.tellg();
        file.close();
        
        // Проверяем количество записей (размер файла / размер одной записи)
        int recordCount = size / sizeof(TelemetryData);
        
        if (recordCount >= MAX_RECORDS) {
            fileCounter++;
            updateFilename();
            cout << "Ротация файла. Новый файл: " << currentFilename << endl;
        }
    }
    
    // Чтение данных из бинарного файла
    vector<TelemetryData> readLogFile(const string& filename) {
        vector<TelemetryData> data;
        
        ifstream file(filename.c_str(), ios::binary);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла для чтения: " << filename << endl;
            return data;
        }
        
        // Определяем размер файла
        file.seekg(0, ios::end);
        streamsize fileSize = file.tellg();
        file.seekg(0, ios::beg);
        
        // Вычисляем количество записей
        int recordCount = fileSize / sizeof(TelemetryData);
        
        // Читаем данные
        for (int i = 0; i < recordCount; i++) {
            TelemetryData record;
            file.read(reinterpret_cast<char*>(&record), sizeof(TelemetryData));
            
            if (file) {
                data.push_back(record);
            } else {
                cerr << "Ошибка чтения записи " << i << " из файла " << filename << endl;
                break;
            }
        }
        
        file.close();
        
        return data;
    }
    
    // Печать сводки лога
    void printLogSummary() {
        cout << "\n=== СВОДКА ЛОГА ТЕЛЕМЕТРИИ ===" << endl;
        
        // Получаем список файлов (упрощенно - только текущий файл)
        vector<string> filenames;
        
        // Проверяем существование файлов от 001 до текущего
        for (int i = 1; i <= fileCounter; i++) {
            stringstream ss;
            ss << "telemetry_" << setfill('0') << setw(3) << i << ".bin";
            string filename = ss.str();
            
            ifstream testFile(filename.c_str());
            if (testFile.good()) {
                filenames.push_back(filename);
                testFile.close();
            }
        }
        
        if (filenames.empty()) {
            cout << "Файлы логов не найдены." << endl;
            return;
        }
        
        // Анализируем каждый файл
        double totalTime = 0;
        double minAltitude = 1e9;
        double maxAltitude = -1e9;
        double minSpeed = 1e9;
        double maxSpeed = -1e9;
        double fuelStart = 0;
        double fuelEnd = 0;
        int totalRecords = 0;
        
        for (size_t i = 0; i < filenames.size(); i++) {
            vector<TelemetryData> fileData = readLogFile(filenames[i]);
            
            if (fileData.empty()) continue;
            
            totalRecords += fileData.size();
            
            // Первая запись в первом файле
            if (i == 0) {
                fuelStart = fileData[0].fuel;
            }
            
            // Последняя запись в последнем файле
            if (i == filenames.size() - 1) {
                fuelEnd = fileData.back().fuel;
            }
            
            // Анализируем все записи в файле
            for (size_t j = 0; j < fileData.size(); j++) {
                const TelemetryData& d = fileData[j];
                
                if (d.altitude < minAltitude) minAltitude = d.altitude;
                if (d.altitude > maxAltitude) maxAltitude = d.altitude;
                if (d.speed < minSpeed) minSpeed = d.speed;
                if (d.speed > maxSpeed) maxSpeed = d.speed;
                
                if (d.time > totalTime) totalTime = d.time;
            }
        }
        
        // Выводим статистику
        cout << "Количество файлов: " << filenames.size() << endl;
        cout << "Общее количество записей: " << totalRecords << endl;
        cout << "Общее время полета: " << fixed << setprecision(1) << totalTime << " с" << endl;
        cout << "Минимальная высота: " << minAltitude << " м" << endl;
        cout << "Максимальная высота: " << maxAltitude << " м" << endl;
        cout << "Минимальная скорость: " << minSpeed << " м/с" << endl;
        cout << "Максимальная скорость: " << maxSpeed << " м/с" << endl;
        cout << "Начальный уровень топлива: " << fuelStart << " %" << endl;
        cout << "Конечный уровень топлива: " << fuelEnd << " %" << endl;
        cout << "Расход топлива: " << fuelStart - fuelEnd << " %" << endl;
        
        // Выводим список файлов
        cout << "\nФайлы логов:" << endl;
        for (size_t i = 0; i < filenames.size(); i++) {
            vector<TelemetryData> fileData = readLogFile(filenames[i]);
            cout << "  " << filenames[i] << ": " << fileData.size() << " записей" << endl;
        }
    }
    
    // Печать содержимого файла в читаемом виде
    void printLogContents(const string& filename) {
        vector<TelemetryData> data = readLogFile(filename);
        
        cout << "\n=== СОДЕРЖИМОЕ ФАЙЛА: " << filename << " ===" << endl;
        cout << "Записей: " << data.size() << endl;
        
        if (data.empty()) {
            cout << "Файл пуст или не содержит данных." << endl;
            return;
        }
        
        // Заголовок таблицы
        cout << left << setw(10) << "Time(с)" 
             << setw(12) << "Altitude(м)" 
             << setw(12) << "Speed(м/с)" 
             << setw(12) << "Heading(°)" 
             << setw(12) << "Fuel(%)" << endl;
        
        cout << string(58, '-') << endl;
        
        // Данные
        cout << fixed << setprecision(1);
        for (size_t i = 0; i < data.size(); i++) {
            const TelemetryData& d = data[i];
            cout << left << setw(10) << d.time 
                 << setw(12) << d.altitude 
                 << setw(12) << d.speed 
                 << setw(12) << d.heading 
                 << setw(12) << d.fuel << endl;
        }
    }
    
    // Получение текущего имени файла
    string getCurrentFilename() const {
        return currentFilename;
    }
    
    // Получение счетчика файлов
    int getFileCounter() const {
        return fileCounter;
    }
    
    // Тестовая функция для записи демо-данных
    void writeDemoData() {
        cout << "Запись демонстрационных данных..." << endl;
        
        logData(0.0, 100.0, 25.0, 45.0, 80.0);
        logData(1.0, 105.0, 27.0, 46.0, 79.5);
        logData(2.0, 110.0, 29.0, 47.0, 79.0);
        logData(3.0, 115.0, 31.0, 48.0, 78.5);
        logData(4.0, 120.0, 33.0, 49.0, 78.0);
        
        cout << "Демонстрационные данные записаны в " << currentFilename << endl;
    }
};

// Главная функция
int main() {
    cout << "=== СИСТЕМА ЛОГИРОВАНИЯ ТЕЛЕМЕТРИИ ===" << endl;
    
    // Создаем логгер
    TelemetryLogger logger;
    
    // Демонстрация записи данных
    cout << "\n1. Запись тестовых данных:" << endl;
    logger.logData(0.0, 100.0, 25.0, 45.0, 80.0);
    logger.logData(1.0, 105.0, 27.0, 46.0, 79.5);
    logger.logData(2.0, 110.0, 29.0, 47.0, 79.0);
    
    cout << "Данные записаны в файл: " << logger.getCurrentFilename() << endl;
    
    // Демонстрация записи дополнительных данных для ротации
    cout << "\n2. Дополнительные записи для демонстрации ротации:" << endl;
    
    // Записываем много данных, чтобы превысить лимит
    for (int i = 3; i < 105; i++) {
        double t = i * 1.0;
        double alt = 110.0 + i * 0.5;
        double spd = 29.0 + i * 0.1;
        double hdg = 47.0 + i * 0.05;
        double fuel = 79.0 - i * 0.05;
        
        logger.logData(t, alt, spd, hdg, fuel);
    }
    
    cout << "Текущий файл: " << logger.getCurrentFilename() << endl;
    
    // Чтение и вывод содержимого первого файла
    cout << "\n3. Чтение первого файла:" << endl;
    logger.printLogContents("telemetry_001.bin");
    
    // Вывод сводки логов
    cout << "\n4. Общая сводка логов:" << endl;
    logger.printLogSummary();
    
    // Демонстрация записи данных вручную
    cout << "\n5. Демонстрация записи данных вручную:" << endl;
    cout << "Введите данные телеметрии (0 для выхода):" << endl;
    
    double time, altitude, speed, heading, fuel;
    char choice;
    
    while (true) {
        cout << "\nДобавить запись? (y/n): ";
        cin >> choice;
        
        if (choice != 'y' && choice != 'Y') {
            break;
        }
        
        cout << "Время (с): ";
        cin >> time;
        cout << "Высота (м): ";
        cin >> altitude;
        cout << "Скорость (м/с): ";
        cin >> speed;
        cout << "Курс (град): ";
        cin >> heading;
        cout << "Топливо (%): ";
        cin >> fuel;
        
        if (logger.logData(time, altitude, speed, heading, fuel)) {
            cout << "Запись добавлена в " << logger.getCurrentFilename() << endl;
        } else {
            cout << "Ошибка записи!" << endl;
        }
    }
    
    // Финальная сводка
    cout << "\n=== ФИНАЛЬНАЯ СВОДКА ===" << endl;
    logger.printLogSummary();
    
    return 0;
}