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
        printf("\nMENU for Gaussian Elimination");
        printf("\n------------------------------");
        printf("\n\n 1. Solve Linear equation input from console");
        printf("\n 2. Solve Linear equation input from file");
        printf("\n 0. EXIT");
        printf("\n\n Enter Your Choice: ");
        choice = getche();
        switch (choice) {
            case '1':
                printf("\n\nYou selected Solve Linear equation input from console\n");
                solver(initMatrixFromConsole(matrix));
                break;
            case '2':
                printf("\n\nYou selected Solve Linear equation input from file\n");
                solver(initMatrixFromFile(matrix, "matrix.csv"));
                break;
            case '0':
                printf("\n\nYou selected exit\n");
                break;
            default:
                printf("\n\nINVALID SELECTION...Please try again\n");
        }
        (void) getch();
    }
}

