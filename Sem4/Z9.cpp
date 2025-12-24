#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

using namespace std;

// Функция сравнения для сортировки точек атмосферы по высоте
bool compareAtmospherePoints(const pair<double, vector<double> >& a, 
                             const pair<double, vector<double> >& b) {
    return a.first < b.first;
}

// Класс для работы с атмосферой
class Environment {
private:
    // Таблица атмосферы: высота -> {плотность, давление, температура}
    vector<pair<double, vector<double> > > atmosphereTable;
    bool tableLoaded;
    
public:
    Environment() : tableLoaded(false) {}
    
    // Загрузка таблицы атмосферы из CSV файла
    bool loadAtmosphereTable(const string& filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }
        
        atmosphereTable.clear();
        string line;
        int lineNumber = 0;
        
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            // Пропускаем заголовок
            if (lineNumber == 1 && line.find("altitude") != string::npos) {
                continue;
            }
            
            // Разбиваем строку на поля
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }
            
            // Должно быть минимум 3 поля: altitude, density, pressure
            if (fields.size() >= 3) {
                double altitude = atof(fields[0].c_str());
                double density = atof(fields[1].c_str());
                double pressure = atof(fields[2].c_str());
                
                vector<double> params;
                params.push_back(density);
                params.push_back(pressure);
                
                // Если есть температура - загружаем
                if (fields.size() >= 4) {
                    double temperature = atof(fields[3].c_str());
                    params.push_back(temperature);
                } else {
                    // Стандартная модель температуры
                    double temperature = 288.15 - 0.0065 * altitude;
                    params.push_back(temperature);
                }
                
                atmosphereTable.push_back(make_pair(altitude, params));
            }
        }
        
        file.close();
        
        // Сортируем по высоте
        sort(atmosphereTable.begin(), atmosphereTable.end(), compareAtmospherePoints);
        
        tableLoaded = true;
        cout << "Таблица атмосферы загружена: " << atmosphereTable.size() 
             << " точек из файла " << filename << endl;
        
        return !atmosphereTable.empty();
    }
    
    // Линейная интерполяция для получения параметров на любой высоте
    double getDensity(double altitude) {
        if (!tableLoaded || atmosphereTable.empty()) {
            cerr << "Таблица атмосферы не загружена!" << endl;
            return 0.0;
        }
        
        // Если высота ниже минимальной - берем первое значение
        if (altitude <= atmosphereTable[0].first) {
            return atmosphereTable[0].second[0];
        }
        
        // Если высота выше максимальной - берем последнее значение
        if (altitude >= atmosphereTable.back().first) {
            return atmosphereTable.back().second[0];
        }
        
        // Ищем два ближайших значения для интерполяции
        for (size_t i = 0; i < atmosphereTable.size() - 1; i++) {
            if (altitude >= atmosphereTable[i].first && 
                altitude <= atmosphereTable[i+1].first) {
                
                double h1 = atmosphereTable[i].first;
                double h2 = atmosphereTable[i+1].first;
                double d1 = atmosphereTable[i].second[0];
                double d2 = atmosphereTable[i+1].second[0];
                
                // Линейная интерполяция
                return d1 + (d2 - d1) * (altitude - h1) / (h2 - h1);
            }
        }
        
        return atmosphereTable.back().second[0];
    }
    
    double getPressure(double altitude) {
        if (!tableLoaded || atmosphereTable.empty()) {
            cerr << "Таблица атмосферы не загружена!" << endl;
            return 0.0;
        }
        
        if (altitude <= atmosphereTable[0].first) {
            return atmosphereTable[0].second[1];
        }
        
        if (altitude >= atmosphereTable.back().first) {
            return atmosphereTable.back().second[1];
        }
        
        for (size_t i = 0; i < atmosphereTable.size() - 1; i++) {
            if (altitude >= atmosphereTable[i].first && 
                altitude <= atmosphereTable[i+1].first) {
                
                double h1 = atmosphereTable[i].first;
                double h2 = atmosphereTable[i+1].first;
                double p1 = atmosphereTable[i].second[1];
                double p2 = atmosphereTable[i+1].second[1];
                
                return p1 + (p2 - p1) * (altitude - h1) / (h2 - h1);
            }
        }
        
        return atmosphereTable.back().second[1];
    }
    
    double getTemperature(double altitude) {
        if (!tableLoaded || atmosphereTable.empty()) {
            cerr << "Таблица атмосферы не загружена!" << endl;
            return 0.0;
        }
        
        if (altitude <= atmosphereTable[0].first) {
            return atmosphereTable[0].second[2];
        }
        
        if (altitude >= atmosphereTable.back().first) {
            return atmosphereTable.back().second[2];
        }
        
        for (size_t i = 0; i < atmosphereTable.size() - 1; i++) {
            if (altitude >= atmosphereTable[i].first && 
                altitude <= atmosphereTable[i+1].first) {
                
                double h1 = atmosphereTable[i].first;
                double h2 = atmosphereTable[i+1].first;
                double t1 = atmosphereTable[i].second[2];
                double t2 = atmosphereTable[i+1].second[2];
                
                return t1 + (t2 - t1) * (altitude - h1) / (h2 - h1);
            }
        }
        
        return atmosphereTable.back().second[2];
    }
    
    // Вывод информации о таблице
    void printAtmosphereInfo() {
        cout << "\n=== ИНФОРМАЦИЯ О ТАБЛИЦЕ АТМОСФЕРЫ ===" << endl;
        
        if (!tableLoaded) {
            cout << "Таблица не загружена." << endl;
            return;
        }
        
        cout << "Количество точек: " << atmosphereTable.size() << endl;
        cout << "Диапазон высот: " << atmosphereTable[0].first << " - " 
             << atmosphereTable.back().first << " м" << endl;
        
        cout << "\nПримеры интерполяции:" << endl;
        cout << fixed << setprecision(2);
        cout << "На высоте 0 м: плотность = " << getDensity(0) 
             << " кг/м³, давление = " << getPressure(0) / 1000 << " кПа" << endl;
        cout << "На высоте 1500 м: плотность = " << getDensity(1500) 
             << " кг/м³, давление = " << getPressure(1500) / 1000 << " кПа" << endl;
        cout << "На высоте 5000 м: плотность = " << getDensity(5000) 
             << " кг/м³, давление = " << getPressure(5000) / 1000 << " кПа" << endl;
    }
    
    // Создание тестового файла атмосферы
    static void createTestAtmosphereFile() {
        ofstream file("atmosphere.csv");
        
        file << "altitude,density,pressure" << endl;
        file << "0,1.225,101325" << endl;
        file << "500,1.167,95461" << endl;
        file << "1000,1.112,89880" << endl;
        file << "1500,1.058,84556" << endl;
        file << "2000,1.007,79500" << endl;
        file << "2500,0.957,74690" << endl;
        file << "3000,0.909,70108" << endl;
        file << "4000,0.819,61640" << endl;
        file << "5000,0.736,54020" << endl;
        file << "6000,0.660,47181" << endl;
        file << "7000,0.590,41061" << endl;
        file << "8000,0.526,35600" << endl;
        file << "9000,0.467,30743" << endl;
        file << "10000,0.414,26436" << endl;
        
        file.close();
        cout << "Тестовый файл атмосферы создан: atmosphere.csv" << endl;
    }
};

