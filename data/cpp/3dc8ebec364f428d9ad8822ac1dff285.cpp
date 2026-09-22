Write a C++ function that performs a 5-level wavelet decomposition on a one-dimensional signal using the biorthogonal bior3.1 filter bank, then reconstructs the signal after applying a hard-threshold denoising step. The function should take a `std::vector<double>` containing the input signal, a threshold value `lim`, and return a `std::vector<double>` containing the denoised signal. The decomposition must use the exact bior3.1 analysis and synthesis filter coefficients provided, with boundary handling via symmetric extension (reflect about the last sample, i.e., mirroring without repeating the edge point). The thresholding rule is: if the absolute value of a wavelet coefficient is less than `lim`, set it to zero; otherwise, keep it unchanged. The approximation coefficients at the deepest level (level 5) are not thresholded. The input signal length must be at least 16 and a power of two (e.g., 16, 32, 64, ...) to ensure the 5-level decomposition is well-defined. The function should be named `waveletDenoiseBior31` and must be self-contained, using only standard library facilities.
#include <cassert>
#include <cmath>
#include <vector>

// Declare the function (or include the header if separate)
std::vector<double> waveletDenoiseBior31(const std::vector<double>& signal, double limit);

int main() {
    // Test 1: A constant signal remains unchanged after denoising (all wavelet coefficients are ~0).
    std::vector<double> constSignal(32, 100.0);
    std::vector<double> out1 = waveletDenoiseBior31(constSignal, 0.00001);
    for (size_t i = 0; i < out1.size(); ++i) {
        assert(std::fabs(out1[i] - 100.0) < 1e-6);
    }

    // Test 2: A signal with a single impulse. After thresholding, the reconstruction should be near zero except near boundaries? 
    // Since impulse creates large wavelet coefficients, thresholding with a small limit may not remove all. We test that the output length is preserved and values are finite.
    std::vector<double> impulse(16, 0.0);
    impulse[8] = 1.0;
    std::vector<double> out2 = waveletDenoiseBior31(impulse, 0.001);
    assert(out2.size() == 16);
    for (double v : out2) {
        assert(std::isfinite(v));
    }

    // Test 3: Perfect reconstruction when threshold is zero (no thresholding).
    std::vector<double> signal(32);
    for (size_t i = 0; i < 32; ++i) {
        signal[i] = std::sin(2.0 * M_PI * i / 32.0) + 0.5 * std::cos(4.0 * M_PI * i / 32.0);
    }
    std::vector<double> out3 = waveletDenoiseBior31(signal, 0.0);
    for (size_t i = 0; i < signal.size(); ++i) {
        assert(std::fabs(out3[i] - signal[i]) < 1e-6);
    }

    // Test 4: Heavy thresholding should smooth a noisy signal but preserve mean roughly.
    std::vector<double> noisy(64, 0.0);
    for (size_t i = 0; i < 64; ++i) {
        noisy[i] = 5.0 + (i % 7) * 0.1; // small variation
    }
    std::vector<double> out4 = waveletDenoiseBior31(noisy, 0.5);
    double sum = 0.0;
    for (double v : out4) sum += v;
    double mean = sum / out4.size();
    assert(std::fabs(mean - 5.0) < 0.5); // mean close to original

    // Test 5: All zero input gives zero output.
    std::vector<double> zeros(16, 0.0);
    std::vector<double> out5 = waveletDenoiseBior31(zeros, 0.1);
    for (double v : out5) {
        assert(v == 0.0);
    }

    return 0;
}
#include <vector>
#include <cstddef>

