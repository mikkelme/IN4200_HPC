#include <stdio.h>
#include <math.h>


// #include <stdio.h>
// #include <stdlib.h>


int main() {
  double val = 0;
  double goal = 4./5.;
  double diff;
  int N = 100;

  for (int i = 0; i < N; i++ )
  {
    val += pow(-1, i)/pow(2,i*2);
    diff = goal-val;
    printf("val: %f, goal %f, diff: %f  \n", val, goal, diff );
  }



}
//  // compile: gcc ex1.1.c -o ex1.1.out
