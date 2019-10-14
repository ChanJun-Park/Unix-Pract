#include <stdio.h>

int main()
{
    char str[6];
    fgets(str, 6, stdin);

    printf("%s\n", str);
    return 0;
}