// Perform 5-level wavelet denoising using bior3.1 filters.
// Input: signal of length N (power of two, N >= 16), threshold limit.
// Output: denoised signal of same length.
std::vector<double> waveletDenoiseBior31(const std::vector<double>& signal, double limit) {
    const std::size_t N = signal.size();
    const std::size_t filterLen = 4; // bior3.1 filters have 4 taps
    const int levels = 5;

    // bior3.1 analysis filters
    const double lowDe[filterLen] = {
        -0.35355339059327, 1.06066017177982, 1.06066017177982, -0.35355339059327
    };
    const double highDe[filterLen] = {
        -0.17677669529664, 0.53033008588991, -0.53033008588991, 0.17677669529664
    };

    // bior3.1 synthesis filters
    const double lowRe[filterLen] = {
        0.17677669529664, 0.53033008588991, 0.53033008588991, 0.17677669529664
    };
    const double highRe[filterLen] = {
        -0.35355339059327, -1.06066017177982, 1.06066017177982, 0.35355339059327
    };

    // Decomposition offsets: for analysis filters, the offset is -2 for both low and high.
    const int decLoOffset = -2;
    const int decHiOffset = -2;
    // Reconstruction offsets: for synthesis filters, offset is -2 for low, -2 for high.
    const int recLoOffset = -2;
    const int recHiOffset = -2;

    // Helper: symmetric extension of a vector, reflecting at edges (without repeating edge sample).
    auto symmetricExtend = [](const std::vector<double>& input, std::size_t extension) {
        std::size_t n = input.size();
        std::vector<double> extended(2 * extension + n);
        for (std::size_t i = 0; i < n; ++i) {
            extended[extension + i] = input[i];
        }
        // Left extension: reflect without repeating edge
        for (std::size_t i = 0; i < extension; ++i) {
            extended[extension - 1 - i] = input[i + 1];
        }
        // Right extension: reflect without repeating edge
        for (std::size_t i = 0; i < extension; ++i) {
            extended[extension + n + i] = input[n - 2 - i];
        }
        return extended;
    };

    // Forward transform: returns a vector of detail vectors (level 1..5) plus final approximation.
    // We store details in a vector-of-vectors, and final approx is last.
    std::vector<std::vector<double>> details(levels);
    std::vector<double> approx = signal;
    std::size_t currentLen = N;
    for (int level = 0; level < levels; ++level) {
        // Extend current approximation
        std::size_t ext = filterLen - 1;
        std::vector<double> extVec = symmetricExtend(approx, ext);
        std::size_t extLen = extVec.size();

        // Compute convolution and downsample by 2
        std::size_t newLen = currentLen / 2;
        std::vector<double> low(newLen, 0.0);
        std::vector<double> high(newLen, 0.0);

        // Convolution: for each output index k, sum over filter taps.
        // The offset shifts the filter relative to the signal.
        for (std::size_t k = 0; k < newLen; ++k) {
            double sumLow = 0.0, sumHigh = 0.0;
            // The downsampling keeps even indices after convolution.
            // With offset -2, the convolution index is 2*k - offset.
            // Using symmetric extension, we map to extended indices.
            for (std::size_t m = 0; m < filterLen; ++m) {
                // The actual signal index before downsampling is: 2*k + m + offset? 
                // We must be careful: standard DWT with offset -2 means the filter is shifted by -2.
                // The convolution output at index i is sum_m filter[m] * signal[i + m + offset].
                // After downsampling, i = 2*k.
                // So we need signal[2*k + m + offset].
                // Because offset = -2, we use signal[2*k + m - 2], but 2*k+m-2 can be negative.
                // Symmetric extension handles this: extended index = (2*k + m - 2) + ext.
                std::ptrdiff_t sigIdx = static_cast<std::ptrdiff_t>(2 * k + m) + decLoOffset;
                std::size_t extIdx = static_cast<std::size_t>(sigIdx + static_cast<std::ptrdiff_t>(ext));
                sumLow += lowDe[m] * extVec[extIdx];
                sumHigh += highDe[m] * extVec[extIdx];
            }
            low[k] = sumLow;
            high[k] = sumHigh;
        }
        // Store detail (high) and set new approximation (low)
        details[level] = high;
        approx = low;
        currentLen = newLen;
    }
    // At this point, approx is the final approximation at deepest level.

    // Threshold all detail coefficients
    for (int level = 0; level < levels; ++level) {
        for (std::size_t i = 0; i < details[level].size(); ++i) {
            if (std::abs(details[level][i]) < limit) {
                details[level][i] = 0.0;
            }
        }
    }

    // Inverse transform: rebuild signal from deepest approximation and details.
    std::vector<double> reconApprox = approx;
    std::size_t reconLen = approx.size();
    for (int level = levels - 1; level >= 0; --level) {
        std::size_t newLen = reconLen * 2;
        std::vector<double> rebuilt(newLen, 0.0);

        // For reconstruction, we upsample (insert zeros) then convolve with synthesis filters.
        // The synthesis offset is -2 as well.
        // Create upsampled low and high (alternating zeros), then extend both.
        std::vector<double> upLow(2 * reconLen, 0.0);
        std::vector<double> upHigh(2 * reconLen, 0.0);
        for (std::size_t i = 0; i < reconLen; ++i) {
            upLow[2 * i] = reconApprox[i];
            upHigh[2 * i] = details[level][i];
        }

        // Symmetric extension for upsampled vectors
        std::size_t ext = filterLen - 1;
        std::vector<double> extLow = symmetricExtend(upLow, ext);
        std::vector<double> extHigh = symmetricExtend(upHigh, ext);

        // Convolve with synthesis filters, combining both branches.
        for (std::size_t k = 0; k < newLen; ++k) {
            double sum = 0.0;
            for (std::size_t m = 0; m < filterLen; ++m) {
                // For synthesis, output index k = sum over m: lowRe[m]*upLow[k - m - recLoOffset]? 
                // Standard reconstruction: y[n] = sum_m lowRe[m]*uLow[n - m - recLoOffset] + highRe[m]*uHigh[n - m - recHiOffset].
                // Here uLow and uHigh are upsampled sequences, indices n - m - offset.
                // Because offset = -2, we get n - m + 2.
                // To avoid negative indices, use symmetric extension on the upsampled vectors.
                std::ptrdiff_t idx = static_cast<std::ptrdiff_t>(k) - static_cast<std::ptrdiff_t>(m) - recLoOffset;
                // idx can be negative or >= 2*reconLen, so map via extension.
                // Use the already extended vectors: extLow has length 2*reconLen + 2*ext.
                // The extended index = idx + ext.
                std::size_t extIdx = static_cast<std::size_t>(idx + static_cast<std::ptrdiff_t>(ext));
                sum += lowRe[m] * extLow[extIdx] + highRe[m] * extHigh[extIdx];
            }
            rebuilt[k] = sum;
        }
        reconApprox = rebuilt;
        reconLen = newLen;
    }

    return reconApprox;
}
// The solution requires implementing a discrete wavelet transform (DWT) with biorthogonal filters. The main algorithm follows a cascade filter bank structure: at each level, the current approximation coefficients (starting with the original signal) are convolved with the analysis low-pass and high-pass filters, then downsampled by 2. To handle boundaries correctly with symmetric extension, we first reflect the signal at both ends by `filterLength - 1` samples without repeating the edge sample (e.g., for signal `[a,b,c,d]`, odd-length symmetric extension yields `[c,b,a,b,c,d,c,b]` for filter length 4). After convolution and downsampling, the coefficients are stored: the low-frequency part becomes the next level's input, and the high-frequency part is stored as wavelet coefficients. This process repeats for 5 levels, resulting in one approximation vector and 5 detail vectors (one per level). For reconstruction, we start from the deepest approximation and successively upsample (insert zeros between samples), convolve with the synthesis filters, and add the contributions from the corresponding detail vector. The inverse transform must also handle boundaries via symmetric extension, but because the synthesis filters are the exact inverses of the analysis pair under the symmetric extension scheme, we can apply the standard inverse algorithm using the given synthesis coefficients with appropriate offsets. The thresholding is applied to all detail coefficients (for levels 1 through 5) before reconstruction; the final approximation at level 5 is kept unchanged. Edge cases: if the input size is not a power of two or is smaller than 16, the function can either return an empty vector or explicitly handle by pre-padding (but the task specifies the input must satisfy these constraints, so no error handling is necessary; we assume valid input). Time complexity for each level is \(O(N)\) where \(N\) is the signal length, and since there are 5 levels, total is \(O(5N) = O(N)\). Space complexity is \(O(N)\) for storing coefficients and intermediate arrays, plus \(O(1)\) for the filter arrays (which are fixed size 4 each).
