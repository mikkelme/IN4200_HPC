#include "../utilities/functions_parallel.h"


void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters){
    int my_rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Status status;

    image *tmp;
   

    float *top_neigh_row, *bottom_neigh_row;
    top_neigh_row = malloc(u->n * sizeof(float));
    bottom_neigh_row = malloc(u->n * sizeof(float));
    size_t i;

    for (size_t iter = 0; iter < iters; iter++)
    {
        // Inner points 
        for (size_t i = 1; i < u->m - 1; i++)
        {
            u_bar->image_data[i][0] = u->image_data[i][0]; // Left column (except corners)
            for (size_t j = 1; j < u->n - 1; j++)
            {
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (   u->image_data[i - 1][j] 
                                                                        +   u->image_data[i][j - 1] 
                                                                        - 4*u->image_data[i][j] 
                                                                        +   u->image_data[i][j + 1] 
                                                                        +   u->image_data[i+1][j] );
            
            }
            u_bar->image_data[i][u->n - 1] = u->image_data[i][u->n - 1]; // Right column (except cornes)
        }


        // Top and bottom row (except corners)
        if (my_rank == 0)
        {
            // Send bottom row down
            MPI_Send(&u->image_data[u->m - 1][0],
                     u->n,
                     MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD);

            // Recieve bottom neighbour
            MPI_Recv(bottom_neigh_row, u->n, MPI_FLOAT, my_rank + 1, 0,
                     MPI_COMM_WORLD, &status);

            // printf("%f\n", bottom_neigh_row[u->n-1]);

            for (size_t j = 1; j < u->n - 1; j++)
            {
                // Top row
                i = 0;
                u_bar->image_data[i][j] = u->image_data[i][j];

                // Bottom row
                i = u->m - 1;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (u->image_data[i - 1][j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + bottom_neigh_row[j]);
            }
        }
        else if (my_rank == num_procs - 1)
        {

            // Recieve top neighbour
            MPI_Recv(top_neigh_row, u->n, MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD, &status);

            // Send top row up
            MPI_Send(&u->image_data[0][0],
                     u->n,
                     MPI_FLOAT, my_rank - 1, 0,
                     MPI_COMM_WORLD);

            for (size_t j = 1; j < u->n - 1; j++)
            {
                // Top row
                i = 0;
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (top_neigh_row[j] + u->image_data[i][j - 1] - 4 * u->image_data[i][j] + u->image_data[i][j + 1] + u->image_data[i + 1][j]);

                // Bottom row
                i = u->m - 1;
                u_bar->image_data[i][j] = u->image_data[i][j];
            }
        }
        else
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

        // Copy corners
        u_bar->image_data[0][0] = u->image_data[0][0];
        u_bar->image_data[0][u->n - 1] = u->image_data[0][u->n - 1];
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
        for (size_t i = 0; i < u->m; i++)
        {
            for (size_t j = 0; j < u->n; j++)
            {
                u_bar->image_data[i][j] = u->image_data[i][j];
            }
        }
    }


}





