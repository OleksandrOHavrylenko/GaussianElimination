#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "gauss.h"
#include "util.h"

void menu(Matrix* matrix);


int main() {

    Matrix * matrix = (Matrix*)malloc(sizeof(Matrix));
    matrix->n = 0;

    menu(matrix);

    freeMemory(matrix);

    return 0;
}

void menu(Matrix* matrix) {
    int choice = -1;
    while (choice != '0') {
        system("cls");
        printf("\n\tMENU for Gaussian Elimination");
        printf("\n\t------------------------------");
        printf("\n\n\t 1. Solve Linear equations. Input matrices from console.");
        printf("\n\t 2. Solve Linear equations. Input matrices from matrix.csv file.");
        printf("\n\t 3. Solve Linear equations. Input matrices from your file.");
        printf("\n\t 0. EXIT");
        printf("\n\n\t Enter Your Choice: ");
        choice = getche();
        switch (choice) {
            case '1':
                printf("\n\n\tYour choice is to input matrices from console\n");
                solver(initMatrixFromConsole(matrix));
                break;
            case '2':
                printf("\n\n\tYour choice is to input matrices from matrix.csv file\n");
                solver(initMatrixFromFile(matrix, "matrix.csv"));
                break;
            case '3':
                printf("\n\n\tYour choice is to input matrices from your file\n");
                printf("\n\n\tInput the file name, please: ");
                solver(initMatrixFromFile(matrix, input_string()));
                break;
            case '0':
                printf("\n\n\tYour choice is to exit\n");
                break;
            default:
                printf("\n\n\tERROR...Invalid selection...Please try again\n");
        }
        (void) getch();
    }
}

