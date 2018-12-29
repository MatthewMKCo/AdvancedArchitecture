#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void bubblesort(){
    // int x[10] = {25,10,13,53,3,12,46,59,32,16};
    int x[10] = {10,9,8,7,6,5,4,3,2,1};
    for(int i = 0; i < 9; i++){
      int swapped = 0;
        for(int j = 0; j < 9 - i; j++){
            if(x[j] > x[j+1]){
                int temp = x[j];
                x[j] = x[j+1];
                x[j+1] = temp;
                swapped = 1;
            }
        }
    if(swapped == 0){
        break;
    }
    }
    for(int i = 0; i < 10; i++){
      printf("%d\n",x[i]);
    }
}

int main(){
  bubblesort();
}
