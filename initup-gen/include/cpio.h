#ifndef INITUP_CPIO
#define INITUP_CPIO

#include <stddef.h>
#include <stdio.h>

#define NEWC_MAGIC "070701"
#define TRAILER_PATH "TRAILER!!!"
#define NEWC_COMMON_FIELD_SIZE 8

/**
 * New ASCII CPIO (newc) Format structure
 */
struct __attribute__((packed)) cpio_newc_header {
  char magic[6];          // Magic signature (070701 for newc)
  char ino[8];            // Inode number
  char mode[8];           // File type
  char uid[8];            // User ID
  char gid[8];            // Group ID
  char links[8];          // Number of hard-links
  char mtime[8];          // Last modification date
  char dsize[8];          // Data size
  char dmajor[8];         // Device major
  char dminor[8];         // Device minor
  char rdmajor[8];        // Block device major
  char rdminor[8];        // Block device minor
  char psize[8];          // Path size
  char checksum[8];       // Checksum
};

struct cpio_data {
  char *path;
  size_t psize;
  char *data;
  size_t dsize;
};

struct cpio {
  struct cpio_newc_header header;
  struct cpio_data data;
};

size_t cpio_newc_sizeof(size_t psize, size_t dsize);
int generate_cpio(struct cpio *cpio, const char *realpath, const char *path);
int generate_cpio_trailer(struct cpio *cpio);
int write_cpio(struct cpio *cpio, FILE *f);
int cpio_setfield(char *field, size_t value);
void free_cpio(struct cpio *cpio);

#endif