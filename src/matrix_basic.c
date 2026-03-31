#include "matrix.h"

static MatrixStatus matrix_validate_pair(
    const Matrix *left,
    const Matrix *right,
    Matrix *result
) {
    if (left == NULL || right == NULL || result == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (left->data == NULL || right->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (left->rows != right->rows || left->cols != right->cols) {
        return MATRIX_ERROR_SIZE;
    }

    return MATRIX_SUCCESS;
}

MatrixStatus matrix_add(const Matrix *left, const Matrix *right, Matrix *result) {
    Matrix temp;
    size_t index;
    MatrixStatus status;

    status = matrix_validate_pair(left, right, result);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_create(&temp, left->rows, left->cols);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (index = 0; index < left->rows * left->cols; ++index) {
        temp.data[index] = left->data[index] + right->data[index];
    }

    matrix_free(result);
    *result = temp;
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_sub(const Matrix *left, const Matrix *right, Matrix *result) {
    Matrix temp;
    size_t index;
    MatrixStatus status;

    status = matrix_validate_pair(left, right, result);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_create(&temp, left->rows, left->cols);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (index = 0; index < left->rows * left->cols; ++index) {
        temp.data[index] = left->data[index] - right->data[index];
    }

    matrix_free(result);
    *result = temp;
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_scalar_mul(const Matrix *matrix, double scalar, Matrix *result) {
    Matrix temp;
    size_t index;
    MatrixStatus status;

    if (matrix == NULL || result == NULL || matrix->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_create(&temp, matrix->rows, matrix->cols);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (index = 0; index < matrix->rows * matrix->cols; ++index) {
        temp.data[index] = matrix->data[index] * scalar;
    }

    matrix_free(result);
    *result = temp;
    return MATRIX_SUCCESS;
}
