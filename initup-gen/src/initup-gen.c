#include "../include/cpio.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

#include "config.h"

#define FILES_NUM 3

void help(const char *prog_name) {
  printf(
          "Usage: %s [OPTION]...\n"
          "Generate an initramfs containing the initup-init program and it's required files\n"
          "\n"
          "Available options (Defaults in <>):\n"
          "  -h, --help                         Show this help menu\n"
          "  -o, --out=<initramfs.cpio>         Output file\n"
          "  -f, --fstab=</etc/fstab>           The fstab file to embed\n"
          "  -i, --init=</sbin/initup-init>     The init program\n"
          "\n"
          "\n"
          "Copyright 2025 Jim Konstantos <konstantosjim@gmail.com>\n"
          "\n"
          "Redistribution and use in source and binary forms, with or without modification,\n"
          "are permitted provided that the following conditions are met:\n"
          "1. Redistributions of source code must retain the above copyright notice,\n"
          "   this list of conditions and the following disclaimer.\n"
          "2. Redistributions in binary form must reproduce the above copyright notice,\n"
          "   this list of conditions and the following disclaimer in the documentation\n"
          "   and/or other materials provided with the distribution.\n"
          "\n"
          "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND\n"
          "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
          "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n"
          "IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\n"
          "INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT\n"
          "NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
          "PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,\n"
          "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n"
          "ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n"
          "POSSIBILITY OF SUCH DAMAGE.\n"

          , prog_name
        );
}

void version() {
  printf(
          "initup-gen " PROJECT_VERSION "\n"
          "Copyright (C) 2025 Jim Konstantos <konstantosjim@gmail.com>\n"
          "License BSD 2-Clause\n"
          "\n"
          "Written by Jim Konstantos <konstantosjim@gmail.com>\n"
  );
}

int append_file(const char *src, const char *dest, FILE *f) {
  struct cpio c;
  int status = 0;
  if(generate_cpio(&c, src, dest) != 0)
    return -1;
  // Attempt a write, free heap either way, return status
  status = write_cpio(&c, f);
  free_cpio(&c);
  if(status != 0)
    return -2;
  return 0;
}

int append_trailer(FILE *f) {
  struct cpio c;
  int status = 0;
  if(generate_cpio_trailer(&c) != 0)
    return -1;
  // Attempt a write, free heap either way, return status
  status = write_cpio(&c, f) != 0;
  free_cpio(&c);
  if(status != 0)
    return -2;
  return 0;
}

int main(int argc, char **argv) {
  const char *prog_name = argv[0];
  int optv = 0;
  FILE *f = NULL;

  const char *out = "initramfs.cpio";
  const char *fstab_path = "/etc/fstab";
  const char *init_prog = "/sbin/initup-init";


  struct option longopts[6] = {
    { .name = "help",     .has_arg = 0,   .flag = NULL,   .val = 'h'  },
    { .name = "version",  .has_arg = 0,   .flag = NULL,   .val = 'v'  },
    { .name = "out",      .has_arg = 1,   .flag = NULL,   .val = 'o'  },
    { .name = "fstab",    .has_arg = 1,   .flag = NULL,   .val = 'f'  },
    { .name = "init",     .has_arg = 1,   .flag = NULL,   .val = 'i'  },
    { .name = NULL,       .has_arg = 0,   .flag = NULL,   .val = 0    }
  };

  while((optv = getopt_long(argc, argv, "hvo:f:i:", longopts, NULL)) != -1) {
    switch(optv) {
    case 'h':
      help(prog_name);
      return 0;
    case 'v':
      version();
      return 0;
    case 'o':
      out = optarg;
      break;
    case 'f':
      fstab_path = optarg;
      break;
    case 'i':
      init_prog = optarg;
      break;
    }
  }

  /* Write archive */
  f = fopen(out, "w");
  if(f == NULL) {
    fprintf(stderr, "Can't open \"%s\" for writing!\n", out);
    return 1;
  }

  // Write init program
  if(append_file(init_prog, "init", f) != 0) {
    fprintf(stderr, "Can't add \"%s\" to \"%s\"\n", init_prog, out);
    return 2;
  }

  // Write /etc/fstab
  if(append_file("/", "etc", f)) { // Copy a root UID+GID directory
    fprintf(stderr, "Can't create \"etc\" directory inside \"%s\"\n", out);
    return 3;
  }

  if(append_file(fstab_path, "etc/fstab", f)) {
    fprintf(stderr, "Can't add \"%s\" to \"%s\"\n", fstab_path, out);
    return 4;
  }

  // Close archive
  append_trailer(f);
  fclose(f);

  printf("Saved %s\n", out);
  return 0;
}