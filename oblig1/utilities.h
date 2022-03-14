#ifndef UTILITIES_H
#define UTILITIES_H


int compare(double *a0, double *a1){
  if (*a0 > *a1){
    return 1;
  }
  return 0;
}

void exchange_double(double *a0, double *a1){
  double tmp = *a0;
  *a0 = *a1;
  *a1 = tmp;
}

void exchange_int(int *a0, int *a1){
  int tmp = *a0;
  *a0 = *a1;
  *a1 = tmp;
}



void print_array_int(int *a, int N){
  printf("[%d", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %d", a[i]);
  }
  printf("]\n");
}

void print_array_double(double *a, int N){
  printf("[%f", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %f", a[i]);
  }
  printf("]\n");
}











#endif
