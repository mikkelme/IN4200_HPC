#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ctype.h>
#include <string.h>
#include <math.h>



void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores);
void top_n_webpages(int N, double *scores, int n);
int compare(double *a0, double *a1);
void exchange_double(double *a0, double *a1);
void exchange_int(int *a0, int *a1);
void print_array_int(int *a, int N);
void print_array_double(double *a, int N);
void test_graph_from_file();


void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val){
  int edges, tmp1, tmp2;
  int *L_j, *L_i, *col_counter;

  // Open file
  FILE *file = fopen(filename, "r");
  if (file == NULL){
    printf("Error: Could not open file %s\n", filename);
    exit(1);
  }

  //--- First read through ---//
  /*  Get number of nodes N, numbers of edges and fill row_ptr.
      Count values per column in L_j  */

  // Create buffer for line input, max 256 bytes
  const unsigned MAX_LENGTH = 256;
  char buffer[MAX_LENGTH];

  // Skip first two lines (comments)
  for (size_t i = 0; i < 2; i++){fgets(buffer, MAX_LENGTH, file);}

  // Get Nodes and Edges
  fscanf(file, "%*s %*s %d %*s %d %*[^\n]\n", N, &edges); // and skip next line


  L_j =  malloc(*N * sizeof(L_j)); // for counting number of values per column
  L_i =  malloc(*N * sizeof(L_i)); // for counting number of values per row


  *row_ptr =  malloc((*N+1) * sizeof(int));
  (*row_ptr)[0] = 0;


  // Count number of rows and columns
  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &tmp1, &tmp2);
    L_i[tmp2] += 1;
    L_j[tmp1] += 1;
  }

  // Fill in row_ptr
  for (size_t i = 1; i < *N+1; i++) {
    (*row_ptr)[i] = (*row_ptr)[i-1] + L_i[i-1];
  }


  //--- Second read ---//
  /* Get: values and col_idx */
  rewind(file);
  *col_idx = malloc((edges) * sizeof(int));
  *val = malloc((edges) * sizeof(double));

  for (size_t i = 0; i < edges; i++) {(*col_idx)[i] = *N;} // fill with high number
  col_counter =  malloc((edges) * sizeof(col_counter));

  // Skip first four coment lines
  for (size_t i = 0; i < 4; i++){fgets(buffer, MAX_LENGTH, file);}

  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &tmp1, &tmp2);


    for (size_t j = 0; j < (*row_ptr)[tmp2+1]-(*row_ptr)[tmp2]; j++) {
      if (tmp1 < (*col_idx)[(*row_ptr)[tmp2] + j]){
        for (size_t k = col_counter[tmp2]; k > j ; k--)
        { // Move values to the right
          (*col_idx)[(*row_ptr)[tmp2] + k] = (*col_idx)[(*row_ptr)[tmp2] + k-1];
          (*val)[(*row_ptr)[tmp2] + k] = (*val)[(*row_ptr)[tmp2] + k-1];
        } // end of k-loop

        (*col_idx)[(*row_ptr)[tmp2] + j] = tmp1;
        (*val)[(*row_ptr)[tmp2] + j] = 1./L_j[tmp1];
        col_counter[tmp2] += 1;
        break;

      } // end of if
    } // end of j-loop
} // end of i-loop


} // end of function



void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
  int edges, stop;
  size_t k;
  double W, iter_factor, dotp, max_diff, diff;
  int *col_check;
  double *old_scores;


  // --- Indices for dangling webpages ---//
  edges = row_ptr[N];
  col_check = malloc(N * sizeof(int)); // assign 1 for used col and 0 for empty
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
    double diff;
    for (size_t i = 0; i < N; i++) {
      diff = fabs(scores[i] - old_scores[i]);
      if (diff > max_diff){
        max_diff = diff;
      }
    }


    k += 1;
    // printf("Iteration %zu | max_diff: %f\n", k, max_diff);

    if (max_diff < epsilon){
      stop = 1;
    }
  } // end of while-loop
} // end of function


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










int compare(double *a0, double *a1){
  if (*a0 > *a1){
    return 1;
  }
  return 0;
}

void exchange_double(double *a0, double *a1){
  double tmp = *a0;
  *a0 = *a1;
  *a1 = tmp;
}

void exchange_int(int *a0, int *a1){
  int tmp = *a0;
  *a0 = *a1;
  *a1 = tmp;
}




void print_array_int(int *a, int N){
  printf("[%d", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %d", a[i]);
  }
  printf("]\n");
}

void print_array_double(double *a, int N){
  printf("[%f", a[0]);
  for (int i = 1; i < N; i++){
    printf(", %f", a[i]);
  }
  printf("]\n");
}

void test_read_graph_from_file(){
  /*  Test read_graph_from_file using predefined
      expected values to the web graph 8-webpages.txt */
  char *filename = "8-webpages.txt";
  int N;
  int *row_ptr, *col_idx;
  double *val;

  // Call function
  read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);

  // Expected values
  static int row_ref[] = {0,1,4,5,6,9,12,14,17};
  static int col_ref[] = {6,0,2,3,0,1,2,3,6,3,4,7,4,7,4,5,6};
  static double val_ref[] = {0.333333,0.500000,0.500000,0.333333,0.500000,
                             1.000000,0.500000,0.333333,0.333333,0.333333,0.333333,
                             0.500000,0.333333,0.500000,0.333333,1.000000,0.333333};

  // check row_ptr
  int test_status = 1;
  for (size_t i = 0; i < 9; i++) {
    int row_diff = row_ref[i] - row_ptr[i];
    if (row_diff > 0){
      printf("Inccorect row_ptr: row_ptr[%zu] = %d != %d\n", i, row_ptr[i],  row_ref[i] );
      test_status = 0;
    }
  }

  // Check col_idx
  for (size_t i = 0; i < 17; i++) {
    int col_diff = col_ref[i] - col_idx[i];
    if (col_diff > 0){
      printf("Inccorect col_idx: col_idx[%zu] = %d != %d\n", i, col_idx[i],  col_ref[i] );
      test_status = 0;
    }
  }

  // Check val
  for (size_t i = 0; i < 17; i++) {
    double val_diff = val_ref[i] - val[i];
    if (val_diff > 0.000001){
      printf("Inccorect val: val[%zu] = %f != %f\n", i, val[i],  val_ref[i] );
      test_status = 0;
    }
  }

  // Result
  if (test_status == 0){
    printf("Test failed\n");
  }
  else {
    printf("Test passed\n");
  }


} // end of function






#endif
