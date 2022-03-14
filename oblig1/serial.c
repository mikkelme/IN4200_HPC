#include <stdlib.h>
#include <stdio.h>
#include "functions.h"



int main(int argc, char const *argv[]) {


  char *filename = "8-webpages.txt";
  int N, n;
  double d, epsilon;
  int *row_ptr, *col_idx;
  double *val, *scores;

  d = 1;
  epsilon = 0.0001;

  read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);


  scores =  malloc((N) * sizeof(double));
  for (size_t i = 0; i < N; i++) {
    scores[i] = 1./N;
  }


  PageRank_iterations(N, row_ptr, col_idx, val, d, epsilon, scores);


  n = 10;
  top_n_webpages(N, scores, n);
  print_array_double(scores, N);

  // test_read_graph_from_file();



  free(row_ptr);
  free(col_idx);
  free(val);
  free(scores);


  return 0;
}
