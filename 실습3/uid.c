#include <stdio.h>
#include <unistd.h>

int main() {
	printf("UID : %d, EUID : %d\n", getuid(), geteuid());
	printf("GID : %d, EGID : %d\n", getgid(), getegid());
	return 0;
}
