#ifndef UTIL_H
#define UTIL_H

#include "gauss.h"
#define BUFF 100

Matrix* allocateMatrix(Matrix* matrix, int n);

Matrix* initMatrixFromConsole(Matrix* matrix);

Matrix* initMatrixFromFile(Matrix* matrix, char *fileName);

double** createCoefficientMatrix(int);

void initMatrixA(Matrix* matrix);

void initMatrixB(Matrix* matrix);

int getInt(char* message);

double getDouble(char* message);

char * input_string();

void printCommonView(int size);

void printMatrices(const Matrix* matrix);

void printResult(const Matrix* matrix);

void freeMemory(Matrix* matrix);
#endif
