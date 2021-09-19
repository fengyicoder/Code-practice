#include "singleton.h"

Singleton * Singleton::instance = NULL;

Singleton* Singleton::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Singleton();
    }
    return instance;
}