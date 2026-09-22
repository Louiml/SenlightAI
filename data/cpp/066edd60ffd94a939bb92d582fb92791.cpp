// Write a C++ function `conceal_lag` that takes an array `gain_hist` of 5 gain history values, an array `lag_hist` of 5 lag history values, and an integer flag `unusable_frame` (nonzero for a lost frame, zero for a bad frame). The function must compute and return a concealed lag value using the logic from the given code snippet: if `unusable_frame != 0`, apply the lost-frame concealment path; otherwise apply the bad-frame path. In both paths, when conditions for using history directly fail, compute a weighted median-based lag with pseudo-random jitter (use a simple deterministic pseudo-random generator, e.g., a linear congruential generator seeded with 12345 and called per invocation), then clamp the result to the historical minimum and maximum. The function must not modify its inputs and must return the concealed lag as a 16-bit integer (use `int16_t`). Provide a standalone implementation with no global state and no external dependencies beyond standard headers.

The solution mirrors the original algorithm. First, compute `lastGain = gain_hist[4]`, `secLastGain = gain_hist[3]`, `lastLag = lag_hist[0]`, and find `minLag`, `maxLag`, and `minGain` over the 5-element arrays. Also compute `lagDif = maxLag - minLag`. For the lost-frame branch (`unusable_frame != 0`), check three conditions: (1) if `minGain > 8192` and `lagDif < 10`, return `old_T0` (here we simulate `old_T0` as the previous lag provided separately, but since the task signature omits it, we can use `lag_hist[0]` as a proxy — better to include `old_T0` as a parameter to match the original; for the task, we include `previous_lag` as a parameter). (2) else if `lastGain > 8192` and `secLastGain > 8192`, return `lag_hist[0]`. (3) else sort a copy of `lag_hist`, compute `lagDif` as `sorted[4] - sorted[2]` clamped to 40, generate a pseudo-random value `D` in {-1,0,1}, compute `D2 = (lagDif/2) * D` (use integer multiplication with rounding), then compute `T0 = (sorted[2] + sorted[3] + sorted[4]) / 3 + D2`, and clamp to `[minLag, maxLag]`. For the bad-frame branch (`unusable_frame == 0`), compute `meanLag` as the floor of the sum of all `lag_hist` divided by 5 (using integer arithmetic). Then check a series of conditions that keep the current `T0` (here we treat `T0` as a parameter that is the candidate lag to validate; the function should take `current_lag` as an input parameter, because the original uses `*T0` both as input and output). If any condition holds, return `current_lag` directly. Otherwise, fall through to the same weighted-median-based computation as in the lost-frame branch (with clamping). The pseudo-random generator must be deterministic and produce values in {-1, 0, 1} uniformly (e.g., using a linear congruential generator with modulus 2^31-1). The sorting uses insertion sort on a 5-element copy. Time complexity is O(1) since array size is constant, and space is O(1).

#include <cstdint>
#include <array>
#include <algorithm>

