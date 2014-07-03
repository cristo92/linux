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
	if(ret != 0) printf("syscall %d %s\n", ret, strerror(ret));
	assert(ret == 0);
	int fd = open("/root/file4", O_WRONLY | O_APPEND), i, err;
	if(fd < 0) printf("FAIL: open %d %s\n", errno, strerror(errno));
	assert(fd >= 0);
	for(i = 0; i < 1; i++) {
		err = write(fd, key1.key_id, 16);
		if(err != 16) printf("write error! %d\n", err);
		assert(err == 16);
	}
	close(fd);

	return 0;
}
