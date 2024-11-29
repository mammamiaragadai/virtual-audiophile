#include <matio.h>
#include <string.h>
#include <fftw3.h>
#include "directivity.hpp"

int load_directivity(const char* matpath, directivity_t *directivity)
{
    int i;
    mat_t* matfp;
    matvar_t* matvar;

    matfp = Mat_Open(matpath, MAT_ACC_RDONLY);
    if (matfp == NULL)
    {
        printf("Error opening mat file: %s\n!", matpath);
        return -1;
    }

    double R;
    double *azimuth, *colatitude;

    matvar = Mat_VarRead(matfp, "R");
    R = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "fs");
    directivity->fs = *(double*) matvar->data;
    matvar = Mat_VarRead(matfp, "order");
    directivity->order = *(double*) matvar->data;

    matvar = Mat_VarRead(matfp, "azimuth");
    azimuth = (double*) malloc(matvar->nbytes);
    memcpy(azimuth, matvar->data, matvar->nbytes);
    matvar = Mat_VarRead(matfp, "colatitude");
    colatitude = (double*) malloc(matvar->nbytes);
    memcpy(colatitude, matvar->data, matvar->nbytes);
    directivity->n_recievers = matvar->nbytes / matvar->data_size;
    directivity->coords = (spherical_coords_t*) malloc(sizeof(spherical_coords_t) * directivity->n_recievers);
    for (i = 0; i < directivity->n_recievers; i++)
    {
        directivity->coords[i] = { .azimuth = azimuth[i], .elevation = M_PI_2 - colatitude[i], .r = R };
    }

    matvar = Mat_VarRead(matfp, "irs");
    printf("dims: %ld, %ld\n", matvar->dims[0], matvar->dims[1]);
    directivity->irs = (double*) malloc(matvar->nbytes);
    memcpy(directivity->irs, matvar->data, matvar->nbytes);
    directivity->n_fft = matvar->nbytes / matvar->data_size / directivity->n_recievers;

    // Obtain FFT of impulse responses of each row
    double* fftw_in = (double*) malloc(sizeof(double) * directivity->n_fft);
    std::complex<double>* fftw_out = (std::complex<double>*) malloc(sizeof(std::complex<double>) * directivity->n_fft);
    directivity->tfs = (std::complex<double>*) malloc(sizeof(std::complex<double>) * directivity->n_recievers * directivity->n_fft);
    fftw_plan plan = fftw_plan_dft_r2c_1d(directivity->n_fft, fftw_in, reinterpret_cast<fftw_complex*>(fftw_out), FFTW_ESTIMATE);
    for (int i = 0; i < directivity->n_recievers; i++)
    {
        memcpy(fftw_in, directivity->irs + i * directivity->n_fft, directivity->n_fft);
        fftw_execute(plan);
        memcpy(directivity->tfs + i * directivity->n_fft, fftw_out, directivity->n_fft);
    }

    free(azimuth); free(colatitude);
    free(fftw_in); free(fftw_out);
    Mat_VarFree(matvar);
    Mat_Close(matfp);
    fftw_destroy_plan(plan);
    return 0;
}

void free_directivity(directivity_t* directivity)
{
    free(directivity->coords);
    free(directivity->irs);
    fftw_free(directivity->tfs);
    free(directivity);
}

cartesian_coords_t sph2cart(spherical_coords_t sph)
{
    cartesian_coords_t cart;
    cart.x = sph.r * cos(sph.elevation) * cos(sph.azimuth);
    cart.y = sph.r * cos(sph.elevation) * sin(sph.azimuth);
    cart.z = sph.r * sin(sph.elevation);
    return cart;
}

spherical_coords_t cart2sph(cartesian_coords_t cart)
{
    spherical_coords_t sph;
    sph.elevation = acos(cart.z / sqrt(cart.x * cart.x + cart.y * cart.y));
    sph.azimuth = atan2(cart.y, cart.x);
    sph.r = sqrt(cart.x * cart.x + cart.y * cart.y + cart.z * cart.z);
    return sph;
}