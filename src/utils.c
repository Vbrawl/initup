#include "../include/utils.h"
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>


void println(int fd, const char *msg) {
  write(fd, msg, strlen(msg));
  write(fd, "\r\n", 2);
}

int strncmp(const char *s1, const char *s2, size_t size) {
  for (size_t i = 0; i < size; i++) {
    int diff = s1[i] - s2[i];
    if(diff != 0) return diff;
  }
  return 0;
}

size_t strconcat(char *dest, const char *src, size_t dest_offset, size_t src_size) {
  dest += dest_offset;
  for (size_t i = 0; i < src_size; i++) {
    dest[i] = src[i];
  }
  dest[src_size] = '\0';
  return dest_offset + src_size;
}

void halt(int fd, const char *msg) {
  println(fd, msg);
  close(fd);
  sync();
  while(1) {}
}