#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void assert_status(MatrixStatus actual, MatrixStatus expected, const char *message) {
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

static void assert_matrix_value(
    const Matrix *matrix,
    size_t row,
    size_t col,
    double expected,
    const char *message
) {
    double actual = 0.0;

    assert_status(matrix_get(matrix, row, col, &actual), MATRIX_SUCCESS, message);
    assert_close(actual, expected, 1e-9, message);
}

static void fill_matrix_from_array(Matrix *matrix, const double *values, size_t count) {
    size_t index;

    for (index = 0; index < count; ++index) {
        matrix->data[index] = values[index];
    }
}

static void test_create_fill_and_free(void) {
    Matrix matrix = {0};

    assert_status(matrix_create(&matrix, 2, 3), MATRIX_SUCCESS, "matrix_create should succeed");
    assert_status(matrix_fill(&matrix, 2.5), MATRIX_SUCCESS, "matrix_fill should succeed");
    assert_matrix_value(&matrix, 0, 0, 2.5, "matrix_fill should write first element");
    assert_matrix_value(&matrix, 1, 2, 2.5, "matrix_fill should write last element");
    matrix_free(&matrix);

    if (matrix.data != NULL || matrix.rows != 0 || matrix.cols != 0) {
        fprintf(stderr, "FAIL: matrix_free should reset matrix fields\n");
        exit(EXIT_FAILURE);
    }
}

static void test_set_and_get(void) {
    Matrix matrix = {0};
    double value = 0.0;

    assert_status(matrix_create(&matrix, 3, 3), MATRIX_SUCCESS, "matrix_create should succeed");
    assert_status(matrix_set(&matrix, 1, 2, 7.25), MATRIX_SUCCESS, "matrix_set should succeed");
    assert_status(matrix_get(&matrix, 1, 2, &value), MATRIX_SUCCESS, "matrix_get should succeed");
    assert_close(value, 7.25, 1e-9, "matrix_get should read stored value");
    assert_status(matrix_get(&matrix, 3, 0, &value), MATRIX_ERROR_BOUNDS, "matrix_get should check bounds");
    matrix_free(&matrix);
}

static void test_copy(void) {
    Matrix source = {0};
    Matrix copy = {0};
    const double values[] = {1.0, 2.0, 3.0, 4.0};

    assert_status(matrix_create(&source, 2, 2), MATRIX_SUCCESS, "matrix_create source");
    fill_matrix_from_array(&source, values, 4);
    assert_status(matrix_copy(&source, &copy), MATRIX_SUCCESS, "matrix_copy should succeed");
    assert_matrix_value(&copy, 0, 0, 1.0, "matrix_copy should copy first element");
    assert_matrix_value(&copy, 1, 1, 4.0, "matrix_copy should copy last element");
    matrix_free(&source);
    matrix_free(&copy);
}

static void test_add_and_sub(void) {
    Matrix left = {0};
    Matrix right = {0};
    Matrix sum = {0};
    Matrix diff = {0};
    const double left_values[] = {1.0, 2.0, 3.0, 4.0};
    const double right_values[] = {4.0, 3.0, 2.0, 1.0};

    assert_status(matrix_create(&left, 2, 2), MATRIX_SUCCESS, "matrix_create left");
    assert_status(matrix_create(&right, 2, 2), MATRIX_SUCCESS, "matrix_create right");
    fill_matrix_from_array(&left, left_values, 4);
    fill_matrix_from_array(&right, right_values, 4);

    assert_status(matrix_add(&left, &right, &sum), MATRIX_SUCCESS, "matrix_add should succeed");
    assert_status(matrix_sub(&left, &right, &diff), MATRIX_SUCCESS, "matrix_sub should succeed");

    assert_matrix_value(&sum, 0, 0, 5.0, "matrix_add first element");
    assert_matrix_value(&sum, 1, 1, 5.0, "matrix_add last element");
    assert_matrix_value(&diff, 0, 0, -3.0, "matrix_sub first element");
    assert_matrix_value(&diff, 1, 1, 3.0, "matrix_sub last element");

    matrix_free(&left);
    matrix_free(&right);
    matrix_free(&sum);
    matrix_free(&diff);
}

static void test_scalar_mul(void) {
    Matrix matrix = {0};
    Matrix result = {0};
    const double values[] = {1.0, -2.0, 3.5, 4.0};

    assert_status(matrix_create(&matrix, 2, 2), MATRIX_SUCCESS, "matrix_create for scalar");
    fill_matrix_from_array(&matrix, values, 4);
    assert_status(matrix_scalar_mul(&matrix, 2.0, &result), MATRIX_SUCCESS, "matrix_scalar_mul should succeed");

    assert_matrix_value(&result, 0, 0, 2.0, "matrix_scalar_mul first element");
    assert_matrix_value(&result, 0, 1, -4.0, "matrix_scalar_mul second element");
    assert_matrix_value(&result, 1, 0, 7.0, "matrix_scalar_mul third element");

    matrix_free(&matrix);
    matrix_free(&result);
}

