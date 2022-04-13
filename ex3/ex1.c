#include <stdlib.h>
#include <stdio.h>
#include <time.h>


double numerical_integration();
void verify();
void fp_division_latency();

int main(){
  int N = 1e9;
  // verify(N);
  fp_division_latency(N);



  return 0;
}


void fp_division_latency(int N){
  clock_t start, end;
  start = clock();
  numerical_integration(0., 1., N);
  end = clock();
  double time = end-start;
  double time_over_N = time/N;
  printf("%f\n", time_over_N );
  // double fp_div_latency = (time - 6*N - 1)/(1 + N);
  // printf("%f\n", fp_div_latency );

}


void verify(int N){
  double I;
  for (int i=1; i < N; i++){
    I = numerical_integration(0., 1., i);
    printf("%f\n", I);
  }


}
double numerical_integration (double x_min, double x_max, int slices)
{
  double delta_x = (x_max-x_min)/slices;
  double x, sum = 0.0;
  for (int i=0; i<slices; i++) {
    x = x_min + (i+0.5)*delta_x;
    sum = sum + 4.0/(1.0+x*x);
  }
  return sum*delta_x;
}
