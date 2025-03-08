#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "../include/linked_list.h"
#include "../include/service.h"

#define PREFIX "/etc/initup/services"

int main() {
  struct linked_list *services = new_ll(NULL);
  struct linked_list *node = NULL;
  struct service *service = NULL;
  pid_t pid = 0;

  DIR *d = NULL;
  struct dirent *dentry;
  size_t namelen = 0;

  if(chdir(PREFIX) != 0) {
    perror("Couldn't access directory: \"" PREFIX "\"");
    return 1;
  }

  d = opendir(".");
  if(d == NULL) {
    perror("Couldn't access directory: \"" PREFIX "\"");
    return 2;
  }

  while((dentry = readdir(d)) != NULL) {
    namelen = strlen(dentry->d_name);
    if(namelen == 1 && dentry->d_name[0] == '.')
      continue;
    if(namelen == 2 && dentry->d_name[0] == '.' && dentry->d_name[1] == '.')
      continue;

    if(service_has_file(dentry->d_name, SERVICE_AUTOSTART) != 0)
      continue;

    pid = service_run(dentry->d_name, SERVICE_AUTOSTART);
    if(pid == -1) {
      perror("Can't run service");
      continue;
    }

    service = new_service(dentry->d_name, pid);
    if(service == NULL) {
      perror("Can't log service for monitoring");
      continue;
    }

    node = new_ll((char*)service);
    if(node == NULL) {
      perror("Can't log service for monitoring");
      del_service(service);
      continue;
    }

    if(add_ll(services, node) != 0) {
      perror("Can't log service for monitoring");
      del_service(service);
      del_ll(node);
      continue;
    }
  }

  closedir(d);

  while(size_ll(services) > 1) {
    pid = wait(NULL);

    node = services;
    while((node->value == NULL || ((struct service*)node->value)->pid != pid) && node->next != NULL)
      node = node->next;
    unlink_ll(services, node);
  }

  return 0;
}