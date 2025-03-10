#ifndef INITUP_LIBC
#define INITUP_LIBC

// size_t
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

int strncmp(const char *s1, const char *s2, size_t size);
size_t strconcat(char *dest, const char *src, size_t dest_offset, size_t src_size);
void halt();
int is_swap(const char *type);
dev_t devfrompath(const char *path);
int is_dir(const char *path);

#endif