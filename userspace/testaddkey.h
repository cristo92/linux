#ifndef _LINUX_ADDKEY_H
#define _LINUX_ADDKEY_H

#include <linux/unistd.h>
#define __NR_addkey 274
_syscall1(long, addkey, unsigned char*, key);

#endif

