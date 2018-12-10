#include <stdio.h>

int main(){
  int x[5] = {1,1};
  for(int i = 0; i < 13; i++){
    for(int j = 0; j < 2; j++){
      printf("Register %d:%d\n",(i * 2 + j) ,x[j]);
    }
    x[0] = x[0] + x[1];
    x[1] = x[1] + x[0];
  }
}
