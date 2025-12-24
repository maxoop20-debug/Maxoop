#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// Класс для анализа расхода топлива
class FuelAnalyzer {
private:
    vector<double> time_data;
    vector<double> fuel_data;
    vector<double> rpm_data;
    vector<bool> anomalies;
    vector<string> anomaly_reasons;
    
    // Функтор для обнаружения аномалий (аналог лямбда-функции)
    class AnomalyDetector {
    private:
        double avg;
        double threshold;
        
    public:
        AnomalyDetector(double avg_consumption, double thresh = 1.5) 
            : avg(avg_consumption), threshold(thresh) {}
        
        bool operator()(double consumption) const {
            return consumption > avg * threshold;
        }
    };
    
public:
    // Загрузка данных из CSV файла
    bool loadData(const string& filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }
        
        // Очищаем данные
        time_data.clear();
        fuel_data.clear();
        rpm_data.clear();
        
        string line;
        int lineNumber = 0;
        bool hasHeader = false;
        
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            // Пропускаем заголовок
            if (lineNumber == 1 && line.find("time") != string::npos) {
                hasHeader = true;
                continue;
            }
            
            // Разбиваем строку на поля
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }
            
            // Должно быть 3 поля: time, fuel_consumption, engine_rpm
            if (fields.size() >= 3) {
                double time_val = atof(fields[0].c_str());
                double fuel_val = atof(fields[1].c_str());
                double rpm_val = atof(fields[2].c_str());
                
                time_data.push_back(time_val);
                fuel_data.push_back(fuel_val);
                rpm_data.push_back(rpm_val);
            } else {
                cerr << "Предупреждение: строка " << lineNumber 
                     << " имеет неверный формат" << endl;
            }
        }
        
        file.close();
        cout << "Загружено " << time_data.size() << " записей из файла " << filename << endl;
        return !time_data.empty();
    }
    
    // Обнаружение аномалий
    void detectAnomalies() {
        if (fuel_data.empty()) {
            cout << "Нет данных для анализа" << endl;
            return;
        }
        
        // Инициализируем векторы для аномалий
        anomalies.clear();
        anomaly_reasons.clear();
        anomalies.resize(fuel_data.size(), false);
        anomaly_reasons.resize(fuel_data.size(), "");
        
        // Расчет среднего расхода
        double avg_consumption = calculateAverageConsumption();
        cout << "Средний расход топлива: " << avg_consumption << " л/ч" << endl;
        
        // Расчет стандартного отклонения
        double std_dev = calculateStandardDeviation();
        cout << "Стандартное отклонение: " << std_dev << endl;
        
        // Создаем детектор аномалий (аналог лямбда-функции)
        AnomalyDetector isAnomaly(avg_consumption, 1.5);
        
        // Порог для обнаружения выбросов (3 сигмы)
        double outlier_threshold = avg_consumption + 3 * std_dev;
        
        // Обнаруживаем аномалии
        int anomaly_count = 0;
        
        for (size_t i = 0; i < fuel_data.size(); i++) {
            double consumption = fuel_data[i];
            double rpm = rpm_data[i];
            
            // Проверка 1: расход выше порога (аналог лямбда-функции)
            if (isAnomaly(consumption)) {
                anomalies[i] = true;
                anomaly_reasons[i] = "Расход топлива " + toString(consumption) + 
                                    " превышает среднее в 1.5 раза";
                anomaly_count++;
                continue;
            }
            
            // Проверка 2: выброс (более 3 стандартных отклонений)
            if (consumption > outlier_threshold) {
                anomalies[i] = true;
                anomaly_reasons[i] = "Выброс: расход " + toString(consumption) + 
                                    " > " + toString(outlier_threshold);
                anomaly_count++;
                continue;
            }
            
            // Проверка 3: резкий скачок расхода
            if (i > 0) {
                double prev_consumption = fuel_data[i-1];
                double change = fabs(consumption - prev_consumption);
                
                if (change > avg_consumption * 0.3) { // Более 30% от среднего
                    anomalies[i] = true;
                    anomaly_reasons[i] = "Резкий скачок расхода: изменение на " + 
                                        toString(change) + " л/ч";
                    anomaly_count++;
                    continue;
                }
            }
            
            // Проверка 4: аномальное соотношение RPM/расход
            if (rpm > 0) {
                double efficiency = consumption / rpm * 1000; // Упрощенный показатель эффективности
                double avg_efficiency = calculateAverageEfficiency();
                
                if (fabs(efficiency - avg_efficiency) > avg_efficiency * 0.5) {
                    anomalies[i] = true;
                    anomaly_reasons[i] = "Аномальное соотношение RPM/расход: " + 
                                        toString(efficiency) + " (среднее: " + 
                                        toString(avg_efficiency) + ")";
                    anomaly_count++;
                }
            }
        }
        
        cout << "Обнаружено аномалий: " << anomaly_count << " из " << fuel_data.size() 
             << " записей (" << (double)anomaly_count / fuel_data.size() * 100 << "%)" << endl;
    }
    
    // Расчет среднего расхода
    double calculateAverageConsumption() {
        if (fuel_data.empty()) return 0.0;
        
        double sum = 0.0;
        for (size_t i = 0; i < fuel_data.size(); i++) {
            sum += fuel_data[i];
        }
        
        return sum / fuel_data.size();
    }
    
    // Генерация отчета
    void generateReport(const string& filename) {
        ofstream report_file(filename.c_str());
        
        if (!report_file.is_open()) {
            cerr << "Ошибка создания файла отчета: " << filename << endl;
            return;
        }
        
        // Заголовок отчета
        report_file << "=== ОТЧЕТ АНАЛИЗА РАСХОДА ТОПЛИВА ===" << endl;
        report_file << "Дата анализа: 2024-12-15" << endl;
        report_file << "Всего записей: " << time_data.size() << endl;
        report_file << endl;
        
        // Общая статистика
        double avg_consumption = calculateAverageConsumption();
        double min_consumption = *min_element(fuel_data.begin(), fuel_data.end());
        double max_consumption = *max_element(fuel_data.begin(), fuel_data.end());
        double std_dev = calculateStandardDeviation();
        
        report_file << "ОБЩАЯ СТАТИСТИКА:" << endl;
        report_file << "Средний расход: " << fixed << setprecision(2) << avg_consumption << " л/ч" << endl;
        report_file << "Минимальный расход: " << min_consumption << " л/ч" << endl;
        report_file << "Максимальный расход: " << max_consumption << " л/ч" << endl;
        report_file << "Стандартное отклонение: " << std_dev << endl;
        report_file << endl;
        
        // Аномалии
        int anomaly_count = 0;
        for (size_t i = 0; i < anomalies.size(); i++) {
            if (anomalies[i]) anomaly_count++;
        }
        
        report_file << "АНАМАЛИИ:" << endl;
        report_file << "Обнаружено аномалий: " << anomaly_count << " (" 
                   << fixed << setprecision(1) 
                   << (double)anomaly_count / anomalies.size() * 100 << "%)" << endl;
        report_file << endl;
        
        if (anomaly_count > 0) {
            report_file << "ДЕТАЛИ АНОМАЛИЙ:" << endl;
            report_file << left << setw(8) << "Время" 
                       << setw(12) << "Расход" 
                       << setw(12) << "RPM" 
                       << "Причина" << endl;
            report_file << string(60, '-') << endl;
            
            for (size_t i = 0; i < anomalies.size(); i++) {
                if (anomalies[i]) {
                    report_file << left << fixed << setprecision(2)
                               << setw(8) << time_data[i]
                               << setw(12) << fuel_data[i]
                               << setw(12) << rpm_data[i]
                               << anomaly_reasons[i] << endl;
                }
            }
            report_file << endl;
        }
        
        // Рекомендации
        report_file << "РЕКОМЕНДАЦИИ:" << endl;
        
        if (anomaly_count == 0) {
            report_file << "Аномалий не обнаружено. Система работает нормально." << endl;
        } else if ((double)anomaly_count / anomalies.size() < 0.1) {
            report_file << "Обнаружены единичные аномалии. Рекомендуется проверить:" << endl;
            report_file << "1. Датчики расхода топлива" << endl;
            report_file << "2. Форсунки двигателя" << endl;
            report_file << "3. Систему впрыска топлива" << endl;
        } else {
            report_file << "Обнаружено много аномалий! Требуется срочная проверка:" << endl;
            report_file << "1. Полная диагностика двигателя" << endl;
            report_file << "2. Проверка топливной системы" << endl;
            report_file << "3. Калибровка датчиков" << endl;
            report_file << "4. Анализ условий полета" << endl;
        }
        
        report_file.close();
        cout << "Отчет сохранен в файл: " << filename << endl;
    }
    
    // Вывод результатов анализа на экран
    void printAnalysis() {
        cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА РАСХОДА ТОПЛИВА ===" << endl;
        
        if (fuel_data.empty()) {
            cout << "Нет данных для анализа." << endl;
            return;
        }
        
        // Общая статистика
        double avg_consumption = calculateAverageConsumption();
        double min_consumption = *min_element(fuel_data.begin(), fuel_data.end());
        double max_consumption = *max_element(fuel_data.begin(), fuel_data.end());
        
        cout << fixed << setprecision(2);
        cout << "Всего записей: " << time_data.size() << endl;
        cout << "Средний расход: " << avg_consumption << " л/ч" << endl;
        cout << "Минимальный расход: " << min_consumption << " л/ч" << endl;
        cout << "Максимальный расход: " << max_consumption << " л/ч" << endl;
        
        // Аномалии
        int anomaly_count = 0;
        for (size_t i = 0; i < anomalies.size(); i++) {
            if (anomalies[i]) anomaly_count++;
        }
        
        cout << "\nОбнаружено аномалий: " << anomaly_count << endl;
        
        if (anomaly_count > 0) {
            cout << "\nСписок аномалий:" << endl;
            cout << left << setw(8) << "Время" 
                 << setw(12) << "Расход" 
                 << setw(12) << "RPM" 
                 << "Причина" << endl;
            cout << string(60, '-') << endl;
            
            for (size_t i = 0; i < anomalies.size(); i++) {
                if (anomalies[i]) {
                    cout << left << fixed << setprecision(2)
                         << setw(8) << time_data[i]
                         << setw(12) << fuel_data[i]
                         << setw(12) << rpm_data[i]
                         << anomaly_reasons[i] << endl;
                }
            }
        }
    }
    
    // Создание тестового файла
    static void createTestFile() {
        ofstream file("fuel_data.csv");
        
        file << "time,fuel_consumption,engine_rpm" << endl;
        file << "0,120,8000" << endl;
        file << "1,125,8100" << endl;
        file << "2,130,8200" << endl;
        file << "3,200,8300" << endl;  // Аномалия: высокий расход
        file << "4,128,8250" << endl;
        file << "5,126,8150" << endl;
        file << "6,300,8400" << endl;  // Еще одна аномалия
        file << "7,122,8050" << endl;
        file << "8,50,7000" << endl;   // Аномалия: слишком низкий расход
        file << "9,124,8080" << endl;
        
        file.close();
        cout << "Тестовый файл создан: fuel_data.csv" << endl;
    }
    
