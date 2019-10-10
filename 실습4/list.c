#include <dirent.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    int res, depth = 0;
    DIR *dirp;
    struct dirent* entry;
    if(argc != 2) {
	perror("Usage : list <path> \n");
	return -1;
    }
    dirp = opendir(argv[1]);
    if(dirp == NULL)
	perror("Directory cannot be opened.\n");
    while ((entry = readdir(dirp)) != NULL)
	printf("%10d : %s\n", entry->d_ino, entry->d_name);
    closedir(dirp);
    return 0;
}
