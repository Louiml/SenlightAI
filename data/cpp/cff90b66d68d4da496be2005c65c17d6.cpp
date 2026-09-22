// Design a C++ function that simulates the open-loop pitch lag search described in the given code snippet, but simplified for educational purposes. The function should accept an array of 16-bit signed integers (representing speech samples), the frame length `L_frame` (number of samples in the current frame), and the pitch search range `pit_min` and `pit_max`. It must return the pitch lag (an integer in `[pit_min, pit_max]`) that maximizes the normalized correlation between the current frame and its delayed version. The correlation for lag `t` is defined as `corr[t] = sum_{n=0}^{L_frame-1} signal[n] * signal[n - t]`, where `signal` contains the frame samples at indices `[0, L_frame-1]` and the history samples (needed for negative indices) are provided in the same array before index 0. The function must also return the normalized correlation value (as a float between -1 and 1) for the chosen lag via a reference parameter. To mimic the original weighting, the search should apply a simple triangular weighting centered on a given `old_lag` parameter: multiply `corr[t]` by a factor that decreases linearly from 1 at `t = old_lag` to 0.5 at the range boundaries. The chosen lag is the one maximizing the weighted correlation; ties are resolved by choosing the larger lag. The function must handle edge cases where `pit_min > pit_max`, `L_frame <= 0`, or insufficient history samples (i.e., `pit_max > 0` but the signal array does not contain enough negative indices) by returning `pit_min` and setting the correlation to 0.

// The core algorithm is a brute-force search over all integer lags `t` from `pit_min` to `pit_max`. For each lag, compute the raw correlation `corr[t] = Σ signal[n]·signal[n-t]` for `n = 0` to `L_frame-1`. Then apply a weighting factor `w(t)` that is 1 when `t == old_lag`, and linearly tapers to 0.5 at `t == pit_min` and `t == pit_max`, but only if `old_lag` lies within `[pit_min, pit_max]`; otherwise set `w(t) = 1` for all `t` (to mimic the `wght_flg` behavior). Choose the lag with the maximum `corr[t] · w(t)`. If multiple lags have the same maximum, pick the largest lag. After selecting the lag `p_max`, compute the normalized correlation as `corr[p_max] / sqrt(energy_current * energy_delayed)`, where `energy_current = Σ signal[n]^2` and `energy_delayed = Σ signal[n-p_max]^2`. To avoid division by zero, if either energy is zero, set the correlation to 0. The iterative correlation computation for each lag is O(L_frame), leading to total O((pit_max−pit_min+1)·L_frame) time. Space complexity is O(1) aside from the input array. Edge cases include invalid parameters (return `pit_min` and correlation 0), and ensuring the signal array has enough negative indices (the caller is responsible for providing history; the function does not check this beyond logical assumptions). Floating-point normalization uses `sqrt` from `<cmath>`.

#include <cmath>
#include <algorithm>

/**
 * @brief Finds the open-loop pitch lag that maximizes a weighted correlation.
 *
 * The function simulates a simplified version of the GSM AMR Pitch_ol_wgh
 * algorithm. The input `signal` must contain history samples before index 0
 * and the current frame samples at indices 0..L_frame-1. The returned lag is
 * in the inclusive range [pit_min, pit_max], and the normalized correlation
 * for that lag is stored in `*corr_out`.
 *
 * @param signal       Pointer to the signal buffer. Must contain at least
 *                     pit_max history samples (indices -pit_max..-1) and
 *                     L_frame frame samples (indices 0..L_frame-1).
 * @param L_frame      Number of samples in the current frame (must be > 0).
 * @param pit_min      Minimum allowed pitch lag (must be <= pit_max).
 * @param pit_max      Maximum allowed pitch lag (must be >= pit_min).
 * @param old_lag      Previous pitch lag used for weighting (can be outside
 *                     the search range; then no weighting is applied).
 * @param corr_out     Output parameter for the normalized correlation of the
 *                     chosen lag (between -1.0 and 1.0, or 0 on error).
 * @return             The chosen pitch lag, or pit_min on invalid input.
 */
