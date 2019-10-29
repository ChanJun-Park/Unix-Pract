#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define NOT_OCTAL -255

/*
   r     w     x     s
u {0400, 0200, 0100, 04000},
g {040,  020,  010,  02000},
o {04,   02,   01,   0}
*/
mode_t permission_arr[3][4] = {
    {0400, 0200, 0100, 04000},
    {040,  020,  010,  02000},
    {04,   02,   01,   0}
};

int octal_to_decimal(char* number);
void symbol_mode(int argc, char** argv, int index);
void octal_mode(int argc, char** argv, int index);

int main(int argc, char ** argv)
{
    if (argv[1][0] == '0') 
        octal_mode(argc, argv, 1);
    else
        symbol_mode(argc, argv, 1);
    
    return 0;
}

/*
    octal_to_decimal
    명령어 창에서 문자열 형태로 입력받은 팔진수 모드를
    십진수 정수로 바꾸는 함수
*/
int octal_to_decimal(char* number)
{
    int i;
    int len = strlen(number);
    int result = 0;
    int oct = 0;
    for (i = 0; i<len; i++) {
        if (number[i] > '7' || number[i] < '0')
            return NOT_OCTAL;
    }
    oct = atoi(number);
    i = 0;
    while(oct != 0) {
        result = result + (oct % 10) * pow(8, i++);
        oct = oct / 10;
    }

    return result;
}

/*
    symbol_mode
    u+r와 같이 기호를 이용하여 파일 모드를 전환할때 사용하는 함수
*/
void symbol_mode(int argc, char** argv, int index)
{
    int i, j;
    struct stat statbuf;
    int user[3] = {0,};     // user(u), group(g), other(o) 기호가 포함되어 있는지 체크하기 위한 배열
    int mode[4] = {0, };    // u, g, o 에 대해서 read(r), write(w), execute(x), setuid(s) 설정이 되었는지 체크하기 위한 배열
    int len = strlen(argv[index]);
    char operator = 0;
    char* filename = argv[argc -1];
    mode_t chgperm = 0;

    if(stat(filename, &statbuf) == -1) {
        fprintf(stderr, "couldn't stat %s\n", filename);
        exit(3);
    }

    // u+w와 같이 하나의 타겟에 설정하는 경우도 있고
    // ug+w 와 같이 여러 타겟에 설정하는 경우도 있기 때문에
    // 이 명령이 어떤 타겟들에 영향을 주는것인지 체크한다.
    for (i = 0; i < len; i++) {
        if (argv[index][i] == 'u') {
            user[0] = 1;
        }
        else if (argv[index][i] == 'g') {
            user[1] = 1;
        }
        else if (argv[index][i] == 'o')  {
            user[2] = 1;
        }
        else if (argv[index][i] == 'a') {
            user[0] = user[1] = user[2] = 1;
        }
        else 
            break;
    }

    // 특정 허가 모드를 추가하는지, 제거하는지, 특정 허가모드로 설정하는지 체크한다.
    if (argv[index][i] != '+' && argv[index][i] != '-' && argv[index][i] != '=') {
        fprintf(stderr, "invalid mychmod operand\n");
        exit(1);
    }
    operator = argv[index][i];
    i++;

    // u+rwx 와 같이 여러 허가를 동시에 설정할 수 있기 때문에
    // 이러한 허가를 한번에 처리하기 위해서 루프를 돌며 mode 배열에 체크해둔다.
    for ( ; i < len; i++) {
        if (argv[index][i] == 'r') {
            mode[0] = 1;
        }
        else if (argv[index][i] == 'w') {
            mode[1] = 1;
        }
        else if (argv[index][i] == 'x') {
            mode[2] = 1;
        }
        else if (argv[index][i] == 's') {
            mode[3] = 1;
        }
        else { // argv[index][i] == ','
        }
    }

    for (i = 0; i <  3; i++) { // u, g, o
        for (j = 0; j < 4; j++) { // r, w, x, s
            if (user[i] && mode[j]) {
                chgperm |= permission_arr[i][j];
            }
        }
    }

    if (operator == '+') {
        statbuf.st_mode |= chgperm;
    }
    else if (operator == '-') {
        statbuf.st_mode &= ~chgperm;
    }
    else if (operator == '=') {
        statbuf.st_mode &= ~(04000);    // 설정되어 있는 setuid 해제
        statbuf.st_mode &= ~(02000);    // 설정되어 있는 setgid 해제

        // = 로 설정할 유저 정보외의 나머지 정보는 그대로 두기 위해서 
        // 아래 코드를 실행한다.
        if (user[0]) {
            statbuf.st_mode &= ~(0700);  // 000111111    
        }
        if (user[1]) {
            statbuf.st_mode &= ~(070); // 111000111
        }
        if (user[2]) {
            statbuf.st_mode &= ~(07);   // 111111000
        }
        statbuf.st_mode |= chgperm;
    }

    if (chmod(filename, statbuf.st_mode) == -1) {
        fprintf(stderr, "couldn't change mode for %s\n", filename);
        exit(2);
    }

    // 여러 명령이 나열되어 있다면(예를 들어 u+w, g+w, ...) 재귀적으로 나머지 명령을 처리한다.
    index++;
    if (index < argc - 1) {
        symbol_mode(argc, argv, index);
    }
}

/*
    octal_mode
    팔진수를 이용해서 파일 모드를 변경할때 사용하는 함수
*/
void octal_mode(int argc, char** argv, int index)
{
    mode_t mode = octal_to_decimal(argv[1]);
    if (mode == NOT_OCTAL) {
        fprintf(stderr, "couldn't change mode for %s\n", argv[2]);
        fprintf(stderr, "invail file mode\n");
        exit(3);
    }
    if (chmod(argv[2], mode) == -1) {
        fprintf(stderr, "couldn't change mode for %s\n", argv[2]);
        perror("chmod error\n");
        exit(4);
    }
}
