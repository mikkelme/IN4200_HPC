#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


double pow100();

int main(){
  clock_t start, end;
  double x = 1;

  start = clock();
  double my_pow = pow100(x);
  end = clock();
  double my_pow_time = 1000000*(end-start)/CLOCKS_PER_SEC;


  start = clock();
  double std_pow = pow(x, 100);
  end = clock();
  double std_pow_time = 1000000*(end-start)/CLOCKS_PER_SEC;

  double diff = my_pow - std_pow;
  printf("Time for std pow: %f micro sec\n", std_pow_time);
  printf("Time for my pow: %f micro sec\n", my_pow_time);
  printf("Difference: %f\n", diff );



  // printf("%f\n", my_pow );

  return 0;
}


// 2
// 4
// 16
// 256
double pow100(double x){
  double xp, x4, x32;
  xp = x*x; // x^2
  x4 = xp*xp; // x^4
  xp = x4*x4; // x^8
  xp = xp*xp; // x^16
  x32 = xp*xp; // x^32
  xp = x32*x32; // x^64
  xp = xp*x32*x4; // x^100

  return xp;
  }






//
