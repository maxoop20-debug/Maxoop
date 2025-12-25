#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>

struct TelemetryData {
    double time;
    double altitude;
    double speed;
    double heading;
    double fuel;

    TelemetryData(double t = 0.0, double alt = 0.0, double spd = 0.0,
        double hdg = 0.0, double fl = 0.0)
        : time(t), altitude(alt), speed(spd), heading(hdg), fuel(fl) {
    }
};

class TelemetryLogger {
private:
    std::string currentFilename;
    int fileCounter;
    const size_t maxFileSize = 1024;


    size_t getFileSize(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file) return 0;
        return file.tellg();
    }

    std::string generateFilename() {
        std::string filename = "telemetry_";
        if (fileCounter < 10) filename += "00";
        else if (fileCounter < 100) filename += "0";
        filename += std::to_string(fileCounter) + ".bin";
        return filename;
    }

public:
    TelemetryLogger() : fileCounter(1) {
        currentFilename = generateFilename();
        std::cout << "Логгер инициализирован. Текущий файл: " << currentFilename << std::endl;
    }


    bool logData(double time, double altitude, double speed,
        double heading, double fuel) {
        rotateFileIfNeeded();
        TelemetryData data(time, altitude, speed, heading, fuel);
        std::ofstream file(currentFilename, std::ios::binary | std::ios::app);

        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла: " << currentFilename << std::endl;
            return false;
        }
        file.write(reinterpret_cast<const char*>(&data), sizeof(TelemetryData));

        if (!file.good()) {
            std::cerr << "Ошибка записи в файл!" << std::endl;
            return false;
        }
        file.close();
        std::cout << "Записано: Time: " << time
            << ", Altitude: " << altitude
            << ", Speed: " << speed
            << ", Heading: " << heading
            << ", Fuel: " << fuel << std::endl;

        return true;
    }
    void rotateFileIfNeeded() {
        if (getFileSize(currentFilename) >= maxFileSize) {
            fileCounter++;
            currentFilename = generateFilename();
            std::cout << "Создан новый файл: " << currentFilename << std::endl;
        }
    }
    std::vector<TelemetryData> readLogFile(const std::string& filename) {
        std::vector<TelemetryData> data;

        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для чтения: " << filename << std::endl;
            return data;
        }
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        while (file.tellg() < fileSize) {
            TelemetryData temp;
            file.read(reinterpret_cast<char*>(&temp), sizeof(TelemetryData));
            if (file.gcount() == sizeof(TelemetryData)) {
                data.push_back(temp);
            }
        }
        file.close();
        return data;
    }

    void printLogSummary() {
        std::cout << "\n=== Сводка логов ===" << std::endl;
        std::cout << "Текущий файл: " << currentFilename << std::endl;
        std::cout << "Размер файла: " << getFileSize(currentFilename) << " байт" << std::endl;

        auto data = readLogFile(currentFilename);

        if (data.empty()) {
            std::cout << "Файл пуст или не существует." << std::endl;
            return;
        }

        std::cout << "Количество записей в файле: " << data.size() << std::endl;
        std::cout << "\nПоследние 3 записи:" << std::endl;
        int start = data.size() > 3 ? data.size() - 3 : 0;
        for (size_t i = start; i < data.size(); i++) {
            const auto& d = data[i];
            std::cout << "[" << i << "] Time: " << std::fixed << std::setprecision(1) << d.time
                << ", Alt: " << d.altitude
                << ", Speed: " << d.speed
                << ", Heading: " << d.heading
                << ", Fuel: " << d.fuel << std::endl;
        }
        if (!data.empty()) {
            double totalTime = data.back().time - data.front().time;
            std::cout << "\nСтатистика:" << std::endl;
            std::cout << "Общее время лога: " << totalTime << " сек" << std::endl;
            std::cout << "Начальная высота: " << data.front().altitude << std::endl;
            std::cout << "Конечная высота: " << data.back().altitude << std::endl;
            std::cout << "Расход топлива: " << data.front().fuel - data.back().fuel << std::endl;
        }
    }
    std::string getCurrentFilename() const {
        return currentFilename;
    }
};


int main() {
    TelemetryLogger logger;
    std::cout << "\nЗапись телеметрических данных:" << std::endl;

    logger.logData(0.0, 100.0, 25.0, 45.0, 80.0);
    logger.logData(1.0, 105.0, 27.0, 46.0, 79.5);
    logger.logData(2.0, 110.0, 29.0, 47.0, 79.0);
    // для проверки ротации
    for (int i = 3; i < 10; i++) {
        logger.logData(i, 100 + i * 5, 25 + i * 2, 45 + i, 80 - i * 0.5);
    }
    logger.printLogSummary();
    std::cout << "\nЧтение данных из файла:" << std::endl;
    std::string filename = logger.getCurrentFilename();
    auto data = logger.readLogFile(filename);

    if (!data.empty()) {
        std::cout << "Прочитано " << data.size() << " записей:" << std::endl;
        for (const auto& d : data) {
            std::cout << "Time: " << d.time
                << ", Altitude: " << d.altitude
                << ", Speed: " << d.speed
                << ", Heading: " << d.heading
                << ", Fuel: " << d.fuel << std::endl;
        }
    }

    return 0;
}