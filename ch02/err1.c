#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd;
    
    if ( (fd = open("nonesuch", O_RDONLY)) == -1)
	perror("error in opening nonesuch");
    return 0;
}
