#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "dotproduct.h"

#define RUNS 20  // Run 20 times for averaging

// Initialize vector with random values
void initialize_vector(double* v, int n, int seed)
{
    srand(seed);
    for (int i = 0; i < n; i++)
    {
        v[i] = (double)(rand() % 1000) / 10.0;  // Gets a Random double 0-100
    }
}

// Get timer
double get_time_ms()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart * 1000.0;
}

// Print timing results
void print_timing(const char* version, double time_ms, int n)
{
    printf("%s (n=%d): %.6f ms\n", version, n, time_ms);
}

// Correctness check
int check_correctness(double c_result, double asm_result, double tolerance)
{
    double diff = fabs(c_result - asm_result);
    double relative_error = diff / fabs(c_result);

    if (relative_error < tolerance)
    {
        printf("[PASS] Correctness check PASSED! \n|difference| = %.2e \nrelative error = %.2e\n", diff, relative_error);
        return 1;
    }
    else
    {
        printf("[FAIL] Correctness check FAILED! \n|difference| = %.2e \nrelative error = %.2e\n", diff, relative_error);
        return 0;
    }
}

int main(int argc, char* argv[])
{
    printf("=== Dot Product Kernel Comparison (C vs x86-64 Assembly) ===\n");
    printf("This program tests both Debug and Release mode performance.\n\n");

    printf("Menu:\n");
    printf("1. Run standard tests (2^20, 2^24, 2^29)\n");
    printf("2. Run custom vector size test\n");
    printf("3. Exit\n");
    printf("Choose option (1-3): ");

    int choice;
    scanf("%d", &choice);

    if (choice == 3)
        return 0;

    int sizes[3];
    int num_sizes;

    if (choice == 1)
    {
        // Standard test sizes
        sizes[0] = 1 << 20;
        sizes[1] = 1 << 24;
        sizes[2] = 1 << 29;
        num_sizes = 3;
    }
    else if (choice == 2)
    {
        printf("\nEnter vector size (as power of 2, e.g., 20 for 2^20): ");
        int power;
        scanf("%d", &power);
        sizes[0] = 1 << power;
        num_sizes = 1;
    }
    else
    {
        printf("Invalid choice. Running standard tests.\n");
        sizes[0] = 1 << 20;
        sizes[1] = 1 << 24;
        sizes[2] = 1 << 29;
        num_sizes = 3;
    }

    for (int s = 0; s < num_sizes; s++)
    {
        int n = sizes[s];
        int powers[3] = { 20, 24, 29 };
        int power = powers[s];
        printf("\n============================================\n");
        printf("Testing with n = 2^%d = %d elements\n", power, n);
        printf("============================================\n");

        // Allocate memory
        double* A = (double*)malloc(n * sizeof(double));
        double* B = (double*)malloc(n * sizeof(double));

        if (A == NULL || B == NULL)
        {
            printf("ERROR: Memory allocation failed!\n");
            return 1;
        }

        // Initialize vectors with same seed for reproducibility
        initialize_vector(A, n, 42);
        initialize_vector(B, n, 123);

        // Test C Version
        double c_result = 0.0;
        double c_total_time = 0.0;

        printf("\n--- Testing C Version ---\n");
        for (int run = 0; run < RUNS; run++)
        {
            double start = get_time_ms();
            c_result = dotproduct_c(n, A, B);
            double end = get_time_ms();
            c_total_time += (end - start);
        }
        double c_time = c_total_time / RUNS;
        printf("C Result: %.10e\n", c_result);
        print_timing("C Time", c_time, n);

        // Test Assembly Version
        double asm_result = 0.0;
        double asm_total_time = 0.0;

        printf("\n--- Testing x86-64 Assembly Version ---\n");
        for (int run = 0; run < RUNS; run++)
        {
            double start = get_time_ms();
            asm_result = dotproduct_asm(n, A, B);
            double end = get_time_ms();
            asm_total_time += (end - start);
        }
        double asm_time = asm_total_time / RUNS;
        printf("Assembly Result: %.10e\n", asm_result);
        print_timing("x86-64 Assembly Time", asm_time, n);

        // Correctness Check
        printf("\n--- Correctness Check ---\n");
        int correct = check_correctness(c_result, asm_result, 1e-10);

        // Performance Comparison
        if (asm_time > 0 && c_time > 0)
        {
            double speedup = c_time / asm_time;
            printf("\n--- Performance Analysis ---\n");
            printf("Performance: Assembly is %.2fx %s than C\n",
                fabs(speedup),
                (speedup > 1.0) ? "faster" : "slower");
        }

        // Cleanup
        free(A);
        free(B);
    }

    printf("\n=== Testing Complete ===\n");
    printf("Press Enter to exit...\n");
    getchar();

    return 0;
}