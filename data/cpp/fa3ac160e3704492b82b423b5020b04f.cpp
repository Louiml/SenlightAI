Write a C++ function `lsp_to_lsf` that converts an array of line spectral pairs (LSPs) from the cosine domain (each value between -1 and 1, inclusive) to normalized line spectral frequencies (LSFs) in the range [0, 0.5]. The conversion uses a provided cosine lookup table `table` (256 entries, covering cosine values for angles from 0 to π/2 in steps of 1/512 of a turn) and a corresponding `slope` table (for linear interpolation of the inverse cosine). Given the LSP array, its length `m`, and the tables, the function should fill the output LSF array. For each LSP value, find the largest index `ind` such that `table[ind] <= lsp[i]` (starting search from the end of the table and moving downward), then compute `lsf[i] = (ind << 8) + (( (lsp[i] - table[ind]) * slope[ind] + 2048) >> 12)`. The result must be stored as a 16‑bit integer (assuming values fit within `int16_t`). Handle the edge case where `lsp[i]` equals 1.0 exactly (the maximum) by ensuring the search does not underflow below index 0 and that the computed LSF does not exceed 0.5 (i.e., 16384 in Q15 notation). The function must be `const`-correct and operate on plain arrays of `int16_t`.

The core operation is an inverse cosine lookup with linear interpolation. The `table` contains cosine values for 256 equally spaced angles from 0 to π/2. For a given LSP value `x` (which is cos(θ) for some θ in [0, π/2] after normalization), we first find the largest `ind` such that `table[ind] <= x`. Because `table` is decreasing (cosine decreases with angle), a linear scan from the end (index 255) downward works: while `table[ind] < x`, decrement `ind`. This yields `ind` where `table[ind] <= x < table[ind+1]` (or `ind` remains 0 if `x` is close to 1). Then we interpolate: the difference `x - table[ind]` is divided by the step size `table[ind] - table[ind+1]` to get a fractional offset. The provided `slope[ind]` is precomputed as `(256 * 4096) / (table[ind] - table[ind+1])` (or similar scaling) allowing the fraction to be computed as `( (x - table[ind]) * slope[ind] ) >> 12`. Adding `ind << 8` gives the final LSF in Q0.8 (i.e., LSF = index * 256 + fractional, where 256 corresponds to 1/2 of a turn). The `+ 2048` in the shift by 12 rounds to the nearest integer. Edge cases: if `x` is exactly 1.0, the loop will decrement `ind` down to 0 (since table[255] < 1.0), and the interpolation gives 0, so LSF = 0. If `x` is -1.0, the loop will stop at some `ind` near 127 (depending on table), producing LSF near 16384 (which is 0.5 in Q15). The algorithm runs in O(m * 256) worst-case time because each LSP may scan the entire table from the end, but in practice the scan is short because LSP values are decreasing (typical order) and we scan from the end each time. Space complexity is O(1) auxiliary.

#include <cstdint>
#include <cstddef>

/**
 * @brief Convert line spectral pairs (LSP) to normalized line spectral frequencies (LSF).
 *
 * Given an array of LSP values in the cosine domain ([−1, 1]), compute the corresponding
 * normalized LSF values (range [0, 0.5] in Q15 notation) using a precomputed cosine
 * lookup table and a slope table for linear interpolation.
 *
 * @param lsp   Input array of LSP values (length m), each within [−32768, 32767] (Q15).
 * @param lsf   Output array of LSF values (length m), each within [0, 16384] (Q15).
 * @param m     Number of elements (LPC order).
 * @param table Lookup table of cosine values (256 entries), decreasing from 32767 down to 0.
 * @param slope Slope table for interpolation (256 entries).
 */
