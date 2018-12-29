#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void matrixmul() {
    int x[10][10] = {{1, 2, 3, 6, 10, 4, 23, 12, 8, 7},
                     {4, 5, 6, 1, 31, 9, 12, 32, 1, 3},
                     {7, 8, 9, 5, 32, 3, 36, 59, 2, 4}};
    int y[10] = {5, 2, 1, 2, 4, 1, 7, 9, 1, 3};
    int z[10] = {0,0,0,0,0,0,0,0,0,0};

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            int count = x[i][j] * y[j];
            z[i] = z[i] + count;
        }
    }
}

int main(){
  matrixmul();
}
