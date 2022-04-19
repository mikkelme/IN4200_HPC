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
    int my_m, my_n, my_rank, num_procs;
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
    iters = 10000;
    input_jpeg_filename = "mona_lisa_noisy.jpg";
    out_extension = "_denoised";
    output_name(input_jpeg_filename, &output_jpeg_filename, out_extension, iters);


    
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


    /* 2D decomposition of the m x n pixels evenly among the MPI processes */

    // 1D partitioning
    my_m = m / (int)num_procs + (my_rank < m % num_procs);
    my_n = n;
    int my_prod = my_m*my_n;
    my_image_chars = malloc(my_prod * sizeof(unsigned char));
    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);

    // /* each process asks process 0 for a partitioned region */
    // /* of image_chars and copy the values into u */
    // /*  ...  */
    

    if (my_rank == 0){


        for (size_t i = 0; i < my_prod; i++)
        {
            my_image_chars[i] = image_chars[i];
        }
        


        int start_index = my_m*my_n;
       
        for (size_t i = 1; i < num_procs; i++)
        {
            int tmp_m = m / (int)num_procs + (i < m % num_procs);
            int tmp_n = n;
            int tmp_prod = tmp_m * tmp_n;
            

            MPI_Send(&image_chars[start_index],
                        tmp_prod,
                        MPI_UNSIGNED_CHAR, i, 0,
                        MPI_COMM_WORLD);
            // printf("Send to thread %zu\n", i);
            // printf("%d\n", image_chars[start_index]);

            start_index += tmp_prod;
                
        }
       

    }
    else {
        MPI_Recv(my_image_chars, my_prod, MPI_UNSIGNED_CHAR, 0, 0,
                 MPI_COMM_WORLD, &status);
        // printf("Recieved from thread %d\n", my_rank);
        // printf("%d\n", my_image_chars[0]);
    }


    convert_jpeg_to_image(my_image_chars, &u);
    iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters);


    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    /*  ...  */

    if (my_rank == 0)
    {

        for (size_t i = 0; i < my_m; i++)
        {
            for (size_t j = 0; j < my_n; j++)
            {
                whole_image.image_data[i][j] = u_bar.image_data[i][j];
            }
            
        }
        
        int start_row = my_m;

        for (size_t i = 1; i < num_procs; i++)
        {
            int tmp_m = m / (int)num_procs + (i < m % num_procs);
            int tmp_n = n;
            int tmp_prod = tmp_m * tmp_n;

            MPI_Recv(&whole_image.image_data[start_row][0],
                     tmp_prod,
                     MPI_FLOAT, i, 0,
                     MPI_COMM_WORLD, &status);
            // printf("Recieved from thread %zu\n", i);
            start_row += tmp_m;
        }
    }
    else
    {
        MPI_Send(&u_bar.image_data[0][0], my_prod, MPI_FLOAT, 0, 0,
                 MPI_COMM_WORLD);
        // printf("Send to thread %d\n", my_rank);
    }



    if (my_rank == 0)
    {
        convert_image_to_jpeg(&whole_image, image_chars);
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

