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
    int user[3] = {0,};
    int mode[4] = {0, };
    int len = strlen(argv[1]);
    char operator = 0;
    mode_t chgperm = 0;

    if(stat(argv[2], &statbuf) == -1) {
        fprintf(stderr, "couldn't stat %s\n", argv[2]);
        exit(3);
    }

    for (i = 0; i < len; i++) {
        if (argv[1][i] == 'u') {
            user[0] = 1;
        }
        else if (argv[1][i] == 'g') {
            user[1] = 1;
        }
        else if (argv[1][i] == 'o')  {
            user[2] = 1;
        }
        else if (argv[1][i] == 'a') {
            user[0] = user[1] = user[2] = 1;
        }
        else 
            break;
    }

    if (argv[1][i] != '+' && argv[1][i] != '-' && argv[1][i] != '=') {
        fprintf(stderr, "invalid chmod operand\n");
        exit(1);
    }
    operator = argv[1][i];
    i++;

    for ( ; i < len; i++) {
        if (argv[1][i] == 'r') {
            mode[0] = 1;
        }
        else if (argv[1][i] == 'w') {
            mode[1] = 1;
        }
        else if (argv[1][i] == 'x') {
            mode[2] = 1;
        }
        else if (argv[1][i] == 's') {
            mode[3] = 1;
        }
    }

    for (i = 0; i <  3; i++) {
        for (j = 0; j < 4; j++) {
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
        statbuf.st_mode = chgperm;
    }

    if (chmod(argv[2], statbuf.st_mode) == -1) {
        fprintf(stderr, "couldn't change mode for %s\n", argv[2]);
        exit(2);
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
