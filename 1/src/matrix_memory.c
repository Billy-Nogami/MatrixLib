#include "matrix.h"

#include <stdlib.h>

static int matrix_has_valid_shape(const Matrix *matrix) {
    return matrix != NULL && matrix->rows > 0 && matrix->cols > 0;
}

MatrixStatus matrix_create(Matrix *matrix, size_t rows, size_t cols) {
    size_t elements;

    if (matrix == NULL) {
        return MATRIX_ERROR_NULL;
    }

    matrix->rows = 0;
    matrix->cols = 0;
    matrix->data = NULL;

    if (rows == 0 || cols == 0) {
        return MATRIX_ERROR_SIZE;
    }

    elements = rows * cols;
    matrix->data = (double *)calloc(elements, sizeof(double));
    if (matrix->data == NULL) {
        return MATRIX_ERROR_ALLOC;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    return MATRIX_SUCCESS;
}

void matrix_free(Matrix *matrix) {
    if (matrix == NULL) {
        return;
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
}

MatrixStatus matrix_copy(const Matrix *source, Matrix *destination) {
    Matrix temp;
    size_t index;

    if (!matrix_has_valid_shape(source) || destination == NULL || source->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (source == destination) {
        return MATRIX_SUCCESS;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    if (matrix_create(&temp, source->rows, source->cols) != MATRIX_SUCCESS) {
        return MATRIX_ERROR_ALLOC;
    }

    for (index = 0; index < source->rows * source->cols; ++index) {
        temp.data[index] = source->data[index];
    }

    matrix_free(destination);
    *destination = temp;
    return MATRIX_SUCCESS;
}
