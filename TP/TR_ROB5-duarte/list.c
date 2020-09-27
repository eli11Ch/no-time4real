/*  
 * Author: Jérôme Pouiller <jerome@sysmic.fr>
 * Created: Fri Dec 11 22:57:54 CET 2009
 * Licence: GPL                                      
 *
 */
#include "list.h"

bool list_isEmpty(list_item_t *l) {
    return (l == EMPTY_LIST);
}

int list_size(list_item_t *list) {
    int count = 0;
    list_item_t *i;

    FOREACH(i, list)
	count++;
    return count;
}

/* Add an element. Don't forget to allocate it before */
void list_add(list_item_t **list, list_item_t *item) {
    item->next = *list;
    *list = item;
}

/* Remove an element. Return removed element. Don't forget to free it AFTER removing */
list_item_t *list_remove(list_item_t **list, list_item_t *item) {
    list_item_t *i;

    if (*list == item) {
	*list = (*list)->next;
	return item;
    }
    FOREACH(i, *list)
	if (i->next == item) {
	    i->next = item->next;
	    return item;
	}
    // Nothing found
    return NULL;
}
