/* 
 *  Krzysztof Leszczyński 319367
 *  Syscall for adding cipher key to proccess,
 *  used in encrypting disc files.
 */

#include <linux/linkage.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/crypto.h>
#include <linux/err.h>
#include <linux/filecrypt.h>
#include <linux/scatterlist.h>
#include <crypto/hash.h>
#include <crypto/md5.h>
#include <asm/thread_info.h>

asmlinkage int sys_addkey(unsigned char *key) {
	struct key_entry *entry; //.key .id
	struct task_struct *task;
	struct hash_desc desc;
	struct scatterlist sg;
	unsigned char hash_buf[CRYPT_BLOCK_SIZE];
	unsigned char hexbuf[(CRYPT_BLOCK_SIZE << 1) + 1];
	int err;

	entry = kmalloc(sizeof(struct key_entry), GFP_KERNEL);
	if(entry == NULL) {
		printk(KERN_WARNING " sys_addkey kmalloc\n");
		goto error1;
	}
	err = copy_from_user(entry->key, key, CRYPT_BLOCK_SIZE);
	if(err != 0) {
		printk(KERN_WARNING " sys_addkey copy_from_user\n");
		goto error2;
	}

	memcpy((void*)hash_buf, entry->key, CRYPT_BLOCK_SIZE); 
	desc.tfm = crypto_alloc_hash("md5", 0, 0);
	desc.flags = 0;
	sg_init_one(&sg, hash_buf, CRYPT_BLOCK_SIZE);
	crypto_hash_init(&desc);
	crypto_hash_update(&desc, &sg, CRYPT_BLOCK_SIZE);
	crypto_hash_final(&desc, entry->id);

	crypto_free_hash(desc.tfm);

	task = current_thread_info()->task;
	list_add(&(entry->list), &task->keys);
	if((err = filecrypt_bin2hex(hexbuf, entry->id, CRYPT_BLOCK_SIZE)) < 0) {
		printk(KERN_WARNING "%s: filecrypt_bin2hex\n", __func__);
		return err;
	}
	hexbuf[CRYPT_BLOCK_SIZE << 1] = '\n';
	printk(KERN_WARNING "addkey %s %s\n", entry->key, hexbuf);
	return 0;
error2:
	kfree(entry);
error1:
	return -1;
}
