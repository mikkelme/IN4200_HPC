#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void orginal_loop(double **A, double *B, double *y, int N);
void unroll_jam_loop(double **A, double *B, double *y, int N);

int main(int argc, char const *argv[]){
  int N = atoi(argv[1]);
  time_t start, end;

  // Allocate arrays
  double **A;
  A = malloc(N * sizeof(*A));
  A[0] = malloc(N * N * sizeof(*A[0]));
  for (size_t i=1; i<N; i++){
    A[i] = &(A[0][i * N]);
  }

  double *B = malloc(N *sizeof(*B));
  double *y = malloc(N *sizeof(*y));

  // Fill arrays
  for (size_t i=0; i<N; i++){
    B[i] = i;
    for (size_t j=0; j<N; j++){
      A[i][j] = i + j;
    }
  }

  // Perform loops and time
  start = clock();
  orginal_loop(A, B, y, N);
  end = clock();
  printf("Orignal loop: %f s\n", (double) (end-start)/CLOCKS_PER_SEC );

  // Perform loops and time
  start = clock();
  unroll_jam_loop(A, B, y, N);
  end = clock();
  printf("Unroll-and-jam loop: %f s\n", (double) (end-start)/CLOCKS_PER_SEC );


  free(B);
  free(y);
  free(A[0]);
  free(A);




  return 0;
}


void unroll_jam_loop(double **A, double *B, double *y, int N){
  int remainder = N%4;
  for (size_t j=0; j<remainder; j++){
    for (size_t i=0; i<=j; i++){
      y[j] += A[j][i]*B[i];
    }
  }

  for (size_t j=remainder; j<N; j+=4){
    for (size_t i=0; i<=j; i++){
      y[j] += A[j][i]*B[i];
      y[j+1] += A[j+1][i]*B[i];
      y[j+2] += A[j+2][i]*B[i];
      y[j+3] += A[j+3][i]*B[i];
    }
    y[j+1] += A[j+1][j+1]*B[j+1];
    y[j+2] += A[j+2][j+1]*B[j+1];
    y[j+2] += A[j+2][j+2]*B[j+2];
    y[j+3] += A[j+3][j+1]*B[j+1];
    y[j+3] += A[j+3][j+2]*B[j+2];
    y[j+3] += A[j+3][j+3]*B[j+3];
  }
}


void orginal_loop(double **A, double *B, double *y, int N){
  for (size_t j=0; j<N; j++){
    for (size_t i=0; i<=j; i++){
      y[j] += A[j][i]*B[i];
    }
  }
}
