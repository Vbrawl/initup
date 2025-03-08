#include "../include/service.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int service_has_file(const char *service_path, const char *filename) {
  char path[PATH_MAX];
  struct stat s;
  snprintf(path, PATH_MAX, "%s/%s", service_path, filename);
  if(stat(path, &s) != 0)
    return -1;
  
  return 0;
}

int service_run(const char *service_path, const char *filename) {
  pid_t pid = fork();
  if(pid == 0) {
    // child
    chdir(service_path);
    // snprintf(path, PATH_MAX, "%s/%s", service_path, filename);
    char * const args[2] = {
      strdup(filename),
      NULL
    };
    execv(filename, args);
    perror("execv failed");
  }
  // parent
  return pid;
}

struct service* new_service(const char *service_name, pid_t pid) {
  struct service *service = malloc(sizeof(struct service));
  if(service == NULL)
    return NULL;

  strncpy(service->name, service_name, sizeof(service->name));
  service->pid = pid;

  return service;
}

void del_service(struct service *service) {
  free(service);
}