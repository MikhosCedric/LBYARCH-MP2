# LBYARCH-MP2
Main Repository for LBYARCH MP2
=======
# Dot Product Kernel - C vs x86-64 Assembly SIMD Comparison

## Project Overview

This project implements a dot product computation in two versions: a sequential C baseline and an optimized x86-64 assembly implementation using SIMD instructions. The objective is to compare correctness and performance between traditional sequential code and hand-optimized assembly.

### Standard Test Sizes

| Power | Size | Elements | Memory per Vector |
|-------|------|----------|-------------------|
| 2^20 | 1,048,576 | ~1 million | ~8 MB |
| 2^24 | 16,777,216 | ~17 million | ~128 MB |
| 2^29 | 536,870,912 | ~537 million | ~4 GB |

## Performance Analysis Tables

### DEBUG Mode

| Array Size | Elements | C Version | Assembly | Speedup |
|---|---|---|---|---|
| 2^20 | 1,048,576 | ~3.22 ms | ~0.63 ms | 5.10x |
| 2^24 | 16,777,216 | ~52 ms | ~11 ms | 4.65x |
| 2^29 | 536,870,912 | ~1687 ms | ~361 ms | 4.67x |

### RELEASE Mode

| Array Size | Elements | C Version | Assembly | Speedup |
|---|---|---|---|---|
| 2^20 | 1,048,576 | ~0.88 ms | ~0.52 ms | 1.67x |
| 2^24 | 16,777,216 | ~14.23 ms | ~10.09 ms | 1.41x |
| 2^29 | 536,870,912 | ~716.33 ms | ~343.99 ms | 2.08x |

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
