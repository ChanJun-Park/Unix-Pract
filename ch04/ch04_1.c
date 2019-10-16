#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int my_double_ls(const char *name, int skip)
{
    struct dirent *d;
    struct stat st;
    DIR * dp;

    /* 디렉토리를 개방하고, 실패여부를 점검함 */
    if ((dp = opendir(name)) == NULL)
        return -1;
    
    /* 디렉토리를 살피면서 루프를 계속한다. 이때 inode 번호가 유효하면 디렉토리항을 프린트한다. */
    while(d = readdir(dp)) {
        if (d->d_ino != 0) {
            if (!skip || d->d_name[0] != '.') {
                printf("%s ", d->d_name);
                if(stat(d->d_name, &st) == -1) {
                    perror("Can't not stat ");
                    return -1;
                }
                if (S_ISDIR(st.st_mode))
                    printf("*");
                printf("\n");
            }
        }
    }

    /* 이제 디렉토리의 시작으로 되돌아간다 */
    rewinddir(dp);

    /* 그리고 디렉토리를 다시 프린트한다. */
    while(d = readdir(dp)) {
        if (d->d_ino != 0) {
            if (!skip || d->d_name[0] != '.') {
                printf("%s ", d->d_name);
                if(stat(d->d_name, &st) == -1) {
                    perror("Can't not stat ");
                    return -1;
                }
                if (S_ISDIR(st.st_mode))
                    printf("*");
                printf("\n");                
            }
        }
    }

    closedir(dp);
    return 0;
}

int main() 
{
    my_double_ls(".", 1);
    return 0;
}