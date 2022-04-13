#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

double init();

int main(){

  int nx, ny, nz;
  nx = ny = nz = 100;

  double *** v = (double ***)malloc(nx * sizeof(double**));
  double *** u = (double ***)malloc(nx * sizeof(double**));

  for (int i = 0; i < nx; i++){
    v[i] = (double **)malloc(ny * sizeof(double*));
    u[i] = (double **)malloc(ny * sizeof(double*));

    for (int j = 0; j < ny; j++){
      v[i][j] = (double *)malloc(nz * sizeof(double));
      u[i][j] = (double *)malloc(nz * sizeof(double));

      for (int k = 0; k < nz; k++){
        v[i][j][k] = init(i, j, k, nx, ny, nz);
        u[i][j][k] = 0;

      }
    }
  }

  int iter = 200, flops;
  clock_t start, timer;

  start = clock();
  for (int it = 0; it < iter; it ++){
    for (int i = 1; i <= nx-2; i++){
      for (int j = 1; j <= ny-2; j++){
        for (int k = 1; k <= nz-2; k++){
          u[i][j][k] = v[i][j][k] + (v[i-1][j][k] + v[i][j-1][k] + v[i][j][k-1] - 6*v[i][j][k] + v[i+1][j][k] + v[i][j+1][k] + v[i][j][k+1])/6;
        }
      }
    }
    // for (int i = 1; i <= nx-2; i++){
    //   for (int j = 1; j <= ny-2; j++){
    //       memcpy(v[i][j], u[i][j], ny*sizeof(double));
    //   }
    // }
  }
  timer = (clock() - start);
  double time_used_G = 1e9*timer/CLOCKS_PER_SEC;
  flops = iter*8*(nx-2)*(ny-2)*(nz-2);
  double Gflops_pr_sec = flops/time_used_G;


  printf("time_used_G: %f\n", time_used_G );
  printf("flops: %d\n", flops);

  printf("flops pr. sec: %f GHz \n", (double) Gflops_pr_sec);



  for (int i = 0; i < nx; i++){
    for (int j = 0; j < ny; j++){
      free(v[i][j]);
      free(u[i][j]);
    }
    free(v[i]);
    free(u[i]);
  }

  free(v);
  free(u);


  return 0;
}



double init(int i, int j, int k, int nx, int ny, int nz){
  return 2.0 + sin(i*j*k*M_PI/((nx -1)*(ny -1)*(nz -1)));
}
