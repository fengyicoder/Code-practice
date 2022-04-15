#include "address.h"

int main()
{
    Contact a;
    a.address = new Address("address", "NY", 10);
    Contact b(a);
    return 0;
}