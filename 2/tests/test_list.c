#include "list.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void assert_status(ListStatus actual, ListStatus expected, const char *message) {
    if (actual != expected) {
        fprintf(stderr, "FAIL: %s (expected status %d, got %d)\n", message, expected, actual);
        exit(EXIT_FAILURE);
    }
}

static void assert_close(double actual, double expected, double epsilon, const char *message) {
    if (fabs(actual - expected) > epsilon) {
        fprintf(stderr, "FAIL: %s (expected %.6f, got %.6f)\n", message, expected, actual);
        exit(EXIT_FAILURE);
    }
}

static void assert_list_value(
    const List *list,
    size_t index,
    double expected,
    const char *message
) {
    double actual = 0.0;

    assert_status(list_get(list, index, &actual), LIST_SUCCESS, message);
    assert_close(actual, expected, 1e-9, message);
}

static void fill_list_from_array(List *list, const double *values, size_t count) {
    size_t index;

    for (index = 0; index < count; ++index) {
        assert_status(list_push_back(list, values[index]), LIST_SUCCESS, "list_push_back in fill");
    }
}

static void test_create_push_and_free(void) {
    List list = {0};

    assert_status(list_create(&list), LIST_SUCCESS, "list_create should succeed");
    assert_status(list_push_back(&list, 2.0), LIST_SUCCESS, "list_push_back should succeed");
    assert_status(list_push_front(&list, 1.0), LIST_SUCCESS, "list_push_front should succeed");

    if (list.size != 2) {
        fprintf(stderr, "FAIL: list size should be 2 after pushes\n");
        exit(EXIT_FAILURE);
    }

    assert_list_value(&list, 0, 1.0, "list_push_front should write first element");
    assert_list_value(&list, 1, 2.0, "list_push_back should write last element");
    list_free(&list);

    if (list.head != NULL || list.tail != NULL || list.size != 0) {
        fprintf(stderr, "FAIL: list_free should reset list fields\n");
        exit(EXIT_FAILURE);
    }
}

static void test_get_set_and_fill(void) {
    List list = {0};
    const double values[] = {1.0, 2.0, 3.0};
    double value = 0.0;

    assert_status(list_create(&list), LIST_SUCCESS, "list_create should succeed");
    fill_list_from_array(&list, values, 3);

    assert_status(list_set(&list, 1, 7.25), LIST_SUCCESS, "list_set should succeed");
    assert_status(list_get(&list, 1, &value), LIST_SUCCESS, "list_get should succeed");
    assert_close(value, 7.25, 1e-9, "list_get should read stored value");
    assert_status(list_get(&list, 3, &value), LIST_ERROR_BOUNDS, "list_get should check bounds");

    assert_status(list_fill(&list, 4.5), LIST_SUCCESS, "list_fill should succeed");
    assert_list_value(&list, 0, 4.5, "list_fill should write first element");
    assert_list_value(&list, 2, 4.5, "list_fill should write last element");

    list_free(&list);
}

static void test_insert_and_remove(void) {
    List list = {0};
    const double values[] = {1.0, 3.0, 4.0};
    double removed = 0.0;

    assert_status(list_create(&list), LIST_SUCCESS, "list_create should succeed");
    fill_list_from_array(&list, values, 3);

    assert_status(list_insert(&list, 1, 2.0), LIST_SUCCESS, "list_insert middle should succeed");
    assert_status(list_insert(&list, 4, 5.0), LIST_SUCCESS, "list_insert end should succeed");
    assert_list_value(&list, 0, 1.0, "list_insert should keep first element");
    assert_list_value(&list, 1, 2.0, "list_insert should add middle element");
    assert_list_value(&list, 4, 5.0, "list_insert should add last element");

    assert_status(list_remove_at(&list, 2, &removed), LIST_SUCCESS, "list_remove_at should succeed");
    assert_close(removed, 3.0, 1e-9, "list_remove_at should return removed value");
    assert_list_value(&list, 2, 4.0, "list_remove_at should link remaining nodes");

    list_free(&list);
}

static void test_pop(void) {
    List list = {0};
    const double values[] = {1.0, 2.0, 3.0};
    double value = 0.0;

    assert_status(list_create(&list), LIST_SUCCESS, "list_create should succeed");
    fill_list_from_array(&list, values, 3);

    assert_status(list_pop_front(&list, &value), LIST_SUCCESS, "list_pop_front should succeed");
    assert_close(value, 1.0, 1e-9, "list_pop_front should return first element");
    assert_status(list_pop_back(&list, &value), LIST_SUCCESS, "list_pop_back should succeed");
    assert_close(value, 3.0, 1e-9, "list_pop_back should return last element");
    assert_list_value(&list, 0, 2.0, "list should keep middle element");

    assert_status(list_pop_back(&list, &value), LIST_SUCCESS, "list_pop_back one element");
    assert_status(list_pop_front(&list, &value), LIST_ERROR_EMPTY, "list_pop_front should reject empty list");

    list_free(&list);
}

