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

    initMatrixA(matrix);
    initMatrixB(matrix);

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
    int n;
    FILE * matrixFile;
    matrixFile = fopen(fileName, "r");
    if(matrixFile == NULL)
    {
        printf("Unable open the file.\n");
        exit(EXIT_FAILURE);
    }
    fscanf(matrixFile, "%d", &n);

    double** A = createCoefficientMatrix(n);

    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            fscanf(matrixFile, "%lf", &A[i][j]);
        }
    }

    double* B = (double *) malloc(n * sizeof(double));
    for(int i = 0; i < n; i++){
        fscanf(matrixFile, "%lf", &B[i]);
    }

    fclose(matrixFile);

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

double** createCoefficientMatrix(int n) {
    double **A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    return A;
}

void initMatrixA(Matrix* matrix) {
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

void initMatrixB(Matrix* matrix) {
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
    char *endptr;
    errno = 0;
    long l = strtol(input, &endptr, 10);
    while (errno || *endptr != '\0' || input == endptr || l < 1 || l > INT_MAX) {
        if(l < 1) {
            printf("The number should be a positive integer\n");
        } else {
            printf("Not a integer, please try again\n");
        }
        printf("%s", message);
        free(input);
        input = input_string();
        l = strtol(input, &endptr, 10);
    }
    free(input);
    return l;
}

double getDouble(char* message) {
    char* input = input_string();
    char *endptr;
    errno = 0;
    double l = strtod(input, &endptr);
    while (errno || *endptr != '\0' || input == endptr || l < INT_MIN || l > INT_MAX) {
        printf("Not a number, please try again\n");
        printf("%s", message);
        free(input);
        input = input_string();
        l = strtod(input, &endptr);
    }
    free(input);
    return l;
}

char * input_string()
{
    char buff[BUFF];
    char * input = NULL;
    if (fgets(buff, BUFF, stdin))
    {
        buff[strcspn(buff, "\n")] = 0;
        input = (char *) malloc(strlen(buff)+1);
        if (!input) exit(1);
        strcpy(input, buff);
        return input;
    }
    return "Invalid input";
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

