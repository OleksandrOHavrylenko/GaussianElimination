#include<stdio.h>
//using namespace std;
int main()
{
    int i, j, k, n;
    float MatA[100][100], MatB[100], X[100];
    float Divisor, Factor, sum;
    scanf("%d", &n);
//    cin >> n;
//reading matrix A
    for(i=0; i< n; i++) {
        for(j=0; j < n; j++) {
            printf("MatA[%d][%d]", i, j);
            scanf("%f", &MatA[i][j]);
//            cin >> MatA[i][j];
        }
    }
//reading matrix B
    for(i=0; i< n; i++) {
        printf("MatB[%d]", i);
        scanf("%f", &MatB[i]);
//        cin >> MatB[i];
    }
//Gauss elimination
    for (i=0; i< n; i++) {
        Divisor = MatA[i][i];
        MatA[i][i] = 1.0;
// divide all values in the row by the divisor
// to recalculate all coefficients in that row
        for (j = i+1; j < n; j++) {
            MatA[i][j] = MatA[i][j]/Divisor;
        }
//Also divide the corresponding RHS element
        MatB[i] = MatB[i]/Divisor;
// now replace subsequent rows, by subtracting the
// appropriate portion of the ith equation from it
        if (i+1 < n) {
            for (k=i+1; k<n; k++) {
                Factor = MatA[k][i];
                MatA[k][i] = 0.0;
                for (j = i+1; j < n; j++) {
                    MatA[k][j] = MatA[k][j] - Factor * MatA[i][j];
                }
                MatB[k] = MatB[k] - Factor * MatB[i];
            }
        }
    }
// back substitution starting with last variable
    X[n-1] = MatB[n-1];
    for (i = n-2; i>=0; i--) {
// Sum up ith row using values of X already determined
        sum = 0.0;
        for (j = i+1; j < n; j++) {
            sum = sum + MatA[i][j] * X[j];
        }
        X[i] = MatB[i] - sum;
    }
//output the results
    for(i=0; i< n; i++) {
        for (j = 0; j < n; j++) {
            printf("%f ", MatA[i][j]);
//            cout << MatA[i][j] << " ";
        }
        printf("%f\n", MatB[i]);
//        cout << " " << MatB[i] << endl;
    }
    for (i=0; i<n; i++) {
        printf("X[%d] is: %f", i, X[i]);
//        cout << "X[" << i << "] is: " ;
//        cout << X[i] << endl;
    }
    return 0;
}