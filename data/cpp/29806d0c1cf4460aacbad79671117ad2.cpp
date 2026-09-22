// Write a standalone C++ function that simulates the construction of a multi-scale image pyramid and counts the total number of "keypoints" detected across all pyramid levels, following the structure of the given ASIFT snippet but simplified. Specifically, given an input 1D signal (represented as a `std::vector<float>`), a number of "octaves" `num_levels`, and a base sigma, the function should: (1) For each level from 0 to `num_levels-1`, compute a smoothed version of the signal by convolving it with a normalized Gaussian kernel whose sigma is `base_sigma * pow(sqrt(2.0), level)`. (2) Count the number of local maxima in the smoothed signal that are strictly greater than both their immediate neighbors (ignore boundary points). (3) Sum these counts across all levels and return the total. The input signal length must be at least 3. The function must handle edge cases where the Gaussian kernel is truncated (kernel size at least 3 and odd), and the convolution must preserve vector length using edge replication (i.e., for positions near the boundaries, the missing neighbors are filled with the closest valid value). Return the total count as an `int`. Do not include a `main` function.
The task requires implementing a 1D Gaussian blur followed by a peak detection across multiple scales. The core algorithm: For each scale level, compute a Gaussian kernel of size `ksize = max(3, 2 * (int)(4.0 * sigma) + 1)` and make it odd. Fill kernel values with `exp(-x*x/(2*sigma*sigma))` and normalize. Convolve the input signal with this kernel using edge replication: create a temporary padded buffer of length `n + 2*halfsize` where `halfsize = ksize/2`, copy the signal in the middle, and fill the left and right padded regions with the first and last signal values respectively. Then perform the convolution for each position, storing results back into a new vector of the original length. After the smoothed signal is ready, count local maxima: for each index `i` from 1 to `n-2`, if `smoothed[i] > smoothed[i-1] && smoothed[i] > smoothed[i+1]`, increment the count. Edge cases: if `num_levels <= 0`, return 0. If the signal has fewer than 3 elements, return 0 (since no interior points exist). The Gaussian sigma grows with `sqrt(2.0)^level`, so kernel size increases; ensure kernel size never exceeds a practical cap (e.g., 100), but the problem constraints likely allow this. Time complexity: For each level, convolution takes O(n * ksize), where ksize is roughly O(sigma). Since sigma doubles every two levels (because sqrt(2)^2 = 2), the total over all levels is O(n * sum(ksize)) = O(n * max_sigma), which is acceptable. Space complexity: O(n + max_ksize) per level, plus the output vector.
#include <vector>
#include <cmath>
#include <algorithm>

// Count total local maxima across multiple Gaussian-smoothed versions of a 1D signal.
int countMultiscalePeaks(const std::vector<float>& signal, int num_levels, float base_sigma = 1.6f) {
    int n = static_cast<int>(signal.size());
    if (n < 3 || num_levels <= 0) {
        return 0;
    }

    int total_peaks = 0;

    for (int level = 0; level < num_levels; ++level) {
        float sigma = base_sigma * std::pow(std::sqrt(2.0f), level);
        int ksize = static_cast<int>(2.0f * 4.0f * sigma + 1.0f);
        ksize = std::max(3, ksize);
        if (ksize % 2 == 0) {
            ++ksize;
        }
        // Cap kernel size to avoid excessive memory (though unlikely for reasonable inputs).
        if (ksize > 101) {
            ksize = 101;
            if (ksize % 2 == 0) ++ksize;
        }

        // Build normalized Gaussian kernel.
        std::vector<float> kernel(ksize);
        float sum = 0.0f;
        int halfsize = ksize / 2;
        for (int i = 0; i < ksize; ++i) {
            float x = static_cast<float>(i - halfsize);
            kernel[i] = std::exp(-x * x / (2.0f * sigma * sigma));
            sum += kernel[i];
        }
        for (int i = 0; i < ksize; ++i) {
            kernel[i] /= sum;
        }

        // Convolve with edge replication.
        std::vector<float> padded(n + 2 * halfsize);
        for (int i = 0; i < halfsize; ++i) {
            padded[i] = signal[0];
        }
        for (int i = 0; i < n; ++i) {
            padded[halfsize + i] = signal[i];
        }
        for (int i = 0; i < halfsize; ++i) {
            padded[halfsize + n + i] = signal[n - 1];
        }

        std::vector<float> smoothed(n);
        for (int i = 0; i < n; ++i) {
            float acc = 0.0f;
            for (int j = 0; j < ksize; ++j) {
                acc += padded[i + j] * kernel[j];
            }
            smoothed[i] = acc;
        }

        // Count local maxima (strictly greater than both neighbors).
        for (int i = 1; i < n - 1; ++i) {
            if (smoothed[i] > smoothed[i - 1] && smoothed[i] > smoothed[i + 1]) {
                ++total_peaks;
            }
        }
    }

    return total_peaks;
}
#include <cassert>
#include <vector>
#include <cmath>

// Include the solution function declaration here (already defined above).

int main() {
    // Test 1: Simple signal with a single peak at each level (n=5, base_sigma small)
    {
        std::vector<float> sig = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
        // At level 0, sigma=1.6, peak likely remains; at higher levels smoothing may remove it.
        // We at least check it's non-negative and returns a value consistent with a single peak at level 0.
        int result = countMultiscalePeaks(sig, 1, 0.1f); // very small sigma => kernel ~3, almost no blur
        // With sigma=0.1, kernel size becomes 3 (since max(3, ...)), center value dominates, peak at index 1 remains.
        assert(result == 1);
    }

    // Test 2: Flat signal has no peaks.
    {
        std::vector<float> sig = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
        assert(countMultiscalePeaks(sig, 3, 1.6f) == 0);
    }

    // Test 3: Monotonic signal has no peaks.
    {
        std::vector<float> sig = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        assert(countMultiscalePeaks(sig, 2, 1.6f) == 0);
    }

    // Test 4: Two peaks at level 0 with very small sigma, but smoothing may reduce counts at higher levels.
    {
        std::vector<float> sig = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        int result = countMultiscalePeaks(sig, 1, 0.1f);
        assert(result == 2);
    }

    // Test 5: Invalid inputs yield 0.
    {
        std::vector<float> sig_short = {1.0f, 2.0f};
        assert(countMultiscalePeaks(sig_short, 2, 1.6f) == 0);
        std::vector<float> sig = {1.0f, 2.0f, 3.0f};
        assert(countMultiscalePeaks(sig, 0, 1.6f) == 0);
    }

    // Test 6: Multiple levels on a sine-like wave; ensure result is non-negative and not crashing.
    {
        std::vector<float> sig(100);
        for (int i = 0; i < 100; ++i) {
            sig[i] = std::sin(static_cast<float>(i) * 0.2f);
        }
        int total = countMultiscalePeaks(sig, 4, 1.6f);
        assert(total >= 0);
    }

    // Test 7: Constant large signal with many levels returns 0.
    {
        std::vector<float> sig(50, 3.14f);
        assert(countMultiscalePeaks(sig, 5, 1.6f) == 0);
    }

    return 0;
}
