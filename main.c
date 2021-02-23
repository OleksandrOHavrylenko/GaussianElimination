#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>
#define ZERO 10e-7

typedef struct Matrix {
    int n;
    double **A;
    double *B;
    double *X;
} Matrix;

void menu();

void solverFromConsole();

void solverFromFile();

Matrix* createMatrixFromFile(char *fileName);

double **createCoefficientMatrix(int);

void initMatrixA(Matrix* matrix);

void initMatrixB(Matrix* matrix);

int forwardElimination(Matrix* matrix);

void backSubstitution(Matrix* matrix);

void swapRow(Matrix* matrix, int row1, int row2);

bool isZeroRow(const Matrix* matrix, int row);

void printCommonView(int size);

void printMatrices(const Matrix* matrix);

void printResult(const Matrix* matrix);

void freeCoefficientMatrix(Matrix* matrix);

int main() {
    menu();
    return 0;
}

void menu() {
    int choice = 0;
    while (choice != '4') {
        system("cls");
        printf("\n\tMENU for Gaussian Elimination");
        printf("\n\t------------------------------");
        printf("\n\n\t 1. Solve Linear equation input from console");
        printf("\n\t 2. Solve Linear equation input from file");
//        printf("\n\t 3. OPTION 3");
        printf("\n\t 4. EXIT");
        printf("\n\n Enter Your Choice: ");
        choice = getche();
        switch (choice) {
            case '1':
                printf("\n\nYou selected Solve Linear equation input from console\n");
                solverFromConsole();
                break;
            case '2':
                printf("\n\nYou selected Solve Linear equation input from file\n");
                solverFromFile();
                break;
//            case '3':
//                printf("\n\nYOU SELECTED OPTION 3\n");
//                break;
            case '4':
                printf("\n\nYou selected exit\n");
                break;
            default:
                printf("\n\nINVALID SELECTION...Please try again\n");
        }
        (void) getch();
    }
}

void solverFromConsole() {
    int n;
    printf("Enter the number of unknown variables : ");
    scanf("%d", &n);
    printf("The general view of the system of linear equations with %d variables : \n", n);
    printCommonView(n);

    Matrix* matrix = NULL;
    matrix = (Matrix *) malloc(sizeof(Matrix));
    matrix->A = createCoefficientMatrix(n);
    matrix->B = (double *) malloc(n * sizeof(double));
    matrix->X = (double *) malloc(n * sizeof(double));
    matrix->n = n;

    initMatrixA(matrix);
    initMatrixB(matrix);
    printf("\nThe starting view of the system of linear equations:");
    printMatrices(matrix);
    printf("\n=======================================================\n");

    int singularFlag = forwardElimination(matrix);

    if(singularFlag != -1) {
        printf("The Matrix is Singular.\n");

        if (matrix->B[singularFlag] > ZERO)
            printf("Inconsistent System.");
        else
            printf("May have infinitely many solutions.");
        return;
    }

    backSubstitution(matrix);

    printf("\nThe Gaussian forward stroke:");
    printMatrices(matrix);
    printf("\nThe result of the Gaussian Elimination is:");
    printResult(matrix);

    freeCoefficientMatrix(matrix);
}

void solverFromFile() {
    Matrix* matrix = NULL;
    matrix = createMatrixFromFile("matrix.txt");
    printf("The general view of the system of linear equations with %d variables : \n", matrix->n);
    printCommonView(matrix->n);

    printf("\nThe starting view of the system of linear equations:");
    printMatrices(matrix);
    printf("\n=======================================================\n");

    int singularFlag = forwardElimination(matrix);

    if(singularFlag != -1) {
        printf("The Matrix is Singular.\n");

        if (matrix->B[singularFlag] > ZERO)
            printf("Inconsistent System.");
        else
            printf("May have infinitely many "
                   "solutions.");
        return;
    }

    backSubstitution(matrix);

    printf("\nThe Gaussian forward stroke:");
    printMatrices(matrix);
    printf("\nThe result of the Gaussian Elimination is:");
    printResult(matrix);

    freeCoefficientMatrix(matrix);
}

