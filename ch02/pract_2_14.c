#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 100

int fileopen(const char * pathname, const char * option)
{
    int filedes;

    if(strcmp(option, "r") == 0) {
	filedes = open(pathname, O_RDONLY);
    }
    else if (strcmp(option, "w") == 0) {
	filedes = open(pathname, O_WRONLY);	
    }
    else if (strcmp(option, "rw") == 0) {
	filedes = open(pathname, O_RDWR);
    }
    else if (strcmp(option, "a") == 0) {
	filedes = open(pathname, O_WRONLY | O_APPEND);
    }
      
    return filedes;    
}

int main(int argc, char* argv[])
{
    int filedes;
    char buffer[BUF_SIZE];
    int len;    

    filedes = fileopen(argv[1], "r");
    len = read(filedes, buffer, BUF_SIZE);
    if(len < 0) {
	printf("error in reading %s\n", argv[1]);
	return -1;
    }
    
    printf("%s's contenst is %s\n",argv[1], buffer);

    return 0;
}
