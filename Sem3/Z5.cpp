#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Point {
    double x, y, z;
};

int main() {
    const double dt = 0.1;
    const double t_max = 10.0;
    
    double vx = 30.0;
    double vy = 20.0;
    double vz = 40.0;
    
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    
    vector<Point> traj;
    vector<double> times;
    
    int estimated_size = static_cast<int>(t_max / dt) + 2;
    traj.reserve(estimated_size);
    times.reserve(estimated_size);
    
    for (double t = 0.0; t <= t_max; t += dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
        
        Point p;
        p.x = x;
        p.y = y;
        p.z = z;
        traj.push_back(p);
        
        times.push_back(t);
    }
    
    cout << fixed << setprecision(2);
    cout << "======================================================================" << endl;
    cout << "   Время (с)     X (м)      Y (м)      Z (м)" << endl;
    cout << "======================================================================" << endl;
    
    for (size_t i = 0; i < traj.size(); i++) {
        cout << setw(10) << times[i] 
             << setw(12) << traj[i].x 
             << setw(12) << traj[i].y 
             << setw(12) << traj[i].z << endl;
    }
    
    cout << "======================================================================" << endl;
    cout << "Начальные скорости: vx = " << vx << " м/с, vy = " << vy 
         << " м/с, vz = " << vz << " м/с" << endl;
    cout << "Шаг времени: " << dt << " с" << endl;
    cout << "Всего точек: " << traj.size() << endl;
    
    return 0;
}