#ifndef INITUP_LIBC
#define INITUP_LIBC

// size_t
#include <stddef.h>

size_t strlen(const char *msg);
void println(int fd, const char *msg);

#endif