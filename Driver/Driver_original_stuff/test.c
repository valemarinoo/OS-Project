#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "lmmodule.h"


int main() {

	int fd = open ("/dev/Crypto_Core",O_RDWR);
	if (fd < 0) {
		perror ("Unable to open device");
	}

	struct CC_data data;
	strcpy(data.input, "ciao mamma");
	data.len = strlen(data.input);

	r = ioctl(fd, ENCRYPT, &data);

	printf("%d encrypting %s : the result is %s\n", r, data.input, data.output);
	
	strcpy(data.input, data.output);

	r = ioctl(fd, DECRYPT, &data);

	printf("%d decrypting %s : the result is %s\n", r, data.input, data.output);
	
	close(fd);

}



**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**