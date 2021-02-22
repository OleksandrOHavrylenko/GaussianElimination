#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>

void menu();

double **createCoefficientMatrix(int);

void freeCoefficientMatrix(double **, int);

void initCoefficientMatrix(double **, int);
void initCoefficientMatrixB(double *, int);

void printCoefficientMatrix(double **, int);

void prinMatrixes(double **, const double *, int);

void printResult(const double *, int n);

bool solveEquations(double **MatA, double *MatB, int n);

int forwardElimination(double **MatA, double *MatB, int n);

void backSubstitution(int n, double **MatA, double *MatB, double *X);

void backSubstitution1(int n, double **MatA, double *MatB, double *X);

void initCoefMatrix(double **MatA, int n);

void initRHSMatrix(double *MatB, int n);

bool isZeroRow(double **pDouble, int row, int size);

bool canSolve(double **M, double *, int size);

void printCommomView(int size);

void solverFromConsole();

void swapRow(double **A, double *B, int row1, int row2, int n);


int main() {
    menu();

    return 0;
}

void solverFromConsole() {
    int n;
    printf("Enter the number of unknown variables : ");
    scanf("%d", &n);
    printf("The general view of the system of linear equations with %d variables : \n", n);
    printCommomView(n);
    double **A = createCoefficientMatrix(n);
//    initCoefficientMatrix(A, n);
//    printCoefficientMatrix(A, n);
    double *B = (double *) malloc(n * sizeof(double));
    double *X = (double *) malloc(n * sizeof(double));
    initCoefficientMatrixB(B, n);

    initCoefMatrix(A, n);
    initRHSMatrix(B, n);
    printf("\nThe starting view of the system of linear equations:");
    prinMatrixes(A, B, n);
    printf("\n=======================================================\n");

//    int isSolved = solveEquations(A, B, n);
    int singularFlag = forwardElimination(A, B, n);

    if(singularFlag != -1) {
        printf("The Matrix is Singular.\n");

        if (A[singularFlag][n])
            printf("Inconsistent System.");
        else
            printf("May have infinitely many "
                   "solutions.");
        return;
    }

    backSubstitution1(n, A, B, X);

    printf("\nThe Gaussian forward stroke:");
    prinMatrixes(A, B, n);
    printf("\nThe result of the Gaussian Elimination is:");
    printResult(X, n);

    freeCoefficientMatrix(A, n);
    free(B);
    free(X);
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

void printCommomView(int size) {
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

void initRHSMatrix(double *B, int n) {
    printf("\n Enter the elements of Matrix B : \n");
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("B[%d] = ", i);
        scanf("%lf", &B[i]);
    }
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

void backSubstitution1(int n, double **A, double *B, double *X) {// back substitution starting with last variable
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

void backSubstitution(int n, double **A, double *B, double *X) {// back substitution starting with last variable
    X[n - 1] = B[n - 1];
    for (int i = n - 2; i >= 0; i--) {
// Sum up ith row using values of X already determined
        double sum = 0.0f;
        for (int j = i + 1; j < n; j++) {
            sum = sum + A[i][j] * X[j];
        }
        X[i] = B[i] - sum;
    }
}

int forwardElimination(double **A, double *B, int n) {
    for (int column = 0; column < n; column++) {
        int maxRow = column;
        int maxValue = A[maxRow][column];

        for (int row = column + 1; row < n; ++row) {
            if (abs(A[row][column]) > maxValue) {
                maxValue = A[row][column];
                maxRow = row;
            }
        }

//        printf("\nSingular: %lf ===> %d \n",A[column][maxRow], !A[column][maxRow]);
        if (fabs(A[column][maxRow]) < 10e-7) {
            return column;
        }

        if(maxRow != column) {
            swapRow(A, B, column, maxRow, n);
        }

        for (int i = column+1; i < n; ++i) {
            double f = A[i][column]/A[column][column];

            for (int j = column+1; j < n; ++j) {
                A[i][j] -= A[column][j] * f;
            }

            B[i] -= B[column] * f;

            A[i][column] = 0;
        }
        printf("\nStep %d", column + 1);
        prinMatrixes(A, B, n);
    }
    return -1;
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

bool solveEquations(double **A, double *B, int n) {
    for (int i = 0; i < n; i++) {
        double divider = A[i][i];
        if (divider != 0) {
            A[i][i] = 1.0f;
            for (int j = i + 1; j < n; j++) {
                A[i][j] = A[i][j] / divider;
            }
            B[i] = B[i] / divider;
            if (i + 1 < n) {
                for (int k = i + 1; k < n; k++) {
                    double factor = A[k][i];
                    A[k][i] = 0.0f;
                    for (int j = i + 1; j < n; j++) {
                        A[k][j] = A[k][j] - factor * A[i][j];
                    }
                    B[k] = B[k] - factor * B[i];
                }
            }
            printf("\nStep %d", i + 1);
            prinMatrixes(A, B, n);
            if (!canSolve(A, B, n)) {
                return false;
            }
        }
    }
    return true;
}

bool canSolve(double **A, double *B, int size) {
    for (int row = 0; row < size; ++row) {
        if (isZeroRow(A, row, size) && B[row] != 0) {
            printf("\nNo solvings because bad rank\n");
            return false;
        }
    }
    return true;
}

bool isZeroRow(double **A, int row, int size) {
    for (int col = 0; col < size; ++col) {
        if (A[row][col] != 0) {
            return false;
        }
    }
    return true;
}

void prinMatrixes(double **A, const double *B, int size) {
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

void printResult(const double *X, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("X[%d] = %5.2f;\n", i, X[i]);
    }
}

double **createCoefficientMatrix(int n) {
    double **A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    return A;
}

void freeCoefficientMatrix(double **A, int size) {
    for (int i = 0; i < size; i++) {
        free(A[i]);
    }
    free(A);
}


void initCoefficientMatrix(double **A, int size) {
    A[0][0] = 1.0;
    A[0][1] = -2.0;
    A[0][2] = -1.0;
    A[0][3] = 3.0;

    A[1][0] = -2.0;
    A[1][1] = 1.0;
    A[1][2] = 3.0;
    A[1][3] = -2.0;

    A[2][0] = 2.0;
    A[2][1] = -1.0;
    A[2][2] = -2.0;
    A[2][3] = 1.0;

    A[3][0] = 3.0;
    A[3][1] = -3.0;
    A[3][2] = -2.0;
    A[3][3] = 3.0;
}

void initCoefficientMatrixB(double *B, int n) {
    B[0] = 2.0;
    B[1] = -3.0;
    B[2] = 2.0;
    B[3] = 3.0;
}

void printCoefficientMatrix(double **A, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%5.2f", A[i][j]);
        }
        printf("\n");
    }
}