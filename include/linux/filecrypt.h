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
#define XATTR_IV "IV"

#ifdef __KERNEL__
struct key_entry {
	unsigned char id[CRYPT_BLOCK_SIZE], key[CRYPT_BLOCK_SIZE];
	struct list_head list;
};

#define FILECRYPT_ENCRYPT 1
#define FILECRYPT_DECRYPT 2

int filecrypt_has_perms(void *key_id);
int filecrypt_get_key(void *key, void *id);
int filecrypt_bin2hex(char* dst, void* src, size_t size);
int filecrypt_is_encrypted(struct inode *inode);
int filecrypt_start_csession(struct inode *inode, void *key);
int filecrypt_encrypt(struct page *page);
int filecrypt_decrypt(struct page *page);
void filecrypt_finish_csession(struct inode *inode);

struct csession {
	struct semaphore sem;
	struct crypto_blkcipher *tfm;
	char iv[CRYPT_BLOCK_SIZE];
};

#endif

#endif
