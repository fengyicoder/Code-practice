#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>

using namespace std;

class Singleton
{
public:
    static Singleton* getInstance();
private:
    Singleton(){}
    static Singleton* instance;
};

#endif