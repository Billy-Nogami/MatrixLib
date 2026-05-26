#include "list.h"

static ListStatus list_validate_pair(const List *left, const List *right, List *result) {
    if (left == NULL || right == NULL || result == NULL) {
        return LIST_ERROR_NULL;
    }

    if (left->size != right->size) {
        return LIST_ERROR_SIZE;
    }

    return LIST_SUCCESS;
}

ListStatus list_add(const List *left, const List *right, List *result) {
    List temp;
    ListNode *left_node;
    ListNode *right_node;
    ListStatus status;

    status = list_validate_pair(left, right, result);
    if (status != LIST_SUCCESS) {
        return status;
    }

    status = list_create(&temp);
    if (status != LIST_SUCCESS) {
        return status;
    }

    left_node = left->head;
    right_node = right->head;
    while (left_node != NULL && right_node != NULL) {
        status = list_push_back(&temp, left_node->value + right_node->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        left_node = left_node->next;
        right_node = right_node->next;
    }

    list_free(result);
    *result = temp;
    return LIST_SUCCESS;
}

ListStatus list_sub(const List *left, const List *right, List *result) {
    List temp;
    ListNode *left_node;
    ListNode *right_node;
    ListStatus status;

    status = list_validate_pair(left, right, result);
    if (status != LIST_SUCCESS) {
        return status;
    }

    status = list_create(&temp);
    if (status != LIST_SUCCESS) {
        return status;
    }

    left_node = left->head;
    right_node = right->head;
    while (left_node != NULL && right_node != NULL) {
        status = list_push_back(&temp, left_node->value - right_node->value);
        if (status != LIST_SUCCESS) {
            list_free(&temp);
            return status;
        }
        left_node = left_node->next;
        right_node = right_node->next;
    }

    list_free(result);
    *result = temp;
    return LIST_SUCCESS;
}

ListStatus list_scalar_mul(const List *list, double scalar, List *result) {
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
        status = list_push_back(&temp, current->value * scalar);
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
