#include "matrix.h"

static MatrixStatus matrix_validate_access(const Matrix *matrix, size_t row, size_t col) {
    if (matrix == NULL || matrix->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (row >= matrix->rows || col >= matrix->cols) {
        return MATRIX_ERROR_BOUNDS;
    }

    return MATRIX_SUCCESS;
}

MatrixStatus matrix_set(Matrix *matrix, size_t row, size_t col, double value) {
    MatrixStatus status;

    status = matrix_validate_access(matrix, row, col);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    matrix->data[row * matrix->cols + col] = value;
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_get(const Matrix *matrix, size_t row, size_t col, double *value) {
    MatrixStatus status;

    if (value == NULL) {
        return MATRIX_ERROR_NULL;
    }

    status = matrix_validate_access(matrix, row, col);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    *value = matrix->data[row * matrix->cols + col];
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_fill(Matrix *matrix, double value) {
    size_t index;

    if (matrix == NULL || matrix->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    for (index = 0; index < matrix->rows * matrix->cols; ++index) {
        matrix->data[index] = value;
    }

    return MATRIX_SUCCESS;
}
