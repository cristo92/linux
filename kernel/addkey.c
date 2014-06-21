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
#include <linux/addkey.h>
#include <crypto/hash.h>
#include <crypto/md5.h>
#include <asm/thread_info.h>

asmlinkage int sys_addkey(unsigned char *key) {
	unsigned char *kkey, *kid;
	struct key_entry *entry;
	struct list_head *it;
	struct task_struct *task;
	int err;

	struct crypto_hash *tfm;
	struct hash_desc desc;

	entry = kmalloc(sizeof(struct key_entry), GFP_KERNEL);
	if(entry == NULL) {
		printk(KERN_WARNING " sys_addkey kmalloc\n");
		goto error1;
	}
	err = copy_from_user(entry->key, key, 16);
	if(err != 0) {
		printk(KERN_WARNING " sys_addkey copy_from_user\n");
		goto error2;
	}

	/*tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if(IS_ERR(tfm)) goto error2;

	desc.tfm = tfm;
	desc.flags = 0;
	err = crypto_shash_update(&desc, kkey, 16);
	err = crypto_shash_final(&desc, kid);*/

	task = current_thread_info()->task;
	list_add(&(entry->list), &task->keys);
	printk(KERN_WARNING "addkey\n");
	list_for_each(it, &task->keys) {
		printk("%s %s\n", list_entry(it, struct key_entry, list)->key, 
						list_entry(it, struct key_entry, list)->id);
	}
	return 163;
error3:
	kfree(kid);
error2:
	kfree(kkey);
error1:
	return -1;
}
