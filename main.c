#include<stdio.h>
#include<stdlib.h>
//using namespace std;
float** createCoefficientMatrix(int);
void initCoefficientMatrix(float**, int);
void printCoefficientMatrix(float**, int);

void printEchelonForm(float**, const float *, int);
void printResult(const float *, int n);

int main()
{
    int  n;
    float** MatA;
    float* MatB;
    float* X;
    float divider, factor, sum;
    printf("Enter the number of unknown variables: ");
    scanf("%d", &n);
    MatA = createCoefficientMatrix(n);
    initCoefficientMatrix(MatA, n);
    printCoefficientMatrix(MatA, n);
    MatB = (float*) malloc(n * sizeof(float));
    X = (float*) malloc(n * sizeof(float));


//reading matrix A
//    for(i=0; i< n; i++) {
//        for(j=0; j < n; j++) {
//            printf("MatA[%d][%d]", i, j);
//            scanf("%f", &MatA[i][j]);
////            cin >> MatA[i][j];
//        }
//    }
//reading matrix B
    for(int i=0; i< n; i++) {
        printf("MatB[%d]", i);
        scanf("%f", &MatB[i]);
//        cin >> MatB[i];
    }
    printEchelonForm(MatA, MatB, n);
//Gauss elimination
    for (int i=0; i< n; i++) {
        divider = MatA[i][i];
        MatA[i][i] = 1.0f;
// divide all values in the row by the divisor
// to recalculate all coefficients in that row
        for (int j = i+1; j < n; j++) {
            MatA[i][j] = MatA[i][j] / divider;
        }
//Also divide the corresponding RHS element
        MatB[i] = MatB[i] / divider;
// now replace subsequent rows, by subtracting the
// appropriate portion of the ith equation from it
        if (i+1 < n) {
            for (int k=i+1; k<n; k++) {
                factor = MatA[k][i];
                MatA[k][i] = 0.0f;
                for (int j = i+1; j < n; j++) {
                    MatA[k][j] = MatA[k][j] - factor * MatA[i][j];
                }
                MatB[k] = MatB[k] - factor * MatB[i];
            }
        }
    }
// back substitution starting with last variable
    X[n-1] = MatB[n-1];
    for (int i = n-2; i>=0; i--) {
// Sum up ith row using values of X already determined
        sum = 0.0;
        for (int j = i+1; j < n; j++) {
            sum = sum + MatA[i][j] * X[j];
        }
        X[i] = MatB[i] - sum;
    }

    printEchelonForm(MatA, MatB, n);
    printResult(X, n);
    return 0;
}

void printEchelonForm(float** M, const float *RHS, int size) {
    for(int i=0; i< size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%5.2f ", M[i][j]);
        }
        printf("%10.2f\n", RHS[i]);
    }
}

void printResult(const float *X, int n) {
    for (int i=0; i < n; i++) {
        printf("X[%d] = %5.2f;\n", i, X[i]);
    }
}

float** createCoefficientMatrix(int n) {
    float** matrix =(float**) malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++) {
        matrix[i] = (float *) malloc(n * sizeof(float));
    }
    return matrix;
}

void initCoefficientMatrix(float** M, int size) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            M[i][j] = i + j +1;
        }
    }
}

void printCoefficientMatrix(float** M, int size) {
    printf("\n");
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%5.2f", M[i][j]);
        }
        printf("\n");
    }
}