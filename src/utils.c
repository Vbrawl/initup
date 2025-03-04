#include "../include/utils.h"
#include <unistd.h>


size_t strlen(const char *msg) {
  size_t size = 0;
  while(msg[size] != '\0') size++;
  return size;
}

void println(int fd, const char *msg) {
  write(fd, msg, strlen(msg));
  write(fd, "\r\n", 2);
}