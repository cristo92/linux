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
#include <crypto/hash.h>
#include <crypto/md5.h>

struct entry {
	unsigned char * id, * key;
	struct list_head list;
};
LIST_HEAD(dict_keys);

asmlinkage int sys_addkey(unsigned char *key) {
	unsigned char *kkey, *kid;
	struct entry *entry;
	struct list_head *it;
	int err;

	struct crypto_hash *tfm;
	struct hash_desc desc;

	kkey = kmalloc(16, GFP_KERNEL);
	if(kkey == NULL) {
		printk(KERN_WARNING " sys_addkey kmalloc\n");
		goto error1;
	}
	err = copy_from_user(kkey, key, 16);
	if(err != 0) {
		printk(KERN_WARNING " sys_addkey copy_from_user\n");
		goto error2;
	}
	kid = kmalloc(16, GFP_KERNEL);
	if(kid == NULL) {
		printk(KERN_WARNING " sys_addkey kmalloc\n");
		goto error2;
	}

	tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if(IS_ERR(tfm)) goto error2;

	desc.tfm = tfm;
	desc.flags = 0;
	err = crypto_shash_update(&desc, kkey, 16);
	crypto_shash(&desc, kid);

	entry = kmalloc(sizeof(struct entry), GFP_KERNEL);
	if(entry == NULL) {
		printk(KERN_WARNING " sys_addkey kmalloc\n");
		goto error3;
	}
	entry->id = kid;
	entry->key = kkey;
	list_add(&(entry->list), &dict_keys);
	printk(KERN_WARNING "addkey\n");
	list_for_each(it, &dict_keys) {
		printk("%s\n", list_entry(it, struct entry, list)->key);
	}
	return 163;
error3:
	kfree(kid);
error2:
	kfree(kkey);
error1:
	return -1;
}