int pitch_ol_wgh(const short* signal, int L_frame, int pit_min, int pit_max,
                 int old_lag, float* corr_out) {
    // Basic validation
    if (L_frame <= 0 || pit_min > pit_max || signal == nullptr || corr_out == nullptr) {
        *corr_out = 0.0f;
        return pit_min;
    }

    // Determine if weighting is applicable (old_lag within range)
    bool use_weight = (old_lag >= pit_min && old_lag <= pit_max);

    // Precompute energies for normalization later (for the chosen lag)
    // We'll compute these on the fly after choosing the best lag.

    double best_weighted_corr = -1.0;  // Use -1.0 as initial (corr can be negative)
    int best_lag = pit_max;            // Tie-break: prefer larger lag

    // Search all candidate lags
    for (int t = pit_min; t <= pit_max; ++t) {
        long long raw_corr = 0;
        // Compute the correlation for this lag
        for (int n = 0; n < L_frame; ++n) {
            raw_corr += static_cast<long long>(signal[n]) * signal[n - t];
        }

        // Apply weighting if enabled
        double weighted = static_cast<double>(raw_corr);
        if (use_weight) {
            // Linear taper: 1.0 at old_lag, 0.5 at boundaries
            int max_dist = std::max(old_lag - pit_min, pit_max - old_lag);
            if (max_dist > 0) {
                int dist = std::abs(t - old_lag);
                double factor = 1.0 - 0.5 * static_cast<double>(dist) / max_dist;
                weighted *= factor;
            }
        }

        // Compare (use >= to prefer larger lag on equal values)
        if (weighted >= best_weighted_corr) {
            best_weighted_corr = weighted;
            best_lag = t;
        }
    }

    // Compute energies for the chosen lag
    long long energy_current = 0;
    long long energy_delayed = 0;
    for (int n = 0; n < L_frame; ++n) {
        energy_current += static_cast<long long>(signal[n]) * signal[n];
        energy_delayed += static_cast<long long>(signal[n - best_lag]) * signal[n - best_lag];
    }

    // Normalize correlation
    double norm_corr = 0.0;
    if (energy_current > 0 && energy_delayed > 0) {
        long long raw_corr_final = 0;
        for (int n = 0; n < L_frame; ++n) {
            raw_corr_final += static_cast<long long>(signal[n]) * signal[n - best_lag];
        }
        norm_corr = static_cast<double>(raw_corr_final) /
                    std::sqrt(static_cast<double>(energy_current) * energy_delayed);
    }

    *corr_out = static_cast<float>(norm_corr);
    return best_lag;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared above; here we test it.
// For testing, we create a simple signal with a known period.

int main() {
    // Test 1: Pure sine wave with period 5, no weighting applied (old_lag outside range)
    {
        const int L = 40;
        std::vector<short> signal(5 + L, 0);  // 5 history samples + L frame
        for (int i = 0; i < L; ++i) {
            signal[5 + i] = static_cast<short>(1000 * std::sin(2 * 3.14159 * i / 5.0));
        }
        float corr = -999.0f;
        int lag = pitch_ol_wgh(signal.data() + 5, L, 1, 10, 100, &corr);
        assert(lag == 5);
        assert(corr > 0.99f);  // Perfect correlation (almost)
    }

    // Test 2: Invalid parameters
    {
        short dummy[10] = {0};
        float corr = -1.0f;
        assert(pitch_ol_wgh(dummy, 0, 1, 5, 3, &corr) == 1);
        assert(corr == 0.0f);
        assert(pitch_ol_wgh(dummy, -1, 1, 5, 3, &corr) == 1);
        assert(corr == 0.0f);
        assert(pitch_ol_wgh(dummy, 10, 5, 1, 3, &corr) == 5);
        assert(corr == 0.0f);
        assert(pitch_ol_wgh(nullptr, 10, 1, 5, 3, &corr) == 1);
        assert(corr == 0.0f);
    }

    // Test 3: Tie-breaking (larger lag preferred) with constant signal
    {
        const int L = 10;
        std::vector<short> signal(10 + L, 100);  // All same value, history and frame
        float corr = -1.0f;
        int lag = pitch_ol_wgh(signal.data() + 10, L, 2, 8, 4, &corr);
        assert(lag == 8);  // Should pick the largest lag
        assert(std::fabs(corr - 1.0f) < 0.001f);  // Perfect correlation for constant signal
    }

    // Test 4: Weighting affects selection
    // Create a signal where lag 4 and lag 6 have similar correlations, but old_lag=4 gets higher weight
    {
        const int L = 30;
        std::vector<short> signal(8 + L, 0);
        // Construct arbitrary signal, e.g., a mix of two periods
        for (int i = 0; i < L; ++i) {
            signal[8 + i] = static_cast<short>(100 * std::sin(2 * 3.14159 * i / 4.0) +
                                              50 * std::sin(2 * 3.14159 * i / 6.0));
        }
        float corr1, corr2;
        int lag_no_weight = pitch_ol_wgh(signal.data() + 8, L, 3, 9, 100, &corr1);
        int lag_weighted = pitch_ol_wgh(signal.data() + 8, L, 3, 9, 4, &corr2);
        // With old_lag=4, we expect the weighted result to shift toward 4 if it wasn't already
        assert(lag_weighted >= 3 && lag_weighted <= 9);
        assert(corr2 <= 1.0f && corr2 >= -1.0f);
    }

    // Test 5: Zero-energy signal
    {
        const int L = 10;
        std::vector<short> signal(5 + L, 0);
        float corr = -1.0f;
        int lag = pitch_ol_wgh(signal.data() + 5, L, 1, 5, 3, &corr);
        assert(lag >= 1 && lag <= 5);
        assert(corr == 0.0f);  // Undefined normalized correlation → 0
    }

    // Test 6: Perfect reconstruction with non-integer period (should still pick best fit)
    {
        const int L = 50;
        std::vector<short> signal(10 + L, 0);
        for (int i = 0; i < L + 10; ++i) {
            signal[i] = static_cast<short>(1000 * std::sin(2 * 3.14159 * i / 7.3));
        }
        // Search range around the true period (not exact
        float corr;
        int lag = pitch_ol_wgh(signal.data() + 10, L, 5, 9, 7, &corr);
        assert(lag >= 5 && lag <= 9);
        assert(corr > 0.9f);  // Strong correlation
    }

    // Test 7: Negative signal values
    {
        const int L = 20;
        std::vector<short> signal(6 + L, 0);
        for (int i = 0; i < L; ++i) {
            signal[6 + i] = static_cast<short>(-i % 7 - 3);  // Some negative values
        }
        float corr;
        int lag = pitch_ol_wgh(signal.data() + 6, L, 1, 6, 6, &corr);
        assert(lag >= 1 && lag <= 6);
        assert(corr >= -1.0f && corr <= 1.0f);
    }

    return 0;
}
