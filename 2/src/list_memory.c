#include "list.h"

#include <stdlib.h>

ListStatus list_create(List *list) {
    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return LIST_SUCCESS;
}

void list_free(List *list) {
    ListNode *current;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

ListStatus list_copy(const List *source, List *destination) {
    List temp;
    ListNode *current;
    ListStatus status;

    if (source == NULL || destination == NULL) {
        return LIST_ERROR_NULL;
    }

    if (source == destination) {
        return LIST_SUCCESS;
    }

    status = list_create(&temp);
    if (status != LIST_SUCCESS) {
        return status;
    }

    current = source->head;
    while (current != NULL) {
        status = list_push_back(&temp, current->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        current = current->next;
    }

    list_free(destination);
    *destination = temp;
    return LIST_SUCCESS;
}
