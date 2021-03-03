#ifndef UTIL_H
#define UTIL_H

#include "gauss.h"
#define BUFF 100

Matrix* allocateMatrix(Matrix* matrix, int n);

Matrix* initMatrixFromConsole(Matrix* matrix);

Matrix* initMatrixFromFile(Matrix* matrix, char *fileName);

double** initMatrixAFromFile(FILE * fileInputStream, int n);

double* initMatrixBFromFile(FILE * fileInputStream, int n);

int getMatrixSize(FILE * fileInputStream, int* error_code);

double** createCoefficientMatrix(int);

void initMatrixAFromConsole(Matrix* matrix);

void initMatrixBFromConsole(Matrix* matrix);

int getInt(char* message);

int getIntFromFile(char* input, int* error_code);

double getDoubleFromFile(char* input, int* error_code);

double getDouble(char* message);

char * input_string();

void printCommonView(int size);

void printMatrices(const Matrix* matrix);

void printResult(const Matrix* matrix);

void freeMemory(Matrix* matrix);
#endif
