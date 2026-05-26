#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    size_t rows;
    size_t cols;
    double *data;
} Matrix;

typedef enum {
    MATRIX_SUCCESS = 0,
    MATRIX_ERROR_NULL = 1,
    MATRIX_ERROR_ALLOC = 2,
    MATRIX_ERROR_BOUNDS = 3,
    MATRIX_ERROR_SIZE = 4,
    MATRIX_ERROR_NOT_SQUARE = 5
} MatrixStatus;

MatrixStatus matrix_create(Matrix *matrix, size_t rows, size_t cols);
void matrix_free(Matrix *matrix);

MatrixStatus matrix_set(Matrix *matrix, size_t row, size_t col, double value);
MatrixStatus matrix_get(const Matrix *matrix, size_t row, size_t col, double *value);
MatrixStatus matrix_fill(Matrix *matrix, double value);
MatrixStatus matrix_copy(const Matrix *source, Matrix *destination);

MatrixStatus matrix_add(const Matrix *left, const Matrix *right, Matrix *result);
MatrixStatus matrix_sub(const Matrix *left, const Matrix *right, Matrix *result);
MatrixStatus matrix_scalar_mul(const Matrix *matrix, double scalar, Matrix *result);

MatrixStatus matrix_mul(const Matrix *left, const Matrix *right, Matrix *result);
MatrixStatus matrix_transpose(const Matrix *matrix, Matrix *result);
MatrixStatus matrix_determinant(const Matrix *matrix, double *result);

#endif
