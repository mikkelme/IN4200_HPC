#include "main.h"
#include <omp.h>

void check_score_top(int n, size_t i, double score, int index, double *top_array, int *top_index);

void top_n_webpages(int N, double *scores, int n){
  int count_exchange;
  int *index, *top_index, *index_th, *top_index_th;
  double *top_scores, *top_scores_th;

  // In case of to high n
  if (n > N){
    n = N;
  }

  top_scores = calloc(n, sizeof(double));
  top_index = calloc(n, sizeof(double));

  #pragma omp parallel private(top_index_th, top_scores_th)
  {
    top_index_th = calloc(n, sizeof(int));
    top_scores_th = calloc(n, sizeof(double));
    #pragma omp for
    for (size_t i = 0; i < N; i++) {
      check_score_top(n, i, scores[i], i, top_scores_th, top_index_th);
    }

    #pragma omp critical
    {
      // int thread_id = omp_get_thread_num();
      // printf("Thread: %d\n", thread_id);
      // print_array_double(top_scores_th, n);
      for (size_t i = 0; i < n; i++) {
        check_score_top(n, i, top_scores_th[i], top_index_th[i], top_scores, top_index);
      }
    } // end of critical
} // end of parallel region

  // Print top n-webpages
  print_top_webpages(n, N, top_index, top_scores);


 } // end of function



void check_score_top(int n, size_t i, double score, int index, double *top_array, int *top_index){
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

    // move over
    for (size_t k = n-1; k > top_idx; k--) {
      top_array[k] = top_array[k-1];
      top_index[k] = top_index[k-1];
    }
    // Assign current place in top_score
    top_array[top_idx] = score;
    top_index[top_idx] = index;

  } // end of if

} // end of function
