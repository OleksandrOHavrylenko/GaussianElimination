#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "gauss.h"

Matrix* initMatrixFromConsole(Matrix* matrix) {
    int n = 0;
    char *message = (char*)malloc(45 * sizeof(char));
    sprintf(message,"\n\n\tEnter the number of unknown variables : ");
    printf("%s", message);
    n = getIntFromConsole(message);
    printf("\n\tThe general view of the system of linear equations with %d variables : \n", n);
    printCommonView(n);

    matrix = allocateMatrix(matrix, n);

    initMatrixAFromConsole(matrix);
    initMatrixBFromConsole(matrix);

    printf("\n\tThe starting view of the system of linear equations:");
    printMatrices(matrix);
    return matrix;
}

Matrix* initMatrixFromFile(Matrix* matrix, char *fileName) {
    FILE * fileInputStream;
    fileInputStream = fopen(fileName, "r");
    if(fileInputStream == NULL)
    {
        printf("\n\tUnable open the file. Please enter other filename \n");
        return NULL;
    }
    int error_code = 0;
    int n = getMatrixSize(fileInputStream, &error_code);
    if (error_code) {
        return NULL;
    }
    double** A = initMatrixAFromFile(fileInputStream, n);

    if (A == NULL) {
        return NULL;
    }

    double* B = initMatrixBFromFile(fileInputStream, n);
    if (B == NULL) {
        return NULL;
    }

    fclose(fileInputStream);

    matrix = allocateMatrix(matrix, n);
    matrix->n = n;
    matrix->A = A;
    matrix->B = B;

    printf("\n\n\tThe general view of the system of linear equations with %d variables : \n", matrix->n);
    printCommonView(matrix->n);

    printf("\n\tThe starting view of the system of linear equations:");
    printMatrices(matrix);

    return matrix;
}

Matrix* allocateMatrix(Matrix* matrix, int n) {
    matrix->A = createCoefficientMatrix(n);
    matrix->B = (double *) malloc(n * sizeof(double));
    matrix->X = (double *) malloc(n * sizeof(double));
    matrix->n = n;
    return matrix;
}

void initMatrixAFromConsole(Matrix* matrix) {
    printf("\n\tEnter the elements of Matrix A : \n");
    char *message = (char*)malloc(10 * sizeof(char));
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            sprintf(message,"\tA[%d][%d] =%s", i+1, j+1, " ");
            printf("%s", message);
            matrix->A[i][j] = getDoubleConsole(message);
        }
    }
    if (message) free(message);
}

void initMatrixBFromConsole(Matrix* matrix) {
    printf("\n\tEnter the elements of Matrix B : \n");
    printf("\n");
    char *message = (char*)malloc(10 * sizeof(char));
    for (int i = 0; i < matrix->n; i++) {
        sprintf(message,"\tB[%d] = ", i+1);
        printf("%s", message);
        matrix->B[i] = getDoubleConsole(message);
    }
    if (message) free(message);
}

int getMatrixSize(FILE * fileInputStream, int* error_code) {
    char buffer[BUFF];
    char *record = NULL, *line = NULL;
    int pos = 0;
    int n = -1;
    if ((line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL && !*error_code) {
            if (pos > 0) {
                printf("\tIncorrect file in line: 1\n");
                *error_code = -1;
                return n;
            }
            n = getIntFromFile(line, error_code);
            record = strtok(NULL, ",");
            pos++;
        }
    }
    return n;
}

double** initMatrixAFromFile(FILE * fileInputStream, int n) {
    char buffer[BUFF];
    char *record = NULL, *line = NULL;
    int i = 0, j = 0;

    double** A = createCoefficientMatrix(n);

    int error_code = 0;
    while (i < n && (line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL && !error_code) {
            if (j > n-1) {
                printf("\n\tIncorrect file in line: %d\n", i+1);
                error_code = -1;
                return NULL;
            }
            double value = getDoubleFromFile(record, &error_code);
            if (error_code) {
                printf("\n\tNot a number at line: %d, position: %d \n", i+1, j+1);
                return NULL;
            }
            A[i][j++] = value;
            record = strtok(NULL, ",");
        }
        if (j != n) {
            printf("\tIncorrect file in line: %d\n" , i+2);
            return NULL;
        }
        i++;
        j=0;
    }
    return A;
}

