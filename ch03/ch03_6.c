#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char * filename;
    int r_flag = 0, w_flag = 0, x_flag = 0;

    if(argc < 2) {
        printf("program usage : ./whatable filename\n");
        return 1;
    }

    filename = argv[1];

    if (access(filename, R_OK) == -1) 
        perror("user cannot read file\n");
    else 
        printf("user can read %s\n", filename);
    
    if (access(filename, W_OK) == -1) 
        perror("user cannot write file\n");
    else 
        printf("user can write %s\n", filename);

    if (access(filename, X_OK) == -1) 
        perror("user cannot excute file\n");
    else 
        printf("user can execute %s\n", filename);

    return 0;
}