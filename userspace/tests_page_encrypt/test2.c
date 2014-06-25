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

int main() {
	int ret = syscall(351, key1.key_id);
	printf("syscall %d %s\n", ret, strerror(ret));
	assert(ret == 0);
	int fd = open("/root/file3", O_WRONLY);
	if(fd < 0) printf("FAIL: creat\n");

	char buf[20];
	ret = read(fd, buf, 14);
	buf[14] = '\0';
	printf("%s\n", buf);
	assert(ret == 14);
	close(fd);

	return 0;
}
