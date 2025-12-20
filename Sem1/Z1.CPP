#include <iostream>
using namespace std;

int main(){
    double s,v,p,c,L;
    cout<<"Input sqare of a wing:\n";
    cin>>s;
    cout<<"Input velocity:\n";
    cin>>v;
    cout<<"Input dencity of air:\n";
    cin>>p;
    cout<<"Input value of lift coefficient:\n";
    cin>>c;
    L=0.5*(p*(v*v)*s*c);
    cout<<"Value of lift power is: " << L << endl;
    return 0;
}