#include "main.h"




void top_n_webpages(int N, double *scores, int n){
  int count_exchange;

  if (n > N){
    n = N;
  }

  // Webpage index array
  int *webpage_index = malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    webpage_index[i] = i;
  }



  // Sort scores (this will change it permanently but we do not need it afterwards i guess)
  for (size_t i = 1; i < N+1; i++){
    count_exchange = 0;
    if (i%2 == 1) // i is odd
    {
     for (size_t j = 0; j < (N - N%2)/2; j++){
       if ( compare(&scores[2*j], &scores[2*j+1]) )
       {
        exchange_double(&scores[2*j], &scores[2*j+1]);
        exchange_int(&webpage_index[2*j], &webpage_index[2*j+1]);

        count_exchange += 1;
       }
     }
    }
    if (i%2 == 0)// i is even
    {
    for (size_t j = 1; j < N/2 + N%2; j++){
      if (compare(&scores[2*j-1], &scores[2*j]) )
      {
        exchange_double(&scores[2*j-1], &scores[2*j]);
        exchange_int(&webpage_index[2*j-1], &webpage_index[2*j]);

        count_exchange += 1;
      }
    }
   }

   // Early stopping
   if (i%2 == 0 & count_exchange == 0){
     printf("Early stop, i = %zu/%d\n", i, N);
     break;
   }
 } // end of sort-loop


 // Print top n-webpages
 for (size_t i = 0; i < n; i++) {
   printf("Rank: %zu, Webpage no.: %d, Score: %f\n", i+1, webpage_index[N-1 -i], scores[N-1 - i]);
 }






} // end of function
