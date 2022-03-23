#include "main.h"
#include <omp.h>

void check_score_top(int n, size_t i, double score, int index, double *top_array, int *top_index);

void top_n_webpages(int N, double *scores, int n){
  int count_exchange;
  int *index, *top_index, *index_th, *top_index_th;
  double *top_scores, *top_scores_th;
  double start, end;


  start = omp_get_wtime(); // start of timing

  // In case of to high n
  if (n > N){
    n = N;
  }

  // Find top n scores and coresponding indexes
  top_scores = calloc(n, sizeof(double));
  top_index = calloc(n, sizeof(double));

  #pragma omp parallel private(top_index_th, top_scores_th)
  {
    #pragma omp single
    { printf("Sorting for top %d webpages (num threads: %d)\n", n, omp_get_num_threads()); }

    // Find top scores and indexes for smaller blocks in parallel
    top_index_th = calloc(n, sizeof(int));
    top_scores_th = calloc(n, sizeof(double));
    #pragma omp for
    for (size_t i = 0; i < N; i++) {
      check_score_top(n, i, scores[i], i, top_scores_th, top_index_th);
    }

    // Combine resuults for each thread
    #pragma omp critical
    {
      for (size_t i = 0; i < n; i++) {
        check_score_top(n, i, top_scores_th[i], top_index_th[i], top_scores, top_index);
      }
    } // end of critical
} // end of parallel region

  // Print top n-webpages
  end = omp_get_wtime(); // end of timing
  print_top_webpages(n, N, top_index, top_scores);
  printf("--> (time used: %g s)\n\n", (double) (end-start));

 } // end of function



void check_score_top(int n, size_t i, double score, int index, double *top_array, int *top_index){
  /* Check whether score is among the top n of top_array
      and place its score and index in top_array and
      top_index if valid  */

  size_t top_idx;
  if (score > top_array[n-1])
  {
    top_idx = 0;
    for (int j = n-2; j >= 0; j--) {
      if (score <= top_array[j])
      {
        top_idx = j+1;
        break;
      }
    } // end of j-loop

    // move over current top scores and indexes
    for (size_t k = n-1; k > top_idx; k--) {
      top_array[k] = top_array[k-1];
      top_index[k] = top_index[k-1];
    }
    // Assign current place in top_score
    top_array[top_idx] = score;
    top_index[top_idx] = index;

  } // end of if

} // end of function
