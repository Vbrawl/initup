#ifndef INITUP_MOUNTENTRY
#define INITUP_MOUNTENTRY

#include <mntent.h>

#define MNTOPT_NOEXEC "noexec"
#define MNTOPT_NOUSER "nouser"

// return: NULL or string in stuck
const char *findmntdev(const char *fsname);
unsigned long parsemountflags(struct mntent *opts);
const char *prefixmountpoint(const char *prefix, const char *mountpoint);
void movemnt(const char *from, const char *to);

#endif