double* initMatrixBFromFile(FILE * fileInputStream, int n) {
    char buffer[BUFF];
    char *record = NULL, *line = NULL;
    int i = n, j = 0;

    double* B = (double *) malloc(n * sizeof(double));

    int error_code = 0;
    while ((line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL && !error_code) {
            if (j > n-1) {
                printf("\tIncorrect file in line: %d\n", i+1);
                error_code = -1;
                return NULL;
            }
            double value = getDoubleFromFile(record, &error_code);
            if (error_code) {
                printf("\n\tNot a number at line: %d, position: %d \n", i+1, j+1);
                return NULL;
            }
            B[j++] = value;
            record = strtok(NULL, ",");
        }
        if (j != n) {
            printf("\tIncorrect file in line: %d\n" , i+2);
            return NULL;
        }
        i++;
        j = 0;
    }
    return B;
}

double** createCoefficientMatrix(int n) {
    double **A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    return A;
}

int getIntFromConsole(char* message) {
    char* input = input_string();
    char *endPtr = NULL;
    errno = 0;
    long value = strtol(input, &endPtr, 10);
    while (errno || *endPtr != '\0' || input == endPtr || value < 1 || value > INT_MAX) {
        if(value < 1) {
            printf("\tThe number should be a positive integer\n");
        } else {
            printf("\tNot a integer, please try again\n");
        }
        printf("%s", message);
        if (input) free(input);
        input = input_string();
        value = strtol(input, &endPtr, 10);
    }
    if (input) free(input);
    return value;
}

int getIntFromFile(char* input, int* error_code) {
    char *endPtr = NULL;
    errno = 0;
    long value = strtol(input, &endPtr, 10);
    while (errno || *endPtr != '\0' || input == endPtr || value < 1 || value > INT_MAX) {
        printf("\tIncorrect file in line: 1\n");
        if(value < 1) {
            printf("\tThe number should be a positive integer\n");
        } else {
            printf("\tNot a integer\n");
        }
        *error_code = -1;
        return value;
    }
    return value;
}

double getDoubleConsole(char* message) {
    char* input = input_string();
    char *endPtr;
    errno = 0;
    double value = strtod(input, &endPtr);
    while (errno || *endPtr != '\0' || input == endPtr || value < INT_MIN || value > INT_MAX) {
        printf("\tNot a number, please try again\n");
        printf("%s", message);
        if (input) free(input);
        input = input_string();
        value = strtod(input, &endPtr);
    }
    if (input) free(input);
    return value;
}

double getDoubleFromFile(char* input, int* error_code) {
    char *endPtr = NULL;
    errno = 0;
    double value = strtod(input, &endPtr);
    if (errno || *endPtr != '\0' || input == endPtr) {
        *error_code = -1;
        return value;
    }
    return value;
}

char * input_string()
{
    char buff[BUFF];
    char * input = NULL;
    if (fgets(buff, BUFF, stdin))
    {
        buff[strcspn(buff, "\n")] = 0;
        input = (char *) malloc(strlen(buff)+1);
        if (!input) {
            return NULL;
        }
        strcpy(input, buff);
        return input;
    }
    return NULL;
}

void printCommonView(int size) {
    printf("\t====================================================================\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(!j) {
                printf("\t");
            }
            if (j == size - 1) {
                printf("A[%d][%d]*x[%d]%s", i+1, j+1, j+1, " ");
            } else {
                printf("A[%d][%d]*x[%d]%s", i+1, j+1, j+1, " + ");
            }
        }
        printf("= B[%d]\n", i+1);
    }
    printf("\t====================================================================\n");
}

void printMatrices(const Matrix* matrix) {
    printf("\n\t======================================\n");
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            if (j == 0) {
                printf("\t%2s", "|");
            }
            printf("%7.2f ", matrix->A[i][j]);
            if (j == matrix->n - 1) {
                printf("%s", "|");
            }
        }
        printf("\t%8s", "|");
        printf("%7.2f", matrix->B[i]);
        printf("%s\n", "|");
    }
    printf("\t======================================\n");
}

void printResult(const Matrix* matrix) {
    printf("\n\t======================================\n");
    for (int i = 0; i < matrix->n; i++) {
        printf("\n\tX[%d] = %5.2f;\n", i+1, matrix->X[i]);
    }
}

