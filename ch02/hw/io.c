#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define SIZE 512

int main(int argc, char* argv[])
{
    ssize_t nread;
    char buf[SIZE];
    int i;
    int filedes;

	// 명령줄 인수가 존재하지 않는다면, 콘솔에서 읽어오기
    if(argc == 1) {
		while((nread = read(0, buf, SIZE)) > 0) {
			write(1, buf, nread);
		}

		return 0;
    }
    else if(argc > 1) {	// 명령줄 인수가 존재한다면 파일에서 읽어오기
		for (i = 1; i < argc; i++) {
			filedes = open(argv[i], O_RDONLY);
			if (filedes == -1) {
				fprintf(stderr, "error in opening %s\n", argv[i]);
				return 1;
			}	    

			while((nread = read(filedes, buf, SIZE)) > 0) {
				write(1, buf, nread);
			}

			close(filedes);
		}
    }

    return 0;
}

