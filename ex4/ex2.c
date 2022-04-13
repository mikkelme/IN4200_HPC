#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void matmul(double **A, double **B, double **C, int n, int m, int p);
void matmul_unrolling(double **A, double **B, double **C, int n, int m, int p);
void alloc2D(double ***A, int m, int n);

int main(){
  int n, m, p;
  clock_t start, end;


  n = 1000;
  m = 1000;
  p = 1000;

  double **A, **B, **C, **D;
  alloc2D(&A, n, m);
  alloc2D(&B, m, p);
  alloc2D(&C, n, p);
  alloc2D(&D, n, p);


  for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
          A[i][j] = i + j + 1;
      }
  }

  for (size_t i = 0; i < m; i++) {
      for (size_t j = 0; j < p; j++) {
          B[i][j] = i*j + 1;
      }
  }

  for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < p; j++) {
          C[i][j] = 0;
          D[i][j] = 0;
      }
  }

  start = clock();
  matmul(A, B, C, n, m, p);
  end = clock();
  printf("Time 1: %f s \n", (double)(end-start)/CLOCKS_PER_SEC);

  start = clock();
  matmul_unrolling(A, B, C, n, m, p);
  end = clock();
  printf("Time 2: %f s  \n", (double)(end-start)/CLOCKS_PER_SEC);







  return 0;
}


void alloc2D(double ***A, int m, int n){
  *A = malloc(m * sizeof *A);
  (*A)[0] = malloc(m*n * sizeof(*A)[0]); // Underlying 2D array

  for (size_t i=1; i<m; i++){
    (*A)[i] = &((*A)[0][i*n]);
  }
}

void matmul(double **A, double **B, double **C, int n, int m, int p){
  // A: n x m
  // B: m x p
  // C: n x p
  for (size_t i=0; i<n; i++){
    for(size_t k=0; k<p; k++){
      for(size_t j=0; j<m; j++){
        C[i][k] += A[i][j]*B[j][k];
      }
    }
  }
}


void matmul_unrolling(double **A, double **B, double **C, int n, int m, int p){
  // A: n x m
  // B: m x p
  // C: n x p
  int rest = p%4;
  for (size_t i=0; i<n; i++){
    for(size_t k=0; k<p-rest; k+=4){
      // Unrolling
      for(size_t j=0; j<m; j++){
        C[i][k] += A[i][j]*B[j][k];
        C[i][k+1] += A[i][j]*B[j][k+1];
        C[i][k+2] += A[i][j]*B[j][k+2];
        C[i][k+3] += A[i][j]*B[j][k+3];
      }
    }
    // Rest loop
    for(size_t k=p-rest; k<p; k++){
      for(size_t j=0; j<m; j++){
        C[i][k] += A[i][j]*B[j][k];
      }
    }
  }
}












/*


for the multiplication (big matrices) we load A with stride 1
such that this is (m*n)/L_c loads. For B (when the rows are longer than L_c)
each column load will be m/L_c loads and will be called n times. For p colums this gives a total of
n*m*p/L_c loads.





*/
