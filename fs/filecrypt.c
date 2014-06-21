#include <linux/filecrypt.h>
#include <linux/thread_info.h>
#include <linux/list.h>
#include <linux/sched.h>

int filecrypt_has_perms(struct ext4_ioctl_encrypt *key) {
	struct key_entry *entry;
	int ret = 0;

	list_for_each_entry(entry, &current->keys, list) {
		if (memcpy(entry->id, key->key_id, CRYPT_BLOCK_SIZE) == 0) {
			ret = 1;
			goto out;
		}
	}

out:
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
