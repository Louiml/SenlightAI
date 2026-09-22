Write a C++ function `butterflyFFT` that performs an in-place iterative Cooley–Tukey Fast Fourier Transform on a `std::vector<std::complex<double>>`. The function must accept a reference to the vector and a boolean parameter `inverse` (where `false` computes the forward DFT and `true` computes the inverse DFT). The input size is guaranteed to be a power of two. The forward transform must produce exactly the standard unnormalized DFT: for output index `k`, `out[k] = sum_{j=0}^{n-1} input[j] * exp(-2*pi*i*j*k/n)`. The inverse transform must return the original data scaled by `1/n` (i.e., the inverse of the forward, normalized by `1/n`), so that applying forward then inverse yields the original vector exactly (within floating‑point precision). The function should not allocate any additional arrays of size `n` (only O(1) extra space beyond the input vector itself). The implementation must be iterative (no recursion), using bit‑reversal permutation followed by butterfly stages. The function should be robust for `n=1` (returns immediately, since DFT of a single element is itself). You may use `<complex>`, `<vector>`, `<cmath>`, and `<algorithm>`.

#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

// Declare the function under test
void butterflyFFT(std::vector<std::complex<double>>& vec, bool inverse);

int main() {
    // Test 1: Single element (power of two: 2^0)
    {
        std::vector<std::complex<double>> v = { {1.0, 2.0} };
        butterflyFFT(v, false);
        assert(std::abs(v[0] - std::complex<double>(1.0, 2.0)) < 1e-12);
    }

    // Test 2: n=2, forward and inverse roundtrip
    {
        std::vector<std::complex<double>> original = { {1.0, 0.0}, {0.0, 1.0} };
        std::vector<std::complex<double>> v = original;
        butterflyFFT(v, false);
        // Analytical forward DFT: k=0: 1+0 + 0+1i = 1+i
        // k=1: (1+0) + exp(-pi*i)*(0+1i) = 1 - i
        assert(std::abs(v[0] - std::complex<double>(1.0, 1.0)) < 1e-12);
        assert(std::abs(v[1] - std::complex<double>(1.0, -1.0)) < 1e-12);
        butterflyFFT(v, true);
        for (size_t i = 0; i < original.size(); ++i) {
            assert(std::abs(v[i] - original[i]) < 1e-12);
        }
    }

    // Test 3: n=4, all ones, forward gives DC=4, other bins=0
    {
        std::vector<std::complex<double>> v(4, {1.0, 0.0});
        butterflyFFT(v, false);
        assert(std::abs(v[0] - std::complex<double>(4.0, 0.0)) < 1e-12);
        for (int i = 1; i < 4; ++i) {
            assert(std::abs(v[i] - std::complex<double>(0.0, 0.0)) < 1e-12);
        }
    }

    // Test 4: n=4, impulse at index 2, forward yields exp(-pi*i*k) = (-1)^k
    {
        std::vector<std::complex<double>> v = { {0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0} };
        butterflyFFT(v, false);
        assert(std::abs(v[0] - std::complex<double>(1.0, 0.0)) < 1e-12);
        assert(std::abs(v[1] - std::complex<double>(-1.0, 0.0)) < 1e-12);
        assert(std::abs(v[2] - std::complex<double>(1.0, 0.0)) < 1e-12);
        assert(std::abs(v[3] - std::complex<double>(-1.0, 0.0)) < 1e-12);
    }

    // Test 5: n=8, random complex data, roundtrip
    {
        std::vector<std::complex<double>> original = {
            {0.1, -0.2}, {1.5, 0.3}, {-0.7, 1.2}, {2.0, -1.1},
            {-0.5, 0.0}, {0.3, 0.8}, {-1.2, -0.4}, {0.9, 0.6}
        };
        std::vector<std::complex<double>> v = original;
        butterflyFFT(v, false);
        butterflyFFT(v, true);
        for (size_t i = 0; i < original.size(); ++i) {
            assert(std::abs(v[i] - original[i]) < 1e-10);
        }
    }

    // Test 6: n=16, verify known identity: DFT of a cosine wave yields two spikes
    {
        const int n = 16;
        std::vector<std::complex<double>> v(n);
        for (int i = 0; i < n; ++i) {
            v[i] = std::complex<double>(std::cos(2 * M_PI * 3 * i / n), 0.0);
        }
        butterflyFFT(v, false);
        for (int k = 0; k < n; ++k) {
            double expected_real = 0.0;
            double expected_imag = 0.0;
            if (k == 3 || k == n - 3) {
                expected_real = n / 2.0;
            }
            assert(std::abs(v[k].real() - expected_real) < 1e-10);
            assert(std::abs(v[k].imag() - expected_imag) < 1e-10);
        }
    }

    return 0;
}

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

/**
 * Performs an in-place iterative Cooley–Tukey FFT on the vector.
 * @param vec The input/output vector of complex numbers. Size must be a power of two.
 * @param inverse If false, computes the forward DFT. If true, computes the inverse DFT (normalized by 1/n).
 * Time complexity: O(n log n). Auxiliary space: O(1) beyond the input vector.
 */
void butterflyFFT(std::vector<std::complex<double>>& vec, bool inverse) {
    const int n = static_cast<int>(vec.size());
    if (n <= 1) return;

    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(vec[i], vec[j]);
        }
    }

    // Butterfly stages
    for (int len = 2; len <= n; len <<= 1) {
        const double ang = 2 * M_PI / len * (inverse ? 1.0 : -1.0);
        const std::complex<double> wlen(std::cos(ang), std::sin(ang));

        for (int i = 0; i < n; i += len) {
            std::complex<double> w(1.0, 0.0);
            const int half = len / 2;
            for (int k = 0; k < half; ++k) {
                std::complex<double> u = vec[i + k];
                std::complex<double> v = vec[i + k + half] * w;
                vec[i + k] = u + v;
                vec[i + k + half] = u - v;
                w *= wlen;
            }
        }
    }

    if (inverse) {
        const double inv_n = 1.0 / static_cast<double>(n);
        for (auto& x : vec) {
            x *= inv_n;
        }
    }
}

// The solution uses the iterative Cooley–Tukey FFT algorithm. The key steps are: (1) Perform a bit‑reversal permutation of the input indices so that the recursive decomposition into even/odd indices is handled implicitly. (2) Then, for each stage `len = 2, 4, 8, ..., n`, combine pairs of elements that are `len/2` apart using twiddle factors `exp(-2*pi*i * k / len)` for the forward transform and `exp(+2*pi*i * k / len)` for the inverse transform. The butterfly operation is `u = a + w*b`, `v = a - w*b`. For the inverse, we additionally multiply by `1/2` at each stage (equivalent to dividing by `2^log2(n) = n` at the end). Bit‑reversal can be done by swapping indices `i` and `j` where `j` is the reverse of `i` in `log2(n)` bits, only when `j > i` to avoid double swaps. Edge case `n=1` requires no work. The algorithm runs in `O(n log n)` time and uses `O(1)` auxiliary space beyond the input vector (the same vector is modified in place). Precision: with double precision complex numbers, applying forward then inverse returns values within ~1e‑12 relative error for typical inputs.
