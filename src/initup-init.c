#include <fcntl.h>
#include <unistd.h>
#include <mntent.h>
#include <sys/mount.h>
#include <sys/swap.h>

#include "../include/utils.h"
#include "../include/mountentry.h"
#include "config.h"

#define MOUNT_PREFIX "/root"

int main() {
  int console = open("/dev/console", O_RDWR);

  debug_println(console, "Loading /etc/fstab...");

  // Load and mount /etc/fstab entries
  FILE *fstab = setmntent("/etc/fstab", "r");
  struct mntent *entry = NULL;
  while((entry = getmntent(fstab)) != NULL) {
    debug_println(console, entry->mnt_fsname);
    if(hasmntopt(entry, MNTOPT_NOAUTO) != NULL) {
      debug_println(console, MNTOPT_NOAUTO " - Skipping!");
      continue;
    }

    unsigned long opts = parsemountflags(entry);
    const char *dev = findmntdev(entry->mnt_fsname);

    if(dev == NULL) {
      halt(console, "Device not found!");
      continue;
    }

    if(is_swap(entry->mnt_type) == 0) {
      if(swapon(dev, 0) != 0) {
        halt(console, "Can't mount swap");
      }
    }
    else {
      const char *mountpoint = prefixmountpoint(MOUNT_PREFIX, entry->mnt_dir);
      if(mount(dev, mountpoint, entry->mnt_type, opts, entry->mnt_opts) != 0) {
        halt(console, "Can't mount device");
      }
    }
  }

  close(console);
  return 0;
}