#include <matio.h>
#include <string.h>
#include "directivity.hpp"

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

    directivity->tfs = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * directivity->n_fft);
    fftw_plan plan = fftw_plan_dft_r2c_1d(directivity->n_fft, directivity->irs, directivity->tfs, FFTW_ESTIMATE);
    fftw_execute(plan);

    Mat_VarFree(matvar);
    Mat_Close(matfp);
    fftw_destroy_plan(plan);
    return 0;
}

void free_directivity(directivity_t* directivity)
{
    free(directivity->azimuth);
    free(directivity->colatitude);
    free(directivity->irs);
    fftw_free(directivity->tfs);
    free(directivity);
}