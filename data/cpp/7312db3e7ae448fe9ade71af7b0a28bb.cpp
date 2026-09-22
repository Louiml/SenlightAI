// Write a C++ function `smoothDerivative` that takes a `std::vector<float>` of time-ordered samples and a `std::vector<uint32_t>` of corresponding timestamps (in milliseconds), both of equal length, and returns a `std::vector<float>` containing the smoothed derivative (slope) at each valid time index. The derivative is computed using a Savitzky–Golay-style finite-difference filter with an order that adapts to the number of available points, and only for indices where there are at least 5 points behind and ahead (a centered window of size up to 11). For boundary positions where insufficient data exists, the output value should be `0.0f`. The function must also handle cases where timestamps are not strictly increasing by skipping any sample whose timestamp equals the previous one (treating it as a duplicate reading with zero time difference). The algorithm must use the convolution coefficients from the polynomial differentiators: for a window of total size `N` (N = 5, 7, 9, or 11), the slope is computed as `(1 / denom) * sum_{k=1}^{m} coeff_k * (sample[i+k] - sample[i-k]) / (timestamp[i+k] - timestamp[i-k])` where `m = (N-1)/2` and the coefficients and denominators are exactly as derived from the central difference formulas: for N=5 coefficients are [2*2, 4*1] with denom 8; for N=7 coefficients [2*5, 4*4, 6*1] with denom 32; for N=9 coefficients [2*14, 4*14, 6*6, 8*1] with denom 128; for N=11 coefficients [2*42, 4*48, 6*27, 8*8, 10*1] with denom 512. Use the largest N such that at least N/2 points exist on both sides of the current index; if even N=5 is not possible, output 0.0f. The function must be robust to numerical issues: if any computed slope is NaN or infinite, replace it with 0.0f. The vectors must not be modified.

// The solution constructs a centered finite-difference estimator for the first derivative at each valid interior point. For each index `i` in the range `[0, n)`, we first filter out timestamps that are not strictly increasing by checking if `timestamps[i] == timestamps[i-1]`; if so, we skip the current point (no derivative computed) and set output to 0.0f. Otherwise, we determine the maximum feasible window size `N` from the set {11, 9, 7, 5} such that `i - (N-1)/2 >= 0` and `i + (N-1)/2 < n`. This ensures symmetric access to `N` points total (the center plus `m` on each side). If no such `N` exists (i.e., fewer than 5 points on either side), output 0.0f. For the chosen `N`, we apply the specific coefficients: for `N=5`, sum `2*2*(f[1]-f[-1])/(t[1]-t[-1]) + 4*1*(f[2]-f[-2])/(t[2]-t[-2])` and divide by 8; similarly for larger N. The divisions use the timestamp differences, which are guaranteed nonzero because we filtered duplicate timestamps (but we still guard against division by zero by checking if the denominator is zero and then treating the term as zero). After summing, we divide by the denominator (8, 32, 128, 512) and check for NaN/infinity (using `std::isnan` and `std::isinf`), replacing with 0.0f. This approach uses a sliding window of fixed maximum size 11, so time complexity is O(n) overall, since each index is processed in constant time (bounded by at most 5 terms). Space complexity is O(1) extra beyond the input and output vectors. The main edge cases are: empty input (return empty vector), length less than 5 (all zeros), constant timestamps (skip), and non-finite results. The coefficients are derived from the paper cited in the original snippet, ensuring the filter smooths noise while preserving the derivative.

#include <vector>
#include <cstdint>
#include <cmath>

/**
 * Compute the smoothed derivative (slope) at each time index using a centered
 * finite-difference filter of adaptive order. The filter size is chosen from
 * {5,7,9,11} based on available symmetric samples. Points with insufficient
 * context or non-increasing timestamps yield 0.0f.
 *
 * @param samples       Time-ordered sample values.
 * @param timestamps    Corresponding timestamps (milliseconds). Must be non-decreasing after
 *                      duplicate removal; duplicate timestamps are ignored.
 * @return              Vector of slope estimates, same length as input.
 */
