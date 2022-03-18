#include "main.h"


void top_n_webpages(int N, double *scores, int n){
  int count_exchange;
  size_t top_idx;
  int *top_index;
  double *top_scores;

  // In case of to high n
  if (n > N){
    n = N;
  }


  top_scores = calloc(n, sizeof(double));
  top_index = calloc(n, sizeof(double));

  // Try this
  // for (size_t i = 0; i < N; i++) {
  //   my_func2(n, i, scores[i], i, top_scores, top_index);
  // }


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

      // move over
      for (size_t k = n-1; k > top_idx; k--) {
        top_scores[k] = top_scores[k-1];
        top_index[k] = top_index[k-1];
      }

      // Assign current place in top_score
      top_scores[top_idx] = scores[i];
      top_index[top_idx] = i;
    } // end of if

  }


 // Print top n-webpages
 print_top_webpages(n, N, top_index, top_scores);


} // end of function


// void top_n_webpages(int N, double *scores, int n){
//   int count_exchange;
//
//   // In case of to high n
//   if (n > N){
//     n = N;
//   }
//
//   // Webpage index array
//   int *webpage_index = malloc(N * sizeof(int));
//   for (int i = 0; i < N; i++) {
//     webpage_index[i] = i;
//   }
//
//
//   // Sort scores (this will change it permanently but we do not need it afterwards i guess)
//   for (size_t i = 1; i < N+1; i++){
//     count_exchange = 0;
//     if (i%2 == 1) // i is odd
//     {
//      for (size_t j = 0; j < (N - N%2)/2; j++){
//        if ( compare(&scores[2*j], &scores[2*j+1]) )
//        {
//         exchange_double(&scores[2*j], &scores[2*j+1]);
//         exchange_int(&webpage_index[2*j], &webpage_index[2*j+1]);
//
//         count_exchange += 1;
//        }
//      }
//     }
//     if (i%2 == 0)// i is even
//     {
//     for (size_t j = 1; j < N/2 + N%2; j++){
//       if (compare(&scores[2*j-1], &scores[2*j]) )
//       {
//         exchange_double(&scores[2*j-1], &scores[2*j]);
//         exchange_int(&webpage_index[2*j-1], &webpage_index[2*j]);
//
//         count_exchange += 1;
//       }
//     }
//    }
//
//    // Early stopping
//    if (i%2 == 0 & count_exchange == 0){
//      // printf("Early stop, i = %zu/%d\n", i, N);
//      break;
//    }
//  } // end of sort-loop
//
//
//  // Print top n-webpages
//  for (size_t i = 0; i < n; i++) {
//    printf("Rank: %zu, Webpage no.: %d, Score: %f\n", i+1, webpage_index[N-1 -i], scores[N-1 - i]);
//  }
//
//
// } // end of function
