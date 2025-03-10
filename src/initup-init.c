#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <errno.h>
#include <sys/stat.h>

#include "../include/utils.h"
#include "../include/mountentry.h"
#include "../include/routines.h"
#include "config.h"

#define MOUNT_PREFIX "/root"
#define FSTAB_PATH "/etc/fstab"
#define INIT_SCRIPT "/sbin/init"

int main() {
  dev_t id = 0;

  // Ensure prefix exists
  mkdir(MOUNT_PREFIX, 0777);

  // Load fstab file
  // NOTE: umount and rmdir are not critical
  mkdir("/dev", 0777);
  if(mount(NULL, "/dev", "devtmpfs", MS_RDONLY, NULL) != 0)
    halt();
  mount_fstab(FSTAB_PATH, MOUNT_PREFIX);
  umount("/dev");
  rmdir("/dev");

  // Delete initramfs (Not a critical operation!)
  id = devfrompath("/");
  if(id != -1)
    if(purge_device("/", id) != 0)
      halt();

  // Relocate disk to rootfs
  if(chdir(MOUNT_PREFIX) != 0)
    halt();
  if(movemnt(".", "/") != 0)
    halt();
  if(chroot(".") != 0)
    halt();

  // Start init as PID 1
  char *init_parameters[2];
  init_parameters[0] = INIT_SCRIPT;
  init_parameters[1] = NULL;
  execv(INIT_SCRIPT, init_parameters);

  return 0;
}