// Класс для работы с параметрами ЛА
class Aircraft {
private:
    // Основные параметры
    double mass;                // масса, кг
    double wing_area;           // площадь крыла, м²
    double Cx;                  // коэффициент лобового сопротивления
    double Cy;                  // коэффициент подъемной силы
    double max_thrust;          // максимальная тяга, Н
    double specific_consumption;// удельный расход топлива, кг/(Н·ч)
    
    // Дополнительные параметры
    string name;
    double max_speed;           // максимальная скорость, м/с
    double max_altitude;        // максимальная высота, м
    double fuel_capacity;       // емкость топливных баков, кг
    
    bool parametersLoaded;
    
public:
    Aircraft() : mass(0), wing_area(0), Cx(0), Cy(0), max_thrust(0), 
                specific_consumption(0), max_speed(0), max_altitude(0),
                fuel_capacity(0), parametersLoaded(false) {}
    
    // Загрузка параметров из файла
    bool loadFromFile(const string& filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }
        
        // Устанавливаем значения по умолчанию
        name = "Неизвестный ЛА";
        max_speed = 300;
        max_altitude = 10000;
        fuel_capacity = 5000;
        
        string line;
        int lineNumber = 0;
        
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            // Удаляем пробелы в начале и конце строки
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            // Пропускаем комментарии
            if (line[0] == '#') continue;
            