void lsp_to_lsf(const int16_t *lsp, int16_t *lsf, int16_t m,
                const int16_t *table, const int16_t *slope)
{
    for (int16_t i = 0; i < m; ++i) {
        // Start at the end of the table (largest index = smallest cosine)
        int16_t ind = 255; // table size - 1 (assuming table length 256)

        // Find the largest index such that table[ind] <= lsp[i]
        // The table is decreasing; scan downward while table[ind] < lsp[i]
        while (ind > 0 && table[ind] < lsp[i]) {
            --ind;
        }

        // Ensure ind is at least 0 (safety, though loop above prevents underflow)
        if (ind < 0) ind = 0;

        // Compute interpolation offset: (lsp[i] - table[ind]) * slope[ind] / 4096
        int32_t diff = (int32_t)lsp[i] - (int32_t)table[ind];
        int32_t prod = diff * (int32_t)slope[ind];
        // Round to nearest (add 0x800 = 2048) then shift right by 12
        int32_t interp = (prod + 0x00000800) >> 12;

        // Combine base index (shifted left by 8) with interpolation
        int32_t result = ((int32_t)ind << 8) + interp;

        // Clamp to valid LSF range [0, 16384] (0.5 in Q15)
        if (result < 0) result = 0;
        if (result > 16384) result = 16384;

        lsf[i] = (int16_t)result;
    }
}

#include <cassert>
#include <cstdint>
#include <cstdio>

// Forward declaration of the function (as defined above)
void lsp_to_lsf(const int16_t *lsp, int16_t *lsf, int16_t m,
                const int16_t *table, const int16_t *slope);