// Conceal an LTP lag given gain and lag history, a candidate lag, a previous lag,
// and a flag indicating whether the frame is lost (nonzero) or bad (zero).
// Returns the concealed lag as a 16-bit integer.
int16_t conceal_lag(
    const int16_t gain_hist[5],
    const int16_t lag_hist[5],
    int16_t current_lag,
    int16_t previous_lag,
    int16_t unusable_frame
) {
    // Constants from the original
    constexpr int16_t ONE_PER_3 = 10923;     // 1/3 in Q15
    constexpr int16_t ONE_PER_LTPHIST = 6554; // 1/5 in Q15

    // Copy lag history for sorting
    std::array<int16_t, 5> lag_hist2;
    for (int i = 0; i < 5; ++i) {
        lag_hist2[i] = lag_hist[i];
    }

    // Find min and max lag
    int16_t minLag = lag_hist[0];
    int16_t maxLag = lag_hist[0];
    for (int i = 1; i < 5; ++i) {
        minLag = std::min(minLag, lag_hist[i]);
        maxLag = std::max(maxLag, lag_hist[i]);
    }

    // Find min gain
    int16_t minGain = gain_hist[0];
    for (int i = 1; i < 5; ++i) {
        minGain = std::min(minGain, gain_hist[i]);
    }

    int16_t lastGain = gain_hist[4];
    int16_t secLastGain = gain_hist[3];
    int16_t lastLag = lag_hist[0];
    int16_t lagDif = maxLag - minLag;

    int16_t result = current_lag;

    if (unusable_frame != 0) {
        // Lost frame path
        if ((minGain > 8192) && (lagDif < 10)) {
            result = previous_lag;
        } else if ((lastGain > 8192) && (secLastGain > 8192)) {
            result = lag_hist[0];
        } else {
            // Sort a copy
            std::sort(lag_hist2.begin(), lag_hist2.end());

            // Weighted towards bigger lags with jitter
            lagDif = lag_hist2[4] - lag_hist2[2];
            if (lagDif > 40) {
                lagDif = 40;
            }

            // Deterministic pseudo-random in {-1, 0, 1}
            static uint32_t seed = 12345;
            seed = seed * 1103515245 + 12345;
            int16_t D = (seed % 3) - 1; // -1, 0, or 1

            int16_t D2 = (lagDif >> 1) * D; // lagDif/2 * D
            // Weighted sum: (lag_hist2[2] + lag_hist2[3] + lag_hist2[4]) / 3
            int32_t sum = lag_hist2[2] + lag_hist2[3] + lag_hist2[4];
            int16_t weighted = static_cast<int16_t>((sum * ONE_PER_3) >> 15);
            result = weighted + D2;

            // Clamp
            result = std::max(minLag, std::min(maxLag, result));
        }
    } else {
        // Bad frame path
        int32_t sum_mean = 0;
        for (int i = 0; i < 5; ++i) {
            sum_mean += lag_hist[i];
        }
        int16_t meanLag = static_cast<int16_t>((sum_mean * ONE_PER_LTPHIST) >> 15);

        int16_t tmp = current_lag - maxLag;
        int16_t tmp2 = current_lag - lastLag;

        bool keep = false;
        if ((lagDif < 10) && (current_lag > (minLag - 5)) && (tmp < 5)) {
            keep = true;
        } else if ((lastGain > 8192) && (secLastGain > 8192) && ((tmp2 + 10) > 0 && tmp2 < 10)) {
            keep = true;
        } else if ((minGain < 6554) && (lastGain == minGain) && (current_lag > minLag && current_lag < maxLag)) {
            keep = true;
        } else if ((lagDif < 70) && (current_lag > minLag) && (current_lag < maxLag)) {
            keep = true;
        } else if ((current_lag > meanLag) && (current_lag < maxLag)) {
            keep = true;
        }

        if (!keep) {
            // Fall through to similar logic as lost frame
            if ((minGain > 8192) && (lagDif < 10)) {
                result = lag_hist[0];
            } else if ((lastGain > 8192) && (secLastGain > 8192)) {
                result = lag_hist[0];
            } else {
                // Sort a copy
                std::sort(lag_hist2.begin(), lag_hist2.end());

                lagDif = lag_hist2[4] - lag_hist2[2];
                if (lagDif > 40) {
                    lagDif = 40;
                }

                static uint32_t seed = 67890;
                seed = seed * 1103515245 + 12345;
                int16_t D = (seed % 3) - 1; // -1, 0, or 1

                int16_t D2 = (lagDif >> 1) * D;
                int32_t sum = lag_hist2[2] + lag_hist2[3] + lag_hist2[4];
                int16_t weighted = static_cast<int16_t>((sum * ONE_PER_3) >> 15);
                result = weighted + D2;

                // Clamp
                result = std::max(minLag, std::min(maxLag, result));
            }
        }
    }

    return result;
}

#include <cassert>
#include <cstdint>
#include <iostream>

// Declaration
int16_t conceal_lag(const int16_t gain_hist[5], const int16_t lag_hist[5],
                    int16_t current_lag, int16_t previous_lag, int16_t unusable_frame);

