#include <stdio.h>

int quadral(int x)
{
    int result;
    result = x*4;
    return result;
}

int main()
{
    int num = 5;
    int value;
    value = quadral(num);

    return 0;
}