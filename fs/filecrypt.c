#include <linux/filecrypt.h>
#include <linux/thread_info.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/scatterlist.h>
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

int filecrypt_start_csession(struct inode *inode, void *key) {
	/* Create AES128 session */
		struct csession *ses_new;
		struct crypto_blkcipher *blk_tfm;
		struct blkcipher_alg *blkalg;
		char alg_name[] = "ctr(aes)";
		int err;

		blk_tfm = crypto_alloc_blkcipher(alg_name, 0, 0);
		if(IS_ERR(blk_tfm)) {
			printk(KERN_WARNING "%s: Failed to load transform from ctr(aes) %d\n",
					__func__, (int)blk_tfm);
			return -EINVAL;
		}

		blkalg = crypto_blkcipher_alg(blk_tfm);
		if(blkalg != NULL) {
			/* Was correct key length supplied? */
			if((CRYPT_BLOCK_SIZE < blkalg->min_keysize) ||
				(CRYPT_BLOCK_SIZE > blkalg->max_keysize)) {
				printk(KERN_WARNING "Wrong keylen %d for algorithm ctr(aes). Use %u to %u.\n",
					CRYPT_BLOCK_SIZE, blkalg->min_keysize, blkalg->max_keysize);
				crypto_free_blkcipher(blk_tfm);
				return -EINVAL;
			}
		}

		/* Set key id for AES128 */
		err = crypto_blkcipher_setkey(blk_tfm, key, CRYPT_BLOCK_SIZE);
		if(err) {
			printk(KERN_WARNING "Setting key failed for ctr(aes), %zu.\n", 
				CRYPT_BLOCK_SIZE * 8);
			crypto_free_blkcipher(blk_tfm);
			return -EINVAL;
		}

		/* Create a session and put it to the inode->i_private */
		ses_new = kmalloc(sizeof(*ses_new), GFP_KERNEL);
		if(!ses_new) {
			crypto_free_blkcipher(blk_tfm);
			return -ENOMEM;
		}

		memset(ses_new, 0, sizeof(*ses_new));
		ses_new->tfm = blk_tfm;
		err = ext4_xattr_get(inode, EXT4_XATTR_INDEX_USER, XATTR_IV, (void*)ses_new->iv,
				CRYPT_BLOCK_SIZE);
		if(err < 0)
			return err;
		mutex_init(&ses_new->sem);

		inode->i_private = ses_new;

		return 0;
}

int filecrypt_run_cipher(struct page *page, int c_op) {
	int i = 0, ret;
	char *data = page_address(page);
	struct inode *inode = page->mapping->host; 
	struct scatterlist sg;
	struct csession *ses_ptr = (struct csession*)inode->i_private;
	printk(KERN_WARNING "csession 0x%x data 0x%x\n", ses_ptr, data);
	struct crypto_blkcipher *tfm = ses_ptr->tfm;
	struct blkcipher_desc desc;
	desc.tfm = tfm;
	desc.flags = 0;

	down(&ses_ptr->sem);

	if(PAGE_CACHE_SIZE % crypto_blkcipher_blocksize(tfm)) {
		printk(KERN_WARNING "data size (%zu) isn't a multiple of block size (%u)\n",
				PAGE_CACHE_SIZE, crypto_blkcipher_blocksize(tfm));
		ret = -EINVAL;
		goto out;
	}

	crypto_blkcipher_set_iv(tfm, ses_ptr->iv, CRYPT_BLOCK_SIZE);
	sg_set_buf(&sg, data, PAGE_CACHE_SIZE);
	switch(c_op) {
		case FILECRYPT_ENCRYPT:
			ret = crypto_blkcipher_encrypt(&desc, &sg, &sg, PAGE_CACHE_SIZE);
			break;
		case FILECRYPT_DECRYPT:
			ret = crypto_blkcipher_decrypt(&desc, &sg, &sg, PAGE_CACHE_SIZE);
		if(unlikely(ret)) {
			printk(KERN_WARNING "crypto_blkcipher_encrypt/decrypt\n");
			goto out;
		}
	}

	if(unlikely(ret)) {
		printk(KERN_WARNING "CryptoApi failure: %d\n", ret);
		goto out;
	}

	switch(c_op) {
		case FILECRYPT_ENCRYPT:
			printk(KERN_WARNING "Encrypted: %s\n", data);
		case FILECRYPT_DECRYPT:
			printk(KERN_WARNING "Decrypted: %s\n", data);
	}

	switch(c_op) {
		case FILECRYPT_ENCRYPT:
			SetPageOwnerPriv1(page);	
			break;
		case FILECRYPT_DECRYPT:
			ClearPageOwnerPriv1(page);
	}

	ret = 0;

out:
	up(&ses_ptr->sem);
	return ret;
}

int filecrypt_decrypt(struct page *page) {
	return filecrypt_run_cipher(page, FILECRYPT_DECRYPT);
}
int filecrypt_encrypt(struct page *page) {
	return filecrypt_run_cipher(page, FILECRYPT_ENCRYPT);
}
