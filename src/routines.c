#include <mntent.h>
#include <sys/mount.h>
#include <sys/swap.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "../include/mountentry.h"
#include "../include/utils.h"


void mount_fstab(int console, const char *fstab_path, const char *prefix) {
  FILE *fstab = setmntent(fstab_path, "r");
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
      endmntent(fstab);
      halt(console, "Device not found!");
    }

    if(is_swap(entry->mnt_type)) {
      if(swapon(dev, 0) != 0) {
        endmntent(fstab);
        halt(console, "Can't mount swap");
      }
    }
    else {
      const char *mountpoint = prefixmountpoint(prefix, entry->mnt_dir);
      if(mount(dev, mountpoint, entry->mnt_type, opts, entry->mnt_opts) != 0) {
        endmntent(fstab);
        halt(console, "Can't mount device");
      }
    }
  }

  endmntent(fstab);
}

void purge_device(int console, const char *path, dev_t rootdev) {
  char next_path[PATH_MAX];
  size_t offset = 0;

  DIR *dir;
  struct dirent *d;
  size_t dsize;

  debug_println(console, path);
  if(!is_dir(path)) halt(console, "purge_device: path is not a directory!");
  if(devfrompath(path) != rootdev) return;

  dir = opendir(path);
  if(dir == NULL) return;

  while((d = readdir(dir)) != NULL) {
    dsize = strlen(d->d_name);
    if(dsize == 1 && d->d_name[0] == '.') continue;
    if(dsize == 2 && d->d_name[0] == '.' && d->d_name[1] == '.') continue;

    offset = strconcat(next_path, path, 0, strlen(path));
    if(next_path[offset - 1] != '/') {
      offset = strconcat(next_path, "/", offset, 1);
    }
    strconcat(next_path, d->d_name, offset, dsize);

    if(is_dir(next_path)) {
      purge_device(console, next_path, rootdev);
    }
    else {
      debug_println(console, next_path);
      if(unlink(next_path) != 0) {
        debug_println(console, strerror(errno));
        halt(console, "Can't unlink");
      }
    }
  }
  closedir(dir);

  debug_println(console, path);
  if(rmdir(path) != 0) {
    // If path != /
    if(strlen(path) != 1 || path[0] != '/') {
      debug_println(console, strerror(errno));
      halt(console, "Can't remove directory");
    }
  }
}