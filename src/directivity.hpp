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