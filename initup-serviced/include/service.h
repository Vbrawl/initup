#ifndef INITUP_SERVICE
#define INITUP_SERVICE

#include <limits.h>
#include <sys/types.h>

#define SERVICE_AUTOSTART "autostart"

struct service {
  char name[NAME_MAX];
  pid_t pid;
};

struct service* new_service(const char *service_name, pid_t pid);
void del_service(struct service *service);

int service_has_file(const char *service_path, const char *filename);
int service_run(const char *service_path, const char *filename);


#endif