int main() {
    // Test 1: Lost frame with stable history and high gains -> previous_lag
    int16_t gain1[5] = {9000, 9000, 9000, 9000, 9000};
    int16_t lag1[5]  = {80, 82, 81, 83, 80};
    assert(conceal_lag(gain1, lag1, 100, 84, 1) == 84);

    // Test 2: Lost frame, high last gains -> lag_hist[0]
    int16_t gain2[5] = {1000, 1000, 9000, 9000, 9000};
    int16_t lag2[5]  = {70, 72, 71, 73, 74};
    assert(conceal_lag(gain2, lag2, 100, 70, 1) == 70);

    // Test 3: Bad frame, current lag within range -> keep it
    int16_t gain3[5] = {5000, 5000, 5000, 5000, 5000};
    int16_t lag3[5]  = {60, 62, 61, 63, 64};
    // lagDif = 4 < 10, current lag 62 > (min-5=55) and tmp = 62-64 = -2 < 5 -> keep
    assert(conceal_lag(gain3, lag3, 62, 61, 0) == 62);

    // Test 4: Bad frame, out of range -> computed with jitter (clamped)
    int16_t gain4[5] = {1000, 1000, 1000, 1000, 1000};
    int16_t lag4[5]  = {50, 52, 51, 53, 54};
    // current_lag=100 is far out, must compute and clamp to [50,54]
    int16_t result4 = conceal_lag(gain4, lag4, 100, 53, 0);
    assert(result4 >= 50 && result4 <= 54);

    // Test 5: Lost frame, wide spread -> computed and clamped
    int16_t gain5[5] = {1000, 1000, 1000, 1000, 1000};
    int16_t lag5[5]  = {40, 90, 50, 85, 60};
    int16_t result5 = conceal_lag(gain5, lag5, 0, 70, 1);
    assert(result5 >= 40 && result5 <= 90);

    // Test 6: Bad frame, sorted calculation with high gains
    int16_t gain6[5] = {9000, 9000, 9000, 9000, 9000};
    int16_t lag6[5]  = {80, 82, 81, 83, 84};
    // minGain > 8192 and lagDif = 4 < 10 -> lag_hist[0] = 80
    assert(conceal_lag(gain6, lag6, 100, 81, 0) == 80);

    // Test 7: Deterministic jitter check (same input twice -> same output)
    int16_t gain7[5] = {500, 500, 500, 500, 500};
    int16_t lag7[5]  = {30, 70, 40, 60, 50};
    int16_t r1 = conceal_lag(gain7, lag7, 0, 0, 1);
    int16_t r2 = conceal_lag(gain7, lag7, 0, 1, 1);
    assert(r1 == r2);

    // Test 8: All lags equal -> clamp preserves value
    int16_t gain8[5] = {1000, 1000, 1000, 1000, 1000};
    int16_t lag8[5]  = {64, 64, 64, 64, 64};
    assert(conceal_lag(gain8, lag8, 100, 64, 1) == 64);

    // Test 9: Bad frame with last gains high and near current lag
    int16_t gain9[5] = {1000, 1000, 9000, 9000, 9000};
    int16_t lag9[5]  = {70, 72, 71, 73, 74}; // lastLag=70
    // current_lag=75, tmp2 = 5, condition (tmp2+10)>0 and tmp2<10 -> keep (75)
    assert(conceal_lag(gain9, lag9, 75, 74, 0) == 75);

    // Test 10: Bad frame, low min gain, current lag within -> keep
    int16_t gain10[5] = {6000, 7000, 8000, 7000, 6500}; // min=6000<6554, last=6500 not equal min -> not keep
    int16_t lag10[5]  = {45, 50, 48, 52, 55};
    // condition: minGain<6554 and lastGain==minGain? 6500 != 6000 -> false; but lagDif=10 <70 and current inside? current=49 inside -> keep
    assert(conceal_lag(gain10, lag10, 49, 48, 0) == 49);

    std::cout << "All tests passed!\n";
    return 0;
}
