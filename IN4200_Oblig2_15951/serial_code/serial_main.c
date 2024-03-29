
#include <stdio.h>
#ifdef __MACH__
#include <stdlib.h>
#else 
#include <malloc.h>
#endif

#include "../utilities/functions.h"

int main(int argc, char *argv[])
{
    int m, n, c, iters;
    float kappa;
    image u, u_bar;
    unsigned char *image_chars;
    char *input_jpeg_filename, *output_jpeg_filename, *out_extension;

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

    // Import image and allocate as 2D array 
    printf("\nImporting image: \"%s\"\n", input_jpeg_filename);
    import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
    printf("--> vertical pixels: %d, horizontal pixels: %d, num components: %d\n\n", m, n, c);
    allocate_image(&u, m, n);
    allocate_image (&u_bar, m, n);
    convert_jpeg_to_image (image_chars, &u);

    // Run denoising algorithm
    iso_diffusion_denoising (&u, &u_bar, kappa, iters);

    // Convert denoised image back to jpg and export
    convert_image_to_jpeg(&u_bar, image_chars);
    printf("Exporting as: \"%s\"\n", output_jpeg_filename);
    export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
    printf("--> complete\n\n");
    deallocate_image (&u);
    deallocate_image (&u_bar);
    free(image_chars);

    return 0;
}

