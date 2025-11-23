#ifndef DOTPRODUCT_H
#define DOTPRODUCT_H

// C kernel declaration
double dotproduct_c(int n, double* a, double* b);

// x86-64 assembly kernel declaration
extern double dotproduct_asm(int n, double* a, double* b);

#endif