            // Ищем знак равенства
            size_t eq_pos = line.find('=');
            if (eq_pos == string::npos) continue;
            
            // Извлекаем ключ и значение
            string key = line.substr(0, eq_pos);
            string value = line.substr(eq_pos + 1);
            
            // Удаляем пробелы вокруг ключа и значения
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Обрабатываем параметры
            if (key == "name") {
                name = value;
            } else if (key == "mass") {
                mass = atof(value.c_str());
            } else if (key == "wing_area") {
                wing_area = atof(value.c_str());
            } else if (key == "Cx") {
                Cx = atof(value.c_str());
            } else if (key == "Cy") {
                Cy = atof(value.c_str());
            } else if (key == "max_thrust") {
                max_thrust = atof(value.c_str());
            } else if (key == "specific_consumption") {
                specific_consumption = atof(value.c_str());
            } else if (key == "max_speed") {
                max_speed = atof(value.c_str());
            } else if (key == "max_altitude") {
                max_altitude = atof(value.c_str());
            } else if (key == "fuel_capacity") {
                fuel_capacity = atof(value.c_str());
            } else {
                cout << "Предупреждение: неизвестный параметр '" << key 
                     << "' в строке " << lineNumber << endl;
            }
        }
        
        file.close();
        
        // Проверяем, что основные параметры загружены
        if (mass > 0 && wing_area > 0 && max_thrust > 0) {
            parametersLoaded = true;
            cout << "Параметры ЛА загружены из файла: " << filename << endl;
            cout << "ЛА: " << name << endl;
            return true;
        } else {
            cerr << "Ошибка: не все основные параметры ЛА загружены!" << endl;
            return false;
        }
    }
    
    // Геттеры для параметров
    double getMass() const { return mass; }
    double getWingArea() const { return wing_area; }
    double getCx() const { return Cx; }
    double getCy() const { return Cy; }
    double getMaxThrust() const { return max_thrust; }
    double getSpecificConsumption() const { return specific_consumption; }
    string getName() const { return name; }
    double getMaxSpeed() const { return max_speed; }
    double getMaxAltitude() const { return max_altitude; }
    double getFuelCapacity() const { return fuel_capacity; }
    bool isLoaded() const { return parametersLoaded; }
    
    // Расчет аэродинамических сил
    double calculateLift(double density, double velocity, double angle_of_attack = 0.1) {
        // Упрощенный расчет подъемной силы
        double q = 0.5 * density * velocity * velocity; // скоростной напор
        double actual_Cy = Cy * angle_of_attack; // Cy зависит от угла атаки
        return q * wing_area * actual_Cy;
    }
    
    double calculateDrag(double density, double velocity, double angle_of_attack = 0.1) {
        // Упрощенный расчет силы сопротивления
        double q = 0.5 * density * velocity * velocity;
        double actual_Cx = Cx + 0.05 * (Cy * angle_of_attack) * (Cy * angle_of_attack); // индуктивное сопротивление
        return q * wing_area * actual_Cx;
    }
    
    // Расчет потребления топлива
    double calculateFuelConsumption(double thrust, double time_seconds) {
        // thrust - текущая тяга в Ньютонах
        // time_seconds - время работы в секундах
        // specific_consumption - кг/(Н·ч)
        // возвращает расход в кг
        return thrust * specific_consumption * time_seconds / 3600.0;
    }
    
    // Вывод информации о ЛА
    void printAircraftInfo() {
        cout << "\n=== ПАРАМЕТРЫ ЛЕТАТЕЛЬНОГО АППАРАТА ===" << endl;
        cout << "Название: " << name << endl;
        cout << fixed << setprecision(2);
        cout << "Масса: " << mass << " кг" << endl;
        cout << "Площадь крыла: " << wing_area << " м²" << endl;
        cout << "Аэродинамические коэффициенты: Cx = " << Cx << ", Cy = " << Cy << endl;
        cout << "Максимальная тяга: " << max_thrust << " Н (" << max_thrust / 1000 << " кН)" << endl;
        cout << "Удельный расход топлива: " << specific_consumption << " кг/(Н·ч)" << endl;
        cout << "Максимальная скорость: " << max_speed << " м/с (" << max_speed * 3.6 << " км/ч)" << endl;
        cout << "Максимальная высота: " << max_altitude << " м" << endl;
        cout << "Емкость топливных баков: " << fuel_capacity << " кг" << endl;
        
        // Расчет некоторых характеристик
        cout << "\nРАСЧЕТНЫЕ ХАРАКТЕРИСТИКИ:" << endl;
        double wing_loading = mass / wing_area; // удельная нагрузка на крыло
        cout << "Удельная нагрузка на крыло: " << wing_loading << " кг/м²" << endl;
        
        double thrust_to_weight = max_thrust / (mass * 9.81); // тяговооруженность
        cout << "Тяговооруженность: " << thrust_to_weight << endl;
        
        if (thrust_to_weight > 1.0) {
            cout << "  ЛА способен к вертикальному взлету" << endl;
        } else {
            cout << "  ЛА требует разбега для взлета" << endl;
        }
    }
    
    // Создание тестового файла параметров ЛА
    static void createTestAircraftFile() {
        ofstream file("aircraft_params.txt");
        
        file << "# Параметры легкого самолета" << endl;
        file << "name=Легкий самолет" << endl;
        file << "mass=18400" << endl;
        file << "wing_area=62" << endl;
        file << "Cx=0.02" << endl;
        file << "Cy=3.45" << endl;
        file << "max_thrust=14500" << endl;
        file << "specific_consumption=0.75" << endl;
        file << "max_speed=280" << endl;
        file << "max_altitude=8500" << endl;
        file << "fuel_capacity=4500" << endl;
        
        file.close();
        cout << "Тестовый файл параметров ЛА создан: aircraft_params.txt" << endl;
    }
};

