#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main()
{
    int fd = open("/dev/arduino", O_WRONLY);
    if (fd < 0) {
    	printf("Error: cannot open file.\n");
	return -1;
    }
    char *msg = "Arduino i2c char device test\n";
    write(fd, msg, strlen(msg));
}