static void test_copy(void) {
    List source = {0};
    List copy = {0};
    const double values[] = {1.0, 2.0, 3.0};

    assert_status(list_create(&source), LIST_SUCCESS, "list_create source");
    assert_status(list_create(&copy), LIST_SUCCESS, "list_create copy");
    fill_list_from_array(&source, values, 3);

    assert_status(list_copy(&source, &copy), LIST_SUCCESS, "list_copy should succeed");
    assert_list_value(&copy, 0, 1.0, "list_copy should copy first element");
    assert_list_value(&copy, 2, 3.0, "list_copy should copy last element");

    assert_status(list_set(&source, 0, 9.0), LIST_SUCCESS, "list_set source after copy");
    assert_list_value(&copy, 0, 1.0, "list_copy should create independent copy");

    list_free(&source);
    list_free(&copy);
}

static void test_add_sub_and_scalar_mul(void) {
    List left = {0};
    List right = {0};
    List sum = {0};
    List diff = {0};
    List scaled = {0};
    const double left_values[] = {1.0, 2.0, 3.0};
    const double right_values[] = {3.0, 2.0, 1.0};

    assert_status(list_create(&left), LIST_SUCCESS, "list_create left");
    assert_status(list_create(&right), LIST_SUCCESS, "list_create right");
    fill_list_from_array(&left, left_values, 3);
    fill_list_from_array(&right, right_values, 3);

    assert_status(list_add(&left, &right, &sum), LIST_SUCCESS, "list_add should succeed");
    assert_status(list_sub(&left, &right, &diff), LIST_SUCCESS, "list_sub should succeed");
    assert_status(list_scalar_mul(&left, 2.0, &scaled), LIST_SUCCESS, "list_scalar_mul should succeed");

    assert_list_value(&sum, 0, 4.0, "list_add first element");
    assert_list_value(&sum, 2, 4.0, "list_add last element");
    assert_list_value(&diff, 0, -2.0, "list_sub first element");
    assert_list_value(&diff, 2, 2.0, "list_sub last element");
    assert_list_value(&scaled, 1, 4.0, "list_scalar_mul middle element");

    list_free(&left);
    list_free(&right);
    list_free(&sum);
    list_free(&diff);
    list_free(&scaled);
}

static void test_concat_reverse_and_find(void) {
    List left = {0};
    List right = {0};
    List concat = {0};
    List reversed = {0};
    const double left_values[] = {1.0, 2.0};
    const double right_values[] = {3.0, 4.0};
    size_t index = 0;

    assert_status(list_create(&left), LIST_SUCCESS, "list_create left");
    assert_status(list_create(&right), LIST_SUCCESS, "list_create right");
    fill_list_from_array(&left, left_values, 2);
    fill_list_from_array(&right, right_values, 2);

    assert_status(list_concat(&left, &right, &concat), LIST_SUCCESS, "list_concat should succeed");
    assert_list_value(&concat, 0, 1.0, "list_concat first element");
    assert_list_value(&concat, 3, 4.0, "list_concat last element");

    assert_status(list_reverse(&concat, &reversed), LIST_SUCCESS, "list_reverse should succeed");
    assert_list_value(&reversed, 0, 4.0, "list_reverse first element");
    assert_list_value(&reversed, 3, 1.0, "list_reverse last element");

    assert_status(list_find(&concat, 3.0, &index), LIST_SUCCESS, "list_find should succeed");
    if (index != 2) {
        fprintf(stderr, "FAIL: list_find should return index 2\n");
        exit(EXIT_FAILURE);
    }
    assert_status(list_find(&concat, 9.0, &index), LIST_ERROR_NOT_FOUND, "list_find should reject missing value");

    list_free(&left);
    list_free(&right);
    list_free(&concat);
    list_free(&reversed);
}

static void test_errors(void) {
    List left = {0};
    List right = {0};
    List result = {0};
    double value = 0.0;

    assert_status(list_create(&left), LIST_SUCCESS, "list_create left");
    assert_status(list_create(&right), LIST_SUCCESS, "list_create right");
    assert_status(list_push_back(&left, 1.0), LIST_SUCCESS, "list_push_back left");
    assert_status(list_push_back(&right, 1.0), LIST_SUCCESS, "list_push_back right");
    assert_status(list_push_back(&right, 2.0), LIST_SUCCESS, "list_push_back right second");

    assert_status(list_add(&left, &right, &result), LIST_ERROR_SIZE, "list_add should reject different sizes");
    assert_status(list_insert(&left, 3, 5.0), LIST_ERROR_BOUNDS, "list_insert should check bounds");
    assert_status(list_remove_at(&left, 5, &value), LIST_ERROR_BOUNDS, "list_remove_at should check bounds");
    assert_status(list_get(NULL, 0, &value), LIST_ERROR_NULL, "list_get should reject NULL list");
    assert_status(list_get(&left, 0, NULL), LIST_ERROR_NULL, "list_get should reject NULL value");

    list_free(&left);
    list_free(&right);
    list_free(&result);
}

int main(void) {
    test_create_push_and_free();
    test_get_set_and_fill();
    test_insert_and_remove();
    test_pop();
    test_copy();
    test_add_sub_and_scalar_mul();
    test_concat_reverse_and_find();
    test_errors();

    printf("All list tests passed.\n");
    return EXIT_SUCCESS;
}
