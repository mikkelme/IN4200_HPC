#include "main.h"


void top_n_webpages(int N, double *scores, int n){
  int count_exchange;
  size_t top_idx;
  int *top_index;
  double *top_scores;
  time_t start, end;


  printf("Sorting for top %d webpages\n", n);
  start = clock(); // start of timing

  // In case of to high n
  if (n > N){
    n = N;
  }

  // Find top n scores and coresponding indexes
  top_scores = calloc(n, sizeof(double));
  top_index = calloc(n, sizeof(double));

  for (size_t i = 0; i < N; i++) {
    if (scores[i] > top_scores[n-1])
    {

      top_idx = 0;
      for (int j = n-2; j >= 0; j--) {
        if (scores[i] <= top_scores[j])
        {
          top_idx = j+1;
          break;
        }
      } // end of j-loop

      // move over current top scores and indexes
      for (size_t k = n-1; k > top_idx; k--) {
        top_scores[k] = top_scores[k-1];
        top_index[k] = top_index[k-1];
      }

      // Assign current place in top_score
      top_scores[top_idx] = scores[i];
      top_index[top_idx] = i;
    } // end of if
  } // end of i-loop

 // Print top n-webpages
 end = clock(); // end of timing
 print_top_webpages(n, N, top_index, top_scores);
 printf("--> (time used: %g s)\n\n", (double) (end-start)/CLOCKS_PER_SEC);


} // end of function