private:
    // Расчет стандартного отклонения
    double calculateStandardDeviation() {
        if (fuel_data.size() <= 1) return 0.0;
        
        double avg = calculateAverageConsumption();
        double sum_sq = 0.0;
        
        for (size_t i = 0; i < fuel_data.size(); i++) {
            double diff = fuel_data[i] - avg;
            sum_sq += diff * diff;
        }
        
        return sqrt(sum_sq / (fuel_data.size() - 1));
    }
    
    // Расчет средней эффективности (расход/RPM)
    double calculateAverageEfficiency() {
        if (fuel_data.empty() || rpm_data.empty()) return 0.0;
        
        double sum = 0.0;
        int count = 0;
        
        for (size_t i = 0; i < fuel_data.size(); i++) {
            if (rpm_data[i] > 0) {
                sum += fuel_data[i] / rpm_data[i] * 1000;
                count++;
            }
        }
        
        return count > 0 ? sum / count : 0.0;
    }
    
    // Преобразование double в string
    string toString(double value) {
        stringstream ss;
        ss << fixed << setprecision(2) << value;
        return ss.str();
    }
};

// Главная функция
int main() {
    cout << "=== АНАЛИЗАТОР РАСХОДА ТОПЛИВА ===" << endl;
    
    // Создаем тестовый файл
    FuelAnalyzer::createTestFile();
    
    // Создаем анализатор
    FuelAnalyzer analyzer;
    
    // Загружаем данные
    if (!analyzer.loadData("fuel_data.csv")) {
        return 1;
    }
    
    // Обнаруживаем аномалии
    cout << "\nОбнаружение аномалий..." << endl;
    analyzer.detectAnomalies();
    
    // Выводим анализ на экран
    analyzer.printAnalysis();
    
    // Генерируем отчет
    analyzer.generateReport("fuel_analysis_report.txt");
    
    // Дополнительный тест с другим файлом
    cout << "\n=== ДОПОЛНИТЕЛЬНЫЙ ТЕСТ ===" << endl;
    
    // Создаем еще один тестовый файл
    ofstream testFile("fuel_test.csv");
    testFile << "time,fuel_consumption,engine_rpm" << endl;
    for (int i = 0; i < 20; i++) {
        double time = i;
        double fuel = 100 + (i % 3) * 10; // Нормальные значения
        double rpm = 8000 + i * 50;
        
        // Добавляем несколько аномалий
        if (i == 5) fuel = 250;  // Аномалия
        if (i == 10) fuel = 40;   // Аномалия (слишком низкий)
        if (i == 15) fuel = 180;  // Аномалия
        
        testFile << time << "," << fuel << "," << rpm << endl;
    }
    testFile.close();
    
    cout << "Создан тестовый файл: fuel_test.csv" << endl;
    
    FuelAnalyzer analyzer2;
    analyzer2.loadData("fuel_test.csv");
    analyzer2.detectAnomalies();
    analyzer2.printAnalysis();
    analyzer2.generateReport("fuel_test_report.txt");
    
    // Интерактивный режим
    cout << "\n=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===" << endl;
    
    char choice;
    do {
        string filename;
        cout << "\nВведите имя CSV файла для анализа (или 'exit' для выхода): ";
        cin >> filename;
        
        if (filename == "exit") break;
        
        FuelAnalyzer userAnalyzer;
        if (userAnalyzer.loadData(filename)) {
            userAnalyzer.detectAnomalies();
            userAnalyzer.printAnalysis();
            
            cout << "Сгенерировать отчет? (y/n): ";
            char gen_report;
            cin >> gen_report;
            
            if (gen_report == 'y' || gen_report == 'Y') {
                string report_name;
                cout << "Введите имя файла для отчета: ";
                cin >> report_name;
                userAnalyzer.generateReport(report_name);
            }
        } else {
            cout << "Не удалось загрузить файл: " << filename << endl;
        }
        
        cout << "\nПроанализировать другой файл? (y/n): ";
        cin >> choice;
        
    } while (choice == 'y' || choice == 'Y');
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    cout << "Созданные файлы:" << endl;
    cout << "  fuel_data.csv - тестовые данные" << endl;
    cout << "  fuel_analysis_report.txt - отчет анализа" << endl;
    cout << "  fuel_test.csv - дополнительные тестовые данные" << endl;
    cout << "  fuel_test_report.txt - отчет по дополнительным данным" << endl;
    
    return 0;
}