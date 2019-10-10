#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    int i, res;
    
    for (i = 1; i <argc; ++i)
    {
	res = rmdir(argv[i]);
	if(res)
	    printf("rmdir for %s failed. \n", argv[i]);
    }
    return 0;
}
