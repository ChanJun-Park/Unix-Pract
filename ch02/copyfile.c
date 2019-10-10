#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE    512

int copyfile2(int fd_src, int fd_tar) {
    
    char buffer[BUF_SIZE];
    ssize_t read_cnt = 0, write_cnt = 0;

    while( (read_cnt = read(fd_src, buffer, BUF_SIZE)) > 0) {
        
        write_cnt = write(fd_tar, buffer, read_cnt);
        if (write_cnt < read_cnt) {
            printf("error in writing %d\n", fd_tar);
            close(fd_src);
            close(fd_tar);
            return -3;
        }
    }

    if(read_cnt == -4) {
        printf("error in reading %d\n", fd_src);
        close(fd_src);
        close(fd_tar);
        return -1;
    }

    close(fd_src);
    close(fd_tar);

    return 0;

}

int copyfile(const char * src, const char * tar) {

    int fd_src, fd_tar;
    char buffer[BUF_SIZE];
    ssize_t read_cnt = 0, write_cnt = 0;

    if( (fd_src = open(src, O_RDONLY, 0644)) == -1) {
        printf("error in opening %s\n", src);
        return -1;
    }

    if( (fd_tar = open(tar, O_WRONLY | O_CREAT, 0644)) == -1) {
        printf("error in opening %s\n", tar);
        close(fd_src);
        return -2;
    }

    while( (read_cnt = read(fd_src, buffer, BUF_SIZE)) > 0) {
        
        write_cnt = write(fd_tar, buffer, read_cnt);
        if (write_cnt < read_cnt) {
            printf("error in writing %s\n", tar);
            close(fd_src);
            close(fd_tar);
            return -3;
        }
    }

    if(read_cnt == -4) {
        printf("error in reading %s\n", src);
        close(fd_src);
        close(fd_tar);
        return -1;
    }

    close(fd_src);
    close(fd_tar);

    return 0;
}

int main(int argc, char * argv[]) {

    if (argc != 3) {
        printf("program usage : copyfile \"filename1\" \"filename2\"\n");
        return -1;
    }

    if (copyfile(argv[1], argv[2]) == 0) {
        printf("success!\n");
    }
    else {
        printf("fail!\n");
    }

    return 0;
}
