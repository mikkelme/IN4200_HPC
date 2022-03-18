#include "main.h"


int main(int argc, char const *argv[]) {
  int N, n, fortran_read_mode;
  double d, epsilon;
  int *row_ptr, *col_idx;
  double *val, *scores;
  char *filename;

  d = 1;
  epsilon = 1e-6;


  // filename = "8-webpages.txt";
  filename = "100nodes_graphs.txt";
  // filename = "web-stanford.txt";

  fortran_read_mode = 0;
  if (fortran_read_mode){ N = 1; }
  else { N = 0; }

  read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);


  scores =  malloc((N) * sizeof(double));
  for (size_t i = 0; i < N; i++) {
    scores[i] = (double) 1./N;
  }


  PageRank_iterations(N, row_ptr, col_idx, val, d, epsilon, scores);

  n = 10;
  top_n_webpages(N, scores, n);

  free(row_ptr);
  free(col_idx);
  free(val);
  free(scores);

  return 0;
}
