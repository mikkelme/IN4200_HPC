#include <stdlib.h>
#include <stdio.h>
#include <omp.h>



int main(){
  int N = 1e5;
  double dotp = 0;
  double start, end;



  double *a = malloc(N*sizeof(a));
  double *b = malloc(N*sizeof(b));

  for (int i = 0; i<N; i++){
    a[i] = i;
    b[i] = 2*i;
  }


  start = omp_get_wtime();
  #pragma omp parallel reduction(+:dotp)
  {
    int num_threads, thread_id;
    num_threads = omp_get_num_threads();
    thread_id = omp_get_thread_num();
    printf("Hello world! I’m thread No.%d out of %d threads.\n", thread_id,num_threads);

    #pragma omp for
    for (int i = 0; i<N; i++){
      dotp += a[i] * b[i];
    }

  } // end of parallel region

  end = omp_get_wtime();

  double time = end-start;
  printf("time %f s\n", time );
  printf("dot product: %f\n", dotp);




  return 0;
}
