#include <stdio.h>
#include <matio.h>

typedef struct {
    double  R; // scalar
    double* azimuth; // vector
    double* colatitude; // vector
    double  fs; // scalar
    double* irs; // matrix
    double order; // scalar
} directivity_t;

int load_mat(const char* path, directivity_t *directivity);

int main(void)
{
    puts("Hello world!");

    directivity_t* directivity = (directivity_t*) malloc(sizeof(directivity_t));
    if (0 != load_mat("Dynaudio_Xeo_3.mat", directivity)) {
        return -1;
    }

    printf("R: %g | fs: %g | order: %g\n", directivity->R, directivity->fs, directivity->order);
    return 0;
}

int load_mat(const char* path, directivity_t *directivity)
{
    mat_t* matfp;
    matvar_t* matvar;

    matfp = Mat_Open(path, MAT_ACC_RDONLY);
    if (matfp == NULL)
    {
        printf("Error opening mat file: %s\n!", path);
        return -1;
    }

    matvar = Mat_VarRead(matfp, "R");
    directivity->R = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "fs");
    directivity->fs = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "order");
    directivity->order = *(double*) matvar->data;

    Mat_Close(matfp);
    return 0;
}
