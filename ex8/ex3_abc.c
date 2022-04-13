#include <stdlib.h>
#include <stdio.h>

void sort(int *a, int N);
int compare_exhange(int *a0, int *a1);
void print_array(int *a, int N);

int main(){
  int N;
  int *a;

  N = 5;
  a = malloc(N * sizeof(a));

  a[0] = 5;
  a[1] = 4;
  a[2] = 3;
  a[3] = 2;
  a[4] = 1;

  print_array(a, N);
  sort(a, N);
  print_array(a, N);



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
  for (int i = 1; i < N+1; i++){
    count_exchange = 0;
    if (i%2 == 1) // i is odd
    {
     for (int j = 0; j < (N - N%2)/2; j++){
       count_exchange += compare_exhange(&a[2*j], &a[2*j+1]);
     }
    }
    if (i%2 == 0)// i is even
    {
    for (int j = 1; j < N/2 + N%2; j++){
      count_exchange += compare_exhange(&a[2*j-1], &a[2*j]);

    }
   }

   if (i%2 == 0 & count_exchange == 0){
     printf("Early stop, i = %d/%d\n", i, N);
     break;
   }
    }
  }


void print_array(int *a, int N){
  printf("[%d", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %d", a[i]);
  }
  printf("]\n");



}
