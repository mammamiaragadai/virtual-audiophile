#include <stdio.h>
#include <matio.h>

void LoadMat(const char* path);

int main(void)
{
    LoadMat("Dynaudio_Xeo_3.mat");
    puts("Hello world!");
    return 0;
}

void LoadMat(const char* path)
{
    mat_t* matfp;
    matvar_t* matvar;

    matfp = Mat_Open(path, MAT_ACC_RDONLY);
    if (matfp == NULL)
    {
        printf("Error opening mat file: %s\n!", path);
    }

    const char* header = Mat_GetHeader(matfp);
    printf("Header: %s\n", header);

    matvar = Mat_VarRead(matfp, "irs");
    if (matvar != NULL)
    {
        Mat_VarPrint(matvar, 1);
        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);
}