#ifndef UTIL_H
#define UTIL_H

#include "gauss.h"

Matrix *initMatrixFromConsole();

Matrix* initMatrixFromFile(char *fileName);

double **createCoefficientMatrix(int);

void initMatrixA(Matrix* matrix);

void initMatrixB(Matrix* matrix);

void printCommonView(int size);

void printMatrices(const Matrix* matrix);

void printResult(const Matrix* matrix);

void freeMemory(Matrix* matrix);
#endif
