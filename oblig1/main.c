#include "main.h"


int main(int argc, char *argv[]) {
  int N, n, fortran_read_mode;
  double d, epsilon;
  int *row_ptr, *col_idx;
  double *val, *scores;
  char *filename;

  // d = 0.98;
  // epsilon = 1e-5;
  // filename = "8-webpages.txt";
  // filename = "100nodes_graphs.txt";
  // filename = "web-stanford.txt";


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


  read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);


  scores =  malloc((N) * sizeof(double));
  for (size_t i = 0; i < N; i++) {
    scores[i] = (double) 1./N;
  }


  PageRank_iterations(N, row_ptr, col_idx, val, d, epsilon, scores);

  top_n_webpages(N, scores, n);

  free(row_ptr);
  free(col_idx);
  free(val);
  free(scores);

  return 0;
}
