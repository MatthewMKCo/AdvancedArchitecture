#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void factorial() {
  int y = 1;
  int x = 10;
  for(int i = x; i > 0; i--){
    y = y * i;
    printf("%d\n",y);
  }

}

int main(){
  factorial();
}
