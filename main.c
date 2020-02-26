#include<stdio.h>
#include<stdlib.h>

float **createCoefficientMatrix(int);

void freeCoefficientMatrix(float **, int);

void initCoefficientMatrix(float **, int);

void printCoefficientMatrix(float **, int);

void prinMatrixes(float **, const float *, int);

void printResult(const float *, int n);

void solveEquations(float **MatA, float *MatB, int n);

void backSubstitution(int n, float **MatA, float *MatB, float *X);

void initCoefMatrix(float **MatA, int n);

void initRHSMatrix(float *MatB, int n);

int main() {
    int n;
    printf("Enter the number of unknown variables: ");
    scanf("%d", &n);
    float **MatA = createCoefficientMatrix(n);
    initCoefficientMatrix(MatA, n);
//    printCoefficientMatrix(MatA, n);
    float *MatB = (float *) malloc(n * sizeof(float));
    float *X = (float *) malloc(n * sizeof(float));

//    initCoefMatrix(MatA, n);
    initRHSMatrix(MatB, n);
    prinMatrixes(MatA, MatB, n);

    solveEquations(MatA, MatB, n);
    backSubstitution(n, MatA, MatB, X);

    prinMatrixes(MatA, MatB, n);
    printResult(X, n);

    freeCoefficientMatrix(MatA, n);
    free(MatB);
    free(X);
    return 0;
}

void initRHSMatrix(float *MatB, int n) {
    for (int i = 0; i < n; i++) {
        printf("MatB[%d]", i);
        scanf("%f", &MatB[i]);
    }
}

void initCoefMatrix(float **MatA, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("MatA[%d][%d]", i, j);
            scanf("%f", &MatA[i][j]);
        }
    }
}

void backSubstitution(int n, float **MatA, float *MatB, float *X) {// back substitution starting with last variable
    X[n - 1] = MatB[n - 1];
    for (int i = n - 2; i >= 0; i--) {
// Sum up ith row using values of X already determined
        float sum = 0.0f;
        for (int j = i + 1; j < n; j++) {
            sum = sum + MatA[i][j] * X[j];
        }
        X[i] = MatB[i] - sum;
    }
}

void solveEquations(float **MatA, float *MatB, int n) {//Gauss elimination
    for (int i = 0; i < n; i++) {
        float divider = MatA[i][i];
        MatA[i][i] = 1.0f;
// divide all values in the row by the divisor
// to recalculate all coefficients in that row
        for (int j = i + 1; j < n; j++) {
            MatA[i][j] = MatA[i][j] / divider;
        }
//Also divide the corresponding RHS element
        MatB[i] = MatB[i] / divider;
// now replace subsequent rows, by subtracting the
// appropriate portion of the ith equation from it
        if (i + 1 < n) {
            for (int k = i + 1; k < n; k++) {
                float factor = MatA[k][i];
                MatA[k][i] = 0.0f;
                for (int j = i + 1; j < n; j++) {
                    MatA[k][j] = MatA[k][j] - factor * MatA[i][j];
                }
                MatB[k] = MatB[k] - factor * MatB[i];
            }
        }
        prinMatrixes(MatA, MatB, n);
    }
}

void prinMatrixes(float **M, const float *RHS, int size) {
    printf("\n======================================\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j == 0) {
                printf("%2s", "|");
            }
            printf("%5.2f ", M[i][j]);
            if (j == size - 1) {
                printf("%s", "|");
            }
        }
        printf("%8s", "|");
        printf("%6.2f", RHS[i]);
        printf("%s\n", "|");
    }
    printf("======================================\n");
}

void printResult(const float *X, int n) {
    for (int i = 0; i < n; i++) {
        printf("X[%d] = %5.2f;\n", i, X[i]);
    }
}

float **createCoefficientMatrix(int n) {
    float **matrix = (float **) malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (float *) malloc(n * sizeof(float));
    }
    return matrix;
}

void freeCoefficientMatrix(float **M, int size) {
    for (int i = 0; i < size; i++) {
        free(M[i]);
    }
    free(M);
}


void initCoefficientMatrix(float **M, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            M[i][j] = i + j + 1;
        }
    }
}

void printCoefficientMatrix(float **M, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%5.2f", M[i][j]);
        }
        printf("\n");
    }
}