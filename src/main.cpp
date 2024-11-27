#include <stdio.h>
#include <stdlib.h>
#include "directivity.hpp"

void print_array(double* array, int n);
void print_matrix(double* matrix, int n_rows, int n_cols);

int main(void)
{
    puts("Hello world!");

    // Initializations
    directivity_t* directivity = (directivity_t*) malloc(sizeof(directivity_t));
    if (0 != load_directivity("deps/sound-source-directivities/musical_instruments/bassoon_modern_TUB_RWTH/Bassoon_modern_et_ff_a2.mat", directivity)) {
        return -1;
    }

    // Processing
    printf("R: %g | fs: %g | order: %g | n_fft: %d | n_recievers: %d\n", directivity->R, directivity->fs, directivity->order, directivity->n_fft, directivity->n_recievers);
    print_array(directivity->azimuth, directivity->n_recievers);
    print_array(directivity->colatitude, directivity->n_recievers);
    print_matrix(directivity->irs, directivity->n_fft, directivity->n_recievers);

    // Destroy
    free_directivity(directivity);
    return 0;
}

void print_array(double* array, int n)
{
    printf("[");
    for (int i = 0; i < n; i++)
    {
        printf("%f ", array[i]);
    }
    printf("]\n");
}

void print_matrix(double* matrix, int n_rows, int n_cols)
{
    int i, j;

    printf("[");
    for (i = 0; i < n_rows; i++)
    {
        print_array(matrix + i * n_cols, n_cols);
    }
    printf("]\n");
}