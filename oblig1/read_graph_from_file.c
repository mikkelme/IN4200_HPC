#include "main.h"

void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val){
  int edges, FromNodeId , ToNodeId;
  int *L_j, *col_counter;
  int fortran_read_mode = *N;
  time_t start, end;


  printf("\nReading file: %s\n", filename);
  start = clock(); // start of timing

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

  // Get Nodes and Edges (and skip next line)
  fscanf(file, "%*s %*s %d %*s %d %*[^\n]\n", N, &edges);

  L_j =  calloc(*N, sizeof(L_j)); // for counting number of values per column
  *row_ptr = calloc(*N+1, sizeof(int));

  // Count number of elements per row and columns
  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &FromNodeId , &ToNodeId);
    if (fortran_read_mode){ FromNodeId -= 1; ToNodeId -= 1;}
    (*row_ptr)[ToNodeId+1] += 1;  // count per row
    L_j[FromNodeId ] += 1;        // count per column
  }

  // Fill in row_ptr (cumulative sum)
  for (size_t i = 1; i < *N+1; i++) {
    (*row_ptr)[i] += (*row_ptr)[i-1];
  }


  //--- Second read through ---//
  /* fill val and col_idx */

  rewind(file); // go back to top of file
  *col_idx = malloc((edges) * sizeof(int));
  *val = malloc((edges) * sizeof(double));
  col_counter = calloc(edges, sizeof(col_counter));

  // Skip first four coment lines
  for (size_t i = 0; i < 4; i++){fgets(buffer, MAX_LENGTH, file);}

  // Get col_idx and val
  for (size_t i = 0; i < edges; i++) {
    fscanf(file, "%d %d", &FromNodeId , &ToNodeId);
    if (fortran_read_mode){ FromNodeId -= 1; ToNodeId -= 1;}

    // Set values
    (*col_idx)[(*row_ptr)[ToNodeId] + col_counter[ToNodeId]] = FromNodeId ;
    (*val)[(*row_ptr)[ToNodeId] + col_counter[ToNodeId]] = (double) 1./L_j[FromNodeId ];
    col_counter[ToNodeId] += 1;

  } // end of i-loop


  end = clock(); // end of timing
  printf("--> File reading completed (time used: %g s)\n\n", (double) (end-start)/CLOCKS_PER_SEC);


  // free memory
  free(L_j);
  free(col_counter);
  
} // end of function
