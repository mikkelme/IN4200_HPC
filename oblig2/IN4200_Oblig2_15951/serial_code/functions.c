#include "functions.h"


void allocate_image(image *u, int m, int n)
{
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
    free((u->image_data)[0]);
    free(u->image_data);
}


void convert_jpeg_to_image(unsigned char *image_chars, image *u)
{
    for (size_t i = 0; i < u->m; i++) {
        for (size_t j = 0; j < u->n; j++)
        {
            u->image_data[i][j] = (float) image_chars[i*u->n + j];
        }   
    }
}

void convert_image_to_jpeg(image *u, unsigned char *image_chars){
    for (size_t i = 0; i < u->m; i++){
        for (size_t j = 0; j < u->n; j++)
        {
            image_chars[i * u->n + j] = (char)u->image_data[i][j];
        }
    }
}

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters)
{
    printf("%f\n", u->image_data[5][5]);
    printf("%f\n", u_bar->image_data[5][5]);

    

    for (size_t iter = 0; iter < iters; iter++)
    {

        for (size_t i = 1; i < u->m-1; i++)
        {
            u_bar->image_data[i][0] = u->image_data[i][0]; // Left column (except corners)
            for (size_t j = 1; j < u->n-1; j++)
            {
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa * (   u->image_data[i - 1][j] 
                                                                        +   u->image_data[i][j - 1] 
                                                                        - 4*u->image_data[i][j] 
                                                                        +   u->image_data[i][j + 1] 
                                                                        +   u->image_data[i+1][j] );
            }
            u_bar->image_data[i][u->n-1] = u->image_data[i][u->n-1]; // Right column (except cornes)
        }

        // Top and bottom row
        for (size_t j = 0; j < u->n; j++)
        {
            u_bar->image_data[0][j] = u->image_data[0][j];         // Top row
            u_bar->image_data[u->m-1][j] = u->image_data[u->m-1][j];   // Bottom row
        }


        // Pointer swap
        image *tmp = u;
        u = u_bar;
        u_bar = tmp;

        printf("%f\n", u->image_data[5][5]);
        printf("%f\n", u_bar->image_data[5][5]);
        // exit(0);
    }

    if (!(iters%2)) // even
    {
        printf("%i\n", iters);

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
