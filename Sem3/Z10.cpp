#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Point {
public:
    double x, y, z;
};

class Rocket {
private:
    double mass;
    double vx, vy, vz;
    double x, y, z;
    
public:
    Rocket(double m, double vel_x, double vel_y, double vel_z) {
        mass = m;
        vx = vel_x;
        vy = vel_y;
        vz = vel_z;
        x = 0;
        y = 0;
        z = 0;
    }
    
    void updatePosition(double dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
    }
    
    void changeVelocity(double ax, double ay, double az, double dt) {
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;
    }
    
    Point getPosition() {
        Point p;
        p.x = x;
        p.y = y;
        p.z = z;
        return p;
    }
    
    void setVelocity(double vel_x, double vel_y, double vel_z) {
        vx = vel_x;
        vy = vel_y;
        vz = vel_z;
    }
};

int main() {
    const int num_rockets = 5;
    const double dt = 0.1;
    const double T = 10.0;
    
    vector<Rocket> squad;
    squad.reserve(num_rockets);
    
    squad.push_back(Rocket(1000, 10, 0, 50));
    squad.push_back(Rocket(900, 15, -5, 40));
    squad.push_back(Rocket(1100, 5, 3, 45));
    squad.push_back(Rocket(950, -8, 2, 55));
    squad.push_back(Rocket(1050, 12, -3, 35));
    
    vector<vector<Point> > trajectories(num_rockets);
    
    for (int step = 0; step <= static_cast<int>(T/dt); step++) {
        double t = step * dt;
        
        for (int i = 0; i < num_rockets; i++) {
            double g = -9.8;
            squad[i].changeVelocity(0, 0, g, dt);
            
            squad[i].updatePosition(dt);
            
            Point current_pos = squad[i].getPosition();
            trajectories[i].push_back(current_pos);
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "========================================================" << endl;
    cout << "  Ракета  |    X (м)   |    Y (м)   |    Z (м)   " << endl;
    cout << "========================================================" << endl;
    
    for (int i = 0; i < num_rockets; i++) {
        Point final_pos = squad[i].getPosition();
        cout << "    " << i+1 << "    | " 
             << setw(10) << final_pos.x << " | " 
             << setw(10) << final_pos.y << " | " 
             << setw(10) << final_pos.z << endl;
    }
    
    cout << "========================================================" << endl;
    cout << "\nПоложения через 10 секунд полета:" << endl;
    
    for (int i = 0; i < num_rockets; i++) {
        cout << "\nРакета " << i+1 << ":" << endl;
        cout << "Количество точек траектории: " << trajectories[i].size() << endl;
        if (!trajectories[i].empty()) {
            Point last = trajectories[i].back();
            cout << "Конечная позиция: (" << last.x << ", " 
                 << last.y << ", " << last.z << ")" << endl;
        }
    }
    
    return 0;
}