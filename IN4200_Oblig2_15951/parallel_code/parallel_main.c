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
    int m, n, c, iters, my_m, my_n, my_prod, my_rank, num_procs;
    float kappa;
    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename, *out_extension;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Status status;

    // Read input variables from command line
    if (argc < 4)
    {
        printf("Please provide arguments: kappa, iters, input_jpeg_filenmae in the command line.\n");
        exit(1);
    }
    kappa = atof(argv[1]);
    iters = atoi(argv[2]);
    input_jpeg_filename = argv[3];
    out_extension = "_denoised";
    output_name(input_jpeg_filename, &output_jpeg_filename, out_extension, iters);

    // Import image and allocate whole image as 2D array
    if (my_rank == 0)
    {
        printf("\nImporting image: \"%s\"\n", input_jpeg_filename);
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        printf("--> vertical pixels: %d, horizontal pixels: %d, num components: %d\n\n", m, n, c);
        allocate_image(&whole_image, m, n);
    }

    // Broadcast image dimensions m x n from thread 0 to other nodes
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
    distribute_regions(m, n, image_chars, my_image_chars,  my_m,  my_n,  my_prod,  my_rank, num_procs);

    
    // Convert jpg to image and run denoising algorithm
    convert_jpeg_to_image(my_image_chars, &u);
    iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters, my_rank, num_procs);

    /* Gather partioned regions of processed image chars 
       into struct whole image from all threads to thread 0 */
    gather_regions( m,  n, &whole_image, &u_bar,  my_m,  my_n,  my_prod,  my_rank, num_procs);

    // Convert whole image to jpg and export
    if (my_rank == 0)
    {
        convert_image_to_jpeg(&whole_image, image_chars);
        output_name(input_jpeg_filename, &output_jpeg_filename, out_extension, iters);
        printf("Exporting as: \"%s\"\n", output_jpeg_filename);
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        printf("--> complete\n\n");
        deallocate_image(&whole_image);
        free(image_chars);

    }
    
    deallocate_image(&u);
    deallocate_image(&u_bar);
    free(my_image_chars);
    MPI_Finalize();


    return 0;
}

