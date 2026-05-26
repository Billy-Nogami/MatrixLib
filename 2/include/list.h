#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct ListNode {
    double value;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t size;
} List;

typedef enum {
    LIST_SUCCESS = 0,
    LIST_ERROR_NULL = 1,
    LIST_ERROR_ALLOC = 2,
    LIST_ERROR_BOUNDS = 3,
    LIST_ERROR_SIZE = 4,
    LIST_ERROR_EMPTY = 5,
    LIST_ERROR_NOT_FOUND = 6
} ListStatus;

ListStatus list_create(List *list);
void list_free(List *list);
ListStatus list_copy(const List *source, List *destination);

ListStatus list_push_back(List *list, double value);
ListStatus list_push_front(List *list, double value);
ListStatus list_pop_back(List *list, double *value);
ListStatus list_pop_front(List *list, double *value);
ListStatus list_insert(List *list, size_t index, double value);
ListStatus list_remove_at(List *list, size_t index, double *value);

ListStatus list_get(const List *list, size_t index, double *value);
ListStatus list_set(List *list, size_t index, double value);
ListStatus list_fill(List *list, double value);
ListStatus list_find(const List *list, double value, size_t *index);

ListStatus list_add(const List *left, const List *right, List *result);
ListStatus list_sub(const List *left, const List *right, List *result);
ListStatus list_scalar_mul(const List *list, double scalar, List *result);

ListStatus list_concat(const List *left, const List *right, List *result);
ListStatus list_reverse(const List *list, List *result);

#endif
