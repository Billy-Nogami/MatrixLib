#include "list.h"

ListStatus list_concat(const List *left, const List *right, List *result) {
    List temp;
    ListNode *current;
    ListStatus status;

    if (left == NULL || right == NULL || result == NULL) {
        return LIST_ERROR_NULL;
    }

    status = list_create(&temp);
    if (status != LIST_SUCCESS) {
        return status;
    }

    current = left->head;
    while (current != NULL) {
        status = list_push_back(&temp, current->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        current = current->next;
    }

    current = right->head;
    while (current != NULL) {
        status = list_push_back(&temp, current->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        current = current->next;
    }

    list_free(result);
    *result = temp;
    return LIST_SUCCESS;
}

ListStatus list_reverse(const List *list, List *result) {
    List temp;
    ListNode *current;
    ListStatus status;

    if (list == NULL || result == NULL) {
        return LIST_ERROR_NULL;
    }

    status = list_create(&temp);
    if (status != LIST_SUCCESS) {
        return status;
    }

    current = list->head;
    while (current != NULL) {
        status = list_push_front(&temp, current->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        current = current->next;
    }

    list_free(result);
    *result = temp;
    return LIST_SUCCESS;
}
