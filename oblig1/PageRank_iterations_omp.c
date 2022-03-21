#include "main.h"
#include <omp.h>



void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
  int edges, stop, num_dangling, non_dangling;
  size_t j, k;
  double W, iter_factor, dotp, max_diff, diff, last_diff;
  int *col_check, *dangling_idx;
  double *old_scores, *tmp;
  double start, end;


  start = omp_get_wtime();
  #pragma omp parallel
  {

    #pragma omp single
    {
      printf("Running page rank algorithm (num threads: %d)\n", omp_get_num_threads());
      // --- Indices for dangling webpages ---//
      edges = row_ptr[N];
      col_check = calloc(N, sizeof(int)); // assign 1 for used col and 0 for empty
      non_dangling = 0;
    }

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
      memcpy(old_scores, scores, N*sizeof(double));
      last_diff = 1e6;

    }


    while (stop == 0){
      // reset variables
      #pragma single
      { W = 0;  max_diff = 0; }


      #pragma single
      { memcpy(old_scores, scores, N*sizeof(double));}



      // Calculate W
      #pragma omp for reduction(+ : W)
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
        // // pointer swap
        // tmp = scores;
        // scores = old_scores;
        // old_scores = tmp;

        if (k%100 == 0){
          printf("Iteration %5zu | max_diff: %.3g\n", k, max_diff);
          printf("%.12f\n", last_diff-max_diff);
          if (last_diff - max_diff < epsilon) {
            printf("--> Algorithm didn't converge\n");
            exit(1);
          }
          last_diff = max_diff;
        }

        if (max_diff < epsilon){
          end = omp_get_wtime();
          printf("--> Converged after %zu iterations (time used: %g s)\n\n", k, (double) (end-start));
          stop = 1;
        }
      }

    } // end of while-loop


  } // end of parallel region

  // if (k%2 == 1){
  //   // pointer swap
  //   tmp = scores;
  //   scores = old_scores;
  //   old_scores = tmp;
  // }


  free(col_check);
  free(old_scores);
} // end of function
