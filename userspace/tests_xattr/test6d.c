#include "../filecrypt.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/errno.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

struct ext4_ioctl_encrypt key1 = { .key_id = "1234567890ABCDEF" };

char buf[1000];

int main() {
	int ret = syscall(351, key1.key_id), err = 1, err2, i = 0;
	assert(ret == 0);
	int fd = open("/root/file4", O_RDWR);
	if(fd < 0) printf("file descriptor: %d\n", fd);
	assert(fd >= 0);
	//printf("errno %d %s\n", errno, strerror(errno));
	for(i = 0; i < 2 && err; i++) {
		err = read(fd, buf, 16);
		buf[err]= '\n';
		assert(err == 16);
		assert(strncmp(buf, key1.key_id, 16) == 0);
		//printf("read errno %d %s", errno, strerror(errno));
		err2 = write(1, buf, err + 1);
		if(err2 == -1) printf("write errno %d %s", errno, strerror(errno));
		assert(err2 == err + 1);
	}
	close(fd);

	return 0;
}
