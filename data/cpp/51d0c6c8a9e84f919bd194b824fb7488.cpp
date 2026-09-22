// Given a vector of complex numbers representing a time-domain signal and an integer FFT size `n` (which may be larger than the input length), write a C++ function that computes the magnitude spectrum (absolute value of the FFT coefficients) after zero-padding the signal to length `n`. The input signal must be placed at the beginning of the zero-padded buffer (index 0 onwards); if the input is longer than `n`, truncate it to the first `n` samples. Return a `std::vector<double>` of length `n` where each element is the magnitude (i.e., `std::abs`) of the corresponding complex FFT output. Implement your own iterative radix-2 Cooley-Tukey FFT (no external libraries) and handle the case where `n` is not a power of two by rounding `n` up to the next power of two (and using that as the actual FFT size). The function must be self-contained and not use any global state.

// The core challenge is implementing an in-place iterative FFT that works for power-of-two sizes. First, determine the effective FFT size `N` as the smallest power of two ≥ the given `n`. If `n` itself is not a power of two, we still must return exactly `n` outputs (the first `n` magnitudes), but the FFT computation uses `N`. Create a vector of complex doubles of length `N`, fill the first `min(input.size(), N)` positions with the input values (truncating if input is longer), and zero-fill the rest. Then apply the iterative radix-2 FFT: bit-reversal permutation to the array, then butterfly stages. For each stage `len` from 2 to N doubling, compute the twiddle factor `w = exp(-2πi/len)`, and for each block of size `len`, combine pairs `(j, j+len/2)` using `a = x[j] + w^k * x[j+len/2]` and `b = x[j] - w^k * x[j+len/2]`, where `k` runs 0..len/2-1. After the FFT, compute magnitude `sqrt(real^2 + imag^2)` for the first `n` outputs (or all N if n > N, but N is always ≥ n). Edge cases: empty input yields all zeros; input length > N implies truncation; n=0 should return an empty vector. Time complexity is O(N log N) for the FFT, with O(N) auxiliary space for the working buffer. Space complexity is O(N). Use double precision for twiddle factors to maintain accuracy.

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

// Compute the magnitude spectrum of a signal using a self-implemented radix-2 FFT.
// The signal is zero-padded to length N (next power of two ≥ n) and the first n magnitudes are returned.
std::vector<double> magnitudeSpectrum(const std::vector<std::complex<double>>& signal, int n) {
    if (n <= 0) return {};

    // Determine next power of two ≥ n
    int N = 1;
    while (N < n) N <<= 1;

    // Prepare zero-padded buffer
    std::vector<std::complex<double>> data(N, std::complex<double>(0.0, 0.0));
    size_t copyLen = std::min(static_cast<size_t>(signal.size()), static_cast<size_t>(N));
    for (size_t i = 0; i < copyLen; ++i) {
        data[i] = signal[i];
    }

    // Iterative radix-2 FFT (in-place)
    // Bit-reversal permutation
    for (int i = 1, j = 0; i < N; ++i) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) std::swap(data[i], data[j]);
    }

    // Butterfly stages
    for (int len = 2; len <= N; len <<= 1) {
        double angle = -2.0 * M_PI / len;
        std::complex<double> wlen(std::cos(angle), std::sin(angle));
        for (int i = 0; i < N; i += len) {
            std::complex<double> w(1.0, 0.0);
            int half = len / 2;
            for (int k = 0; k < half; ++k) {
                std::complex<double> u = data[i + k];
                std::complex<double> v = data[i + k + half] * w;
                data[i + k] = u + v;
                data[i + k + half] = u - v;
                w *= wlen;
            }
        }
    }

    // Extract magnitudes for first n outputs
    std::vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = std::abs(data[i]);
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

// The solution function is declared above; this main tests it.

int main() {
    // Test 1: Empty input, n=4 → all zeros
    {
        std::vector<std::complex<double>> signal;
        auto mag = magnitudeSpectrum(signal, 4);
        assert(mag.size() == 4);
        for (auto m : mag) assert(m == 0.0);
    }

    // Test 2: Single DC component (constant signal), n=4
    {
        std::vector<std::complex<double>> signal(4, std::complex<double>(2.0, 0.0));
        auto mag = magnitudeSpectrum(signal, 4);
        // DC bin magnitude = 8 (sum of 2*4), others 0 (within tolerance)
        assert(std::fabs(mag[0] - 8.0) < 1e-12);
        for (int i = 1; i < 4; ++i) assert(std::fabs(mag[i]) < 1e-12);
    }

    // Test 3: n not power of two (n=3, N=4), signal length 2
    {
        std::vector<std::complex<double>> signal = {{1.0, 0.0}, {1.0, 0.0}};
        auto mag = magnitudeSpectrum(signal, 3);
        assert(mag.size() == 3);
        // FFT of [1,1,0,0] → [2, 1-i, 0, 1+i]; magnitudes: 2, sqrt(2), 0
        assert(std::fabs(mag[0] - 2.0) < 1e-12);
        assert(std::fabs(mag[1] - std::sqrt(2.0)) < 1e-12);
        assert(std::fabs(mag[2] - 0.0) < 1e-12);
    }

    // Test 4: Truncation if signal longer than N
    {
        std::vector<std::complex<double>> signal(8, std::complex<double>(1.0, 0.0));
        auto mag = magnitudeSpectrum(signal, 4); // N=4, take only first 4 samples
        assert(mag.size() == 4);
        assert(std::fabs(mag[0] - 4.0) < 1e-12); // sum of 4 ones
        for (int i = 1; i < 4; ++i) assert(std::fabs(mag[i]) < 1e-12);
    }

    // Test 5: n=1 (N=1), single sample
    {
        std::vector<std::complex<double>> signal = {{3.0, 4.0}};
        auto mag = magnitudeSpectrum(signal, 1);
        assert(mag.size() == 1);
        assert(std::fabs(mag[0] - 5.0) < 1e-12); // magnitude of 3+4i
    }

    // Test 6: Pure sine wave of frequency 1 at n=8 → peak at bin 1
    {
        int n = 8;
        std::vector<std::complex<double>> signal(n);
        for (int i = 0; i < n; ++i) {
            signal[i] = std::complex<double>(std::sin(2.0 * M_PI * i / n), 0.0);
        }
        auto mag = magnitudeSpectrum(signal, n);
        assert(mag.size() == n);
        // Expected: bin 1 magnitude = 4 (n/2), bin 7 magnitude = 4, others near 0
        assert(std::fabs(mag[1] - 4.0) < 1e-10);
        assert(std::fabs(mag[7] - 4.0) < 1e-10);
        for (int i = 0; i < n; ++i) {
            if (i != 1 && i != 7) assert(mag[i] < 1e-10);
        }
    }

    // Test 7: n=0 returns empty
    {
        std::vector<std::complex<double>> signal = {{1.0, 0.0}};
        auto mag = magnitudeSpectrum(signal, 0);
        assert(mag.empty());
    }

    return 0;
}
