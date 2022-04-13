#include <stdlib.h>
#include <stdio.h>
#include <omp.h>



int main(){
  int N, flag;

  N = 100;
  // double *A = (double *)malloc(N * sizeof(double));
  double *a = malloc(N * sizeof(*a));
  double *b = malloc(N * sizeof(*b));

  for (int i = 0; i<N; i++){
    a[i] = i;
    b[i] = 2*i;
  }


  double dotp  = 0;
  #pragma omp for
  for (int i = 0; i < N; i++){
    dotp += a[i] *b[i];
  }

  printf("%f\n", dotp );
  // for (int i = 0; i<N; i++){
  //   printf("%f\n", a[i] );
  // }



  return 0;
}


// Commands
// gcc -Xpreprocessor -fopenmp -o ex1.out ex1.c -lomp
// export OMP_NUM_THREADS=4
