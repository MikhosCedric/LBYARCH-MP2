# LBYARCH-MP2
Main Repository for LBYARCH MP2
=======
# Dot Product Kernel - C vs x86-64 Assembly SIMD Comparison

## Project Overview

This project implements a dot product computation in two versions: a sequential C baseline and an optimized x86-64 assembly implementation using SIMD instructions. The objective is to compare correctness and performance between traditional sequential code and hand-optimized assembly.

## Project Structure

```
LBYARCH-MP2/
??? dotproduct.h           # Function declarations
??? dotproduct.c           # C implementation (sequential baseline)
??? dotproduct_asm.asm     # x86-64 assembly implementation (SIMD)
??? main.c                 # Test harness with timing and verification
??? LBYARCH-MP2.vcxproj    # Visual Studio project configuration
??? README.md              # This file
```

### Standard Test Sizes

| Power | Size | Elements | Memory per Vector |
|-------|------|----------|-------------------|
| 2^20 | 1,048,576 | ~1 million | ~8 MB |
| 2^24 | 16,777,216 | ~17 million | ~128 MB |
| 2^29 | 536,870,912 | ~537 million | ~4 GB |

## Performance Analysis

### Expected Results

#### For n = 2^20 (1,048,576 elements)
- **C Version Time:** ~3.22 ms
- **Assembly Time:** ~0.63 ms
- **Speedup:** ~5.10x

#### For n = 2^24 (16,777,216 elements)
- **C Version Time:** ~52 ms
- **Assembly Time:** ~11 ms
- **Speedup:** ~4.65x

#### For n = 2^29 (536,870,912 elements)
- **C Version Time:** ~1687 ms
- **Assembly Time:** ~361 ms
- **Speedup:** ~4.67x

### Correctness Verification

**Tolerance:** 1e-10 (relative error)
- **Definition:** `relative_error = |c_result - asm_result| / |c_result|`
- **Meaning:** Maximum 0.00000001% difference allowed

Both implementations produce numerically identical results within this tolerance.

## Calling Convention Details

### x86-64 Microsoft x64 Calling Convention

Both implementations follow Windows 64-bit calling convention:

| Parameter | Register | Purpose |
|-----------|----------|---------|
| 1st (n) | rcx | Vector size (number of elements) |
| 2nd (a) | rdx | Pointer to vector A |
| 3rd (b) | r8 | Pointer to vector B |
| Return | xmm0 | Double-precision result |

**SIMD Registers:**
- xmm0-xmm3: Caller-saved (can be freely used)
- xmm4-xmm5: Callee-saved (must be preserved)

## Testing Methodology

### Reproducibility
- Fixed seeds (42 for A, 123 for B) ensure identical vectors across runs
- Enables fair performance comparison

### Accuracy
- 20 iterations per test, results averaged
- Eliminates timing noise from system interrupts and cache effects
- High-resolution QueryPerformanceCounter provides microsecond-level precision

### Correctness
- Relative error tolerance of 1e-10
- Stricter than typical floating-point comparisons
- Ensures assembly produces correct results, not approximations
