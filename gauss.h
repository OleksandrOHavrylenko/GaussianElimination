#ifndef GAUSS_H
#define GAUSS_H

#define ZERO 10e-7

typedef struct Matrix {
    int n;
    double **A;
    double *B;
    double *X;
} Matrix;

void solver(Matrix* matrix);

int forwardElimination(Matrix* matrix);

int backSubstitution(Matrix* matrix);

int isSingular(int singularFlag, Matrix* matrix);

void swapRow(Matrix* matrix, int row1, int row2);

int isZeroRow(const Matrix* matrix, int row);

#endif
