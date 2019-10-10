#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#if _FILE_OFFSET_BITS == 64
#define fmt_off "%lld"
#else
#define fmt_off "%ld"
#endif

off_t filesize(int filedes) {
    
    off_t fs;
    off_t prev_fileptr;

    prev_fileptr = lseek(filedes, (off_t)0, SEEK_CUR);
    fs = lseek(filedes, (off_t)0, SEEK_END);

    if(fs == -1) {
        printf("error in filesize function\n");
        lseek(filedes, prev_fileptr, SEEK_SET);
        return -1;
    }

    lseek(filedes, prev_fileptr, SEEK_SET);
    return fs;
}

int main() {

    int filedes;
    filedes = open("count_test.txt", O_RDONLY);

    off_t value = filesize(filedes);
    printf("filesize = "fmt_off"\n", value); 

    close(filedes);
    return 0;
}
