#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <locale.h>

float **createCoefficientMatrix(int);

void freeCoefficientMatrix(float **, int);

void initCoefficientMatrix(float **, int);

void printCoefficientMatrix(float **, int);

void prinMatrixes(float **, const float *, int);

void printResult(const float *, int n);

int solveEquations(float **MatA, float *MatB, int n);

void backSubstitution(int n, float **MatA, float *MatB, float *X);

void initCoefMatrix(float **MatA, int n);

void initRHSMatrix(float *MatB, int n);

int isZeroRow(float **pDouble, int row, int size);

bool canSolve(float **M, float *, int size);

void printCommomView(int size);

int main() {
    setlocale(LC_CTYPE, "ukr");
    int n;
    printf("Введіть кількість невідомих змінних : ");
    scanf("%d", &n);
    printf("Загальний вигляд системи лінійних рівнянь з %d невідомими має наступний вигляд : \n");
    printCommomView(n);
    float **A = createCoefficientMatrix(n);
//    initCoefficientMatrix(A, n);
//    printCoefficientMatrix(MatA, n);
    float *B = (float *) malloc(n * sizeof(float));
    float *X = (float *) malloc(n * sizeof(float));

    initCoefMatrix(A, n);
    initRHSMatrix(B, n);
    printf("\nПочатковий вигляд системи лінійних рівнянь:");
    prinMatrixes(A, B, n);

    int isSolved = solveEquations(A, B, n);
    if(isSolved) {
        backSubstitution(n, A, B, X);

        printf("\nПрямий хід метода Гауса завершено:");
        prinMatrixes(A, B, n);
        printf("\nКорені системи лінійних рівнянь наступні:");
        printResult(X, n);
    }

    freeCoefficientMatrix(A, n);
    free(B);
    free(X);
    return 0;
}

void printCommomView(int size) {
    printf("===============================\n");
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(j == size - 1) {
                printf("A[%d][%d]*x[%d]%s", i, j, j, " ");
            } else {
                printf("A[%d][%d]*x[%d]%s", i, j, j, " + ");
            }
        }
        printf("= B[%d]\n", i);
    }
    printf("===============================\n");
}void initRHSMatrix(float *B, int n) {
    printf("\n Введіть з клавіатури елементи матриці B : \n");
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("B[%d] = ", i);
        scanf("%f", &B[i]);
    }
}

void initCoefMatrix(float **A, int n) {
    printf("\nВведіть з клавіатури елементи матриці А : \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("A[%d][%d] =%s", i, j, " ");
            scanf("%f", &A[i][j]);
        }
    }
}

void backSubstitution(int n, float **A, float *B, float *X) {// back substitution starting with last variable
    X[n - 1] = B[n - 1];
    for (int i = n - 2; i >= 0; i--) {
// Sum up ith row using values of X already determined
        float sum = 0.0f;
        for (int j = i + 1; j < n; j++) {
            sum = sum + A[i][j] * X[j];
        }
        X[i] = B[i] - sum;
    }
}

int solveEquations(float **A, float *B, int n) {
    for (int i = 0; i < n; i++) {
        float divider = A[i][i];
        if(divider != 0) {
            A[i][i] = 1.0f;
// divide all values in the row by the divisor
// to recalculate all coefficients in that row
            for (int j = i + 1; j < n; j++) {
                A[i][j] = A[i][j] / divider;
            }
//Also divide the corresponding RHS element
            B[i] = B[i] / divider;
// now replace subsequent rows, by subtracting the
// appropriate portion of the ith equation from it
            if (i + 1 < n) {
                for (int k = i + 1; k < n; k++) {
                    float factor = A[k][i];
                    A[k][i] = 0.0f;
                    for (int j = i + 1; j < n; j++) {
                        A[k][j] = A[k][j] - factor * A[i][j];
                    }
                    B[k] = B[k] - factor * B[i];
                }
            }
            printf("\nКрок %d", i + 1);
            prinMatrixes(A, B, n);
            if(!canSolve(A, B, n)) {
                return 0;
            }
        }
    }
    return 1;
}

bool canSolve(float **A, float* B, int size) {
    for (int row = 0; row < size; ++row) {
        if(isZeroRow(A, row, size) && B[row] != 0) {
            printf("\nNo solvings because bad rank\n");
            return 0;
        }
    }
    return 1;
}

int isZeroRow(float **A, int row, int size) {
    for (int col = 0; col < size; ++col) {
        if(A[row][col]!= 0){
            return 0;
        }
    }
    return 1;
}

void prinMatrixes(float **A, const float *B, int size) {
    printf("\n======================================\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j == 0) {
                printf("%2s", "|");
            }
            printf("%7.2f ", A[i][j]);
            if (j == size - 1) {
                printf("%s", "|");
            }
        }
        printf("%8s", "|");
        printf("%7.2f", B[i]);
        printf("%s\n", "|");
    }
    printf("======================================\n");
}

void printResult(const float *X, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("X[%d] = %5.2f;\n", i, X[i]);
    }
}

float **createCoefficientMatrix(int n) {
    float **A = (float **) malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        A[i] = (float *) malloc(n * sizeof(float));
    }
    return A;
}

void freeCoefficientMatrix(float **A, int size) {
    for (int i = 0; i < size; i++) {
        free(A[i]);
    }
    free(A);
}


void initCoefficientMatrix(float **A, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            A[i][j] = i*2 + j + 1;
        }
    }
}

void printCoefficientMatrix(float **A, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%5.2f", A[i][j]);
        }
        printf("\n");
    }
}