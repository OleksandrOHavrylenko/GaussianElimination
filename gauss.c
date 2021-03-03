#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gauss.h"
#include "util.h"

void solver(Matrix* matrix) {
    if (matrix == NULL) {
        printf("\tMatrix is not initialized.\n");
        return;
    }
    int singularFlag = forwardElimination(matrix);

    if(singularFlag != -1) {
        printf("\tThe Matrix is Singular.\n");

        if (matrix->B[singularFlag] > ZERO)
            printf("\tInconsistent System.");
        else
            printf("\tMay have infinitely many solutions.");
        return;
    }

    backSubstitution(matrix);

    printf("\n\tThe Gaussian forward stroke:");
    printMatrices(matrix);
    printf("\n\tThe result of the Gaussian Elimination is:");
    printResult(matrix);
}

int forwardElimination(Matrix* matrix) {
    int nextRow = 0;
    for (int column = 0; column < matrix->n; column++) {
        int maxRow = column;
        double maxValue = matrix->A[maxRow][column];

        for (int row = nextRow; row < matrix->n; ++row) {
            if (fabs(matrix->A[row][column]) > maxValue) {
                maxValue = matrix->A[row][column];
                maxRow = row;
            }
        }

        if (fabs(matrix->A[column][maxRow]) < ZERO) {
            if (isZeroRow(matrix, maxRow)) {
                return column;
            }
        } else {
            if(maxRow != column) {
                swapRow(matrix, nextRow, maxRow);
            }

            for (int i = column+1; i < matrix->n; ++i) {
                double divider = matrix->A[column][column];
                if (fabs(divider) > ZERO) {
                    double f = matrix->A[i][column] / divider;

                    for (int j = column+1; j < matrix->n; ++j) {
                        matrix->A[i][j] -= matrix->A[column][j] * f;
                    }

                    matrix->B[i] -= matrix->B[column] * f;
                }

                matrix->A[i][column] = 0;
            }
            printf("\n\tStep %d", column + 1);

            printMatrices(matrix);
            nextRow++;
        }
    }
    return -1;
}

void backSubstitution(Matrix* matrix) {
    for (int i = matrix->n - 1; i >= 0; i--) {
        matrix->X[i] = matrix->B[i];
        for (int j=i+1; j<matrix->n; j++)
        {
            matrix->X[i] -= matrix->A[i][j] * matrix->X[j];
        }
        matrix->X[i] = matrix->X[i] / matrix->A[i][i];
    }
}

void swapRow(Matrix* matrix, int row1, int row2) {
    printf("\tSwapped rows %d and %d\n", row1+1, row2+1);

    for (int k = 0; k < matrix->n; ++k) {
        double temp = matrix->A[row1][k];
        matrix->A[row1][k] = matrix->A[row2][k];
        matrix->A[row2][k] = temp;
    }

    double temp = matrix->B[row1];
    matrix->B[row1] = matrix->B[row2];
    matrix->B[row2] = temp;
}

int isZeroRow(const Matrix* matrix, int row) {
    for (int col = 0; col < matrix->n; ++col) {
        if (fabs(matrix->A[row][col]) > ZERO) {
            return 0;
        }
    }
    return 1;
}

void freeMemory(Matrix* matrix) {
    for (int i = 0; i < matrix->n; i++) {
        if (matrix->A[i]) free(matrix->A[i]);
    }

    if (matrix->A) free(matrix->A);
    if (matrix->B) free(matrix->B);
    if (matrix->X) free(matrix->X);
}