// Демонстрационный класс для расчета траектории
class TrajectoryCalculator {
private:
    Aircraft aircraft;
    Environment atmosphere;
    
public:
    TrajectoryCalculator() {}
    
    // Инициализация системы
    bool initialize(const string& aircraft_file, const string& atmosphere_file) {
        if (!aircraft.loadFromFile(aircraft_file)) {
            return false;
        }
        
        if (!atmosphere.loadAtmosphereTable(atmosphere_file)) {
            return false;
        }
        
        return true;
    }
    
    // Простой расчет траектории подъема
    void calculateClimbTrajectory(double initial_altitude, double final_altitude, 
                                  double initial_speed, double climb_angle) {
        cout << "\n=== РАСЧЕТ ТРАЕКТОРИИ НАБОРА ВЫСОТЫ ===" << endl;
        
        if (!aircraft.isLoaded()) {
            cout << "Параметры ЛА не загружены!" << endl;
            return;
        }
        
        double altitude = initial_altitude;
        double velocity = initial_speed;
        double time = 0;
        double fuel_used = 0;
        double distance = 0;
        
        const double dt = 1.0; // шаг 1 секунда
        const double g = 9.81;
        const double PI = 3.14159265358979323846;
        
        cout << fixed << setprecision(2);
        cout << "Начальные условия:" << endl;
        cout << "  Высота: " << initial_altitude << " м" << endl;
        cout << "  Скорость: " << initial_speed << " м/с (" << initial_speed * 3.6 << " км/ч)" << endl;
        cout << "  Угол набора: " << climb_angle << " град" << endl;
        
        cout << "\nПроцесс набора высоты:" << endl;
        cout << left << setw(10) << "Время(с)" 
             << setw(12) << "Высота(м)" 
             << setw(12) << "Скорость(м/с)" 
             << setw(12) << "Расход(кг)" << endl;
        cout << string(46, '-') << endl;
        
        int step = 0;
        while (altitude < final_altitude && step < 1000) {
            // Получаем плотность на текущей высоте
            double density = atmosphere.getDensity(altitude);
            
            // Расчет сил
            double lift = aircraft.calculateLift(density, velocity, 0.1);
            double drag = aircraft.calculateDrag(density, velocity, 0.1);
            
            // Тяга (используем 80% от максимальной для набора)
            double thrust = aircraft.getMaxThrust() * 0.8;
            
            // Уравнения движения (упрощенные)
            double climb_angle_rad = climb_angle * PI / 180.0;
            
            // Ускорение по траектории
            double acceleration = (thrust - drag) / aircraft.getMass() - g * sin(climb_angle_rad);
            velocity += acceleration * dt;
            
            // Изменение высоты
            altitude += velocity * sin(climb_angle_rad) * dt;
            
            // Изменение горизонтального расстояния
            distance += velocity * cos(climb_angle_rad) * dt;
            
            // Расход топлива
            double fuel_step = aircraft.calculateFuelConsumption(thrust, dt);
            fuel_used += fuel_step;
            
            time += dt;
            step++;
            
            // Выводим каждые 10 секунд
            if (fmod(time, 10.0) < dt) {
                cout << left << setw(10) << time
                     << setw(12) << altitude
                     << setw(12) << velocity
                     << setw(12) << fuel_used << endl;
            }
        }
        
        cout << "\nИтоговые показатели:" << endl;
        cout << "Время набора: " << time << " с (" << time / 60.0 << " мин)" << endl;
        cout << "Конечная высота: " << altitude << " м" << endl;
        cout << "Конечная скорость: " << velocity << " м/с (" << velocity * 3.6 << " км/ч)" << endl;
        cout << "Общий расход топлива: " << fuel_used << " кг" << endl;
        cout << "Пройденное расстояние: " << distance << " м (" << distance / 1000.0 << " км)" << endl;
        cout << "Средняя скорость набора: " << (altitude - initial_altitude) / time << " м/с" << endl;
    }
    
