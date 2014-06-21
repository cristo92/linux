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
#include <crypto/hash.h>
#include <crypto/md5.h>
#include <asm/thread_info.h>

asmlinkage int sys_addkey(unsigned char *key) {
	struct key_entry *entry;
	struct task_struct *task;
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
	memcpy(entry->id, entry->key, CRYPT_BLOCK_SIZE);

	/*tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if(IS_ERR(tfm)) goto error2;

	desc.tfm = tfm;
	desc.flags = 0;
	err = crypto_shash_update(&desc, kkey, 16);
	err = crypto_shash_final(&desc, kid);*/

	task = current_thread_info()->task;
	list_add(&(entry->list), &task->keys);
	return 0;
error2:
error1:
	return -1;
}
