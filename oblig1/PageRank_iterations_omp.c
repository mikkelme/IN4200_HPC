#include "main.h"
#include <omp.h>


// PageRank change for 100nodes - check that


void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
  int edges, stop, num_dangling, non_dangling;
  size_t j, k;
  double W, iter_factor, dotp, max_diff, diff;
  int *col_check, *dangling_idx;
  double *old_scores;

  printf("Running page rank algorithm\n");
  // --- Indices for dangling webpages ---//
  edges = row_ptr[N];
  col_check = calloc(N, sizeof(int)); // assign 1 for used col and 0 for empty
  non_dangling = 0;


  #pragma omp parallel
  {

    #pragma omp for
    for (size_t i = 0; i < edges; i++) {
      col_check[col_idx[i]] = 1;
    }

    #pragma omp for reduction(+ : non_dangling)
    for (size_t i = 0; i < N; i++) {
        non_dangling += col_check[i];
    }

    #pragma omp single
      {
        num_dangling = N - non_dangling;
        dangling_idx = malloc(num_dangling * sizeof(int));

        j = 0;
        for (size_t i = 0; i < N; i++) {
          if (col_check[i] == 0){
            dangling_idx[j] = i;
            j+= 1;
          }
        }
      } // end of single




    // --- Iterative procedure ---//
    #pragma omp single
    {
      stop = 0;
      k = 0;
      old_scores = malloc((N) * sizeof(double));
    }


    while (stop == 0){
      // reset variables
      #pragma single
      { W = 0;  max_diff = 0; }


      // Copy scores to old_scores
      #pragma omp single
      { memcpy(old_scores, scores, N*sizeof(double)); }


      // Calculate W
      #pragma omp for reduction(+ : W) nowait
      for (size_t i = 0; i < num_dangling; i++) {
        W += scores[dangling_idx[i]];
      }


      // Calculate scores
      #pragma omp single
      { iter_factor = (1-d + d*W)/N; }

      #pragma omp for private(j, dotp)
      for (size_t i = 0; i < N; i++) {
        dotp = 0;
        for (size_t j = row_ptr[i]; j < row_ptr[i+1]; j++) {
          dotp += val[j]*old_scores[col_idx[j]];
        }
        scores[i] = iter_factor + d*dotp;
      }


      // Evaluate stopping criteria
      #pragma omp parallel for private(diff) reduction(max : max_diff)
      for (size_t i = 0; i < N; i++) {
        diff = fabs(scores[i] - old_scores[i]);
        if (diff > max_diff){
          max_diff = diff;
        }
      }

      #pragma omp single
      {
        k += 1;
        if (k%100 == 0){ printf("Iteration %5zu | max_diff: %.3g\n", k, max_diff); }

        if (max_diff < epsilon){
          printf("--> Converged after %zu iterations\n\n", k);
          stop = 1;
        }
      }

    } // end of while-loop

  } // end of parallel region


  free(col_check);
  free(old_scores);
} // end of function
