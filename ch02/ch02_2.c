#include <stdlib.h>
#include <fcntl.h>

#define PERMS 0644

char *filename = "newfile";

int main() {
    int filedes;

    if ((filedes = open(filename, O_RDWR | O_CREAT, PERMS) == -1)) {
        printf("Couldn't open file %s\n", filename);
	exit(1);
    }

    return 0;
}
