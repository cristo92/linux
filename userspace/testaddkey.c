#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <stdlib.h>
#include "filecrypt.h"

unsigned char a1[16] = "0123456789ABCDEF";
unsigned char a2[16] = "roszpuntynkatra\0";
unsigned char a3[16] = "child_processss\0";
struct ext4_ioctl_encrypt key1 = {.key_id = "1234567890ABCDEF"};
struct ext4_ioctl_encrypt key2 = {.key_id = "qwerasdfzxcvtyui"};
struct ext4_ioctl_encrypt key3 = {.key_id = "1122334455667788"};

int main() {
	int pid, fd, t;
	printf("%d\n", syscall(351, a1));
	printf("%d\n", syscall(351, a2));
	printf("%u %u\n", EXT4_ENCRYPT, (int) EXT4_ENCRYPT);
	fd = open("test_ioctl", O_CREAT | O_WRONLY, S_IRWXU );
	if(fd < 0) printf("open sie rypl\n");
	ioctl(fd, EXT4_ENCRYPT, &key1);

	pid = fork();

	if(pid >= 0) {
		printf("fork udany\n");
		if(pid == 0) printf("%d\n", syscall(351, a3));
		else {
		}
	}
	else {
		printf("zjeabels forka\n");
	}

	return 0;
}
