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
struct ext4_ioctl_encrypt key2 = { .key_id = "1234567890abcdef" };

int main() {
	int fd = creat("/root/file3", S_IRWXU);
	if(fd < 0) printf("FAIL: creat\n");
	int ret = syscall(351, key1.key_id);
	assert(ret == 0);
	ret = ioctl(fd, EXT4_ENCRYPT, &key1);
	if(errno != EPERM) printf("errno %d %s\n", errno, strerror(errno));
	close(fd);
	assert(errno == EPERM);

	return 0;
}
