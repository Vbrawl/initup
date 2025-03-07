#include "../include/cpio.h"
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/sysmacros.h>
#include <stdlib.h>

// Maximum header entry size (8) + null (1) = 9
#define HEXDATA_BUFFER_SIZE 9

size_t cpio_newc_sizeof(size_t psize, size_t dsize) {
  size_t s = sizeof(struct cpio_newc_header);
  int offset = 0;
  s += psize;
  offset = s % 4;
  if(offset > 0)
    s += 4 - offset;
  s += dsize;
  offset = s % 4;
  if(offset > 0)
    s += 4 - offset;
  return s;
}

int generate_cpio(struct cpio *cpio, const char *realpath, const char *path) {
  FILE *f;
  struct stat s;

  if(lstat(realpath, &s) != 0) {
    return -1;
  }

  // Setup data and path lengths
  cpio->data.psize = strlen(path);
  cpio->data.dsize = s.st_size;

  // Setup header
  memcpy(cpio->header.magic, NEWC_MAGIC, sizeof(cpio->header.magic));

  cpio_setfield(cpio->header.ino, s.st_ino);
  cpio_setfield(cpio->header.mode, s.st_mode);
  cpio_setfield(cpio->header.uid, s.st_uid);
  cpio_setfield(cpio->header.gid, s.st_gid);
  memcpy(cpio->header.links, "00000001", sizeof(cpio->header.links));
  cpio_setfield(cpio->header.mtime, s.st_mtime);
  cpio_setfield(cpio->header.dsize, cpio->data.dsize);
  cpio_setfield(cpio->header.dmajor, major(s.st_dev));
  cpio_setfield(cpio->header.dminor, minor(s.st_dev));
  cpio_setfield(cpio->header.rdmajor, major(s.st_rdev));
  cpio_setfield(cpio->header.rdminor, minor(s.st_rdev));
  cpio_setfield(cpio->header.psize, cpio->data.psize + 1);
  memset(cpio->header.checksum, '0', sizeof(cpio->header.checksum));

  cpio->data.path = strdup(path);
  cpio->data.data = NULL;

  if(S_ISREG(s.st_mode)) {
    cpio->data.data = malloc(cpio->data.dsize);

    f = fopen(realpath, "r");
    if(f == NULL) {
      free_cpio(cpio);
      fclose(f);
      return -2;
    }
    if(fread(cpio->data.data, 1, cpio->data.dsize, f) != cpio->data.dsize) {
      free_cpio(cpio);
      fclose(f);
      return -3;
    }
    fclose(f);
  }

  return 0;
}

int cpio_setfield(char *field, size_t value) {
  char tmp[NEWC_COMMON_FIELD_SIZE + 1];
  int error = snprintf(tmp, sizeof(tmp), "%08lX", value);
  memcpy(field, tmp, NEWC_COMMON_FIELD_SIZE);
  return error;
}

int generate_cpio_trailer(struct cpio *cpio) {
  // Setup data and path lengths
  cpio->data.psize = strlen(TRAILER_PATH);
  cpio->data.dsize = 0;

  // Setup header
  memcpy(cpio->header.magic, NEWC_MAGIC, sizeof(cpio->header.magic));

  memset(cpio->header.ino, '0', sizeof(cpio->header.ino) + sizeof(cpio->header.mode)
                            + sizeof(cpio->header.uid) + sizeof(cpio->header.gid)
                            + sizeof(cpio->header.links) + sizeof(cpio->header.mtime)
                            + sizeof(cpio->header.dsize) + sizeof(cpio->header.dmajor)
                            + sizeof(cpio->header.dminor) + sizeof(cpio->header.rdmajor)
                            + sizeof(cpio->header.rdminor));
  if(snprintf(cpio->header.psize, sizeof(cpio->header.psize), "%lX", cpio->data.psize + 1) < 0)
    return -1;
  memset(cpio->header.checksum, '0', sizeof(cpio->header.checksum));

  cpio->data.path = malloc(cpio->data.psize);
  if(cpio->data.path == NULL)
    return -2;
  memcpy(cpio->data.path, TRAILER_PATH, cpio->data.psize);

  cpio->data.data = NULL;

  return 0;
}

int write_cpio(struct cpio *cpio, FILE *f) {
  char paddstring[4] = {0};
  size_t padding_size = 0;

  if(fwrite(&cpio->header, sizeof(struct cpio_newc_header), 1, f) != 1)
    return -1;

  if(fwrite(cpio->data.path, cpio->data.psize + 1, 1, f) != 1)
    return -2;

  padding_size = sizeof(paddstring) - (sizeof(struct cpio_newc_header) + cpio->data.psize + 1) % sizeof(paddstring);
  if(padding_size < 4)
    if(fwrite(paddstring, padding_size, 1, f) != 1)
      return -3;

  if(cpio->data.data != NULL)
    if(fwrite(cpio->data.data, cpio->data.dsize, 1, f) != 1)
      return -4;

  padding_size = sizeof(paddstring) - cpio->data.dsize % sizeof(paddstring);
  if(padding_size < 4)
    if(fwrite(paddstring, padding_size, 1, f) != 1)
      return -5;

  return 0;
}

void free_cpio(struct cpio *cpio) {
  free(cpio->data.path);

  if(cpio->data.data != NULL)
    free(cpio->data.data);

  cpio->data.path = NULL;
  cpio->data.data = NULL;
}