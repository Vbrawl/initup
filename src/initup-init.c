#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <errno.h>

#include "../include/utils.h"
#include "../include/mountentry.h"
#include "../include/routines.h"
#include "config.h"

#define MOUNT_PREFIX "/root"
#define FSTAB_PATH "/etc/fstab"
#define INIT_SCRIPT "/sbin/init"

int main() {
  int console = open("/dev/console", O_RDWR);

  // Mount /dev to be able to access other devices
  println(console, "Loading /dev...");
  if(mount(NULL, "/dev", "devtmpfs", MS_RDONLY, NULL) != 0) {
    println(console, strerror(errno));
    halt(console, "Can't mount /dev");
  }

  // Load and mount /etc/fstab entries
  println(console, "Loading /etc/fstab...");
  mount_fstab(console, FSTAB_PATH, MOUNT_PREFIX);

  // Delete everything
  println(console, "Deleting all initramfs contents...");
  dev_t id = devfrompath("/");
  if(id == -1) {
    halt(console, "Can't find rootfs???");
  }
  purge_device(console, "/", id);

  // Unmount /dev since we don't need it any more
  if(umount("/dev") != 0) {
    println(console, strerror(errno));
    halt(console, "Can't unmount /dev");
  }

  // Move root to / and chroot to it
  println(console, "Moving root...");
  chdir(MOUNT_PREFIX);
  movemnt(".", "/");
  chroot(".");

  // Start /init as PID 1
  println(console, "Start " INIT_SCRIPT " as PID 1");
  close(console);

  char *init_parameters[2];
  init_parameters[0] = INIT_SCRIPT;
  init_parameters[1] = NULL;

  execv(INIT_SCRIPT, init_parameters);
  halt(console, "Can't start " INIT_SCRIPT);
  return 0;
}