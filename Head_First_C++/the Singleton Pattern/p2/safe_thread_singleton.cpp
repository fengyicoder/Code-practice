#include "safe_thread_singleton.h"

int main()
{
    static Singleton *ptr = Singleton::getInstance();
    return 0;
}