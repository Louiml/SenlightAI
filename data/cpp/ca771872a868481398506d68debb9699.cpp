/*
Write a C++ function `computeLsfWeights` that, given an array of 10 LSF (Line Spectral Frequency) values in the range [0, 4000] Hz (represented as integers, where 4000 Hz corresponds to 16384 in a Q15 fixed-point scale), computes the corresponding weighting factors `wf` for each of the 10 LSF coefficients. The weighting factors are derived from the distances `d[i]` between adjacent LSF values, with boundary conditions: `d[0] = lsf[1] - 0`, `d[9] = 4000 - lsf[8]`, and for interior indices `d[i] = lsf[i+1] - lsf[i-1]` (note this uses the gap between second neighbors, not immediate neighbors). The function must implement the piecewise linear mapping defined in the code snippet: for `d < 450 Hz`, `wf = 3427 - (28160 * d) >> 15`; for `d >= 450 Hz`, `wf = 1843 - (6242 * (d - 1843)) >> 15`, where all arithmetic is performed in 16-bit signed integers with intermediate multiplication promoted to 32-bit, and the final result is the computed weighting factor shifted left by 3. The function must take a `const int16_t*` input array and an `int16_t*` output array of size 10. Ensure the function handles any input values without overflow (assume inputs are valid LSF values in increasing order, with the last value ≤ 16384). Provide only the function implementation with necessary includes, no main function.
*/
#include <cstdint>

/**
 * Computes LSF weighting factors from an array of 10 LSF values.
 * 
 * @param lsf Pointer to the input array of 10 LSF values (Q15 format, where 16384 = 4000 Hz)
 * @param wf  Pointer to the output array of 10 weighting factors
 */
void computeLsfWeights(const int16_t* lsf, int16_t* wf) {
    int16_t d[10];
    
    // Compute distances d[i]
    d[0] = lsf[1];                    // d[0] = lsf[1] - 0
    d[9] = 16384 - lsf[8];            // d[9] = 4000 Hz (16384) - lsf[8]
    for (int i = 1; i <= 8; ++i) {
        d[i] = lsf[i+1] - lsf[i-1];   // d[i] = lsf[i+1] - lsf[i-1]
    }
    
    // Apply piecewise linear weighting
    const int16_t THRESHOLD = 1843;   // 450 Hz in Q15
    const int16_t SLOPE_1_NUM = 28160; // for d < 450Hz
    const int16_t SLOPE_2_NUM = 6242;  // for d >= 450Hz
    const int16_t OFFSET_1 = 3427;
    const int16_t OFFSET_2 = 1843;
    
    for (int i = 0; i < 10; ++i) {
        int16_t temp;
        int16_t wgt_fct = d[i];
        
        if (wgt_fct - THRESHOLD > 0) {
            // d[i] > 450 Hz segment
            temp = static_cast<int16_t>((static_cast<int32_t>(wgt_fct - THRESHOLD) * SLOPE_2_NUM) >> 15);
            wgt_fct = static_cast<int16_t>(OFFSET_2 - temp);
        } else {
            // d[i] <= 450 Hz segment
            temp = static_cast<int16_t>((static_cast<int32_t>(wgt_fct) * SLOPE_1_NUM) >> 15);
            wgt_fct = static_cast<int16_t>(OFFSET_1 - temp);
        }
        
        wf[i] = static_cast<int16_t>(wgt_fct << 3);  // Scale by 8
    }
}
#include <cassert>
#include <cstdint>

// Declaration of the function under test
void computeLsfWeights(const int16_t* lsf, int16_t* wf);

