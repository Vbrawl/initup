#include "../include/mountentry.h"
#include "../include/utils.h"
#include "unistd.h"
#include <limits.h>
#include <stdlib.h>
#include <sys/mount.h>

const char *findmntdev_by_link(const char *type, const char *identifier) {
  // readlink: /dev/disk/by-uuid/<uuid>
  // readlink: /dev/disk/by-label/<label>
  // readlink: /dev/disk/<type>/<identifier>
  static char devpath[PATH_MAX];
  char s[PATH_MAX];
  size_t offset = 0;

  const char *prefix = "/dev/disk/";
  size_t part_size = strlen(prefix);
  offset = strconcat(s, prefix, offset, part_size);

  part_size = strlen(type);
  offset = strconcat(s, type, offset, part_size);

  offset = strconcat(s, "/", offset, 1);

  part_size = strlen(identifier);
  strconcat(s, identifier, offset, part_size);

  realpath(s, devpath);
  return devpath;
}

const char *findmntdev(const char *fsname) {
  // We need to parse:
  // * UUID=
  // * LABEL=
  // * /dev/sdaX

  if(strncmp("UUID=", fsname, 5) == 0) {
    return findmntdev_by_link("by-uuid", fsname + 5);
  }
  else if(strncmp("LABEL=", fsname, 6) == 0) {
    return findmntdev_by_link("by-label", fsname + 6);
  }
  else { // Assume /dev/sdaX
    return fsname;
  }
}

unsigned long parsemountflags(struct mntent *ent) {
  if(hasmntopt(ent, MNTOPT_DEFAULTS) != NULL) {
    // auto, rw, nouser, exec, and suid
    return MS_NOUSER; // Others are default either way
  }

  unsigned long opts = 0;

  if(hasmntopt(ent, MNTOPT_RO) != NULL) {
    opts |= MS_RDONLY;
  }

  if(hasmntopt(ent, MNTOPT_NOSUID) != NULL) {
    opts |= MS_NOSUID;
  }

  if(hasmntopt(ent, MNTOPT_NOEXEC) != NULL) {
    opts |= MS_NOEXEC;
  }

  if(hasmntopt(ent, MNTOPT_NOUSER) != NULL) {
    opts |= MS_NOUSER;
  }

  return opts;
}

const char *prefixmountpoint(const char *prefix, const char *mountpoint) {
  static char pmountpoint[PATH_MAX];
  size_t offset = 0;

  offset = strconcat(pmountpoint, prefix, offset, strlen(prefix));
  strconcat(pmountpoint, mountpoint, offset, strlen(mountpoint));
  return pmountpoint;
}

void movemnt(const char *from, const char *to) {
  mount(from, to, NULL, MS_MOVE, NULL);
}