#include <stdio.h>

int addUp(int n);

int main()
{
    int n;
    scanf("%d", &n);
    int sum = addUp(n);
    printf("sum of 1 to n: %d\n", sum);
}

int addUp(int n)
{
    int sum = 0;
    for(int i = 1; i <= n; i++)
        sum += i;
    return sum;
}