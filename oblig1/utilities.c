#include "main.h"

void print_array_int(int *a, int N){
  /* Print array of type int with length N */

  printf("[%d", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %d", a[i]);
  }
  printf("]\n");
}

void print_array_double(double *a, int N){
  /* Print array of type double with length N */

  printf("[%f", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %f", a[i]);
  }
  printf("]\n");
}


void print_top_webpages(int n, int N, int *top_index, double *top_scores){
  /* Print top n webpages in a nice formatted way */

  int rank_digits = 1 + log10(n);
  int webpage_digits = 1 + log10(N);

  printf("-----------------------------------------\n");
  printf("           Top %02d Page Ranking           \n", n);
  printf("-----------------------------------------\n");
  for (size_t i = 0; i < n; i++) {
    printf("Rank: %0*zu, Webpage no.: %0*d, Score: %f\n", rank_digits, i+1, webpage_digits, top_index[i], top_scores[i]);
  }
  printf("-----------------------------------------\n");

}
