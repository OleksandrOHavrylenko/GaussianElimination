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


Matrix *initMatrixFromConsole();

Matrix* initMatrixFromFile(char *fileName);

double **createCoefficientMatrix(int);

void initMatrixA(Matrix* matrix);

void initMatrixB(Matrix* matrix);

int forwardElimination(Matrix* matrix);

void backSubstitution(Matrix* matrix);

void swapRow(Matrix* matrix, int row1, int row2);

int isZeroRow(const Matrix* matrix, int row);

void printCommonView(int size);

void printMatrices(const Matrix* matrix);

void printResult(const Matrix* matrix);

void freeMemory(Matrix* matrix);

#endif
