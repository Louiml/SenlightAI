/*
Write a standalone C++ function named `computeDiscreteFourierTransform` that takes a `std::vector<double>` of real-valued time-domain samples (of arbitrary length, possibly empty) and returns a `std::vector<std::complex<double>>` containing the Discrete Fourier Transform (DFT) coefficients. For each frequency index `k` from 0 to N-1, compute: `X[k] = sum_{n=0}^{N-1} x[n] * exp(-2*pi*i*k*n/N)`, where `i` is the imaginary unit. The function must use a straightforward O(N^2) algorithm (no FFT). Also provide a helper `magnitudeAndPhase` that takes the output vector and returns a `std::vector<std::pair<double,double>>` where each pair is (magnitude, phase in radians). Handle the empty input case by returning an empty vector. Use `const` correctness appropriately.
*/
#include <vector>
#include <complex>
#include <cmath>
#include <utility>

// Compute the Discrete Fourier Transform of a real-valued signal.
// Returns a vector of complex coefficients X[k] = sum_{n=0}^{N-1} x[n]*exp(-2*pi*i*k*n/N).
std::vector<std::complex<double>> computeDiscreteFourierTransform(const std::vector<double>& signal) {
    const std::size_t N = signal.size();
    if (N == 0) {
        return {};
    }
    std::vector<std::complex<double>> dft(N);
    const double factor = -2.0 * M_PI / static_cast<double>(N);
    for (std::size_t k = 0; k < N; ++k) {
        std::complex<double> sum(0.0, 0.0);
        const double angleBase = factor * static_cast<double>(k);
        for (std::size_t n = 0; n < N; ++n) {
            double angle = angleBase * static_cast<double>(n);
            sum += signal[n] * std::polar(1.0, angle);
        }
        dft[k] = sum;
    }
    return dft;
}

// Convert a vector of complex numbers to (magnitude, phase in radians) pairs.
std::vector<std::pair<double, double>> magnitudeAndPhase(const std::vector<std::complex<double>>& dft) {
    std::vector<std::pair<double, double>> result;
    result.reserve(dft.size());
    for (const auto& value : dft) {
        result.emplace_back(std::abs(value), std::arg(value));
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <complex>
#include <vector>
#include <utility>

// Include the solution functions (or paste them here for compilation)

int main() {
    // Test 1: Empty input
    std::vector<double> empty;
    assert(computeDiscreteFourierTransform(empty).empty());
    assert(magnitudeAndPhase(empty).empty());

    // Test 2: Single sample N=1
    std::vector<double> single = {3.5};
    auto dft_single = computeDiscreteFourierTransform(single);
    assert(dft_single.size() == 1);
    assert(std::abs(dft_single[0].real() - 3.5) < 1e-12);
    assert(std::abs(dft_single[0].imag()) < 1e-12);

    // Test 3: Two samples x = {1, -1}
    std::vector<double> two = {1.0, -1.0};
    auto dft_two = computeDiscreteFourierTransform(two);
    assert(dft_two.size() == 2);
    // X[0] = 1 + (-1) = 0
    assert(std::abs(dft_two[0].real()) < 1e-12);
    assert(std::abs(dft_two[0].imag()) < 1e-12);
    // X[1] = 1*exp(0) + (-1)*exp(-pi*i) = 1 + (-1)*(-1) = 2
    assert(std::abs(dft_two[1].real() - 2.0) < 1e-12);
    assert(std::abs(dft_two[1].imag()) < 1e-12);

    // Test 4: Four samples of a constant signal {2,2,2,2}
    std::vector<double> four_const = {2.0, 2.0, 2.0, 2.0};
    auto dft_four = computeDiscreteFourierTransform(four_const);
    assert(dft_four.size() == 4);
    // X[0] = 8
    assert(std::abs(dft_four[0].real() - 8.0) < 1e-12);
    assert(std::abs(dft_four[0].imag()) < 1e-12);
    // All other X[k] = 0
    for (std::size_t k = 1; k < 4; ++k) {
        assert(std::abs(dft_four[k].real()) < 1e-12);
        assert(std::abs(dft_four[k].imag()) < 1e-12);
    }

    // Test 5: Magnitude and phase for a cosine-like input
    // Use x[n] = cos(2*pi*1*n/4) but sampled? Instead test with simple known: x = {1, 0, -1, 0}
    std::vector<double> cosine = {1.0, 0.0, -1.0, 0.0};
    auto dft_cos = computeDiscreteFourierTransform(cosine);
    auto mag_phase = magnitudeAndPhase(dft_cos);
    assert(mag_phase.size() == 4);
    // X[1] should be 2 (for this pattern: 1*exp(0) + 0 -1*exp(-pi*i/2) + 0 = 1 + i? Let's compute manually: 
    // Actually X[1] = 1 + 0*exp(-pi*i/2) + (-1)*exp(-pi*i) + 0*exp(-3pi*i/2) = 1 + 0 + 1 + 0 = 2. So magnitude 2, phase 0.
    assert(std::abs(mag_phase[1].first - 2.0) < 1e-12);
    assert(std::abs(mag_phase[1].second) < 1e-12);

    // Test 6: Random check with N=3 using numerical sum
    std::vector<double> three = {1.0, 2.0, -0.5};
    auto dft_three = computeDiscreteFourierTransform(three);
    assert(dft_three.size() == 3);
    // Manually compute X[1] and compare
    std::complex<double> expected(0,0);
    for (std::size_t n = 0; n < 3; ++n) {
        double angle = -2.0 * M_PI * 1.0 * static_cast<double>(n) / 3.0;
        expected += three[n] * std::polar(1.0, angle);
    }
    assert(std::abs(dft_three[1].real() - expected.real()) < 1e-12);
    assert(std::abs(dft_three[1].imag() - expected.imag()) < 1e-12);

    // Test 7: Verify magnitude and phase for a single complex value
    std::vector<std::complex<double>> one_complex = {std::complex<double>(3.0, 4.0)};
    auto mp = magnitudeAndPhase(one_complex);
    assert(std::abs(mp[0].first - 5.0) < 1e-12);
    assert(std::abs(mp[0].second - std::atan2(4.0, 3.0)) < 1e-12);

    return 0;
}
// The solution requires implementing the DFT definition directly: for each of the N frequency bins, accumulate the sum over all N time samples. The core computation is `std::polar(1.0, -2.0 * M_PI * k * n / N)` or equivalently `std::exp(std::complex<double>(0, -2.0 * M_PI * k * n / N))`. For each pair (k,n), multiply the sample value by this complex exponential and sum. The result is a complex vector of length N. Important edge cases: (1) Empty input — immediately return an empty vector since there are no samples; the algorithm would otherwise divide by zero. (2) N=1 — just return [x[0]] because the DFT of a single sample is itself (the phase is zero). (3) Values can be negative or fractional; std::complex handles them. For magnitude/phase, use `std::abs` and `std::arg`; phase is in radians. Time complexity is O(N^2) due to double loop; space complexity is O(N) for the output vectors. The implementation uses `std::vector` and `<complex>`, `<cmath>`, `<utility>`.
