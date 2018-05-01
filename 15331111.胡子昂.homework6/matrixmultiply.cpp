//****************************************
// Author : huziang
// Date : 2018年5月1日 星期二
//   this is a small program to calculate
// matrix multiply
//****************************************

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>

using namespace std;
const int n = 5;

void printMatrix(int *add, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << add[i * n + j] << " ";
        }
        cout << endl;
    }
}

int main(int argc,char *argv[])  
{  
# ifdef OPENMP   
    printf("Compiled by an OpenMP-compliant implementation.\n");  
# endif  
    int matrixA[n][n];
    int matrixB[n][n];
    int matrixC[n][n];
    int i, j, k;
    
    #pragma omp parallel private(i, j)
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            matrixA[i][j] = i * n + j;
            matrixB[i][j] = i * n + j;
        }
    }
    
    #pragma omp parallel for shared(matrixA, matrixB, n) private(i, j, k)  
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
           #pragma omp task
           {
               matrixC[i][j] = 0;
               for (k = 0; k < n; ++k) {
                   matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
               }
           }
        }
    }
        
    cout << "Matrix A:" << endl;
    printMatrix((int*)matrixA, n);
    cout << "Matrix B:" << endl;
    printMatrix((int*)matrixB, n);
    cout << "Matrix C:" << endl;
    printMatrix((int*)matrixC, n);

    return 0;  
}  