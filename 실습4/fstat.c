#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    struct stat buf;
    stat(argv[0], &buf);

    printf("Mode = %d\n", (unsigned int)buf.st_mode);
    printf("UID = %d\n", (int)buf.st_uid);
    printf("Size = %d\n", (int)buf.st_size);
    printf("Modification Time = %d\n", (int)buf.st_mtime);
    return 0;
}
