#ifndef FILECRYPT_H
#define FILECRYPT_H

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/fs.h>
#else
#include <stdint.h>
#endif

#include <linux/ioctl.h>

#define CRYPT_BLOCK_SIZE 16

struct ext4_ioctl_encrypt {
	uint8_t key_id[CRYPT_BLOCK_SIZE];
};
#define EXT4_ENCRYPT _IOR('e', 0xcf, struct ext4_ioctl_encrypt)

#define __NR_addkey 351

#define XATTR_NAME "AES_KEY"

#ifdef __KERNEL__
struct key_entry {
	unsigned char id[CRYPT_BLOCK_SIZE], key[CRYPT_BLOCK_SIZE];
	struct list_head list;
};

int filecrypt_has_perms(struct ext4_ioctl_encrypt *key);
int filecrypt_bin2hex(char* from, void* to, size_t size);
int filecrypt_is_encrypted(struct inode *inode);
void filecrypt_encrypt(char *page);
#endif

#endif