    // Вывод всей информации о системе
    void printSystemInfo() {
        aircraft.printAircraftInfo();
        atmosphere.printAtmosphereInfo();
    }
};

// Главная функция
int main() {
    cout << "=== СИСТЕМА ЗАГРУЗКИ ПАРАМЕТРОВ ЛА И АТМОСФЕРЫ ===" << endl;
    
    // Создаем тестовые файлы
    Aircraft::createTestAircraftFile();
    Environment::createTestAtmosphereFile();
    
    // Создаем и инициализируем калькулятор траектории
    TrajectoryCalculator calculator;
    
    if (!calculator.initialize("aircraft_params.txt", "atmosphere.csv")) {
        cerr << "Ошибка инициализации системы!" << endl;
        return 1;
    }
    
    // Выводим информацию о системе
    calculator.printSystemInfo();
    
    // Демонстрация расчета траектории
    cout << "\n=== ДЕМОНСТРАЦИЯ РАСЧЕТА ТРАЕКТОРИИ ===" << endl;
    calculator.calculateClimbTrajectory(500, 5000, 100, 15);
    
    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    cout << "Созданные файлы:" << endl;
    cout << "  aircraft_params.txt - параметры легкого самолета" << endl;
    cout << "  atmosphere.csv - таблица атмосферы" << endl;
    
    return 0;
}