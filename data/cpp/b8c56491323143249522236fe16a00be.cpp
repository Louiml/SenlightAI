Given a vector of real-valued floating-point numbers whose length is a power of two and at least 2, write a C++ function `computeRealFFT` that returns a vector of `std::complex<double>` representing the one-sided positive-frequency spectrum (bins 0 through N/2 inclusive, where N is the input length) of the discrete Fourier transform of the real input signal. You must implement the transform yourself using the split-radix decimation-in-time approach: first perform a complex FFT of size N/2 on a packed array where even-indexed samples form the real part and odd-indexed samples form the imaginary part, then combine the resulting half-size spectrum using twiddle factors to reconstruct the full real FFT spectrum. The function must handle both real and imaginary components correctly, enforce that the input length is even, and produce output such that bin 0 contains the DC component and bin N/2 contains the Nyquist frequency component, with all Nyquist and DC imaginary parts set to zero. The input vector must not be modified.

The solution implements a real-input FFT by leveraging an existing complex FFT routine of half the size. First, we validate that the input length `N` is even and at least 2; if not, we return an empty vector. The core idea: pack the real time-domain samples into a complex array `packed` of length `N/2` by setting `packed[k] = complex(input[2k], input[2k+1])`. Then we perform a standard complex FFT (implemented iteratively with bit-reversal and Cooley–Tukey) on `packed` to obtain `Z` of length `M = N/2`. The real FFT spectrum `X` of length `N` is reconstructed using the symmetry properties: for k from 1 to M/2, we compute even and odd parts from `Z[k]` and `Z[M-k]` (with conjugation for the negative-frequency part), multiply the odd part by a twiddle factor `exp(-2*pi*i*k/N)`, and then form `X[k]` and `X[M-k]` by adding/subtracting the even and twiddled-odd parts and scaling by 0.5. The DC bin `X[0]` is the real part of `Z[0]` plus the imaginary part, and the Nyquist bin `X[M]` is the real part minus the imaginary part; both have zero imaginary components. This approach correctly reconstructs the full real FFT. Edge cases include N=2 (where M=1 and the loop over k is empty) and when M is odd (but since N is a power of two, M is also a power of two, so M/2 is integer). The algorithm runs in O(N log N) time and O(N) auxiliary space, and it preserves the input vector via const reference. Numerical accuracy is maintained by using `std::complex<double>` and standard twiddle calculations.

#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

