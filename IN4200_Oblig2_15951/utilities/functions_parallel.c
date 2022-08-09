#include "../utilities/functions_parallel.h"

void partion_1D(int m, int n, int *my_m, int *my_n, int *my_prod, int my_rank, int num_procs){
    /*  Make a 1D partioning with horizontal cuts and calculate 
        corresponding pixel dimension m x n and product (prod) for each region */
    *my_m = m / (int)num_procs + (my_rank < m % num_procs);
    *my_n = n;
    *my_prod = *my_m * *my_n;
}

void distribute_regions(int m, int n, unsigned char *image_chars, unsigned char *my_image_chars, int my_m, int my_n, int my_prod, int my_rank, int num_procs) {
    /*  Distribute partioned regions of image chars
        from thread 0 to other threads */
    MPI_Status status;

    if (my_rank == 0) {
        // Copy region for thread 0
        for (size_t i = 0; i < my_prod; i++){
            my_image_chars[i] = image_chars[i];
        }

        // Send regions to other threads
        int start_index = my_m * my_n;
        for (size_t i = 1; i < num_procs; i++)
        {
            int i_m, i_n, i_prod;
            partion_1D(m, n, &i_m, &i_n, &i_prod, i, num_procs); // m, n, prod for thread i
            MPI_Send(&image_chars[start_index],
                     i_prod,
                     MPI_UNSIGNED_CHAR, i, 0,
                     MPI_COMM_WORLD);

            start_index += i_prod;
        }
    }
    else {
        // Revieve regions from thread 0
        MPI_Recv(my_image_chars, my_prod, MPI_UNSIGNED_CHAR, 0, 0,
                 MPI_COMM_WORLD, &status);
    }
}

void gather_regions(int m, int n, image *whole_image, image *u_bar, int my_m, int my_n, int my_prod, int my_rank, int num_procs) {
    /*  Gather partioned regions of processed image chars
        into struct whole image from all threads to thread 0 */
    MPI_Status status;

    if (my_rank == 0) {
        // Region from thread 0 itself
        for (size_t i = 0; i < my_m; i++){
            for (size_t j = 0; j < my_n; j++){
                whole_image->image_data[i][j] = u_bar->image_data[i][j];
            }
        }

        // Gather remaining regions
        int start_row = my_m;
        for (size_t i = 1; i < num_procs; i++)
        {
            // Recieve regions
            int i_m, i_n, i_prod;
            partion_1D(m, n, &i_m, &i_n, &i_prod, i, num_procs); // m, n, prod for thread i
            MPI_Recv(&whole_image->image_data[start_row][0],
                     i_prod,
                     MPI_FLOAT, i, 0,
                     MPI_COMM_WORLD, &status);
            start_row += i_m;
        }
    }
    else {
        // Send region from threads to thread 0
        MPI_Send(&u_bar->image_data[0][0], my_prod, MPI_FLOAT, 0, 0,
                 MPI_COMM_WORLD);
    }
}

void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters, int my_rank, int num_procs){
    /*  Run parallized denoising algorithm  */
    MPI_Status status;
    image *tmp;
   
    float *top_neigh_row, *bottom_neigh_row;
    size_t i;
    double start, end;
    top_neigh_row = malloc(u->n * sizeof(float));
    bottom_neigh_row = malloc(u->n * sizeof(float));
    
    if (my_rank == 0) {
        printf("Processing image | #threads = %d, iters = %d\n", num_procs, iters);
        start = MPI_Wtime(); // timing
    }

    // Copy columns from u to u_bar
    for (size_t i = 0; i < u->m; i++)
    {
        u_bar->image_data[i][0] = u->image_data[i][0];               // Left column
        u_bar->image_data[i][u->n - 1] = u->image_data[i][u->n - 1]; // Left column
    }


    for (size_t iter = 0; iter < iters; iter++){
        
        // Inner points 
        for (size_t i = 1; i < u->m - 1; i++){
            for (size_t j = 1; j < u->n - 1; j++){
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (   u->image_data[i - 1][j] 
                                                                        +   u->image_data[i][j - 1] 
                                                                        - 4*u->image_data[i][j] 
                                                                        +   u->image_data[i][j + 1] 
                                                                        +   u->image_data[i+1][j] );
            }
        }

        /*  Handle boundaries: 
            top and bottom row for each region  */
        if (my_rank == 0) // Top region
        {
            // Send bottom row down
            MPI_Send(&u->image_data[u->m - 1][0],
                     u->n,
                     MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD);

            // Recieve bottom neighbour
            MPI_Recv(bottom_neigh_row, u->n, MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD, &status);

            // Assign values
            for (size_t j = 1; j < u->n - 1; j++){
                // Top row (copy from original)
                i = 0;
                u_bar->image_data[i][j] = u->image_data[i][j];

                // Bottom row
                i = u->m - 1;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (u->image_data[i - 1][j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + bottom_neigh_row[j]);
            }
        }

        else if (my_rank == num_procs - 1) // Bottom region
        {
            // Recieve top neighbour
            MPI_Recv(top_neigh_row, u->n, MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD, &status);

            // Send top row up
            MPI_Send(&u->image_data[0][0],
                     u->n,
                     MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD);

            // Assign values
            for (size_t j = 1; j < u->n - 1; j++){
                // Top row 
                i = 0;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (top_neigh_row[j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + u->image_data[i + 1][j]);

                // Bottom row (copy from original)
                i = u->m - 1;
                u_bar->image_data[i][j] = u->image_data[i][j];
            }
        }
        else // Middle regions
        {
            // Recieve top neighbour
            MPI_Recv(top_neigh_row, u->n, MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD, &status);

            // Send top row up
            MPI_Send(&u->image_data[0][0],
                     u->n,
                     MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD);

            // Send bottom row down
            MPI_Send(&u->image_data[u->m - 1][0],
                     u->n,
                     MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD);

            // Recieve bottom neighbour
            MPI_Recv(bottom_neigh_row, u->n, MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD, &status);

            // Assign values
            for (size_t j = 1; j < u->n - 1; j++)
            {

                // Top row
                i = 0;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (top_neigh_row[j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + u->image_data[i + 1][j]);

                // Bottom row
                i = u->m - 1;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (u->image_data[i - 1][j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + bottom_neigh_row[j]);
            }
        }

        // Assign values to row corners regions cornes (copy from original)
        u_bar->image_data[u->m - 1][0] = u->image_data[u->m - 1][0];
        u_bar->image_data[u->m - 1][u->n - 1] = u->image_data[u->m - 1][u->n - 1];

        // Pointer swap
        tmp = u;
        u = u_bar;
        u_bar = tmp;

    } // end of iters

    if (!(iters % 2)) // even
    {
        // memcpy
        for (size_t i = 0; i < u->m; i++) {
            for (size_t j = 0; j < u->n; j++) {
                u_bar->image_data[i][j] = u->image_data[i][j];
            }
        }
    }

    if (my_rank == 0){
        end = MPI_Wtime();
        printf("--> complete (time used = %g s)\n\n", (double)(end - start));
    }
}