Matrix* createMatrixFromFile(char *fileName) {
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

    Matrix * matrix = (Matrix*)malloc(sizeof(Matrix));
    matrix->n = n;
    matrix->A = A;
    matrix->B = B;
    matrix->X = (double *) malloc(matrix->n * sizeof(double));

    return matrix;
}

double **createCoefficientMatrix(int n) {
    double **A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    return A;
}

void initMatrixA(Matrix* matrix) {
    printf("\nEnter the elements of Matrix А : \n");
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->n; j++) {
            printf("A[%d][%d] =%s", i, j, " ");
            scanf("%lf", &matrix->A[i][j]);
        }
    }
}

void initMatrixB(Matrix* matrix) {
    printf("\n Enter the elements of Matrix B : \n");
    printf("\n");
    for (int i = 0; i < matrix->n; i++) {
        printf("B[%d] = ", i);
        scanf("%lf", &matrix->B[i]);
    }
}

int forwardElimination(Matrix* matrix) {
    for (int column = 0; column < matrix->n; column++) {
        int maxRow = column;
        double maxValue = matrix->A[maxRow][column];

        for (int row = column + 1; row < matrix->n; ++row) {
            if (fabs(matrix->A[row][column]) > maxValue) {
                maxValue = matrix->A[row][column];
                maxRow = row;
            }
        }

        if (fabs(matrix->A[column][maxRow]) < ZERO) {
            if (isZeroRow(matrix, maxRow)) {
                return column;
            }
        }

        if(maxRow != column) {
            swapRow(matrix, column, maxRow);
        }

        for (int i = column+1; i < matrix->n; ++i) {
            double divider = matrix->A[column][column];
            if (fabs(divider) > ZERO) {
                double f = matrix->A[i][column] / divider;

                for (int j = column+1; j < matrix->n; ++j) {
                    matrix->A[i][j] -= matrix->A[column][j] * f;
                }

                matrix->B[i] -= matrix->B[column] * f;
            }

            matrix->A[i][column] = 0;
        }
        printf("\nStep %d", column + 1);

        printMatrices(matrix);
    }
    return -1;
}

void backSubstitution(Matrix* matrix) {// back substitution starting with last variable
    for (int i = matrix->n - 1; i >= 0; i--) {
        matrix->X[i] = matrix->B[i];
        for (int j=i+1; j<matrix->n; j++)
        {
            /* subtract all the lhs values
             * except the coefficient of the variable
             * whose value is being calculated */
            matrix->X[i] -= matrix->A[i][j] * matrix->X[j];
        }
        matrix->X[i] = matrix->X[i] / matrix->A[i][i];
    }
}

void swapRow(Matrix* matrix, int row1, int row2) {
    printf("Swapped rows %d and %d\n", row1, row2);

    for (int k = 0; k < matrix->n; ++k) {
        double temp = matrix->A[row1][k];
        matrix->A[row1][k] = matrix->A[row2][k];
        matrix->A[row2][k] = temp;
    }

    double temp = matrix->B[row1];
    matrix->B[row1] = matrix->B[row2];
    matrix->B[row2] = temp;
}

bool isZeroRow(const Matrix* matrix, int row) {
    for (int col = 0; col < matrix->n; ++col) {
        if (fabs(matrix->A[row][col]) > ZERO) {
            return false;
        }
    }
    return true;
}

void printCommonView(int size) {
    printf("===============================\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j == size - 1) {
                printf("A[%d][%d]*x[%d]%s", i, j, j, " ");
            } else {
                printf("A[%d][%d]*x[%d]%s", i, j, j, " + ");
            }
        }
        printf("= B[%d]\n", i);
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
    printf("\n");
    for (int i = 0; i < matrix->n; i++) {
        printf("X[%d] = %5.2f;\n", i, matrix->X[i]);
    }
}

void freeCoefficientMatrix(Matrix* matrix) {
    for (int i = 0; i < matrix->n; i++) {
        free(matrix->A[i]);
    }
    free(matrix->A);

    free(matrix->B);
    free(matrix->X);
}