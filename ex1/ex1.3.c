
#include <stdlib.h>
#include <stdio.h>
#include <time.h>




int main(){
  int m, n;
  clock_t start, timer_rowwise, timer_colwise;

  m = n = 10000;

  double **A = (double**)malloc(m*sizeof(double*));
  for (int i=0; i<m; i++){
    A[i] = (double*)malloc(n*sizeof(double));
  }


  start = clock();
  for (int i=0; i<n; i++) {
    for(int j=0; j<m; j++) {
      A[i][j] = m * i + j; // arbitrary input choice
    }
  }
  timer_rowwise = clock() - start;




  start = clock();
  for (int j=0; j<n; j++){
    for(int i=0; i<m; i++){
      A[i][j] = m * i + j; // arbitrary input choice
    }
  }
  timer_colwise = clock() - start;

  printf("Time elapsed using i-j-loop: %lu millisec.\n", 1000*timer_rowwise/CLOCKS_PER_SEC);
  printf("Time elapsed using j-i-loop: %lu millisec.\n", 1000*timer_colwise/CLOCKS_PER_SEC);







}
