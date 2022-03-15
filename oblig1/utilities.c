#include "main.h"


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
