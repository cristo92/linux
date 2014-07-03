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
	int fd = creat("/root/file4", S_IRWXU), i, err;
	if(fd < 0) printf("FAIL: creat\n");
	int ret = syscall(351, key1.key_id);
	if(ret != 0) printf("syscall %d %s\n", ret, strerror(ret));
	assert(ret == 0);
	ret = ioctl(fd, EXT4_ENCRYPT, &key2);
	if(ret != 0) printf("ioctl errno %d %s\n", errno, strerror(errno));
	for(i = 0; i < 1; i++) {
		err = write(fd, key1.key_id, 16);
		if(err != 16) printf("write error! %d\n", err);
	}
	close(fd);
	assert(ret == 0);

	return 0;
}
