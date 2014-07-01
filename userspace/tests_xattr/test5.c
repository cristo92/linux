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
struct ext4_ioctl_encrypt key2 = { .key_id = {0x3a, 0x6b, 0xff, 0x07, 0x99, 0xc7, 0x38, 0x9f, 
	0x52, 0x2f, 0x38, 0x47, 0xc3, 0x3a, 0x46, 0x8f} };

int main() {
	int ret = syscall(351, key1.key_id);
	assert(ret == 0);
	int fd = open("/root/file3", O_WRONLY);
	if(fd < 0) printf("FAIL: open\n");
	printf("errno %d %s\n", errno, strerror(errno));
	close(fd);
	assert(fd >= 0);

	return 0;
}
