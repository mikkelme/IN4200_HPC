#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void foo (int N, double **mat, double **s, int *v);
void optimized (int N, double **mat, double **s, int *v);
void optimized2 (int N, double **mat, double **s, int *v, double *vtab);



int main(int argc, char const *argv[]){
  int N = atoi(argv[1]);
  time_t start, end;

  // Allocate arrays
  double **mat, **s;
  mat = malloc(N * sizeof(*mat));
  mat[0] = malloc(N * N * sizeof(*mat[0]));
  s = malloc(N * sizeof(*s));
  s[0] = malloc(N * N * sizeof(*s[0]));


  for (size_t i=1; i<N; i++){
    mat[i] = &(mat[0][i * N]);
    s[i] = &(s[0][i * N]);
  }

  int *v = malloc(N *sizeof(*v));
  double *vtab = malloc(N *sizeof(*v));


  // Fill arrays
  for (size_t i=0; i<N; i++){
    v[i] = i;
    vtab[i] = -cos(2.0*i);

    for (size_t j=0; j<N; j++){
      mat[i][j] = i + j;
      s[i][j] = i * j;

    }
  }




  // Perform loops and time
  start = clock();
  foo(N, mat, s, v);
  end = clock();
  printf("foo: %f s\n", (double) (end-start)/CLOCKS_PER_SEC );

  start = clock();
  optimized(N, mat, s, v);
  end = clock();
  printf("Optimized: %f s\n", (double) (end-start)/CLOCKS_PER_SEC );

  start = clock();
  optimized2(N, mat, s, v, vtab);
  end = clock();
  printf("Optimized2: %f s\n", (double) (end-start)/CLOCKS_PER_SEC );


  free(v);
  free(vtab);
  free(mat[0]);
  free(mat);
  free(s[0]);
  free(s);



  return 0;
}

void foo (int N, double **mat, double **s, int *v) {
  int i,j;
  double val;
  for (j=0; j<N; j++)
    for (i=0; i<N; i++) {
      val = 1.0*(v[j]%256);
      mat[i][j] = s[i][j]*(sin(val)*sin(val)-cos(val)*cos(val));
    }
}

void optimized (int N, double **mat, double **s, int *v) {
  int i,j;
  double val, tmp;
  for (j=0; j<N; j++)
    tmp = -cos(2.0*(v[j]%256));
    for (i=0; i<N; i++) {
      mat[i][j] = s[i][j]*tmp;
    }
}



void optimized2 (int N, double **mat, double **s, int *v, double *vtab) {
  int i,j;
  double val, tmp;
  for (j=0; j<N; j++)
    tmp = vtab[v[j]%256];
    for (i=0; i<N; i++) {
      mat[i][j] = s[i][j]*tmp;
    }
}
