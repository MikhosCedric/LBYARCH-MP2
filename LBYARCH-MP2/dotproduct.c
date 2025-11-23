#include "dotproduct.h"

// C version of dot product kernel
double dotproduct_c(int n, double* a, double* b)
{
    double sdot = 0.0;
    for (int i = 0; i < n; i++)
    {
        sdot += a[i] * b[i];
    }
    return sdot;
}