// Compute the one-sided real FFT (bins 0..N/2) of a real input vector.
// Input must have even length; otherwise returns an empty vector.
std::vector<std::complex<double>> computeRealFFT(const std::vector<double>& input) {
    const std::size_t N = input.size();
    if (N < 2 || (N % 2) != 0) {
        return {}; // invalid input
    }
    const std::size_t M = N / 2; // half size for complex FFT

    // Pack even and odd samples into complex array.
    std::vector<std::complex<double>> packed(M);
    for (std::size_t i = 0; i < M; ++i) {
        packed[i] = std::complex<double>(input[2*i], input[2*i+1]);
    }

    // Iterative complex FFT (Cooley-Tukey) on 'packed'.
    // Bit-reversal permutation.
    for (std::size_t i = 1, j = 0; i < M; ++i) {
        std::size_t bit = M >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(packed[i], packed[j]);
        }
    }

    // FFT butterflies.
    for (std::size_t len = 2; len <= M; len <<= 1) {
        double angle = -2.0 * M_PI / static_cast<double>(len);
        std::complex<double> wlen(std::cos(angle), std::sin(angle));
        for (std::size_t i = 0; i < M; i += len) {
            std::complex<double> w(1.0, 0.0);
            for (std::size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = packed[i + j];
                std::complex<double> v = packed[i + j + len / 2] * w;
                packed[i + j] = u + v;
                packed[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // Reconstruct real FFT spectrum.
    std::vector<std::complex<double>> spectrum(N / 2 + 1);
    // DC and Nyquist.
    std::complex<double> z0 = packed[0];
    spectrum[0] = std::complex<double>(z0.real() + z0.imag(), 0.0);
    spectrum[M] = std::complex<double>(z0.real() - z0.imag(), 0.0);

    // Intermediate bins.
    for (std::size_t k = 1; k <= M / 2; ++k) {
        std::complex<double> zk = packed[k];
        std::complex<double> znk(packed[M - k].real(), -packed[M - k].imag()); // conjugate
        std::complex<double> even = (zk + znk) * 0.5;
        std::complex<double> odd  = (zk - znk) * 0.5;
        double phase = -2.0 * M_PI * static_cast<double>(k) / static_cast<double>(N);
        std::complex<double> twiddle(std::cos(phase), std::sin(phase));
        std::complex<double> twiddled_odd = odd * twiddle;
        spectrum[k] = even + twiddled_odd;
        spectrum[M - k] = even - twiddled_odd;
        if (M - k != k) {
            // Ensure conjugate symmetry for negative frequencies (not needed for one-sided but set imag sign)
            // The real FFT spectrum for positive frequencies should have proper conjugate pairs.
            // For the one-sided output, we keep both k and M-k; they must be conjugates for real input.
            // Actually the formula above gives correct values; no further adjustment needed.
        }
    }

    return spectrum;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: DC-only signal (constant).
    std::vector<double> x1 = {1.0, 1.0, 1.0, 1.0};
    auto s1 = computeRealFFT(x1);
    assert(s1.size() == 3);
    assert(std::abs(s1[0].real() - 4.0) < 1e-9);
    assert(std::abs(s1[0].imag()) < 1e-9);
    assert(std::abs(s1[1].real()) < 1e-9);
    assert(std::abs(s1[2].real()) < 1e-9);

    // Test 2: Pure cosine at frequency 1 (N=8).
    std::vector<double> x2;
    for (int n = 0; n < 8; ++n) {
        x2.push_back(std::cos(2.0 * M_PI * n / 8.0));
    }
    auto s2 = computeRealFFT(x2);
    assert(s2.size() == 5);
    assert(std::abs(s2[1].real() - 4.0) < 1e-9); // amplitude*N/2
    assert(std::abs(s2[1].imag()) < 1e-9);
    assert(std::abs(s2[0].real()) < 1e-9);
    assert(std::abs(s2[4].real()) < 1e-9);

    // Test 3: Impulse at index 0 (N=4).
    std::vector<double> x3 = {1.0, 0.0, 0.0, 0.0};
    auto s3 = computeRealFFT(x3);
    assert(s3.size() == 3);
    for (int k = 0; k < 3; ++k) {
        assert(std::abs(s3[k].real() - 1.0) < 1e-9);
        assert(std::abs(s3[k].imag()) < 1e-9);
    }

    // Test 4: Nyquist frequency (N=2).
    std::vector<double> x4 = {1.0, -1.0};
    auto s4 = computeRealFFT(x4);
    assert(s4.size() == 2);
    assert(std::abs(s4[0].real()) < 1e-9);
    assert(std::abs(s4[1].real() - 2.0) < 1e-9);

    // Test 5: Random real signal; verify reconstruction via inverse (simplified check: Parseval's theorem).
    std::vector<double> x5 = {0.5, -1.2, 2.0, 3.3, -0.7, 1.1, 0.0, 2.5};
    auto s5 = computeRealFFT(x5);
    double time_energy = 0.0;
    for (double v : x5) time_energy += v * v;
    double freq_energy = s5[0].real() * s5[0].real() + s5[4].real() * s5[4].real();
    for (int k = 1; k < 4; ++k) {
        freq_energy += 2.0 * (s5[k].real() * s5[k].real() + s5[k].imag() * s5[k].imag());
    }
    assert(std::abs(time_energy - freq_energy / 8.0) < 1e-9);

    // Test 6: Invalid input (odd length).
    std::vector<double> x6 = {1.0, 2.0, 3.0};
    assert(computeRealFFT(x6).empty());

    return 0;
}
