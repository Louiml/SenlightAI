Write a standalone C++ function named `openMPFFT` that performs an in-place iterative radix-2 Cooley–Tukey Fast Fourier Transform on a single array of complex numbers. The function must accept three parameters: a pointer to the complex array `x`, the integer `n` (the length of the array), and an integer `dir` (direction: +1 for forward FFT and -1 for inverse FFT). The input array `n` is guaranteed to be a power of two. The transform must exactly follow the algorithm given in the code snippet: first perform bit-reversal permutation, then iterate through stages, each time applying butterfly operations with a butterfly constant `c` computed from square roots using the recurrence `c = sqrt((1 + real(c))/2) + i * sqrt((1 - real(c))/2)` and negating the imaginary part if `dir == 1` (forward). The function should be templated on the real type (e.g., `template<typename ValueType>`), using `std::complex<ValueType>`. It should not use any external library or dynamic allocation; use only standard headers and simple loops. Handle `n == 1` gracefully, returning without any operation. The routine must be deterministic and match the exact mathematical behavior of a discrete Fourier transform.

#include <cassert>
#include <complex>
#include <cmath>

// Declare the function (assume it's defined in same file or include header)
template<typename ValueType>
void openMPFFT(std::complex<ValueType>* x, int n, int dir);

int main() {
    // Test 1: n=1, identity
    {
        std::complex<double> x[] = { {3.0, -2.0} };
        openMPFFT(x, 1, 1);
        assert(x[0] == std::complex<double>(3.0, -2.0));
    }

    // Test 2: n=2, forward FFT of {1, 0} -> {1, 1}
    {
        std::complex<double> x[] = { {1.0, 0.0}, {0.0, 0.0} };
        openMPFFT(x, 2, 1);
        assert(x[0] == std::complex<double>(1.0, 0.0));
        assert(std::abs(x[1] - std::complex<double>(1.0, 0.0)) < 1e-12);
    }

    // Test 3: n=4, forward FFT of impulse at position 0 -> all ones
    {
        std::complex<double> x[] = { {1,0}, {0,0}, {0,0}, {0,0} };
        openMPFFT(x, 4, 1);
        for (int i = 0; i < 4; ++i) {
            assert(std::abs(x[i] - std::complex<double>(1.0, 0.0)) < 1e-12);
        }
    }

    // Test 4: n=4, forward then inverse should return original (scaled)
    {
        std::complex<double> x[] = { {1,2}, {3,-1}, {0,0}, {5,4} };
        std::complex<double> original[] = { {1,2}, {3,-1}, {0,0}, {5,4} };
        openMPFFT(x, 4, 1);
        openMPFFT(x, 4, -1);
        for (int i = 0; i < 4; ++i) {
            // Divide by n=4 to get original
            std::complex<double> scaled = x[i] / 4.0;
            assert(std::abs(scaled - original[i]) < 1e-12);
        }
    }

    // Test 5: n=8, forward FFT of a sine-like sequence, check output against manual DFT (spot check)
    {
        std::complex<double> x[8];
        for (int i = 0; i < 8; ++i) x[i] = std::complex<double>(std::sin(2 * M_PI * i / 8), 0.0);
        openMPFFT(x, 8, 1);
        // DFT of sin(2πk/8) has non-zero at index 1 and 7 (depending on sign convention)
        // Our algorithm uses positive exponent, so sin gives -i/2 at index 1 and i/2 at index 7
        assert(std::abs(x[1] - std::complex<double>(0.0, -4.0)) < 1e-12); // -i*4
        assert(std::abs(x[7] - std::complex<double>(0.0, 4.0)) < 1e-12);  // i*4
        for (int i = 0; i < 8; ++i) {
            if (i != 1 && i != 7) {
                assert(std::abs(x[i]) < 1e-12);
            }
        }
    }

    // Test 6: n=16, forward of all zeros remains zeros
    {
        std::complex<double> x[16] = {};
        openMPFFT(x, 16, 1);
        for (int i = 0; i < 16; ++i) {
            assert(x[i] == std::complex<double>(0.0, 0.0));
        }
    }

    // Test 7: Verify forward transform of constant sequence (all 2) gives n*2 at index 0 and zeros elsewhere
    {
        std::complex<double> x[8];
        for (int i = 0; i < 8; ++i) x[i] = std::complex<double>(2.0, 0.0);
        openMPFFT(x, 8, 1);
        assert(std::abs(x[0] - std::complex<double>(16.0, 0.0)) < 1e-12);
        for (int i = 1; i < 8; ++i) {
            assert(std::abs(x[i]) < 1e-12);
        }
    }

    // Test 8: Test with float type
    {
        std::complex<float> x[] = { {1,0}, {0,0}, {0,0}, {0,0} };
        openMPFFT(x, 4, 1);
        for (int i = 0; i < 4; ++i) {
            assert(std::abs(x[i] - std::complex<float>(1.0f, 0.0f)) < 1e-5f);
        }
    }

    // Test 9: Test with long double
    {
        std::complex<long double> x[] = { {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
        openMPFFT(x, 8, 1);
        for (int i = 0; i < 8; ++i) {
            assert(std::abs(x[i] - std::complex<long double>(1.0L, 0.0L)) < 1e-15L);
        }
    }

    // Test 10: Non-trivial input, verify Parseval's theorem (energy preservation)
    {
        std::complex<double> x[8];
        std::complex<double> input[8];
        for (int i = 0; i < 8; ++i) {
            x[i] = std::complex<double>(std::cos(i * 1.3), std::sin(i * 0.7));
            input[i] = x[i];
        }
        openMPFFT(x, 8, 1);
        double energyIn = 0, energyOut = 0;
        for (int i = 0; i < 8; ++i) {
            energyIn  += std::norm(input[i]);
            energyOut += std::norm(x[i]);
        }
        assert(std::abs(energyIn - energyOut) < 1e-12);
    }

    return 0;
}

#include <complex>
#include <cmath>

/**
 * Performs an in-place iterative radix-2 FFT on a single array of complex numbers.
 * The algorithm follows the exact structure from the provided snippet: bit-reversal
 * permutation followed by butterfly stages. The direction parameter `dir` is +1 for
 * forward transform and -1 for inverse. No scaling is performed; for inverse, the
 * caller must divide by n.
 *
 * @tparam ValueType Real type (float, double, long double)
 * @param x Pointer to the complex array of length n (modified in place)
 * @param n Length of the array, must be a power of two (assert if debug)
 * @param dir Direction: +1 forward, -1 inverse
 */
template<typename ValueType>
void openMPFFT(std::complex<ValueType>* x, int n, int dir) {
    // Bit-reversal permutation (identity if n == 1)
    int i2 = n >> 1;
    int j = 0;
    for (int i = 0; i < n - 1; ++i) {
        if (i < j) {
            std::swap(x[i], x[j]);
        }
        int k = i2;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
    }

    // Butterfly stages
    int m = 0;
    while ((1 << m) < n) ++m;  // m = log2(n)

    std::complex<ValueType> u, t1, c(-1, 0);  // c initialized to -1 + 0i
    int l2 = 1;
    for (int l = 0; l < m; ++l) {
        int l1 = l2;
        l2 <<= 1;
        u = std::complex<ValueType>(1, 0);
        for (int jj = 0; jj < l1; ++jj) {
            for (int i = jj; i < n; i += l2) {
                int i1 = i + l1;
                t1 = u * x[i1];
                x[i1] = x[i] - t1;
                x[i] += t1;
            }
            u = u * c;
        }
        // Update c using recurrence: imag = sqrt((1 - real)/2), then real = sqrt((1 + real)/2)
        ValueType newImag = std::sqrt((ValueType(1) - c.real()) / ValueType(2));
        if (dir == 1) {
            newImag = -newImag;
        }
        ValueType newReal = std::sqrt((ValueType(1) + c.real()) / ValueType(2));
        c.real(newReal);
        c.imag(newImag);
    }
}

// The core algorithm is a straightforward iterative radix-2 FFT. The first phase performs a bit-reversal permutation: for each index `i` from 0 to `n-1`, we compute the bit-reversed index `j` and swap the elements at `i` and `j` if `i < j` to avoid double swaps. The bit-reversal is computed efficiently using the standard technique: start with `j = 0` and `i2 = n >> 1`, then for each `i`, update `j` by finding the highest bit position `k` that is set in `j` and resetting lower bits; the update is `j += k` where `k` is the largest power of two not exceeding `j`'s highest zero bit. This yields an `O(n)` bit-reversal with only constant space.
//
// The second phase is the butterfly computation. There are `m = log2(n)` stages. In each stage `l` from 0 to `m-1`, we set `l1 = 2^l` and `l2 = 2^(l+1)`. The butterfly constant `c` is initialized to -1 (i.e., complex(-1,0)) at the start of all stages. For each stage, we compute `u` starting at 1+0i. For each `j` from 0 to `l1-1`, we loop over every block of size `l2` and apply the butterfly: `t1 = u * x[i + l1]`, `x[i + l1] = x[i] - t1`, `x[i] += t1`. Then update `u = u * c`. After finishing the inner `j` loop for a stage, we update `c` using the recurrence: first compute `c.imag = sqrt((1 - c.real)/2)` (positive imaginary), then if `dir == 1` (forward transform) negate it; then compute `c.real = sqrt((1 + c.real)/2)`. Note that the recurrence relies on the previous value of `c.real` when computing the imaginary part; careful ordering matters. The algorithm is exactly as described in the snippet, requiring no additional scaling. For inverse FFT, the user is responsible for dividing by `n` outside the function; the provided code does not do that scaling.
//
// Edge cases: `n = 1` is handled by the bit-reversal loop (the for loop from `i = 0` to `n-1` does nothing) and the outer stage loop (the `for (l = 0; l < m; l++)` where `m = 0` does nothing), so the function naturally returns. For `n > 1`, the algorithm is stable for floating point types because it uses only arithmetic operations and no trigonometric function calls (only sqrt), matching the original implementation. Time complexity is `O(n log n)` for the butterfly stages plus `O(n)` for bit reversal, so total `O(n log n)`. Space complexity is `O(1)` auxiliary (only a few scalar variables). The templated approach works for `float`, `double`, `long double`, etc. Important: the function modifies the input array in place; it does not allocate memory.
