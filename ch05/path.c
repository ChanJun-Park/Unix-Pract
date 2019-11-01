#include <stdlib.h>
#include <stdio.h>

int main()
{
    printf("PATH=%s\n", getenv("PATH"));
    return 0;
}