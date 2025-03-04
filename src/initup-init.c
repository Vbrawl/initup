#include <fcntl.h>
#include <unistd.h>

#include "../include/utils.h"
#include "config.h"



int main() {
  int console = open("/dev/console", O_RDWR);

  println(console, "Hello World!");

  close(console);
  return 0;
}