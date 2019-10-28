#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#define PERMLEN 9
#define NOT_OCTAL -255

const char * permission = "rwxrwxrwx";

int lsoct(const char * mode);
int octal_to_decimal(char* number);

int main(int argc, char* argv[])
{
    int mode = 0;
    if (argc < 3) {
        fprintf(stderr, "program usage : ./setperm filename (octal_permission | ls_style_permission)\n");
        return 1;
    }

    if (argv[2][0] != '0') 
        mode = lsoct(argv[2]);
    else 
        mode = octal_to_decimal(argv[2]);

    if (chmod(argv[1], mode) == -1) {
        fprintf(stderr, "couldn't change mode %s\n", argv[1]);
        perror("chmode error");
        return 2;
    }
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
    lsoct
    ls 스타일로 되어있는 permission을 정수로 바꾼다
*/
int lsoct(const char * mode)
{
    int oct_mode = 0;
    int i, j;
    for (i = 0; i < PERMLEN; i++) {
        j = PERMLEN - i - 1;
        if (mode[i] == permission[i]) {
            oct_mode += (1 << j);
        }
    }
    return oct_mode;
}