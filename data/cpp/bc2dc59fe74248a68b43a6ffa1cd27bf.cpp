Implement a C++ function `computeVisibilityModeCumulative` that takes a vector of non-negative integers representing the per-FEdge quality index (QI) samples for a single ViewEdge, along with a majority threshold fraction (a double between 0.0 and 1.0, representing the fraction of samples that must be covered). The function must return the smallest QI value `i` such that the cumulative count of samples with QI ≤ `i` is at least `ceil(fraction * totalSampleCount)`. This mimics the cumulative visibility algorithm from the provided code, where instead of selecting the most frequent QI (mode), it selects the lowest QI that covers at least a majority of samples (as in `computeCumulativeVisibility`). Handle edge cases: empty vector (return 0), fraction ≤ 0 (return 0), fraction ≥ 1 (return maximum QI in the samples, or 255 if empty), and QI values up to 255. The function must be `const`-correct where applicable and use only standard library headers.

// The solution approach is to compute the total number of samples (`n`). The required coverage count is `required = (int)std::ceil(fraction * n)`. If the vector is empty or fraction <= 0, return 0. If fraction >= 1, return the maximum QI if the vector is non-empty, else 255. Otherwise, create a frequency array of size 256 (since QI values are 0‑255) initialized to zero. Iterate over the samples to increment the frequency for each QI. Then iterate over the frequency array from 0 upward, maintaining a running count. When the running count is at least `required`, return that index. The algorithm runs in O(n + 256) = O(n) time, where n is the number of samples, and uses O(1) auxiliary space (the frequency array is fixed size). Edge cases include samples with QI ≥ 256 (clamp to 255), and the fraction exactly 1.0 requiring the maximum value; the cumulative approach naturally handles this. Also note that if `required` equals 0 (e.g., empty vector), the function returns 0.

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>

/*
 * Compute the smallest QI value that covers at least a given fraction of samples,
 * using cumulative counts across QI classes (0..255).
 * 
 * Parameters:
 *   samples  - vector of non-negative integer QI values (each 0..255, but values >255 are clamped).
 *   fraction - double in [0,1] representing the required coverage fraction.
 * 
 * Returns:
 *   The smallest QI 'i' such that count(samples <= i) >= ceil(fraction * samples.size()).
 *   Special cases:
 *     - empty samples or fraction <= 0: return 0
 *     - fraction >= 1: return maximum QI (or 255 if samples is empty)
 */
int computeVisibilityModeCumulative(const std::vector<int>& samples, double fraction) {
    if (samples.empty() || fraction <= 0.0) {
        return 0;
    }

    const int maxQI = 255;
    int total = static_cast<int>(samples.size());

    if (fraction >= 1.0) {
        // Return the maximum QI present, or 255 if empty (but empty already handled above)
        int maxVal = 0;
        for (int q : samples) {
            maxVal = std::max(maxVal, std::min(q, maxQI));
        }
        return maxVal;
    }

    // Required cumulative count (round up)
    int required = static_cast<int>(std::ceil(fraction * total));
    if (required <= 0) {
        return 0;
    }

    // Frequency array for QI values 0..255
    int freq[256] = {0};
    for (int q : samples) {
        // Clamp QI to 0..255
        int clamped = std::min(std::max(q, 0), maxQI);
        freq[clamped]++;
    }

    // Cumulative scan
    int cumulative = 0;
    for (int i = 0; i <= maxQI; ++i) {
        cumulative += freq[i];
        if (cumulative >= required) {
            return i;
        }
    }

    // Should never reach here if required <= total, but fallback
    return maxQI;
}

#include <cassert>
#include <vector>

// The solution function is declared above
int computeVisibilityModeCumulative(const std::vector<int>& samples, double fraction);

int main() {
    // Basic test: samples [0, 5, 5, 5, 10], fraction 0.5 -> total=5, required=3
    // Cumulative: QI=0 count=1 (less than 3), QI=5 count=4 (>=3) -> return 5
    assert(computeVisibilityModeCumulative({0, 5, 5, 5, 10}, 0.5) == 5);

    // All same value
    assert(computeVisibilityModeCumulative({3, 3, 3}, 0.7) == 3);

    // Edge case: single sample, any positive fraction <=1
    assert(computeVisibilityModeCumulative({42}, 0.1) == 42);
    assert(computeVisibilityModeCumulative({7}, 1.0) == 7);

    // Edge case: empty vector returns 0
    assert(computeVisibilityModeCumulative({}, 0.5) == 0);

    // Edge case: fraction <= 0 returns 0
    assert(computeVisibilityModeCumulative({1, 2, 3}, 0.0) == 0);
    assert(computeVisibilityModeCumulative({1, 2, 3}, -1.0) == 0);

    // Fraction >= 1 returns the maximum value in samples
    assert(computeVisibilityModeCumulative({0, 10, 20}, 1.0) == 20);

    // Clamping to 255
    assert(computeVisibilityModeCumulative({300, 0, 0}, 1.0) == 255);

    // Cumulative covers lower values even if not the mode
    // samples: 0, 0, 10, 20, 30; fraction=0.4 -> total=5, required=2
    // Cumulative: QI=0 count=2 -> return 0
    assert(computeVisibilityModeCumulative({0, 0, 10, 20, 30}, 0.4) == 0);

    // More complex: samples [1,2,2,3,4], fraction=0.8 -> total=5, required=4
    // Cumulative: QI=1 count=1, QI=2 count=3 (less than 4), QI=3 count=4 -> return 3
    assert(computeVisibilityModeCumulative({1, 2, 2, 3, 4}, 0.8) == 3);

    // Fraction exactly covers a boundary
    // samples [0,1,2,3], fraction=0.75 -> total=4, required=3
    // Cumulative: QI=0 count=1, QI=1 count=2 (less than 3), QI=2 count=3 -> return 2
    assert(computeVisibilityModeCumulative({0, 1, 2, 3}, 0.75) == 2);

    return 0;
}
