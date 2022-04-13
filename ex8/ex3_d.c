#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

void sort(int *a, int N);
int compare_exhange(int *a0, int *a1);
void print_array(int *a, int N);
int* RN_array(int N);

int main(int argc, char *argv[]){
  int N;
  int *a;
  double start, end;

  if (argc <= 2){
    printf("Please give: number of threads, length of array as 10^N\n");
    exit(1);
  }
  // Settings
  N = pow(10, atoi(argv[1]));
  omp_set_num_threads(atoi(argv[2]));

  // # pragma omp parallel
  // {
  // int num_threads = omp_get_num_threads();
  // printf("%d\n", num_threads );
  // }
  // exit(0);

  // Random array
  a = RN_array(N);

  // Sort and time
  start = omp_get_wtime();
  sort(a, N);
  end = omp_get_wtime();

  // Show results
  double time = end-start;
  printf("time %f s\n", time );


  return 0;
}

int compare_exhange(int *a0, int *a1){

  if (*a0 > *a1){
    int tmp = *a0;
    *a0 = *a1;
    *a1 = tmp;
    return 1;
  }
  return 0;
}

void sort(int *a, int N){

  // works for even N
  int count_exchange;
  count_exchange = 0;

  # pragma omp parallel
  {

  for (int i = 1; i < N+1; i++){
    if (i%2 == 1) // i is odd
    {
     #pragma omp for reduction(+:count_exchange)
     for (int j = 0; j < (N - N%2)/2; j++){
       count_exchange += compare_exhange(&a[2*j], &a[2*j+1]);
     }
    }
    if (i%2 == 0)// i is even
    {
    #pragma omp for reduction(+:count_exchange)
    for (int j = 1; j < N/2 + N%2; j++){
      count_exchange += compare_exhange(&a[2*j-1], &a[2*j]);

    }
   }

     if (i%2 == 0 & count_exchange == 0){
       printf("Early stop, i = %d/%d\n", i, N);
       break;
     }

   #pragma omp barrier
   #pragma omp single
   {
     count_exchange = 0;
   }
  } // End of i for-loop


  } // End of parallel region
} // end of sort function



void print_array(int *a, int N){
  printf("[%d", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %d", a[i]);
  }
  printf("]\n");



}


int* RN_array(int N){
  srand(time(0));
  int *a = malloc(N * sizeof(a));
  for (int i = 0; i < N; i++){
    int r = rand() % N + 1;
    a[i] = r;
  }
  return a;
}




// Commands
// gcc -Xpreprocessor -fopenmp -o ex1.out ex1.c -lomp
// export OMP_NUM_THREADS=4
