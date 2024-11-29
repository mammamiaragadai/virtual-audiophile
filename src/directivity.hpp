#ifndef DIRECTIVITY_HPP
#define DIRECTIVITY_HPP

#include <complex.h>

typedef struct {
    double azimuth;
    double elevation;
    double r;
} spherical_coords_t;

typedef struct {
    double x;
    double y;
    double z;
} cartesian_coords_t;

typedef struct {
    double  fs; // sampling rate; scalar
    double* irs; // impulse response; matrix
    double order; // spherical harmonics order; scalar

    unsigned int n_recievers; // number of receivers; scalar
    unsigned int n_fft; // number of FFT points; scalar

    std::complex<double>* tfs; // FFT of impulse responses; matrix
    spherical_coords_t* coords; // spherical coordinates

} directivity_t;

int load_directivity(const char* matpath, directivity_t *directivity);
void free_directivity(directivity_t* directivity);
cartesian_coords_t sph2cart(spherical_coords_t sph);
spherical_coords_t cart2sph(cartesian_coords_t cart);

#endif  // DIRECTIVITY_HPP