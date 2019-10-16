#include <sys/stat.h>
#include <ftw.h>
#include <stdio.h>

int list(const char * name, const struct stat * status, int type) 
{
    /* 만일 stat 호출이 실패하면, 그냥 복귀한다. */
    if (type == FTW_NS)
        return 0;
    /* 아니면 객체 이름, 허가 그리고 만일 객체가 디렉토리이거나 상징형 링크이면 "*"를 첨가한다 */
    if (type == FTW_F)
        printf("%-30s\t0%3o\n", name, status->st_mode & 0777);
    else
        printf("%-30s*\t0%3o\n", name, status->st_mode & 0777);
    
    return 0;
}

int main(int argc, char * argv[])
{
    int list(const char*, const struct stat *, int);

    if(argc == 1)
        ftw(".", list, 1);
    else 
        ftw(argv[1], list, 1);
    return 0;
}