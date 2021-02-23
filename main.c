#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>

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

void initCoefMatrix(double **MatA, int n);

void initRHSMatrix(double *MatB, int n);

int forwardElimination(Matrix* matrix);

void backSubstitution(int n, double **MatA, double *MatB, double *X);

void swapRow(double **A, double *B, int row1, int row2, int n);

bool isZeroRow(double **pDouble, int row, int size);

void printCommonView(int size);

void printMatrices(Matrix* matrix);

void printResult(const double *, int n);

void freeCoefficientMatrix(double **, int);

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

    initCoefMatrix(matrix->A, matrix->n);
    initRHSMatrix(matrix->B, n);
    printf("\nThe starting view of the system of linear equations:");
    printMatrices(matrix);
    printf("\n=======================================================\n");

    int singularFlag = forwardElimination(matrix);

    if(singularFlag != -1) {
        printf("The Matrix is Singular.\n");

        if (matrix->B[singularFlag] > 10e-7)
            printf("Inconsistent System.");
        else
            printf("May have infinitely many "
                   "solutions.");
        return;
    }

    backSubstitution(n, matrix->A, matrix->B, matrix->X);

    printf("\nThe Gaussian forward stroke:");
    printMatrices(matrix);
    printf("\nThe result of the Gaussian Elimination is:");
    printResult(matrix->X, n);

    freeCoefficientMatrix(matrix->A, n);
    free(matrix->B);
    free(matrix->X);
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

        if (matrix->B[singularFlag] > 10e-7)
            printf("Inconsistent System.");
        else
            printf("May have infinitely many "
                   "solutions.");
        return;
    }

    backSubstitution(matrix->n, matrix->A, matrix->B, matrix->X);

    printf("\nThe Gaussian forward stroke:");
    printMatrices(matrix);
    printf("\nThe result of the Gaussian Elimination is:");
    printResult(matrix->X, matrix->n);

    freeCoefficientMatrix(matrix->A, matrix->n);
    free(matrix->B);
    free(matrix->X);
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

    double** A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }

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

void initCoefMatrix(double **A, int n) {
    printf("\nEnter the elements of Matrix А : \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("A[%d][%d] =%s", i, j, " ");
            scanf("%lf", &A[i][j]);
        }
    }
}

void initRHSMatrix(double *B, int n) {
    printf("\n Enter the elements of Matrix B : \n");
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("B[%d] = ", i);
        scanf("%lf", &B[i]);
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

//        printf("\nSingular: %lf ===> %d \n",A[column][maxRow], !A[column][maxRow]);
        if (fabs(matrix->A[column][maxRow]) < 10e-7) {
            if (isZeroRow(matrix->A, maxRow, matrix->n)) {
                return column;
            }
        }

        if(maxRow != column) {
            swapRow(matrix->A, matrix->B, column, maxRow, matrix->n);
        }

        for (int i = column+1; i < matrix->n; ++i) {
            double divider = matrix->A[column][column];
            if (fabs(divider) > 10e-7) {
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

void backSubstitution(int n, double **A, double *B, double *X) {// back substitution starting with last variable
    for (int i = n - 1; i >= 0; i--) {
        X[i] = B[i];
        for (int j=i+1; j<n; j++)
        {
            /* subtract all the lhs values
             * except the coefficient of the variable
             * whose value is being calculated */
            X[i] -= A[i][j]*X[j];
        }
        X[i] = X[i]/A[i][i];

    }
}

void swapRow(double **A, double *B, int row1, int row2, int n) {
    printf("Swapped rows %d and %d\n", row1, row2);

    for (int k = 0; k < n; ++k) {
        double temp = A[row1][k];
        A[row1][k] = A[row2][k];
        A[row2][k] = temp;
    }

    double temp = B[row1];
    B[row1] = B[row2];
    B[row2] = temp;
}

bool isZeroRow(double **A, int row, int size) {
    for (int col = 0; col < size; ++col) {
        if (fabs(A[row][col]) > 10e-7) {
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

void printMatrices(Matrix* matrix) {
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

void printResult(const double *X, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("X[%d] = %5.2f;\n", i, X[i]);
    }
}

void freeCoefficientMatrix(double **A, int size) {
    for (int i = 0; i < size; i++) {
        free(A[i]);
    }
    free(A);
}