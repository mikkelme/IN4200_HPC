#include "main.h"


int main(int argc, char *argv[]) {
  int N, n, fortran_read_mode;
  double d, epsilon;
  int *row_ptr, *col_idx;
  double *val, *scores;
  char *filename;


  // Read input variables from command line
  if (argc < 5){
    printf("Please provide arguments: d, epsilon, n, filename, (read mode) in the command line.\n");
    exit(1);
  }

  d = atof(argv[1]);
  epsilon = atof(argv[2]);
  n = atoi(argv[3]);
  filename = argv[4];

  if (argc == 6){ fortran_read_mode = atoi(argv[5]); }
  else          { fortran_read_mode = 0; }
  N = fortran_read_mode;

  // Read graph
  read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);

  // Intialize scores and run pagerank algorithm
  scores =  malloc((N) * sizeof(double));
  for (size_t i = 0; i < N; i++) {
    scores[i] = (double) 1./N;
  }
  PageRank_iterations(N, row_ptr, col_idx, val, d, epsilon, scores);


  // Show top n webpages
  top_n_webpages(N, scores, n);


  // Free memory 
  free(row_ptr);
  free(col_idx);
  free(val);
  free(scores);

  return 0;
}
