#include <stdio.h>
#include <stdlib.h>

int main()
{
    for (int i=0; i < 10; i++)
    {
        int *a = (int*)malloc(1);
        printf("the address is %x\n", a);
        free(a);
    }
    return 0;
}