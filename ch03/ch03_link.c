#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    link(argv[1], "newfile.txt");
    return 0;
}