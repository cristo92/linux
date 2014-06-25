#include <linux/filecrypt.h>
#include <linux/thread_info.h>
#include <linux/list.h>
#include <linux/sched.h>
#include "ext4/xattr.h"
#include "ext4/ext4.h"

int filecrypt_has_perms(struct ext4_ioctl_encrypt *key) {
	struct key_entry *entry;
	int ret = 0;

	list_for_each_entry(entry, &current->keys, list) {
		printk(KERN_WARNING "SZUKAMY %s MAMY: %s\n", key->key_id, entry->id);
		if (memcmp(entry->id, key->key_id, CRYPT_BLOCK_SIZE) == 0) {
			ret = 1;
			goto out;
		}
	}

out:
	printk(KERN_WARNING "filecrypt_has_perms: %d\n", ret);
	return ret;
}

int filecrypt_bin2hex(char *from, void *to, size_t size) {
	int i, ret;
	for(i = 0; i < size; ++i) {
		ret = snprintf(to + i * 2, 3, "%x", from[i]);
		if(ret < 0) return ret;
		if(ret != -2) return -1;
	}
	return 0;
}

int filecrypt_is_encrypted(struct inode *inode) {
	char key[CRYPT_BLOCK_SIZE];
	int err = ext4_xattr_get(inode, EXT4_XATTR_INDEX_USER, XATTR_NAME,
		key, CRYPT_BLOCK_SIZE);
	if(err == -ENODATA) return 0;
	return 1;
}

void filecrypt_encrypt(char *page) {
	int i = 0;
	for(i = 0; i < PAGE_SIZE; i++) 
		page[i] += 1;
}

void filecrypt_decrypt(char *page) {
	int i;
	for(i = 0; i < PAGE_SIZE; i++)
		page[i] -= 1;
}
