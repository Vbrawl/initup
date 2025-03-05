#ifndef INITUP_LIBC
#define INITUP_LIBC

// size_t
#include <stddef.h>
#include <string.h>

#ifdef NDEBUG
#define debug_println(fd, msg)
#else
#define debug_println(fd, msg) println(fd, msg)
#endif

void println(int fd, const char *msg);
int strncmp(const char *s1, const char *s2, size_t size);
size_t strconcat(char *dest, const char *src, size_t dest_offset, size_t src_size);
void halt(int fd, const char *msg);
int is_swap(const char *type);

#endif