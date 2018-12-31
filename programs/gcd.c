#include <stdio.h>
int main()
{
    int n1 = 100;
    int n2 = 20;

    while(n1!=n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    printf("GCD = %d",n1);

    return 0;
}
