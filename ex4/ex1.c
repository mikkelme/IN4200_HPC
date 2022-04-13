#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void loop1();
void loop2();
void mem_band_width();
void fill_arrays();
double randfrom();



int main(int argc, char const *argv[]){

  if (argc < 3){
    printf("%s\n", "Please provide N and stride" );
    return 1;
  }

  srand(time(NULL));
  int N = atoi(argv[1]);
  int stride = atoi(argv[2]);
  double s;
  clock_t start, end;

  s = 1.5;

  double *A = (double *)malloc(N * sizeof(double));
  double *B = (double *)malloc(N * sizeof(double));

  printf("Filling array, N = %d\n\n", N );
  fill_arrays(A, B, N);


  printf("%s\n", "Performing loop1" );
  start = clock();
  loop1(A, B, s, N);
  end = clock();
  double time1 = (double)(end-start)/CLOCKS_PER_SEC;
  printf("Loop1 time: %f ms\n", 1000*time1 );
  mem_band_width(N, time1, A, 1);


  printf("%s\n", "Performing loop2" );
  start = clock();
  loop2(A, B, s, N, stride);
  end = clock();
  double time2 = (double)(end-start)/CLOCKS_PER_SEC;
  printf("Loop2 time: %f ms\n", 1000*time2 );
  mem_band_width(N, time2, A, stride);





  free(A);
  free(B);


  return 0;
}



void fill_arrays(double *A, double *B, int N){
  for (int i=0; i<N; i++){
    A[i] = randfrom(0., 2.);
    B[i] = randfrom(0., 2.);
  }
}



void mem_band_width(int N, double tot_sec, double *A, int stride){
  int tot_bytes = 2 * (int)(N/(double)stride + 0.5) * sizeof *A;
  long double bandwidth = tot_bytes/tot_sec;
  printf("Memory bandwidth: %3Le\n\n", bandwidth  );
}



void loop1(double *A, double *B, double s, int N){
  for (int i = 0; i < N; i++){
    A[i] = s*B[i];
  }
}

void loop2(double *A, double *B, double s, int N, int stride){
  for (int i = 0; i < N; i+=stride){
    A[i] = s*B[i];
  }
}


double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
