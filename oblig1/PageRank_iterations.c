#include "main.h"


void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
  int edges, stop;
  size_t k;
  double W, iter_factor, dotp, max_diff, diff, last_diff;
  int *col_check;
  double *old_scores;
  time_t start, end;

  printf("Running page rank algorithm\n");
  start = clock();

  // --- Indices for dangling webpages ---//
  edges = row_ptr[N];
  col_check = calloc(N, sizeof(int)); // assign 1 for used col and 0 for empty
  for (size_t i = 0; i < edges; i++) {
    col_check[col_idx[i]] = 1;
  }

  int num_dangling = N;
  for (size_t i = 0; i < N; i++) {
      num_dangling -= col_check[i];
  }

  int *dangling_idx = malloc(num_dangling * sizeof(int));
  size_t j = 0;
  for (size_t i = 0; i < N; i++) {
    if (col_check[i] == 0){
      dangling_idx[j] = i;
      j+= 1;
    }
  }


  // --- Iterative procedure ---//
  stop = 0;
  k = 0;
  old_scores = malloc((N) * sizeof(double));
  last_diff = 1e6;

  while (stop == 0){
    // Copy scores to old_scores
    memcpy(old_scores, scores, N*sizeof(double));

    // Calculate W
    W = 0;
    for (size_t i = 0; i < num_dangling; i++) {
      W += scores[dangling_idx[i]];
    }


    // Calculate scores
    iter_factor = (1-d + d*W)/N;
    for (size_t i = 0; i < N; i++) {
      dotp = 0;
      for (size_t j = row_ptr[i]; j < row_ptr[i+1]; j++) {
        dotp += val[j]*old_scores[col_idx[j]];
      }
      scores[i] = iter_factor + d*dotp;
    }


    // Evaluate stopping criteria
    max_diff = 0;
    for (size_t i = 0; i < N; i++) {
      diff = fabs(scores[i] - old_scores[i]);
      if (diff > max_diff){
        max_diff = diff;
      }
    }


    k += 1;
    if (k%100 == 0){
      printf("Iteration %5zu | max_diff: %.3g\n", k, max_diff);
      if (last_diff - max_diff < 1e-10) {
        printf("--> Algorithm didn't converge\n");
        exit(1);
      }
      last_diff = max_diff;
    }


    if (max_diff < epsilon){
      end = clock();
      printf("--> Converged after %zu iterations (time used: %g s)\n\n", k, (double) (end-start)/CLOCKS_PER_SEC);
      stop = 1;
    }
  } // end of while-loop


  free(col_check);
  free(old_scores);
} // end of function
