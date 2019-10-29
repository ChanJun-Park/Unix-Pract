#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char * filename;
    char cmd;
    if (argc < 2) {
        fprintf(stderr, "program usage : ./myrm filename\n");
        return 1;
    }

    filename = argv[1];
    if (access(filename, W_OK) == -1) {
        while(1) { // 쓰기 권한이 없는 경우 파일을 지울것인지 다시 한번 확인한다.
            printf("You don't have write permission to the file %s\n", filename);
            printf("Do you want to remove %s file? [y/n] : ", filename);
            scanf(" %c", &cmd);
            if(cmd == 'y' || cmd == 'Y') {
                break;
            }
            else if (cmd == 'n' || cmd == 'N') {
                printf("remove command canceled\n");
                return 0;
            }
            else {
                printf("invalid command\n");
                continue;
            }
        }
    }

    if (unlink(filename) == -1) {
        perror("error in unlink");
        return 1;
    }

    return 0;
}