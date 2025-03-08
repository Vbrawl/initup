#include "../include/linked_list.h"
#include <stdlib.h>

struct linked_list* new_ll(char *value) {
  struct linked_list *list = malloc(sizeof(struct linked_list));

  list->value = value;
  list->next = NULL;
}

inline char* value_ll(struct linked_list *list) {
  return list->value;
}

void del_ll(struct linked_list *list) {
  struct linked_list *next = NULL;
  while(list != NULL) {
    next = list->next;
    free(list);
    list = next;
  }
}

int unlink_ll(struct linked_list *ll, struct linked_list *node) {
  while(ll->next != NULL && ll->next != node)
    ll = ll->next;

  if(ll == NULL)
    return -1;
  
  ll->next = node->next;
  free(node);

  return 0;
}

int add_ll(struct linked_list *ll, struct linked_list *node) {
  while(ll->next != NULL)
    ll = ll->next;

  if(ll == NULL)
    return -1;
  
  ll->next = node;
  return 0;
}

size_t size_ll(struct linked_list *ll) {
  size_t size = 0;

  while(ll != NULL) {
    size += 1;
    ll = ll->next;
  }

  return size;
}