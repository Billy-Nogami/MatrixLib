#include "matrix.h"

#include <math.h>

static void matrix_swap_rows(Matrix *matrix, size_t first_row, size_t second_row) {
    size_t col;

    for (col = 0; col < matrix->cols; ++col) {
        double temp = matrix->data[first_row * matrix->cols + col];
        matrix->data[first_row * matrix->cols + col] =
            matrix->data[second_row * matrix->cols + col];
        matrix->data[second_row * matrix->cols + col] = temp;
    }
}

MatrixStatus matrix_mul(const Matrix *left, const Matrix *right, Matrix *result) {
    Matrix temp;
    size_t row;
    size_t col;
    size_t k;
    MatrixStatus status;

    if (left == NULL || right == NULL || result == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (left->data == NULL || right->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (left->cols != right->rows) {
        return MATRIX_ERROR_SIZE;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_create(&temp, left->rows, right->cols);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (row = 0; row < left->rows; ++row) {
        for (col = 0; col < right->cols; ++col) {
            double sum = 0.0;
            for (k = 0; k < left->cols; ++k) {
                sum += left->data[row * left->cols + k] *
                       right->data[k * right->cols + col];
            }
            temp.data[row * temp.cols + col] = sum;
        }
    }

    matrix_free(result);
    *result = temp;
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_transpose(const Matrix *matrix, Matrix *result) {
    Matrix temp;
    size_t row;
    size_t col;
    MatrixStatus status;

    if (matrix == NULL || result == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (matrix->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_create(&temp, matrix->cols, matrix->rows);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (row = 0; row < matrix->rows; ++row) {
        for (col = 0; col < matrix->cols; ++col) {
            temp.data[col * temp.cols + row] = matrix->data[row * matrix->cols + col];
        }
    }

    matrix_free(result);
    *result = temp;
    return MATRIX_SUCCESS;
}

MatrixStatus matrix_determinant(const Matrix *matrix, double *result) {
    Matrix temp;
    size_t pivot_index;
    size_t row;
    size_t col;
    int swap_count = 0;
    double determinant = 1.0;
    const double epsilon = 1e-9;
    MatrixStatus status;

    if (matrix == NULL || result == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (matrix->data == NULL) {
        return MATRIX_ERROR_NULL;
    }

    if (matrix->rows != matrix->cols) {
        return MATRIX_ERROR_NOT_SQUARE;
    }

    temp.rows = 0;
    temp.cols = 0;
    temp.data = NULL;

    status = matrix_copy(matrix, &temp);
    if (status != MATRIX_SUCCESS) {
        return status;
    }

    for (pivot_index = 0; pivot_index < temp.rows; ++pivot_index) {
        size_t best_row = pivot_index;

        for (row = pivot_index + 1; row < temp.rows; ++row) {
            double candidate = fabs(temp.data[row * temp.cols + pivot_index]);
            double current = fabs(temp.data[best_row * temp.cols + pivot_index]);
            if (candidate > current) {
                best_row = row;
            }
        }

        if (fabs(temp.data[best_row * temp.cols + pivot_index]) < epsilon) {
            *result = 0.0;
            matrix_free(&temp);
            return MATRIX_SUCCESS;
        }

        if (best_row != pivot_index) {
            matrix_swap_rows(&temp, pivot_index, best_row);
            swap_count++;
        }

        for (row = pivot_index + 1; row < temp.rows; ++row) {
            double factor = temp.data[row * temp.cols + pivot_index] /
                            temp.data[pivot_index * temp.cols + pivot_index];
            for (col = pivot_index; col < temp.cols; ++col) {
                temp.data[row * temp.cols + col] -=
                    factor * temp.data[pivot_index * temp.cols + col];
            }
        }
    }

    for (pivot_index = 0; pivot_index < temp.rows; ++pivot_index) {
        determinant *= temp.data[pivot_index * temp.cols + pivot_index];
    }

    if (swap_count % 2 != 0) {
        determinant = -determinant;
    }

    *result = determinant;
    matrix_free(&temp);
    return MATRIX_SUCCESS;
}
