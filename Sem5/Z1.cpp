#include <iostream>
#include <cmath>

using namespace std;

class Aircraft {
protected:
    double mass;
    double x, y, z;
    double vx, vy, vz;

public:
    Aircraft(double m, double x0, double y0, double z0,
        double vx0, double vy0, double vz0)
        : mass(m), x(x0), y(y0), z(z0),
        vx(vx0), vy(vy0), vz(vz0) {
    }

    virtual ~Aircraft() {}

    void updatePosition(double dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
    }

    virtual void printStatus() {
        cout << "Position: (" << x << ", " << y << ", " << z << ") m" << endl;
        cout << "Velocity: (" << vx << ", " << vy << ", " << vz << ") m/s" << endl;
    }

    double getVx() const { return vx; }
    double getVy() const { return vy; }
    double getVz() const { return vz; }
};

class JetAircraft : public Aircraft {
private:
    double thrust;
    double Cd;
    double S;
    double rho;
    double fuel;
    double g;

public:
    JetAircraft(double m, double x0, double y0, double z0,
        double vx0, double vy0, double vz0,
        double t, double cd, double s,
        double r, double f, double gravity = 9.81)
        : Aircraft(m, x0, y0, z0, vx0, vy0, vz0),
        thrust(t), Cd(cd), S(s), rho(r), fuel(f), g(gravity) {
    }

    double computeDrag() {
        double speed = sqrt(vx * vx + vy * vy + vz * vz);
        return 0.5 * Cd * rho * S * speed * speed;
    }

    void simulateStep(double dt) {
        if (fuel <= 0) return;

        double drag = computeDrag();
        double speed = sqrt(vx * vx + vy * vy + vz * vz + 1e-6);
        double drag_x = -drag * (vx / speed);
        double drag_y = -drag * (vy / speed);
        double drag_z = -drag * (vz / speed);

        double thrust_z = thrust;

        double gravity_force = mass * g;

        double ax = drag_x / mass;
        double ay = drag_y / mass;
        double az = (thrust_z - gravity_force) / mass - drag_z / mass;

        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;

        updatePosition(dt);

        double fuel_consumption = 0.001 * thrust * dt;
        fuel = max(0.0, fuel - fuel_consumption);
    }

    virtual void printStatus() override {
        Aircraft::printStatus();
        cout << "Fuel: " << fuel << " kg" << endl;
        cout << "Thrust: " << thrust << " N" << endl;
    }

    double getFuel() const { return fuel; }
};

int main() {
    JetAircraft plane(20000, 0, 0, 0, 100, 0, 50,
        150000, 0.02, 50, 1.225, 5000);

    double dt = 0.5;
    int step = 0;

    while (plane.getFuel() > 0 && plane.getVz() > 0) {
        cout << "\nStep " << step << ":" << endl;
        plane.printStatus();
        plane.simulateStep(dt);
        step++;
    }

    cout << "\nSimulation ended." << endl;
    plane.printStatus();

    return 0;
}