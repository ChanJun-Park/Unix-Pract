#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// whitespace and punctuation mark array
char ws_em[7] = {' ', '\t', '\n', ',', '.', '!', '?'};

// check whether argument character is whitespace and punctuation mark or not
int check_ws_em(char c) {
    int len = sizeof(ws_em);
    for(int i = 0; i < len; i++) {
        if(c == ws_em[i]) return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    
    char buffer[BUFSIZ];
    ssize_t nread;
    long total = 0;

    long word_cnt = 0;
    long line_cnt = 0;

    // this flag indicates that some normal characters(not white space)
    // appear continuously in file stream
    int word_flag = 0;
    int ws_em_size = sizeof(ws_em);

    if (argc <= 1) {
        printf("program 사용법 : count filename\n");
        return 1;
    }

    int filedes = open(argv[1], O_RDONLY);
    if(filedes == -1) {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }
    
    while((nread = read(filedes, buffer, BUFSIZ)) > 0) {
        total += nread;

        for (int i = 0; i < nread; i++) {
            if(check_ws_em(buffer[i])) {

                if(buffer[i] == '\n') line_cnt++;
                if(word_flag) word_cnt++;

                word_flag = 0;       
            }
            else {
                word_flag = 1;
            }
        }
    }

    if(word_flag) word_cnt++;

    printf("total character count : %ld\n", total);
    printf("total word count : %ld\n", word_cnt);
    printf("total line count : %ld\n", line_cnt);
    return 0;
}
