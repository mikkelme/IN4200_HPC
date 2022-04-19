#ifndef FUNCTIONS_PARALLEL_H
#define FUNCTIONS_PARALLEL_H

#include "../utilities/functions.h"
#include <mpi.h>

void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters);


#endif // FUNCTIONS_PARALLEL_H