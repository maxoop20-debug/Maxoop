#include <iostream>
#include <iomanip>

class Engine {
private:
    double thrust;    // Н
    double fuelFlow;  // кг/с
    double fuel;      // кг
public:
    Engine(double t, double ff, double f) : thrust(t), fuelFlow(ff), fuel(f) {}

    double getThrust() const { return thrust; }
    bool hasFuel() const { return fuel > 0; }

    void burn(double dt) {
        if (fuel <= 0) return;
        double consumed = fuelFlow * dt;
        if (consumed > fuel) consumed = fuel;
        fuel -= consumed;
    }
};

class Navigation {
private:
    double altitude;      // м
    double velocity;      // м/с
    double acceleration;  // м/с²
    double mass;          // кг
public:
    Navigation(double h, double v, double m) : altitude(h), velocity(v), mass(m), acceleration(0) {}

    void update(double thrust, double dt) {
        acceleration = thrust / mass - 9.81;
        velocity += acceleration * dt;
        altitude += velocity * dt;
        if (altitude < 0) altitude = 0;
    }

    void printStatus(double time) const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "t=" << time << "c | h=" << altitude << "м | v=" 
                  << velocity << "м/с | a=" << acceleration << "м/с²" << std::endl;
    }
};

class AutonomousFlightSystem {
private:
    Engine engine;
    Navigation nav;
    double time;
public:
    AutonomousFlightSystem(const Engine& e, const Navigation& n) 
        : engine(e), nav(n), time(0) {}

    void simulate(double dt, double totalTime) {
        while (time < totalTime && engine.hasFuel()) {
            engine.burn(dt);
            nav.update(engine.getThrust(), dt);
            nav.printStatus(time);
            time += dt;
        }
        std::cout << "--- Полёт завершён ---" << std::endl;
    }
};

int main() {
    Engine eng(15000, 5, 50);      // тяга 15000 Н, расход 5 кг/с, топливо 50 кг
    Navigation nav(0, 0, 1000);    // высота 0, скорость 0, масса 1000 кг
    AutonomousFlightSystem afs(eng, nav);
    
    afs.simulate(1.0, 20.0);
    
    return 0;
}