#include <stdlib.h>
#include <fcntl.h>

char * workfile = "junk";

int main() {
  int filedes;
  
  if ((filedes = open(workfile, O_RDWR) == -1)) {
    printf("Couldn't open %s\n", workfile);
    exit(1);
  }

  exit(0);
}
