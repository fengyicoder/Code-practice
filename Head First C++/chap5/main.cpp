#include "singleton.h"

int main() 
{
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
    cout << "address of p1: " << hex << p1 << endl;
    cout << "address of p2: " << hex << p2 << endl;

    return 0;
}