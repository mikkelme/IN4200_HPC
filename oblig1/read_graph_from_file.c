
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


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



//
// void test_read_graph_from_file(){
//   /*  Test read_graph_from_file using predefined
//       expected values to the web graph 8-webpages.txt */
//   char *filename = "8-webpages.txt";
//   int N;
//   int *row_ptr, *col_idx;
//   double *val;
//
//   // Call function
//   read_graph_from_file(filename, &N, &row_ptr, &col_idx, &val);
//
//   // Expected values
//   static int row_ref[] = {0,1,4,5,6,9,12,14,17};
//   static int col_ref[] = {6,0,2,3,0,1,2,3,6,3,4,7,4,7,4,5,6};
//   static double val_ref[] = {0.333333,0.500000,0.500000,0.333333,0.500000,
//                              1.000000,0.500000,0.333333,0.333333,0.333333,0.333333,
//                              0.500000,0.333333,0.500000,0.333333,1.000000,0.333333};
//
//   // check row_ptr
//   int test_status = 1;
//   for (size_t i = 0; i < 9; i++) {
//     int row_diff = row_ref[i] - row_ptr[i];
//     if (row_diff > 0){
//       printf("Inccorect row_ptr: row_ptr[%zu] = %d != %d\n", i, row_ptr[i],  row_ref[i] );
//       test_status = 0;
//     }
//   }
//
//   // Check col_idx
//   for (size_t i = 0; i < 17; i++) {
//     int col_diff = col_ref[i] - col_idx[i];
//     if (col_diff > 0){
//       printf("Inccorect col_idx: col_idx[%zu] = %d != %d\n", i, col_idx[i],  col_ref[i] );
//       test_status = 0;
//     }
//   }
//
//   // Check val
//   for (size_t i = 0; i < 17; i++) {
//     double val_diff = val_ref[i] - val[i];
//     if (val_diff > 0.000001){
//       printf("Inccorect val: val[%zu] = %f != %f\n", i, val[i],  val_ref[i] );
//       test_status = 0;
//     }
//   }
//
//   // Result
//   if (test_status == 0){
//     printf("Test failed\n");
//   }
//   else {
//     printf("Test passed\n");
//   }
//
//
// } // end of function
