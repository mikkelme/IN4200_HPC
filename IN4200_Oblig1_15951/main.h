#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Fundamental functions
void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores);
void top_n_webpages(int N, double *scores, int n);

// Utilities
void print_array_int(int *a, int N);
void print_array_double(double *a, int N);
void print_top_webpages(int n, int N, int *top_index, double *top_scores);


#endif
