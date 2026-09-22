// Write a C++ function named `compute_roundtrip_error` that computes the normalized roundtrip root-mean-square error (RMSE) for a forward and inverse Fast Fourier Transform (FFT) applied to a vector of real-valued `double` samples. The function must accept a single argument: a `std::vector<double>` containing time-domain real samples. It should internally perform a complex FFT of the input, then invert that FFT back to the time domain, and return the normalized error defined as `sum(|original - reconstructed|^2) / sum(|original|^2)`. The implementation must be self-contained, requiring no external libraries beyond the C++ standard library. The function must handle empty input by returning `0.0`, and it should assume that the input length is arbitrary (not necessarily a power of two), though the FFT implementation used internally is free to pad or use any algorithm as long as the output is correct for any length. The reconstruction may have a scaling factor; your function must automatically normalize the reconstructed signal so that it is directly comparable to the original before computing the error. Specifically, if the inverse FFT produces values scaled by `1/N`, you must multiply the reconstructed signal by `N` to match the original scale, where `N` is the size of the input vector.
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared above (already included via header or same file).
// This is a separate test file.

int main() {
    // Empty vector -> 0 error
    std::vector<double> empty;
    assert(compute_roundtrip_error(empty) == 0.0);

    // Single element
    std::vector<double> single = {3.0};
    assert(std::fabs(compute_roundtrip_error(single) - 0.0) < 1e-12);

    // All zeros -> denominator zero, return 0
    std::vector<double> zeros(8, 0.0);
    assert(compute_roundtrip_error(zeros) == 0.0);

    // Simple non-power-of-two length
    std::vector<double> v1 = {1.0, -2.0, 3.5, 0.5, -1.0};
    double err1 = compute_roundtrip_error(v1);
    assert(err1 < 1e-12); // Should be essentially zero (numerical precision)

    // Power-of-two length with random data
    std::vector<double> v2 = {0.1, 0.2, -0.3, 0.4, -0.5, 0.6, -0.7, 0.8};
    double err2 = compute_roundtrip_error(v2);
    assert(err2 < 1e-12);

    // Longer arbitrary length
    std::vector<double> v3(1000);
    for (int i = 0; i < 1000; ++i) {
        v3[i] = std::sin(static_cast<double>(i) * 0.01) + 0.001 * i;
    }
    double err3 = compute_roundtrip_error(v3);
    assert(err3 < 1e-12);

    // Large power of two
    std::vector<double> v4(1024);
    for (int i = 0; i < 1024; ++i) {
        v4[i] = static_cast<double>(i % 7) * 0.5;
    }
    double err4 = compute_roundtrip_error(v4);
    assert(err4 < 1e-12);

    return 0;
}
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

// Iterative radix-2 FFT (in-place) using Cooley-Tukey.
// Requires size to be a power of two.
static void fft_radix2(std::vector<std::complex<double>>& a, bool invert) {
    int n = static_cast<int>(a.size());
    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
    // FFT butterflies
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * M_PI / len * (invert ? -1 : 1);
        std::complex<double> wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            std::complex<double> w(1.0, 0.0);
            for (int j = 0; j < len / 2; ++j) {
                std::complex<double> u = a[i + j];
                std::complex<double> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= static_cast<double>(n);
        }
    }
}

// Compute normalized roundtrip error for real vector input.
double compute_roundtrip_error(const std::vector<double>& x) {
    if (x.empty()) {
        return 0.0;
    }
    int N = static_cast<int>(x.size());
    // Find next power of two >= N
    int M = 1;
    while (M < N) {
        M <<= 1;
    }
    // Pad and copy
    std::vector<std::complex<double>> data(M, std::complex<double>(0.0, 0.0));
    for (int i = 0; i < N; ++i) {
        data[i] = std::complex<double>(x[i], 0.0);
    }
    // Forward FFT (invert=false)
    fft_radix2(data, false);
    // Copy for inverse
    std::vector<std::complex<double>> recon(data);
    // Inverse FFT (invert=true)
    fft_radix2(recon, true);
    // Compute normalized error on first N entries
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < N; ++i) {
        double orig = x[i];
        double rec = recon[i].real();
        double diff = orig - rec;
        numerator += diff * diff;
        denominator += orig * orig;
    }
    if (denominator == 0.0) {
        return 0.0; // All zeros, perfect reconstruction technically
    }
    return numerator / denominator;
}
// The core challenge is to implement a working FFT (forward and inverse) from scratch without external libraries, and then compute a normalized error. The approach: first, handle the empty case by returning `0.0`. For a non-empty vector `x` of size `N`, we create a complex vector `X` by copying each real sample into the real part. Then we perform a Cooley-Tukey iterative FFT on `X` (in place) to get the frequency-domain representation. Next, we copy `X` into a new complex vector `Y` for inverse transformation. The inverse FFT is implemented by taking the conjugate of the forward FFT of the conjugate, then scaling by `1/N`. That is, to invert `Y`, we compute `conj(fft(conj(Y)))` and then divide each element by `N`. This yields a complex vector `y_recon` whose real parts should equal the original samples up to numerical precision. After obtaining `y_recon`, we extract the real parts into a vector `recon_real`, then compute the sum of squared magnitudes of `original - recon_real` and sum of squared magnitudes of `original`. The ratio is returned. Edge cases: empty input returns `0.0`; input of size 1 works trivially; input lengths that are not powers of two are handled by the iterative FFT algorithm which only requires the length to be factorizable into 2s (we can pad with zeros to next power of two, but that changes the frequency representation, so instead we implement a generic FFT that works for any composite length? Simpler: implement a radix-2 FFT that requires power-of-two length; to handle arbitrary N, we can pad the input with zeros to the next power of two, perform the FFT and inverse on that padded size, then truncate the reconstructed signal to the original N. However, that would introduce a scaling factor of `padded_N / original_N`? Actually, if we pad zeros, the FFT of the padded signal will have a different frequency spectrum, but the inverse fully reconstructs the padded signal (zeros at the end). So truncating the reconstructed signal back to N gives exactly the original samples (up to floating error) because the first N entries of the padded reconstructed signal equal the original entries. So that works. To keep implementation simple, we will pad to the next power of two, do radix-2 FFT and inverse, then truncate. However, the inverse scaling is by `padded_N`; after truncation, the first N values are correct. We must multiply the truncated values by `padded_N` (since the inverse FFT gives `1/padded_N` scaled values). Since padded_N is a power of two, this is fine. The algorithm: compute `M = next power of two >= N`. Create complex vector `padded` of size M, fill first N with original real values, rest zero. Perform iterative radix-2 FFT on `padded` (in place). Copy to `recon` and perform inverse FFT (by conjugating, forward, conjugate, then divide by M). Truncate `recon` to first N, extract real parts, multiply by M to scale back to original amplitude? Actually, the standard inverse FFT formula is `x[n] = (1/N) * sum_{k=0}^{N-1} X[k] * exp(2πi nk/N)`. If we use the forward FFT with negative exponent and inverse with positive, we need to divide by M. After dividing by M, we get the original padded signal exactly. So after inverse we have `recon_padded` with real parts equal to original (for first N). No need to multiply by M again because we already divided by M. So just truncate. Then compute error. The time complexity is O(M log M) where M is the next power of two. Space O(M). Precision is double. We must be careful with complex arithmetic and using `std::complex<double>`.
