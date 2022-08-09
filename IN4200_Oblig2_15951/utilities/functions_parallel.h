#ifndef FUNCTIONS_PARALLEL_H
#define FUNCTIONS_PARALLEL_H

#include "../utilities/functions.h"
#include <mpi.h>

void partion_1D(int m, int n, int *my_m, int *my_n, int *my_prod, int my_rank, int num_procs);
void distribute_regions(int m, int n, unsigned char *image_chars, unsigned char *my_image_chars, int my_m, int my_n, int my_prod, int my_rank, int num_procs);
void gather_regions(int m, int n, image *whole_image, image *u_bar, int my_m, int my_n, int my_prod, int my_rank, int num_procs);
void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters, int my_rank, int num_procs);

#endif // FUNCTIONS_PARALLEL_H