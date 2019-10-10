#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    
    if (argc != 3) {
	perror("Usage : ./mv <old> <new>\n");
	return -1;
    }
    if (link(argv[1], argv[2])) {
	printf("Link failed. : %s -> %s \n", argv[1], argv[2]);
	return -2;
    }    
    /*if (unlink(argv[1])) {
	printf("Unlink failed. : %s\n", argv[1]);
	return -3;
    }*/
    printf("File moved : %s -> %s\n", argv[1], argv[2]);
    return 0;
}
