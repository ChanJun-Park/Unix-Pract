/*
    ch03_10.c
    실행파일 이름 : slowwatch
    명령줄 인자로 넘겨준 이름에 해당하는 파일의 수정 정보를 추적하는 프로그램
    10초를 주기로 파일의 수정 정보를 추적한다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

// 파일이 마지막으로 수정된 시간을 저장하는 변수
time_t last_mtime = 0;

int main(int argc, char * argv[])
{
    char * filename;
    int filedes;
    struct stat filestat;
    time_t tmptime;

    char buf[BUFSIZ];
    int nread;

    if (argc < 2) {
        fprintf(stderr, "program usage : ./slowwatch filename\n");
        return 1;
    }

    filename = argv[1];

    // 터미널에서 kill 명령을 통해 종료하지 않는 경우, 
    // 백그라운드에서 계속 실행되는 프로그램이다.
    while(1) {
        // 10초를 주기로 파일의 상태를 확인한다.
        sleep(10);
        if ((filedes = open(filename, O_RDONLY)) == -1) {
            if (errno == ENOENT) {
                fprintf(stderr, "there is no file named %s\n", filename);
                last_mtime = -1;
            }
            perror("error in reading file\n");
            continue;
        }
        // 파일이 존재하지 않는 경우 이 위 부분만 반복적으로 실행된다.

        if (fstat(filedes, &filestat) == -1) {
            perror("Couldn't stat file");
            close(filedes);
        }
        else {
            tmptime = filestat.st_mtime;

            // 파일이 처음 생성되는 경우
            if (last_mtime == -1) {
                printf("%s was created at %s\n", filename, asctime(gmtime(&tmptime)));
                last_mtime = tmptime;
            }
            else if (last_mtime != tmptime) { // 파일이 갱신되는 경우

                // 모든 갱신이 이루어질 때까지 기다리기 위해서
                // 10초 간격으로 파일의 상태를 다시 체크한다.
                // 10초 이후에 파일 갱신이 없으면 모든 갱신이 완료되었다고 가정하고
                // 갱신시간을 출력한다.
                while(last_mtime != tmptime) {
                    last_mtime = tmptime;
                    sleep(10);
                    fstat(filedes, &filestat);
                    tmptime = filestat.st_mtime;
                }
                printf("%s was modified at %s\n", filename, asctime(gmtime(&tmptime)));
                last_mtime = tmptime;

                printf("-----------------------%s content------------------------\n", filename);
                while((nread = read(filedes, buf, BUFSIZ)) > 0) {
                    write(1, buf, nread);
                }
                printf("\n---------------------------------------------------------\n");
            }
            close(filedes);
        }
    }

    return 0;
}