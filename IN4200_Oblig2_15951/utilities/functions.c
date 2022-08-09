// #include "functions.h"
#include "../utilities/functions.h"

void allocate_image(image *u, int m, int n){
    /* Allocate 2D array in image struct  */
    u->m = m;
    u->n = n;
 
    u->image_data = malloc(m * sizeof u->image_data);
    (u->image_data)[0] = malloc(m * n * sizeof (u->image_data)[0]); // Underlying 2D array

    for (size_t i = 1; i < m; i++)
    {
        (u->image_data)[i] = &( (u->image_data)[0][i * n] );
    }
 
}

void deallocate_image(image *u){
    /* Deallocate 2D array in image struct */
    free((u->image_data)[0]);
    free(u->image_data);
}


void convert_jpeg_to_image(unsigned char *image_chars, image *u){
    /* Assigns values from image_chars (jpg) to 2D array in image struct */
    for (size_t i = 0; i < u->m; i++) {
        for (size_t j = 0; j < u->n; j++)
        {
            u->image_data[i][j] = (float) image_chars[i*u->n + j];
        }   
    }
}

void convert_image_to_jpeg(image *u, unsigned char *image_chars){
    /* Assigns values from 2D array in image struct back to image_chars (jpg) */
    for (size_t i = 0; i < u->m; i++){
        for (size_t j = 0; j < u->n; j++)
        {
            image_chars[i * u->n + j] = (char)u->image_data[i][j];
        }
    }
}

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters){
    /* Denoising algorithm */
    time_t start, end;
    image *tmp;

    printf("Processing image | iters = %d\n", iters);
    start = clock(); // timing

    

    // Copy boundaries from u to u_bar
    for (size_t i = 0; i < u->m; i++){
        u_bar->image_data[i][0] = u->image_data[i][0];                  // Left column
        u_bar->image_data[i][u->n - 1] = u->image_data[i][u->n - 1];    // Left column
    }

    for (size_t j = 1; j < u->n-1; j++){
        u_bar->image_data[0][j] = u->image_data[0][j];                  // Top row (except cornes)
        u_bar->image_data[u->m - 1][j] = u->image_data[u->m - 1][j];    // Bottom row (except cornes)
    }


    // Inner points 
    for (size_t iter = 0; iter < iters; iter++){
        for (size_t i = 1; i < u->m-1; i++){    
            for (size_t j = 1; j < u->n-1; j++){
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (   u->image_data[i - 1][j] 
                                                                        +   u->image_data[i][j - 1] 
                                                                        - 4*u->image_data[i][j] 
                                                                        +   u->image_data[i][j + 1] 
                                                                        +   u->image_data[i+1][j] );
            }
        }


        // Pointer swap
        tmp = u;
        u = u_bar;
        u_bar = tmp;

    }

    if (!(iters%2)) // even
    {
        // memcpy 
        for (size_t i = 0; i < u->m; i++){
            for (size_t j = 0; j < u->n; j++){
                u_bar->image_data[i][j] = u->image_data[i][j];
            }
        }
     }

     end = clock();
     printf("--> complete (time used = %g s)\n\n", (double)(end - start) / CLOCKS_PER_SEC);
}





void output_name(char *input_jpeg_filename, char **output_jpeg_filename, char *out_extension, int iters){
    /*  Produce output filename on the format:
        "input name (without format extension) + out_extension + iters + input format extension"
    */
    char *output_tmp;
    char iters_char[20] ;
    int input_divider;
    sprintf(iters_char, "%d", iters);
      
    // Allocate space for output name
    output_tmp = malloc(strlen(input_jpeg_filename) + strlen(out_extension) + 1);

    // Use input filename characters for output name
    // until the beginning of the filename extension (e.g. .jpg)
    for (size_t i = 0; i < strlen(input_jpeg_filename); i++){
        output_tmp[i] = input_jpeg_filename[i]; 

        if (input_jpeg_filename[i] == '.')
        {
            input_divider = i;
            break;
        }
    }

    // Add chosen output_extension (e.g. _denoised)
    for (size_t i = 0; i < strlen(out_extension); i++){
        output_tmp[input_divider + i] = out_extension[i]; 
    }

    // Add iters to output name
    for (size_t i = 0; i < strlen(iters_char); i++)
    {
        output_tmp[input_divider + strlen(out_extension) + i] = iters_char[i];
    }

    // Add the input filename extension (e.g. .jpg)
    for (size_t i = input_divider; i < strlen(input_jpeg_filename); i++){
        output_tmp[strlen(iters_char) + strlen(out_extension) + i] = input_jpeg_filename[i];
    }

    // Assign output to output filename
    *output_jpeg_filename = output_tmp;    

}