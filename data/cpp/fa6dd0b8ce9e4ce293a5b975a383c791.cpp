// Write a C++ function `double spectralRoundTripError(size_t nfft, double tolerance)` that generates a random real-valued signal of length `nfft` with values uniformly distributed in `[-0.5, 0.5]`, performs a forward fast Fourier transform (FFT) using the `Eigen::FFT` class (from the `unsupported/Eigen/FFT` header), then performs an inverse FFT to recover the time-domain signal. The function must return the normalized root-mean-square error (NRMSE) between the original and reconstructed signals, computed as `||original - reconstructed||² / ||original||²` (i.e., the squared L2 norm of the difference divided by the squared L2 norm of the original), where the norms are over the complex magnitude squared. The function should assert that the output error is less than the provided `tolerance`, and return the error value. Ensure the function works for any `nfft >= 1`, and uses proper random seeding with `srand(42)` for reproducibility. Note: The Eigen FFT's `inv` function does not scale by `1/N`, so the inverse output must be scaled by `1/nfft` before comparison.
#include <cassert>

int main() {
    // Test various lengths, including non-power-of-two and small sizes.
    assert(spectralRoundTripError(1, 1e-12) < 1e-12);
    assert(spectralRoundTripError(2, 1e-12) < 1e-12);
    assert(spectralRoundTripError(3, 1e-12) < 1e-12);
    assert(spectralRoundTripError(7, 1e-12) < 1e-12);
    assert(spectralRoundTripError(2*3*4*5*7, 1e-12) < 1e-12);
    assert(spectralRoundTripError(2*9*16*25, 1e-12) < 1e-12);
    assert(spectralRoundTripError(1024, 1e-12) < 1e-12);
    assert(spectralRoundTripError(1000, 1e-12) < 1e-12);

    // Verify the returned error is a finite non-negative number.
    double err = spectralRoundTripError(64, 1e-12);
    assert(err >= 0.0);
    assert(err == err); // not NaN

    return 0;
}
#include <vector>
#include <complex>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <unsupported/Eigen/FFT>

// Compute the normalized squared L2 error between a random real signal and its
// forward/inverse FFT reconstruction using Eigen's FFT. Returns the error.
// Asserts that the error is below the given tolerance.
double spectralRoundTripError(size_t nfft, double tolerance) {
    // Seed for reproducible random numbers.
    std::srand(42);

    // Generate random real time-domain signal in [-0.5, 0.5].
    std::vector<double> timebuf(nfft);
    for (size_t i = 0; i < nfft; ++i) {
        timebuf[i] = static_cast<double>(std::rand()) / RAND_MAX - 0.5;
    }

    // Forward FFT: real -> complex.
    std::vector<std::complex<double> > freqbuf;
    Eigen::FFT<double> fft;
    fft.fwd(freqbuf, timebuf);

    // Inverse FFT: complex -> complex (note: Eigen does not scale by 1/N).
    std::vector<std::complex<double> > recoveredComplex;
    fft.inv(recoveredComplex, freqbuf);

    // Scale inverse output by 1/nfft to get the true inverse.
    std::vector<double> recovered(nfft);
    for (size_t i = 0; i < nfft; ++i) {
        recovered[i] = recoveredComplex[i].real() / static_cast<double>(nfft);
    }

    // Compute squared L2 norm of original and of difference.
    double originalNorm = 0.0;
    double diffNorm = 0.0;
    for (size_t i = 0; i < nfft; ++i) {
        originalNorm += timebuf[i] * timebuf[i];
        double diff = timebuf[i] - recovered[i];
        diffNorm += diff * diff;
    }

    // Guard against zero original norm (should not happen with random data).
    if (originalNorm == 0.0) {
        return 0.0;
    }

    double error = diffNorm / originalNorm;
    assert(error < tolerance);
    return error;
}
// The solution uses the Eigen library's FFT implementation, which is a simple and efficient Cooley-Tukey algorithm. The main steps are:
// 1. Generate a random real-valued time-domain vector `timebuf` of length `nfft` using `rand()` scaled to `[-0.5, 0.5]`. Seeding with a fixed value ensures reproducibility.
// 2. Perform forward FFT: `fft.fwd(freqbuf, timebuf)` produces a complex frequency-domain vector of length `nfft`.
// 3. Perform inverse FFT: `fft.inv(recovered, freqbuf)` produces a complex time-domain vector of length `nfft`. Since Eigen's `inv` does not normalize, multiply each element by `1.0/nfft` to get the correct inverse.
// 4. Compute the squared L2 norm of the original signal (sum of squares of real values) and the squared L2 norm of the difference between original and scaled recovered signal (sum of squared magnitudes of complex differences). Since the original is real, the magnitude squared of a real value is just `x*x`.
// 5. The normalized error is `error = diff_norm / original_norm`. Because the inverse with scaling is mathematically exact up to floating-point precision, the error should be tiny (typically < 1e-10 for double).
// 6. Edge cases: `nfft=1` works trivially; `nfft` can be any positive integer (not necessarily a power of two; Eigen's FFT handles composite sizes). If `original_norm` is zero (extremely unlikely with random data, but possible if all values are exactly 0.5 and -0.5 cancel? Actually, the range is continuous, so zero norm only if all values are exactly 0, impossible with random), we guard by returning 0.
// Time complexity is O(nfft log nfft) for the FFT operations, and O(nfft) for norm calculations. Space complexity is O(nfft) for the vectors.
