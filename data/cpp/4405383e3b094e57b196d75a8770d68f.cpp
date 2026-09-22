// Write a standalone C++ function named `dotProductAvx2Compatible` that computes the dot product of two arrays of `double` values, mimicking the behavior of the SIMD-optimized `rdotv_avx2` routine from the given code snippet. Your function must handle arrays of any non-negative length `n`, process the data in blocks of 4 elements (as if using AVX2 256-bit registers), and correctly handle any remainder elements (0 to 3) at the end. It should not rely on actual SIMD intrinsics but rather implement the same logical grouping and accumulation strategy. The function must return the sum of `x[i] * y[i]` for all `i` from 0 to `n-1`. Additionally, provide a helper function `dotSelf` that computes the dot product of a single array with itself (equivalent to sum of squares), mirroring `rdotv2_avx2`. The solution must be self-contained, include appropriate headers, and use `const` correctly. Provide a reference implementation and test code.

// The core task is to replicate the vectorized dot-product logic without using actual SIMD instructions. The AVX2 code processes 4 doubles per 256-bit vector. The strategy: split the array length `n` into groups of 4 (`avx2len = n >> 2`). For each full group of 4, we compute the element-wise products and accumulate them in a running sum. However, to mimic the unrolled and partial-unrolled structure, we can simplify: first, accumulate over all full 4-element groups using a loop, then handle the remainder (0 to 3 elements) with a simple loop. This preserves clarity while maintaining the same mathematical result. The trick is to be careful with floating-point summation order: the AVX2 version reduces the four lanes of each vector together, and then combines across vectors, but the exact order does not affect correctness (only tiny floating-point differences). For the task, we accept standard left-to-right accumulation for simplicity. For `dotSelf`, we apply the same logic but use `x[i]*x[i]`. Time complexity is O(n), space O(1). Edge cases: n=0 returns 0.0; n not multiple of 4 handled by remainder loop.

#include <cstddef>  // for size_t

// Compute dot product of two arrays: sum_{i=0}^{n-1} x[i] * y[i]
// This mimics the logical structure of an AVX2-optimized routine,
// processing 4 elements at a time and handling the remainder.
double dotProductAvx2Compatible(const double* x, const double* y, size_t n) {
    double result = 0.0;
    size_t i = 0;

    // Process full groups of 4
    size_t fullGroups = n / 4;
    for (size_t group = 0; group < fullGroups; ++group) {
        double groupSum = 0.0;
        for (int lane = 0; lane < 4; ++lane) {
            groupSum += x[i + lane] * y[i + lane];
        }
        result += groupSum;
        i += 4;
    }

    // Handle remainder (0 to 3 elements)
    while (i < n) {
        result += x[i] * y[i];
        ++i;
    }

    return result;
}

// Compute sum of squares: sum_{i=0}^{n-1} x[i] * x[i]
double dotSelf(const double* x, size_t n) {
    return dotProductAvx2Compatible(x, x, n);
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: basic dot product with n multiple of 4
    double x1[] = {1.0, 2.0, 3.0, 4.0};
    double y1[] = {5.0, 6.0, 7.0, 8.0};
    assert(std::abs(dotProductAvx2Compatible(x1, y1, 4) - 70.0) < 1e-12);

    // Test 2: n not multiple of 4 (remainder 2)
    double x2[] = {1.0, -2.0, 3.5, 0.5, 10.0};
    double y2[] = {2.0, 1.0, -1.0, 2.0, 0.1};
    // Manual: 1*2 + (-2)*1 + 3.5*(-1) + 0.5*2 + 10*0.1 = 2 -2 -3.5 +1 +1 = -1.5
    assert(std::abs(dotProductAvx2Compatible(x2, y2, 5) - (-1.5)) < 1e-12);

    // Test 3: n=0 returns 0
    double x3[] = {1.0};
    double y3[] = {2.0};
    assert(dotProductAvx2Compatible(x3, y3, 0) == 0.0);

    // Test 4: n=1
    double x4[] = {3.0};
    double y4[] = {4.0};
    assert(std::abs(dotProductAvx2Compatible(x4, y4, 1) - 12.0) < 1e-12);

    // Test 5: negative values and n=3
    double x5[] = {-1.0, 2.0, -3.0};
    double y5[] = {2.0, -2.0, 1.0};
    assert(std::abs(dotProductAvx2Compatible(x5, y5, 3) - (-9.0)) < 1e-12); // -2 -4 -3 = -9

    // Test dotSelf: sum of squares
    double x6[] = {1.0, 2.0, 3.0, 4.0};
    assert(std::abs(dotSelf(x6, 4) - 30.0) < 1e-12); // 1+4+9+16=30

    double x7[] = {0.5, -1.5, 2.0};
    assert(std::abs(dotSelf(x7, 3) - (0.25 + 2.25 + 4.0)) < 1e-12); // 6.5

    // Test dotSelf with n=0
    assert(dotSelf(x7, 0) == 0.0);

    // Test larger array to verify correctness
    double x8[10] = {0,1,2,3,4,5,6,7,8,9};
    double y8[10] = {9,8,7,6,5,4,3,2,1,0};
    // Expected: sum i*(9-i) for i=0..9 = 0+8+14+18+20+20+18+14+8+0 = 120
    assert(std::abs(dotProductAvx2Compatible(x8, y8, 10) - 120.0) < 1e-12);

    return 0;
}
