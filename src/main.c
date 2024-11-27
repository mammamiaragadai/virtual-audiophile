#include <stdio.h>
#include <matio.h>
#include <string.h>

typedef struct {
    double  R; // scalar
    double* azimuth; // vector
    double* colatitude; // vector
    double  fs; // scalar
    double* irs; // matrix
    double order; // scalar

    unsigned int n_recievers; // scalar
    unsigned int n_fft;
} directivity_t;

int load_directivity(const char* matpath, directivity_t *directivity);
void free_directivity(directivity_t* directivity);
void print_array(double* array, int n);
void print_matrix(double* matrix, int n_rows, int n_cols);

int main(void)
{
    puts("Hello world!");

    // Initializations
    directivity_t* directivity = (directivity_t*) malloc(sizeof(directivity_t));
    if (0 != load_directivity("Dynaudio_Xeo_3.mat", directivity)) {
        return -1;
    }

    // Processing
    printf("R: %g | fs: %g | order: %g | n_fft: %d | n_recievers: %d\n", directivity->R, directivity->fs, directivity->order, directivity->n_fft, directivity->n_recievers);
    // print_array(directivity->azimuth, directivity->n_recievers);
    // print_array(directivity->colatitude, directivity->n_recievers);
    print_matrix(directivity->irs, directivity->n_fft, directivity->n_recievers);

    // Destroy
    free_directivity(directivity);
    return 0;
}

int load_directivity(const char* matpath, directivity_t *directivity)
{
    mat_t* matfp;
    matvar_t* matvar;

    matfp = Mat_Open(matpath, MAT_ACC_RDONLY);
    if (matfp == NULL)
    {
        printf("Error opening mat file: %s\n!", matpath);
        return -1;
    }

    matvar = Mat_VarRead(matfp, "R");
    directivity->R = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "fs");
    directivity->fs = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "order");
    directivity->order = *(double*) matvar->data;

    matvar = Mat_VarRead(matfp, "azimuth");
    directivity->azimuth = (double*) malloc(matvar->nbytes);
    memcpy(directivity->azimuth, matvar->data, matvar->nbytes);
    matvar = Mat_VarRead(matfp, "colatitude");
    directivity->colatitude = (double*) malloc(matvar->nbytes);
    memcpy(directivity->colatitude, matvar->data, matvar->nbytes);
    directivity->n_recievers = matvar->nbytes / matvar->data_size;
    
    matvar = Mat_VarRead(matfp, "irs");
    printf("dims: %ld, %ld\n", matvar->dims[0], matvar->dims[1]);
    directivity->irs = (double*) malloc(matvar->nbytes);
    memcpy(directivity->irs, matvar->data, matvar->nbytes);
    directivity->n_fft = matvar->nbytes / matvar->data_size / directivity->n_recievers;

    Mat_VarFree(matvar);
    Mat_Close(matfp);
    return 0;
}

void free_directivity(directivity_t* directivity)
{
    free(directivity->azimuth);
    free(directivity->colatitude);
    free(directivity->irs);
    free(directivity);
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