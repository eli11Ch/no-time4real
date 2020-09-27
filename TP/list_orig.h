/*
 * Author: Jérôme Pouiller <jerome@sysmic.fr>
 * Created: Fri Dec 11 22:57:54 CET 2009
 * Licence: GPL
 *
 */
#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct list_item_s {
    /* ADD YOUR MEMBERS HERE */
    struct list_item_s *next;
} list_item_t;

#define EMPTY_LIST NULL
#define NEXT(s) (s)->next
#define FOREACH(I, LIST) for (I = LIST; I; I = NEXT(I))

bool list_isEmpty(list_item_t *l);

int list_size(list_item_t *list);

/* Add an element. Don't forget to allocate it before */
void list_add(list_item_t **list, list_item_t *item);

/* Remove an element. Return removed element. Don't forget to free it AFTER removing */
list_item_t *list_remove(list_item_t **list, list_item_t *item);

#endif /* LIST_H */