static void test_matrix_mul(void) {
    Matrix left = {0};
    Matrix right = {0};
    Matrix result = {0};
    const double left_values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    const double right_values[] = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};

    assert_status(matrix_create(&left, 2, 3), MATRIX_SUCCESS, "matrix_create left for mul");
    assert_status(matrix_create(&right, 3, 2), MATRIX_SUCCESS, "matrix_create right for mul");
    fill_matrix_from_array(&left, left_values, 6);
    fill_matrix_from_array(&right, right_values, 6);

    assert_status(matrix_mul(&left, &right, &result), MATRIX_SUCCESS, "matrix_mul should succeed");
    assert_matrix_value(&result, 0, 0, 58.0, "matrix_mul element 0 0");
    assert_matrix_value(&result, 0, 1, 64.0, "matrix_mul element 0 1");
    assert_matrix_value(&result, 1, 0, 139.0, "matrix_mul element 1 0");
    assert_matrix_value(&result, 1, 1, 154.0, "matrix_mul element 1 1");

    matrix_free(&left);
    matrix_free(&right);
    matrix_free(&result);
}

static void test_transpose(void) {
    Matrix matrix = {0};
    Matrix transposed = {0};
    const double values[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    assert_status(matrix_create(&matrix, 2, 3), MATRIX_SUCCESS, "matrix_create for transpose");
    fill_matrix_from_array(&matrix, values, 6);
    assert_status(matrix_transpose(&matrix, &transposed), MATRIX_SUCCESS, "matrix_transpose should succeed");

    assert_matrix_value(&transposed, 0, 0, 1.0, "matrix_transpose element 0 0");
    assert_matrix_value(&transposed, 1, 0, 2.0, "matrix_transpose element 1 0");
    assert_matrix_value(&transposed, 2, 1, 6.0, "matrix_transpose element 2 1");

    matrix_free(&matrix);
    matrix_free(&transposed);
}

static void test_determinant_2x2(void) {
    Matrix matrix = {0};
    double determinant = 0.0;
    const double values[] = {4.0, 7.0, 2.0, 6.0};

    assert_status(matrix_create(&matrix, 2, 2), MATRIX_SUCCESS, "matrix_create determinant 2x2");
    fill_matrix_from_array(&matrix, values, 4);
    assert_status(
        matrix_determinant(&matrix, &determinant),
        MATRIX_SUCCESS,
        "matrix_determinant should succeed for 2x2"
    );
    assert_close(determinant, 10.0, 1e-9, "matrix_determinant should compute 2x2 determinant");
    matrix_free(&matrix);
}

static void test_determinant_3x3(void) {
    Matrix matrix = {0};
    double determinant = 0.0;
    const double values[] = {
        6.0, 1.0, 1.0,
        4.0, -2.0, 5.0,
        2.0, 8.0, 7.0
    };

    assert_status(matrix_create(&matrix, 3, 3), MATRIX_SUCCESS, "matrix_create determinant 3x3");
    fill_matrix_from_array(&matrix, values, 9);
    assert_status(
        matrix_determinant(&matrix, &determinant),
        MATRIX_SUCCESS,
        "matrix_determinant should succeed for 3x3"
    );
    assert_close(determinant, -306.0, 1e-9, "matrix_determinant should compute 3x3 determinant");
    matrix_free(&matrix);
}

static void test_size_errors(void) {
    Matrix left = {0};
    Matrix right = {0};
    Matrix result = {0};
    double determinant = 0.0;

    assert_status(matrix_create(&left, 2, 3), MATRIX_SUCCESS, "matrix_create left for size errors");
    assert_status(matrix_create(&right, 2, 2), MATRIX_SUCCESS, "matrix_create right for size errors");

    assert_status(
        matrix_add(&left, &right, &result),
        MATRIX_ERROR_SIZE,
        "matrix_add should reject different sizes"
    );
    assert_status(
        matrix_mul(&left, &right, &result),
        MATRIX_ERROR_SIZE,
        "matrix_mul should reject incompatible dimensions"
    );
    assert_status(
        matrix_determinant(&left, &determinant),
        MATRIX_ERROR_NOT_SQUARE,
        "matrix_determinant should reject non-square matrix"
    );

    matrix_free(&left);
    matrix_free(&right);
    matrix_free(&result);
}

int main(void) {
    test_create_fill_and_free();
    test_set_and_get();
    test_copy();
    test_add_and_sub();
    test_scalar_mul();
    test_matrix_mul();
    test_transpose();
    test_determinant_2x2();
    test_determinant_3x3();
    test_size_errors();

    printf("All matrix tests passed.\n");
    return EXIT_SUCCESS;
}
