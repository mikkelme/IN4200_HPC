#include <stdio.h>
#ifdef __MACH__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../utilities/functions.h"
#include "../utilities/functions_parallel.h"

int main(int argc, char *argv[])
{
    int m, n, c, iters;
    int my_m, my_n, my_prod, my_rank, num_procs;
    float kappa;
    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename, *out_extension;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Status status;

    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_file
    name */

    // temporary hardcoded inputs
    kappa = 0.2 ;
    iters = 100;
    input_jpeg_filename = "mona_lisa_noisy.jpg";
    out_extension = "_denoised";
    


    
    // Import image and allocate whole image
    if (my_rank == 0)
    {
        printf("\nImporting image: \"%s\"\n", input_jpeg_filename);
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        printf("--> vertical pixels: %d, horizontal pixels: %d, num components: %d\n\n", m, n, c);
        allocate_image(&whole_image, m, n);
    }

    // Broadcast m, n from 0 to all the other nodes
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /*  2D decomposition of the m x n pixels evenly among the MPI processes
        using 1D Partitioning  */

    
    partion_1D(m, n, &my_m, &my_n, &my_prod, my_rank, num_procs);
    my_image_chars = malloc(my_prod * sizeof(unsigned char));
    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);

    
    /* Distribute partioned regions of image chars 
       from thread 0 to other threads */
    if (my_rank == 0){


        // Copy region for thread 0
        for (size_t i = 0; i < my_prod; i++){
            my_image_chars[i] = image_chars[i];
        }
        

        // Send regions to other threads
        int start_index = my_m*my_n;
        for (size_t i = 1; i < num_procs; i++)
        {
            // m, n, prod for thread i
            int tmp_m, tmp_n, tmp_prod;
            partion_1D(m, n, &tmp_m, &tmp_n, &tmp_prod, i, num_procs);

            

            MPI_Send(&image_chars[start_index],
                        tmp_prod,
                        MPI_UNSIGNED_CHAR, i, 0,
                        MPI_COMM_WORLD);


            start_index += tmp_prod;
                
        }
       

    }
    else {
        MPI_Recv(my_image_chars, my_prod, MPI_UNSIGNED_CHAR, 0, 0,
                 MPI_COMM_WORLD, &status);
        // printf("Recieved from thread %d\n", my_rank);
        // printf("%d\n", my_image_chars[0]);
    }


    // convert jpg to image and run denoising algorithm
    convert_jpeg_to_image(my_image_chars, &u);
    iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters);



    /* Gather partioned regions of updated image chars 
       into struct whole image from all threads to thread 0 */
    if (my_rank == 0)
    {

        // Region from thread 0 itself
        for (size_t i = 0; i < my_m; i++){
            for (size_t j = 0; j < my_n; j++){
                whole_image.image_data[i][j] = u_bar.image_data[i][j];
            }
        }
        

        // Gather from remaining regions
        int start_row = my_m;
        for (size_t i = 1; i < num_procs; i++)
        {
            // m, n, prod for thread i
            int tmp_m = m / (int)num_procs + (i < m % num_procs);
            int tmp_n = n;
            int tmp_prod = tmp_m * tmp_n;

            MPI_Recv(&whole_image.image_data[start_row][0],
                     tmp_prod,
                     MPI_FLOAT, i, 0,
                     MPI_COMM_WORLD, &status);
            start_row += tmp_m;
        }
    }
    else
    {
        // Send region from threads to thread 0
        MPI_Send(&u_bar.image_data[0][0], my_prod, MPI_FLOAT, 0, 0,
                 MPI_COMM_WORLD);
    }

    // Convert whole image and export
    if (my_rank == 0)
    {
        convert_image_to_jpeg(&whole_image, image_chars);
        output_name(input_jpeg_filename, &output_jpeg_filename, out_extension, iters);
        printf("Exporting as: \"%s\"\n", output_jpeg_filename);
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        printf("--> complete\n\n");
        deallocate_image(&whole_image);

    }
    
    deallocate_image(&u);
    deallocate_image(&u_bar);
    MPI_Finalize();

    return 0;
}

