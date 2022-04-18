#include <stdio.h>
#ifdef __MACH__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

// #include "functions.h"
#include "../utilities/functions.h"

int main(int argc, char *argv[])
{
    int m, n, c, iters;
    int my_m, my_n, my_rank, num_procs;
    float kappa;
    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename, *out_extension;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_file
    name */

    // temporary hardcoded inputs
    kappa = 0.2;
    iters = 10;
    input_jpeg_filename = "mona_lisa_noisy.jpg";
    out_extension = "_denoised";
    output_name(input_jpeg_filename, &output_jpeg_filename, out_extension, iters);


    
    if (my_rank == 0)
    {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image(&whole_image, m, n);
    }


    // Broadcast m, n from 0 to all the other nodes
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


    /* 2D decomposition of the m x n pixels evenly among the MPI processes */

    // 1D partitioning
    if (my_rank == 0){
        printf("%d x %d\n", m, n);
    }

    my_m = m / (int)num_procs + (my_rank < m % num_procs);
    my_n = n;
    int my_prod = my_m*my_n;
    my_image_chars = malloc(10 * sizeof(unsigned char));

    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);

    // /* each process asks process 0 for a partitioned region */
    // /* of image_chars and copy the values into u */
    // /*  ...  */
    MPI_Status status;

    if (my_rank == 0){
        int tmp_m = m / (int)num_procs + (1 < m % num_procs);
        int tmp_n = n;
        int tmp_prod = tmp_m*tmp_n;

        printf("%d\n", tmp_prod);
        for (size_t i = 0; i < 10; i++)
        {
            printf("%d\n", image_chars[i]);
        }

        MPI_Send(&image_chars[0],
                10,
                MPI_UNSIGNED_CHAR, 1, 0,
                MPI_COMM_WORLD);
    }
    
    if (my_rank == 1){
        // recieve
        printf("%d\n", my_prod);
        MPI_Recv(my_image_chars, 10, MPI_UNSIGNED_CHAR, 0, 0,
                 MPI_COMM_WORLD, &status);

        for (size_t i = 0; i < 10; i++)
        {
            printf("%d\n", my_image_chars[i]);
        }
        
    }

    // printf("%c\n", my_image_chars[0]);

    /////////////////////////

        // if (my_rank == 0){
        //     for (size_t i = 1; i < num_procs - 1; i++)
        //     {
        //         int prod = (m / (int)num_procs + (my_rank < m % num_procs))*n;
        //         int index = prod; // I think this is start index
        //         // index = i * elements_per_process;

        //         MPI_Send(&image_chars[index],
        //                  prod,
        //                  MPI_UNSIGNED_CHAR, i, 0,
        //                  MPI_COMM_WORLD);
        //     }
        // }
        // else {

        //     MPI_Recv(&my_image_chars, my_prod, MPI_UNSIGNED_CHAR, 0, 0,
        //          MPI_COMM_WORLD, &status);

        // }

        // printf("Rank %d, %d, %d, prod: %d\n", my_rank, my_m, my_n, my_m*my_n);

        // int counts[4] = {3041955, 3039120, 3039120, 3039120}; /* how many pieces of data everyone has */
        // int mynum = counts[my_rank];
        // int displs[4] = {0, 3041955, 6081075, 9120195}; /* the starting point of everyone's data */
        //                                                 /* in the global array */

        // MPI_Scatterv(image_chars, counts, displs, /* proc i gets counts[i] pts from displs[i] */
        //              MPI_UNSIGNED_CHAR,
        //              my_image_chars, mynum, MPI_UNSIGNED_CHAR, /* I'm receiving mynum MPI_INTs into local */
        //              0, MPI_COMM_WORLD);

        // if (rank == 0){
        //     for (size_t i = 0; i < counts[my_rank]; i++)
        //     {
        //         my_image_chars[i] = (char) 0;
        //     }

        // }

        // convert_jpeg_to_image(my_image_chars, &u);

        // iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters);
        // /* each process sends its resulting content of u_bar to process 0 */
        // /* process 0 receives from each process incoming values and */
        // /* copy them into the designated region of struct whole_image */
        // /*  ...  */
        // if (my_rank == 0)
        // {
        //     convert_image_to_jpeg(&whole_image, image_chars);
        //     export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        //     deallocate_image(&whole_image);
        //         4
        //     }
        //     deallocate_image(&u);
        //     deallocate_image(&u_bar);
        MPI_Finalize();

        return 0;
}
