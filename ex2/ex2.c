#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


int main(){
  int n = 1e8;
  int * A, * B;
  clock_t start, timer_std, timer_memcpy;

  A = (int *)malloc(n * sizeof(int));
  B = (int *)malloc(n * sizeof(int));

  for (int i = 0; i < n; i++){
    A[i] = i;
  }


  start = clock();
  for (int i = 0; i < n; i++){
    B[i] = A[i];
  }
  timer_std = clock() - start;



  start = clock();
  memcpy(A, B, n*sizeof(int));
  timer_memcpy = clock() - start;


  printf("Time for std copy: %lu ms\n", 1000*timer_std/CLOCKS_PER_SEC);
  printf("Time for mpmcpy copy: %lu ms\n", 1000*timer_memcpy/CLOCKS_PER_SEC);

  return 0;
}
