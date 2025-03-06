#ifndef INITUP_ROUTINES
#define INITUP_ROUTINES


void mount_fstab(int console, const char *fstab_path, const char *prefix);
void purge_device(int console, const char *path, dev_t rootdev);

#endif