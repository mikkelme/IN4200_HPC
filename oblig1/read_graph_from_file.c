#include "main.h"

void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val){
  int edges, FromNodeId , ToNodeId;
  int *L_j, *col_counter;

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


  L_j =  calloc(*N, sizeof(L_j)); // for counting number of values per column
  *row_ptr = calloc(*N+1, sizeof(int));


  // Count number of elements per row and columns
  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &FromNodeId , &ToNodeId);
    (*row_ptr)[ToNodeId+1] += 1;  // count per row
    L_j[FromNodeId ] += 1;           // count per column

  }

  // Fill in row_ptr (cumulative sum)
  for (size_t i = 1; i < *N+1; i++) {
    (*row_ptr)[i] += (*row_ptr)[i-1];
  }


  //--- Second read ---//
  /* fill val and col_idx */
  rewind(file);
  *col_idx = malloc((edges) * sizeof(int));
  *val = malloc((edges) * sizeof(double));
  col_counter = calloc(edges, sizeof(col_counter));

  for (size_t i = 0; i < edges; i++) {(*col_idx)[i] = *N;} // fill col_idx with high number

  // Skip first four coment lines
  for (size_t i = 0; i < 4; i++){fgets(buffer, MAX_LENGTH, file);}

  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &FromNodeId , &ToNodeId);

    // Set values
    (*col_idx)[(*row_ptr)[ToNodeId] + col_counter[ToNodeId]] = FromNodeId ;
    (*val)[(*row_ptr)[ToNodeId] + col_counter[ToNodeId]] = (double) 1./L_j[FromNodeId ];
    col_counter[ToNodeId] += 1;
    
  } // end of i-loop


  free(L_j);
  free(col_counter);


} // end of function





// old loop (sorting each column in col_idx)

// for (size_t i = 0; i < edges; i++) {
//   fscanf(file, "%d %d", &FromNodeId , &ToNodeId);
//
//   for (size_t j = 0; j < (*row_ptr)[ToNodeId+1]-(*row_ptr)[ToNodeId]; j++) {
//     if (FromNodeId  < (*col_idx)[(*row_ptr)[ToNodeId] + j]){
//       for (size_t k = col_counter[ToNodeId]; k > j ; k--)
//       { // Move values to the right
//         (*col_idx)[(*row_ptr)[ToNodeId] + k] = (*col_idx)[(*row_ptr)[ToNodeId] + k-1];
//         (*val)[(*row_ptr)[ToNodeId] + k] = (*val)[(*row_ptr)[ToNodeId] + k-1];
//       } // end of k-loop
//
//       (*col_idx)[(*row_ptr)[ToNodeId] + j] = FromNodeId ;
//       (*val)[(*row_ptr)[ToNodeId] + j] = 1./L_j[FromNodeId ];
//       col_counter[ToNodeId] += 1;
//       break;
//
//     } // end of if
//   } // end of j-loop
// } // end of i-loop
