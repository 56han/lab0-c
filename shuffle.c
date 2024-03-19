#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "shuffle.h"

/* Fisher-Yates shuffle Algorithm */
void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *new = head->prev;
    int size = q_size(head);
    char *tmp;
    while (size > 0) {
        int index = rand() % size;
        struct list_head *old = head->next;
        for (int i = 0; i < index; i++) {
            old = old->next;
        }
        element_t *oldnode =
            list_entry(old, element_t, list);  // cppcheck-suppress nullPointer
        tmp = oldnode->value;
        element_t *newnode =
            list_entry(new, element_t, list);  // cppcheck-suppress nullPointer
        oldnode->value = newnode->value;
        newnode->value = tmp;
        size--;
        new = new->prev;
    }
    return;
}