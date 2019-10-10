#include <stdio.h>

int main(int argc, char* argv[]) {
    
    int count;
    printf("argc: %d\n", argc);
    printf("---------- arguments list -------------\n");
    for (count = 0; count < argc; count++) {
	printf("argv[%d] : %s\n", count, argv[count]);
    }

    return 0;
}
