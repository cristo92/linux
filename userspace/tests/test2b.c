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
	int fd = open("/root/file3", O_WRONLY, S_IRWXU);
	printf("errno %d %s\n", errno, strerror(errno));
	if(fd >= 0) close(fd);
	assert(errno == EPERM);

	return 0;
}
