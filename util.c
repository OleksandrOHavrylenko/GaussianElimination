#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "gauss.h"

Matrix* initMatrixFromConsole(Matrix* matrix) {
    int n = 0;
    char *message = (char*)malloc(45 * sizeof(char));
    sprintf(message,"Enter the number of unknown variables : ");
    printf("%s", message);
    n = getInt(message);
    printf("The general view of the system of linear equations with %d variables : \n", n);
    printCommonView(n);

    matrix = allocateMatrix(matrix, n);

    initMatrixAFromConsole(matrix);
    initMatrixBFromConsole(matrix);

    printf("\nThe starting view of the system of linear equations:");
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

Matrix* initMatrixFromFile(Matrix* matrix, char *fileName) {
    FILE * fileInputStream;
    fileInputStream = fopen(fileName, "r");
    if(fileInputStream == NULL)
    {
        printf("\nUnable open the file. Please enter enter other file \n");
        return NULL;
    }
    int n = getMatrixSize(fileInputStream);
    if (n < 0) {
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

    printf("The general view of the system of linear equations with %d variables : \n", matrix->n);
    printCommonView(matrix->n);

    printf("\nThe starting view of the system of linear equations:");
    printMatrices(matrix);

    return matrix;
}

double** initMatrixAFromFile(FILE * fileInputStream, int n) {
    char buffer[BUFF];
    char *record = NULL, *line = NULL;
    int i = 0, j = 0;

    double** A = createCoefficientMatrix(n);

    while (i < n && (line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL) {
            int error_code = 0;
            double value = getDoubleFromFile(record, i, j, &error_code);
            if (error_code != 0) {
                return NULL;
            }
            A[i][j++] = value;
            record = strtok(NULL, ",");
        }
        if (j != n) {
            printf("Incorrect file in line: %d\n" , i+2);
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

    while ((line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL) {
            int error_code = 0;
            double value = getDoubleFromFile(record, j, j, &error_code);
            if (error_code != 0) {
                return NULL;
            }
            B[j++] = value;
            record = strtok(NULL, ",");
        }
        if (j != n) {
            printf("Incorrect file in line: %d\n" , i+2);
            return NULL;
        }
        i++;
        j = 0;
    }
    return B;
}



int getMatrixSize(FILE * fileInputStream) {
    char buffer[BUFF];
    char *record = NULL, *line = NULL;
    int pos = 0;
    int n = -1;
    if ((line = fgets(buffer, sizeof(buffer), fileInputStream)) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        record = strtok(line, ",");
        while (record != NULL) {
            if (pos > 0) {
                printf("Incorrect file in line: 1\n");
                return -1;
            }
            n = getIntFromFile(line);
            record = strtok(NULL, ",");
            pos++;
        }
    }
    return n;
}

double** createCoefficientMatrix(int n) {
    double **A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    return A;
}

void initMatrixAFromConsole(Matrix* matrix) {
    printf("\nEnter the elements of Matrix A : \n");
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            char *message = (char*)malloc(10 * sizeof(char));
            sprintf(message,"A[%d][%d] =%s", i+1, j+1, " ");
            printf("%s", message);
            matrix->A[i][j] = getDouble(message);
        }
    }
}

void initMatrixBFromConsole(Matrix* matrix) {
    printf("\n Enter the elements of Matrix B : \n");
    printf("\n");
    for (int i = 0; i < matrix->n; i++) {
        char *message = (char*)malloc(10 * sizeof(char));
        sprintf(message,"B[%d] = ", i+1);
        printf("%s", message);
        matrix->B[i] = getDouble(message);
    }
}

int getInt(char* message) {
    char* input = input_string();
    char *endPtr = NULL;
    errno = 0;
    long value = strtol(input, &endPtr, 10);
    while (errno || *endPtr != '\0' || input == endPtr || value < 1 || value > INT_MAX) {
        if(value < 1) {
            printf("The number should be a positive integer\n");
        } else {
            printf("Not a integer, please try again\n");
        }
        printf("%s", message);
        if (input) free(input);
        input = input_string();
        value = strtol(input, &endPtr, 10);
    }
    if (input) free(input);
    return value;
}

int getIntFromFile(char* input) {
    char *endPtr = NULL;
    errno = 0;
    long value = strtol(input, &endPtr, 10);
    while (errno || *endPtr != '\0' || input == endPtr || value < 1 || value > INT_MAX) {
        printf("Incorrect file in line: 1\n");
        if(value < 1) {
            printf("The number should be a positive integer\n");
        } else {
            printf("Not a integer\n");
        }
        return -1;
    }
    return value;
}

double getDoubleFromFile(char* input, int line, int position, int* error_code) {
    char *endPtr = NULL;
    errno = 0;
    double value = strtod(input, &endPtr);
    if (errno || *endPtr != '\0' || input == endPtr) {
        printf("\nNot a number at line: %d, position: %d \n", line+2, position+1);
        *error_code = -1;
        return value;
    }
    return value;
}

double getDouble(char* message) {
    char* input = input_string();
    char *endPtr;
    errno = 0;
    double value = strtod(input, &endPtr);
    while (errno || *endPtr != '\0' || input == endPtr || value < INT_MIN || value > INT_MAX) {
        printf("Not a number, please try again\n");
        printf("%s", message);
        if (input) free(input);
        input = input_string();
        value = strtod(input, &endPtr);
    }
    if (input) free(input);
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
    printf("===============================\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j == size - 1) {
                printf("A[%d][%d]*x[%d]%s", i+1, j+1, j+1, " ");
            } else {
                printf("A[%d][%d]*x[%d]%s", i+1, j+1, j+1, " + ");
            }
        }
        printf("= B[%d]\n", i+1);
    }
    printf("===============================\n");
}

void printMatrices(const Matrix* matrix) {
    printf("\n======================================\n");
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            if (j == 0) {
                printf("%2s", "|");
            }
            printf("%7.2f ", matrix->A[i][j]);
            if (j == matrix->n - 1) {
                printf("%s", "|");
            }
        }
        printf("%8s", "|");
        printf("%7.2f", matrix->B[i]);
        printf("%s\n", "|");
    }
    printf("======================================\n");
}

void printResult(const Matrix* matrix) {
    printf("\n======================================\n");
    for (int i = 0; i < matrix->n; i++) {
        printf("X[%d] = %5.2f;\n", i+1, matrix->X[i]);
    }
}

