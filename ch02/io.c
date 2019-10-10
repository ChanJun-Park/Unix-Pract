#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define SIZE 512

int main(int argc, char* argv[])
{
    ssize_t nread;
    char buf[SIZE];
    int i;
    int filedes;

    if(argc == 1) {
	while((nread = read(0, buf, SIZE)) > 0) {
	    write(1, buf, nread);	
	}

	return 0;
    }
    else if(argc > 1) {
	for (i = 1; i < argc; i++) {
	    filedes = open(argv[i], O_RDONLY);
	    if (filedes == -1) {
		printf("error in opening %s\n", argv[i]);
		return 1;
	    }	    

	    while((nread = read(filedes, buf, SIZE)) > 0) {
		write(1, buf, nread);
	    }

	    close(filedes);
	}
    }

    return 0;
}

