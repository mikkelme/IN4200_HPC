
#include <stdio.h>
#ifdef __MACH__
#include <stdlib.h>
#else 
#include <malloc.h>
#endif

#include "functions.h"

/* needed header files .... */
/* declarations of functions import_JPEG_file and export_JPEG_file */

void import_JPEG_file (const char* filename, unsigned char** image_chars,
                       int* image_height, int* image_width,
                       int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,
                       int image_height, int image_width,
                       int num_components, int quality);


int main(int argc, char *argv[])
{
    int m, n, c, iters;
    float kappa;
    image u, u_bar;
    unsigned char *image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;
   
    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */
    
    /* temporary hardcoded inputs */
    kappa = 0.2;
    iters = 10;
    // input_jpeg_filename = "mona_lisa_noisy.jpg";
    input_jpeg_filename = "gray.jpg";

    output_jpeg_filename = "image_out.jpg";

    import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
    printf("Succeeded! vertical pixels: %d, horizontal pixels: %d, num components: %d\n", m, n, c);


    allocate_image (&u, m, n);
    allocate_image (&u_bar, m, n);
    convert_jpeg_to_image (image_chars, &u);

    iso_diffusion_denoising (&u, &u_bar, kappa, iters);

    printf("%f\n", u.image_data[5][5]);
    printf("%f\n", u_bar.image_data[5][5]);


    convert_image_to_jpeg(&u_bar, image_chars);
    export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
    deallocate_image (&u);
    deallocate_image (&u_bar);
        return 0;
}

