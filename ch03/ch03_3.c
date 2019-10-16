#include <stdio.h>
#define PERMLEN 9

const char * permission = "rwxrwxrwx";

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

void octls(int mode, char * convted_mode)
{
    int i, j;
    for (i = 0; i < PERMLEN; i++) {
        j = PERMLEN - i - 1;
        if (mode & (1 << j)) {
            convted_mode[i] = permission[i];
        }
    }
}

int main()
{
    char * mode0 = "rwxr-wr-w";
    char mode[10] = {'\0',};
    printf("rwxr-xr-x mode's octal number is %o\n", lsoct(mode0));
    octls(0777, mode);
    printf("0777 is converted to %s\n", mode);
    return 0;
}