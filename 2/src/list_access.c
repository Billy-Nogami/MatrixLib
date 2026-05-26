#include "list.h"

#include <math.h>
#include <stdlib.h>

static ListNode *list_node_at(const List *list, size_t index) {
    ListNode *current = list->head;
    size_t current_index = 0;

    while (current_index < index) {
        current = current->next;
        current_index++;
    }

    return current;
}

ListStatus list_push_back(List *list, double value) {
    ListNode *node;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return LIST_ERROR_ALLOC;
    }

    node->value = value;
    node->next = NULL;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return LIST_SUCCESS;
}

ListStatus list_push_front(List *list, double value) {
    ListNode *node;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return LIST_ERROR_ALLOC;
    }

    node->value = value;
    node->next = list->head;
    list->head = node;

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->size++;
    return LIST_SUCCESS;
}

ListStatus list_pop_back(List *list, double *value) {
    ListNode *current;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    if (list->head == NULL) {
        return LIST_ERROR_EMPTY;
    }

    if (list->head == list->tail) {
        if (value != NULL) {
            *value = list->head->value;
        }
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return LIST_SUCCESS;
    }

    current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }

    if (value != NULL) {
        *value = list->tail->value;
    }

    free(list->tail);
    list->tail = current;
    list->tail->next = NULL;
    list->size--;
    return LIST_SUCCESS;
}

ListStatus list_pop_front(List *list, double *value) {
    ListNode *node;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    if (list->head == NULL) {
        return LIST_ERROR_EMPTY;
    }

    node = list->head;
    if (value != NULL) {
        *value = node->value;
    }

    list->head = node->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return LIST_SUCCESS;
}

ListStatus list_insert(List *list, size_t index, double value) {
    ListNode *previous;
    ListNode *node;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    if (index > list->size) {
        return LIST_ERROR_BOUNDS;
    }

    if (index == 0) {
        return list_push_front(list, value);
    }

    if (index == list->size) {
        return list_push_back(list, value);
    }

    previous = list_node_at(list, index - 1);
    node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return LIST_ERROR_ALLOC;
    }

    node->value = value;
    node->next = previous->next;
    previous->next = node;
    list->size++;
    return LIST_SUCCESS;
}

ListStatus list_remove_at(List *list, size_t index, double *value) {
    ListNode *previous;
    ListNode *node;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    if (list->size == 0) {
        return LIST_ERROR_EMPTY;
    }

    if (index >= list->size) {
        return LIST_ERROR_BOUNDS;
    }

    if (index == 0) {
        return list_pop_front(list, value);
    }

    if (index + 1 == list->size) {
        return list_pop_back(list, value);
    }

    previous = list_node_at(list, index - 1);
    node = previous->next;
    if (value != NULL) {
        *value = node->value;
    }

    previous->next = node->next;
    free(node);
    list->size--;
    return LIST_SUCCESS;
}

ListStatus list_get(const List *list, size_t index, double *value) {
    if (list == NULL || value == NULL) {
        return LIST_ERROR_NULL;
    }

    if (index >= list->size) {
        return LIST_ERROR_BOUNDS;
    }

    *value = list_node_at(list, index)->value;
    return LIST_SUCCESS;
}

ListStatus list_set(List *list, size_t index, double value) {
    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    if (index >= list->size) {
        return LIST_ERROR_BOUNDS;
    }

    list_node_at(list, index)->value = value;
    return LIST_SUCCESS;
}

ListStatus list_fill(List *list, double value) {
    ListNode *current;

    if (list == NULL) {
        return LIST_ERROR_NULL;
    }

    current = list->head;
    while (current != NULL) {
        current->value = value;
        current = current->next;
    }

    return LIST_SUCCESS;
}

ListStatus list_find(const List *list, double value, size_t *index) {
    ListNode *current;
    size_t current_index = 0;
    const double epsilon = 1e-9;

    if (list == NULL || index == NULL) {
        return LIST_ERROR_NULL;
    }

    current = list->head;
    while (current != NULL) {
        if (fabs(current->value - value) <= epsilon) {
            *index = current_index;
            return LIST_SUCCESS;
        }
        current = current->next;
        current_index++;
    }

    return LIST_ERROR_NOT_FOUND;
}
