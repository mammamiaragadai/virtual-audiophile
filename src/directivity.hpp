#ifndef DIRECTIVITY_HPP
#define DIRECTIVITY_HPP

#include <complex.h>
#include <fftw3.h>

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
    double  fs; // scalar
    double* irs; // matrix
    double order; // scalar

    unsigned int n_recievers; // scalar
    unsigned int n_fft;

    fftw_complex* tfs;
    spherical_coords_t* coords;

} directivity_t;

int load_directivity(const char* matpath, directivity_t *directivity);
void free_directivity(directivity_t* directivity);
cartesian_coords_t sph2cart(spherical_coords_t sph);
spherical_coords_t cart2sph(cartesian_coords_t cart);

#endif  // DIRECTIVITY_HPP