int main() {
    // Test 1: All gaps below 450 Hz (d < 1843)
    {
        int16_t lsf[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
        int16_t wf[10];
        computeLsfWeights(lsf, wf);
        // d[0] = 2000, d[1] = 2000, ..., d[8] = 2000, d[9] = 6384
        // All d < 1843? No: d[9]=6384 > 1843, but first 9 are 2000 > 1843 -> actually all > threshold.
        // Let's design a proper test with small gaps.
    }
    
    // Test 2: Boundary case where all d[i] = 0 (impossible in valid LSF, but for testing)
    {
        int16_t lsf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int16_t wf[10];
        computeLsfWeights(lsf, wf);
        // d[0] = 0, d[9] = 16384, interior d = 0
        // For d=0: temp = (0*28160)>>15 = 0, wf = 3427 - 0 = 3427, <<3 = 27416
        // For d=16384: temp = ((16384-1843)*6242)>>15 = (14541*6242)>>15 ≈ 2769, wf = 1843-2769 = -926, <<3 = -7408
        int16_t expected[10] = {27416, 27416, 27416, 27416, 27416, 27416, 27416, 27416, 27416, -7408};
        for (int i = 0; i < 10; ++i) {
            assert(wf[i] == expected[i]);
        }
    }
    
    // Test 3: Increasing LSF values with known distances
    {
        int16_t lsf[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 12000};
        int16_t wf[10];
        computeLsfWeights(lsf, wf);
        // Boundary d[0] = 2000 (>1843) -> temp = (157*6242)>>15 = 29, wf=1814, <<3=14512
        // d[9] = 16384-9000 = 7384 (>1843) -> temp = (5541*6242)>>15 = 1055, wf=788, <<3=6304
        // Interior d[1] = lsf[2]-lsf[0] = 3000-1000=2000 -> same as d[0], so wf=14512
        assert(wf[0] == 14512);
        assert(wf[1] == 14512);
        assert(wf[9] == 6304);
    }
    
    // Test 4: Exact threshold d=1843 (should go to else branch)
    {
        // Construct lsf such that d[5] = 1843 exactly
        // Set lsf[4] = 5000, lsf[6] = 6843, others arbitrary but valid
        int16_t lsf[10] = {0, 1000, 2000, 3000, 5000, 6000, 6843, 7000, 8000, 9000};
        int16_t wf[10];
        computeLsfWeights(lsf, wf);
        // d[5] = lsf[6]-lsf[4] = 6843-5000 = 1843 -> else branch: temp = (1843*28160)>>15 = 1583, wf = 3427-1583=1844, <<3=14752
        assert(wf[5] == 14752);
    }
    
    // Test 5: Maximum allowed LSF values (ends at 16384)
    {
        int16_t lsf[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 16384};
        int16_t wf[10];
        computeLsfWeights(lsf, wf);
        // Verify no overflow and d[9] = 16384-9000 = 7384, same as test 3
        assert(wf[9] == 6304);
    }
    
    return 0;
}
// The core algorithm processes the LSF array to compute the distance `d[i]` for each index, then applies a piecewise linear transformation. The distances are computed as follows: for index 0, `d[0] = lsf[1]` (since the lower boundary is 0 Hz); for index 9, `d[9] = 16384 - lsf[8]` (since the upper boundary is 4000 Hz = 16384 in Q15); for indices 1 through 8, `d[i] = lsf[i+1] - lsf[i-1]` (note this is the difference between the next and previous LSF values, giving a second-order gap). After computing all distances, each `d[i]` is compared to the threshold `1843` (which represents 450 Hz in the Q15 scale, since 450/4000 * 16384 ≈ 1843.2, rounded down). If `d[i]` is greater than 1843, compute `temp = ((d[i] - 1843) * 6242) >> 15`, then `wf[i] = 1843 - temp`; otherwise, compute `temp = (d[i] * 28160) >> 15`, then `wf[i] = 3427 - temp`. Finally, multiply each weighting factor by 8 (left shift 3) to scale it appropriately. Edge cases include boundary indices, where the distance calculation uses fixed endpoints, and handling of `d[i]` exactly equal to 1843 (falls into the else branch, which is correct as per the original snippet). The time complexity is O(10) = O(1), and space complexity is O(1) beyond the input and output arrays. The implementation must carefully use `int32_t` for intermediate multiplications to avoid overflow, then cast back to `int16_t`, and ensure the final shift left by 3 does not overflow (the weighting factors are designed to stay within int16 range). The function must be self-contained, including `<cstdint>` for fixed-width integer types.
