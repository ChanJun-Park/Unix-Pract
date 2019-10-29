#include <stdio.h>
#define PERMLEN 9

const char * permission = "rwxrwxrwx";

/*
    lsoct
    ls style 허가를 팔진수 허가로 변경한다.
    전역적으로 선언된 permission 문자열과 비교해서
    관련 부분이 동일하면 해당 자리 비트를 1로 셋팅한다.
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

/*
    octls
    팔진수 허가를 ls style 허가로 변경한다.
    화일 허가 모드의 각 비트를 확인하고 비트가 설정되어 있으면
    인자로 전달한 convted_mode 배열에 해당 자리의 permission 배열값을 
    저장한다.
*/
void octls(int mode, char * converted_mode)
{
    int i, j;
    for (i = 0; i < PERMLEN; i++) {
        j = PERMLEN - i - 1;
        if (mode & (1 << j)) {
            converted_mode[i] = permission[i];
        }
    }
}

int main()
{
    char * mode0 = "rwxr-xr-x";
    char mode[10] = {'\0',};
    printf("rwxr-xr-x mode's octal number is %o\n", lsoct(mode0));
    octls(0777, mode);
    printf("0777 is converted to %s\n", mode);
    return 0;
}