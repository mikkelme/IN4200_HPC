#include "main.h"

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

  // *row_ptr =  malloc((*N+1) * sizeof(int));
  *row_ptr =  calloc(*N+1, sizeof(int));

  // check form here...
  exit(0);
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
