#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

char ws_em[7] = {' ', '\t', '\n', ',', '.', '!', '?'};

int check_ws_em(char c) {
    int len = 7;
    for(int i = 0; i < len; i++) {
        if(c == ws_em[i]) return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    
    if (argc <= 1) {
        printf("program 사용법 : count \"filename\"\n");
        return 1;
    }

    char * target = argv[1];
    int filedes = open(target, O_RDONLY);
    if(filedes == -1) {
        printf("Cannot open %s\n", target);
        return 1;
    }

    char buffer[BUFSIZ];
    ssize_t nread;
    long total = 0;

    long word_cnt = 0;
    long line_cnt = 0;

    int word_flag = 0;
    int ws_em_size = sizeof(ws_em);

    while( (nread = read(filedes, buffer, BUFSIZ)) >0) {
        total += nread;
        printf("test\n");

        for (int i = 0; i < nread; i++) {
            if(check_ws_em(buffer[i])) {
                if(buffer[i] == '\n') {
                    printf("%c\n", buffer[i]);
                    line_cnt++;
                }
                if(word_flag == 1) word_cnt++;
                word_flag = 0;       
            }
            else {
                word_flag = 1;
            }
        }
    }

    if(word_flag) word_cnt++;

    printf("total chars in anotherfile: %ld\n", total);
    printf("total word count : %ld\n", word_cnt);
    printf("total line count : %ld\n", line_cnt);
    return 0;
}
