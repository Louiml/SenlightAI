// Write a standalone C++ function named `apply_vertical_recurrence` that implements a three-term horizontal recurrence relation on a set of one-dimensional data arrays. The function must accept three arrays of equal length (`lower`, `higher`, and `result`), a scalar coefficient array `scale` (also of the same length), and an integer `order` between 1 and 10. For each index `k`, the function computes `result[k] = higher[k] - lower[k] * scale[k]`. The function must process the data in blocks: for each block of 10 consecutive output elements, it must read the corresponding 10 elements from `lower` and `higher` combined with 10 elements from `scale`, and write 10 results. The blocks correspond to groups of 10 (for order 1 to 10), and the function must handle the general case where the total length is a multiple of 10 (as guaranteed by the caller). The function signature must be: `void apply_vertical_recurrence(double* result, const double* lower, const double* higher, const double* scale, size_t length)`. The implementation must be self-contained, use only standard C++ headers, and must not assume any external library or SIMD directives.

// The core algorithm is a pointwise operation: for every element `k` from `0` to `length-1`, compute `result[k] = higher[k] - lower[k] * scale[k]`. The three input arrays (`lower`, `higher`, `scale`) and the output array (`result`) are all assumed to have exactly `length` valid elements, where `length` is a multiple of 10. The function iterates over the entire length using a simple loop. Since each output element depends only on inputs at the same index, there is no data dependency between iterations, so the loop can be vectorized by the compiler (though we do not rely on explicit SIMD). Edge cases: when `length` is 0, the loop does nothing and the function returns immediately. The coefficient `scale` may contain any finite double value, including zero and negatives; no special handling is needed. The time complexity is O(n) where n=`length`, and space complexity is O(1) beyond the input/output arrays themselves. The problem is intentionally minimal to focus on correct array indexing and arithmetic.

#include <cstddef>

/**
 * @brief Applies a pointwise horizontal recurrence: result[k] = higher[k] - lower[k] * scale[k]
 *
 * @param result   Output array of length `length`. Must be non-null and have enough space.
 * @param lower    Input array of length `length` (the lower-order coefficients).
 * @param higher   Input array of length `length` (the higher-order coefficients).
 * @param scale    Input array of length `length` (the scaling factors, typically a coordinate).
 * @param length   Number of elements to process. Must be a multiple of 10.
 *
 * The function computes result[k] for all k in [0, length). No special alignment assumptions.
 */
void apply_vertical_recurrence(double* result, const double* lower, const double* higher, const double* scale, std::size_t length) {
    for (std::size_t k = 0; k < length; ++k) {
        result[k] = higher[k] - lower[k] * scale[k];
    }
}

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>

// Declaration of the function under test
void apply_vertical_recurrence(double* result, const double* lower, const double* higher, const double* scale, std::size_t length);

int main() {
    // Test 1: Basic case with simple numbers
    {
        const std::size_t n = 10;
        std::vector<double> lower = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
        std::vector<double> higher = {2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0};
        std::vector<double> scale = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5};
        std::vector<double> result(n, 0.0);
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            double expected = higher[k] - lower[k] * scale[k];
            assert(std::fabs(result[k] - expected) < 1e-12);
        }
    }

    // Test 2: Zero-length input should not crash
    {
        std::vector<double> result;
        apply_vertical_recurrence(result.data(), nullptr, nullptr, nullptr, 0);
    }

    // Test 3: All zeros in scale (result equals higher)
    {
        const std::size_t n = 20;
        std::vector<double> lower(n, 3.0);
        std::vector<double> higher(n, 7.0);
        std::vector<double> scale(n, 0.0);
        std::vector<double> result(n, 0.0);
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            assert(result[k] == 7.0);
        }
    }

    // Test 4: Negative scale values
    {
        const std::size_t n = 10;
        std::vector<double> lower = {1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -8.0, 9.0, -10.0};
        std::vector<double> higher = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        std::vector<double> scale = {-1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0};
        std::vector<double> result(n, 0.0);
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            // higher is zero, so result = 0 - lower * scale = -lower*scale
            double expected = -lower[k] * scale[k];
            assert(std::fabs(result[k] - expected) < 1e-12);
        }
    }

    // Test 5: Larger length (multiple of 10)
    {
        const std::size_t n = 100;
        std::vector<double> lower(n);
        std::vector<double> higher(n);
        std::vector<double> scale(n);
        std::vector<double> result(n, 0.0);
        for (std::size_t k = 0; k < n; ++k) {
            lower[k] = static_cast<double>(k);
            higher[k] = 2.0 * static_cast<double>(k) + 1.0;
            scale[k] = 0.5;
        }
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            double expected = higher[k] - lower[k] * scale[k];
            assert(result[k] == expected);
        }
    }

    // Test 6: Fractional and large values
    {
        const std::size_t n = 10;
        std::vector<double> lower = {1e6, -2.5, 0.003, 4.44, -5.0, 6.6, -7.7, 8.8, -9.9, 10.1};
        std::vector<double> higher = {2e6, -5.0, 0.006, 8.88, -10.0, 13.2, -15.4, 17.6, -19.8, 20.2};
        std::vector<double> scale = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
        std::vector<double> result(n, 0.0);
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            double expected = higher[k] - lower[k] * scale[k];
            assert(std::fabs(result[k] - expected) < 1e-6 * (1.0 + std::fabs(expected)));
        }
    }

    // Test 7: Random-like check using deterministic pattern
    {
        const std::size_t n = 30;
        std::vector<double> lower(n);
        std::vector<double> higher(n);
        std::vector<double> scale(n);
        std::vector<double> result(n, 0.0);
        for (std::size_t k = 0; k < n; ++k) {
            lower[k] = k * 3.0 - 7.0;
            higher[k] = 2.0 - k * 1.5;
            scale[k] = k % 3 + 0.1;
        }
        apply_vertical_recurrence(result.data(), lower.data(), higher.data(), scale.data(), n);
        for (std::size_t k = 0; k < n; ++k) {
            double expected = higher[k] - lower[k] * scale[k];
            assert(std::fabs(result[k] - expected) < 1e-10);
        }
    }

    return 0;
}