std::vector<float> smoothDerivative(const std::vector<float>& samples,
                                    const std::vector<uint32_t>& timestamps) {
    const size_t n = samples.size();
    std::vector<float> result(n, 0.0f);
    if (n < 5) {
        return result;
    }

    // Predefined coefficient tables for N = 5, 7, 9, 11.
    // For each N, coeff[k] corresponds to the term for sample[i+k] - sample[i-k].
    const int Ns[] = {11, 9, 7, 5};
    const int denoms[] = {512, 128, 32, 8};
    // For N=11: coefficients [2*42=84, 4*48=192, 6*27=162, 8*8=64, 10*1=10]
    // For N=9:  [2*14=28, 4*14=56, 6*6=36, 8*1=8]
    // For N=7:  [2*5=10, 4*4=16, 6*1=6]
    // For N=5:  [2*2=4, 4*1=4]
    const float coeffs[4][5] = {
        {84.0f, 192.0f, 162.0f, 64.0f, 10.0f}, // N=11
        {28.0f, 56.0f, 36.0f, 8.0f, 0.0f},     // N=9 (unused entries 0)
        {10.0f, 16.0f, 6.0f, 0.0f, 0.0f},      // N=7
        {4.0f, 4.0f, 0.0f, 0.0f, 0.0f}         // N=5
    };

    for (size_t i = 0; i < n; ++i) {
        // Skip if this timestamp duplicates the previous one (no new time step)
        if (i > 0 && timestamps[i] == timestamps[i-1]) {
            result[i] = 0.0f;
            continue;
        }

        // Find the largest window size that fits symmetrically
        int chosen = -1;
        int m = 0;
        for (int idx = 0; idx < 4; ++idx) {
            int N = Ns[idx];
            int half = (N - 1) / 2;
            if (i >= static_cast<size_t>(half) && i + half < n) {
                chosen = idx;
                m = half;
                break;
            }
        }
        if (chosen == -1) {
            result[i] = 0.0f;
            continue;
        }

        float sum = 0.0f;
        bool valid = true;
        for (int k = 1; k <= m; ++k) {
            float dt = static_cast<float>(timestamps[i + k]) - static_cast<float>(timestamps[i - k]);
            if (dt == 0.0f) {
                valid = false;
                break;
            }
            sum += coeffs[chosen][k-1] * (samples[i + k] - samples[i - k]) / dt;
        }
        if (!valid) {
            result[i] = 0.0f;
            continue;
        }

        float slope = sum / static_cast<float>(denoms[chosen]);
        if (std::isnan(slope) || std::isinf(slope)) {
            result[i] = 0.0f;
        } else {
            result[i] = slope;
        }
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <cstdint>

// Function declaration (as defined in solution)
std::vector<float> smoothDerivative(const std::vector<float>& samples,
                                    const std::vector<uint32_t>& timestamps);

int main() {
    // Test 1: Linear function y = 2t + 1, t = 0..10 (dt=1)
    std::vector<float> s1, r1;
    std::vector<uint32_t> t1;
    for (int i = 0; i <= 10; ++i) {
        s1.push_back(2.0f * i + 1.0f);
        t1.push_back(static_cast<uint32_t>(i));
    }
    r1 = smoothDerivative(s1, t1);
    // Interior points (i=5..10) should be exactly 2.0 (because N=11 fits for i>=5 and i<=5? Actually i=5 with N=11 is not possible; but i=5 has 5 left and 5 right? For N=11 half=5, i=5 has left indices 0..4, right 6..10 => fits. So slope should be 2)
    for (size_t i = 5; i <= 5; ++i) {
        assert(std::fabs(r1[i] - 2.0f) < 1e-4);
    }
    // Boundary points (i=0..4, i=6..10) that don't have full N=11 but may have N=5, etc. For i=4, N=9? i=4 has left 0..3, right 5..8 => N=9 fits, slope should still be 2.
    for (size_t i = 4; i <= 4; ++i) {
        assert(std::fabs(r1[i] - 2.0f) < 1e-4);
    }

    // Test 2: Constant function => all slopes 0
    std::vector<float> s2(9, 5.0f);
    std::vector<uint32_t> t2;
    for (int i = 0; i < 9; ++i) t2.push_back(i);
    auto r2 = smoothDerivative(s2, t2);
    for (float v : r2) {
        assert(v == 0.0f);
    }

    // Test 3: Duplicate timestamps are skipped
    std::vector<float> s3 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<uint32_t> t3 = {0, 1, 1, 2, 3, 4, 5, 6, 7}; // duplicate at index 2
    auto r3 = smoothDerivative(s3, t3);
    // At index 2 (the second equal timestamp), result must be 0
    assert(r3[2] == 0.0f);
    // At index 3, it should compute using window (since timestamps now increasing)
    // Expect slope 1.0 approximately (since y = x, but with a gap)
    assert(std::fabs(r3[3] - 0.0f) < 1e-4 || std::fabs(r3[3] - 1.0f) < 1e-4); // not too strict

    // Test 4: Empty input
    auto r4 = smoothDerivative({}, {});
    assert(r4.empty());

    // Test 5: Fewer than 5 points => all zeros
    std::vector<float> s5 = {1,2,3};
    std::vector<uint32_t> t5 = {0,1,2};
    auto r5 = smoothDerivative(s5, t5);
    for (float v : r5) assert(v == 0.0f);

    // Test 6: Known non-linear curve: y = t^2, t=0..10. True derivative = 2t.
    std::vector<float> s6;
    std::vector<uint32_t> t6;
    for (int i = 0; i <= 10; ++i) {
        s6.push_back(static_cast<float>(i*i));
        t6.push_back(static_cast<uint32_t>(i));
    }
    auto r6 = smoothDerivative(s6, t6);
    // At i=5, true value = 10.0, but filter may approximate. Check it's positive and near 10 within 10% tolerance.
    assert(r6[5] > 8.0f && r6[5] < 12.0f);

    return 0;
}
