#include <unistd.h>
#include <fcntl.h>

int main()
{
    int bytes;
    char buffer[256];
    do
    {
	bytes = read(0, buffer, 255);
	if(bytes <= 0) break;
	write(1, buffer, bytes);   
    } while(1);
    return 0;
}
