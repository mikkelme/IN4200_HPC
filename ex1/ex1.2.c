
#include <stdlib.h>
#include <stdio.h>




int main(int num_arg, char ** args){
  if (num_arg < 2) {
    printf("%s\n", "Provide array length" );
    return EXIT_FAILURE;
  }
  int n = atoi(args[1]);


  int min, max;
  int *rand_array = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    rand_array[i] = rand();
    printf("%i\n", rand_array[i] );
  }


  min = rand_array[0];
  max = rand_array[0];

  for (int i=0; i<n; i++){
    if (rand_array[i] < min){
      min = rand_array[i];
    }

    if (rand_array[i] > max){
      max = rand_array[i];
    }


  }
  printf("%i, %i \n", min, max );





}