int main() {
    // Build a simple cosine table for angles 0..π/2 in 256 steps.
    // table[i] = cos(i * π / (2 * 255)) * 32767, scaled to int16_t.
    // We'll use a minimal but correct table for testing.
    const int16_t table[256] = {
        32767, 32767, 32766, 32765, 32763, 32761, 32758, 32755,
        32752, 32748, 32744, 32739, 32734, 32728, 32722, 32715,
        32708, 32700, 32692, 32683, 32674, 32664, 32654, 32643,
        32632, 32620, 32608, 32595, 32582, 32568, 32554, 32539,
        32524, 32508, 32492, 32475, 32458, 32440, 32422, 32403,
        32384, 32364, 32344, 32323, 32302, 32280, 32258, 32235,
        32212, 32188, 32164, 32139, 32114, 32088, 32062, 32035,
        32008, 31980, 31952, 31923, 31894, 31864, 31834, 31803,
        31772, 31740, 31708, 31675, 31642, 31608, 31574, 31539,
        31504, 31468, 31432, 31395, 31358, 31320, 31282, 31243,
        31204, 31164, 31124, 31083, 31042, 31000, 30958, 30915,
        30872, 30828, 30784, 30739, 30694, 30648, 30602, 30555,
        30508, 30460, 30412, 30363, 30314, 30264, 30214, 30163,
        30112, 30060, 30008, 29955, 29902, 29848, 29794, 29739,
        29684, 29628, 29572, 29515, 29458, 29400, 29342, 29283,
        29224, 29164, 29104, 29043, 28982, 28920, 28858, 28795,
        28732, 28668, 28604, 28539, 28474, 28408, 28342, 28275,
        28208, 28140, 28072, 28003, 27934, 27864, 27794, 27723,
        27652, 27580, 27508, 27435, 27362, 27288, 27214, 27139,
        27064, 26988, 26912, 26835, 26758, 26680, 26602, 26523,
        26444, 26364, 26284, 26203, 26122, 26040, 25958, 25875,
        25792, 25708, 25624, 25539, 25454, 25368, 25282, 25195,
        25108, 25020, 24932, 24843, 24754, 24664, 24574, 24483,
        24392, 24300, 24208, 24115, 24022, 23928, 23834, 23739,
        23644, 23548, 23452, 23355, 23258, 23160, 23062, 22963,
        22864, 22764, 22664, 22563, 22462, 22360, 22258, 22155,
        22052, 21948, 21844, 21739, 21634, 21528, 21422, 21315,
        21208, 21100, 20992, 20883, 20774, 20664, 20554, 20443,
        20332, 20220, 20108, 19995, 19882, 19768, 19654, 19539,
        19424, 19308, 19192, 19075, 18958, 18840, 18722, 18603,
        18484, 18364, 18244, 18123, 18002, 17880, 17758, 17635,
        17512, 17388, 17264, 17139, 17014, 16888, 16762, 16635,
        0 // placeholder for simplicity – it won't be reached for typical tests
    };
    // Use a constant slope (e.g., 256*4096/(step) ) – for testing with the fake table,
    // just set slope to 1 to make interpolation trivial. For real usage, slope would be precomputed.
    const int16_t slope[256] = {0}; // we'll override below for actual computations

    // For a real test, we need a meaningful slope. Since the table above is a mock,
    // we will test with an explicit simple case: lsp = {32767, 0, -32768}
    // For lsp=32767 (1.0), expected lsf≈0
    // For lsp=0 (0.0), expected lsf≈8192 (0.25)
    // For lsp=-32768 (-1.0), expected lsf≈16384 (0.5)
    // We'll compute slope as 256*4096 / (table[0]-table[1]) etc., but for the test,
    // we can just set slope to a constant that produces correct results based on the table spacing.
    // Since the table above is not accurate, we'll replace it with a correct one in the test below.

    // Build a proper cosine table: for i from 0 to 255, angle = i * PI / (2 * 255)
    // cos(angle) * 32767, rounded.
    for (int i = 0; i < 256; ++i) {
        double angle = i * 3.141592653589793 / (2.0 * 255.0);
        double cos_val = cos(angle);
        table[i] = (int16_t)(cos_val * 32767.0 + 0.5);
    }
    // Compute slope: difference between consecutive table entries is negative.
    // slope[i] = (256 << 12) / (-(table[i+1] - table[i]))? Actually slope is defined as
    // (1 / (table[i] - table[i+1])) * 4096, but the given formula in the task uses
    // (lsp[i] - table[ind]) * slope[ind] >> 12 to get fractional part in units of 1/256.
    // So slope[i] should be (256 * 4096) / (table[i] - table[i+1]).
    for (int i = 0; i < 255; ++i) {
        int32_t diff = (int32_t)table[i] - (int32_t)table[i+1];
        if (diff > 0) {
            slope[i] = (int16_t)((256 * 4096) / diff);
        } else {
            slope[i] = 0;
        }
    }
    slope[255] = slope[254]; // dummy for last

    // Test 1: LSP = [1.0, 0.0, -1.0] (Q15: 32767, 0, -32768)
    int16_t lsp[3] = {32767, 0, -32768};
    int16_t lsf[3];
    lsp_to_lsf(lsp, lsf, 3, table, slope);
    // Expected: lsf[0] ≈ 0, lsf[1] ≈ 8192, lsf[2] ≈ 16384 (with some tolerance)
    assert(lsf[0] < 10);       // close to 0
    assert(lsf[1] > 8000 && lsf[1] < 8400); // ≈8192
    assert(lsf[2] > 16350 && lsf[2] <= 16384); // close to 16384

    // Test 2: LSP = [1.0, 1.0] (duplicate max)
    int16_t lsp2[2] = {32767, 32767};
    int16_t lsf2[2];
    lsp_to_lsf(lsp2, lsf2, 2, table, slope);
    assert(lsf2[0] < 10);
    assert(lsf2[1] < 10);

    // Test 3: LSP = [-1.0, -1.0] (duplicate min)
    int16_t lsp3[2] = {-32768, -32768};
    int16_t lsf3[2];
    lsp_to_lsf(lsp3, lsf3, 2, table, slope);
    assert(lsf3[0] > 16350 && lsf3[0] <= 16384);
    assert(lsf3[1] > 16350 && lsf3[1] <= 16384);

    // Test 4: LSP = [0.5, -0.5] (Q15: 16384, -16384)
    int16_t lsp4[2] = {16384, -16384};
    int16_t lsf4[2];
    lsp_to_lsf(lsp4, lsf4, 2, table, slope);
    // For 0.5, angle = π/3 → lsf ≈ 0.1667 → in Q15: 0.1667*32768 ≈ 5461? Actually normalized LSF is fraction of 0.5, so 0.5 corresponds to 0.25? Wait: LSF range is [0,0.5], and Q15 scaling 0.5 = 32768/2 = 16384. 0.5 LSP means cos(θ)=0.5 → θ=π/3. Normalized LSF = θ/(2π)=1/6≈0.1667. Times 32768 = 5461. So we expect ~5461.
    // Similarly -0.5 → θ=2π/3 → LSF=1/3≈0.3333 *32768≈10922.
    assert(lsf4[0] > 5400 && lsf4[0] < 5520);
    assert(lsf4[1] > 10850 && lsf4[1] < 11000);

    printf("All tests passed.\n");
    return 0;
}
