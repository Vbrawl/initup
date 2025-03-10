#ifndef INITUP_ROUTINES
#define INITUP_ROUTINES


int mount_fstab(const char *fstab_path, const char *prefix);
int purge_device(const char *path, dev_t rootdev);

#endif