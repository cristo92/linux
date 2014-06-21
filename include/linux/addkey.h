#ifndef _LINUX_ADDKEY_H
#define _LINUX_ADDKEY_H

struct key_entry {
	unsigned char id[16], key[16];
	struct list_head list;
};

#endif
