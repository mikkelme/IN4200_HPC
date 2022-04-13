#include <stdlib.h>
#include <stdio.h>



int main() {

  int nx, ny, nz;

  nx = ny = nz = 3;



  int * A= (int *)malloc(nz * ny * nx * sizeof(int));
  #define ARR(i,j,k) (A[ny*nz*i + nz*j + k])

  for (int i = 0; i < nx; i++){
    for (int j = 0; j < nx; j++){
      for (int k = 0; k < nx; k++){
        // A[ny*nz*i + nz*j + k] = nx * ny * i + ny * j + k;
        ARR(i,j,k) = nx * ny * i + ny * j + k;
      }
    }
  }

  printf("%d\n", ARR(1,2,3) );

  
















}
