#ifndef INITUP_LINKED_LIST
#define INITUP_LINKED_LIST

#include <stddef.h>

struct linked_list {
  char *value;
  struct linked_list *next;
};

struct linked_list* new_ll(char *value);
char* value_ll(struct linked_list *list);
size_t size_ll(struct linked_list *ll);
void del_ll(struct linked_list *list);

/* ll can be the root of the linked_list
  but it's better if it's the previous node
  of the one being unlinked */
int unlink_ll(struct linked_list *ll, struct linked_list *node);

/* ll can be the root of the linked_list
  but it's better if it's the last node */
int add_ll(struct linked_list *ll, struct linked_list *node);


#endif