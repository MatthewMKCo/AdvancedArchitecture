#include <stdio.h>

int main(){
  int x[5] = {1,1,1,1,1};
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 5; j++){
      printf("Register %d:%d\n",(i * 5 + j) ,x[j]);
    }
    x[0] = x[0] + x[1];
    x[1] = x[1] + x[2];
    x[2] = x[2] + x[3];
    x[3] = x[3] + x[4];
    x[4] = x[4] + x[0];
  }
}
