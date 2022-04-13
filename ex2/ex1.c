#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int main(){
  FILE *fp;
  char filename_ASC[] = "ASCII.txt";
  char filename_bin[] = "BIN.bin";

  int N = 1e7;
  clock_t start, timer_ASC, timer_bin;
  int *data = (int *)malloc(N * sizeof(int));


  // ASCII file
  start = clock();

  fp = fopen(filename_ASC, "w");
  for (int i=0; i < N; i++){
    fprintf(fp, "%d\n", i);
  }
  fclose(fp);
  fp = fopen(filename_ASC, "r");
  for (int i=0; i < N; i++){
    fscanf(fp, "%d\n", &data[i]);
  }

  timer_ASC = clock() - start;


  // Binary file
  start = clock();

  fp = fopen(filename_bin, "wb");
  for (int i=0; i < N; i++){
    fwrite(&i, sizeof(int), 1, fp);
    // fwrite(fp, "%d\n", i);
  }
  fclose(fp);
  fp = fopen(filename_bin, "rb");
  for (int i=0; i < N; i++){
    fread(&data[i], sizeof(int), 1, fp);
  }

  timer_bin = clock() - start;

  // // fread()/fwrite()
  //
  printf("Time ASCII: %lu ms \n", 1000*timer_ASC/CLOCKS_PER_SEC);
  printf("Time Binary: %lu ms\n", 1000*timer_bin/CLOCKS_PER_SEC);





  // for (int i=0; i < N; i++){
  //   printf("%d\n", data[i]);
  // }









  return 0;
}
