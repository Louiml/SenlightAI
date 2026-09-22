// Write a C++ function named `applyStereoMatrix` that takes two arrays of complex numbers (representing left and right stereo sub-band signals), a collection of 22 mixing coefficient groups, and a maximum usable sub-band count. For each of the first 10 coefficient groups, apply the 2×2 matrix `[H11 H21; H12 H22]` to the corresponding pair of complex input samples (left and right) at a single fixed index per group, updating them in place. For groups 11 through 21, apply the same matrix operation to all complex samples from each group's starting index up to (but not including) either the next group's starting index or `maxSubband`, whichever is smaller. Each coefficient group provides four 16-bit signed integer coefficients stored in 32-bit containers; the effective coefficient is the top 16 bits (i.e., `coeff >> 16`). The matrix multiply for each sample is: `newLeft = 2 * (H11 * left + H21 * right)` and `newRight = 2 * (H12 * left + H22 * right)`, where `left` and `right` are the original values at that sample. All arithmetic is integer, and the results replace the original samples. The function must not modify the coefficient storage, must not assume any particular array length beyond what the indices imply, and must safely handle `maxSubband` being less than a group's end boundary. Use `std::complex<int32_t>` for the sample type and `std::vector<std::vector<int32_t>>` for the coefficients, with the coefficient vectors each having exactly 22 entries.
// The algorithm processes 22 coefficient groups in three phases to match the original snippet's structure. Phase 1 (groups 0–9) applies the matrix to exactly one complex sample per group, at a pre-determined index stored in a `groupBorders` vector. Phase 2 (group 10) processes exactly one sample at index 3 (simulating the special hybrid sub-band handling). Phase 3 (groups 11–21) iterates over a range of samples from `groupBorders[group]` to `min(groupBorders[group+1], maxSubband)`. For each sample, the original left and right complex values are read once, then the new left and right values are computed using integer multiplication and addition. The key details: coefficients are extracted as `coeff >> 16` to get the 16-bit signed value; the matrix multiply formula doubles the inputs before multiplication and doubles the result after addition, matching the fixed-point scaling in the original code. Edge cases: if `maxSubband` is 0 or smaller than the group's start, no samples are processed for that group; groups 0–10 always process exactly one sample regardless of `maxSubband` (matching the original behavior), but the caller must ensure `maxSubband` is at least the index processed by group 10 (which is 3). The function only reads coefficients, so it takes them by const reference; it modifies the sample arrays in place. Time complexity is O(maxSubband) because each sample from index 0 up to `maxSubband` is processed at most once (group 10 processes index 3, and later groups cover indices from 4 upward). Space complexity is O(1) beyond the input arrays.
#include <complex>
#include <cstdint>
#include <vector>
#include <algorithm>

/**
 * Apply a 2x2 stereo mixing matrix to complex sub-band samples.
 *
 * @param leftSamples  Complex left-channel samples (modified in place).
 * @param rightSamples Complex right-channel samples (modified in place).
 * @param coefficients 22 groups of four 32-bit values; the effective 16-bit
 *                     coefficient is stored in the top 16 bits of each value.
 * @param groupBorders Starting index for each of the 22 groups; must have
 *                     at least 23 entries (last entry marks the end of group 21).
 * @param maxSubband   Maximum usable sub-band index (exclusive upper bound for
 *                     groups 11..21). Must be at least 4 for correct behavior.
 */
void applyStereoMatrix(
    std::vector<std::complex<int32_t>>& leftSamples,
    std::vector<std::complex<int32_t>>& rightSamples,
    const std::vector<std::vector<int32_t>>& coefficients,
    const std::vector<int>& groupBorders,
    int maxSubband)
{
    static constexpr int NUM_GROUPS = 22;
    static constexpr int FIRST_SINGLE_GROUPS = 10; // groups 0..9
    static constexpr int SECOND_SINGLE_GROUP = 10; // group index 10

    // Helper lambda to extract the 16-bit coefficient from a 32-bit value.
    auto getCoeff = [](int32_t raw) -> int16_t {
        return static_cast<int16_t>(raw >> 16);
    };

    // Process groups 0..9: one sample per group at groupBorders[group].
    for (int group = 0; group < FIRST_SINGLE_GROUPS; ++group) {
        int index = groupBorders[group];

        int16_t h11 = getCoeff(coefficients[group][0]);
        int16_t h12 = getCoeff(coefficients[group][1]);
        int16_t h21 = getCoeff(coefficients[group][2]);
        int16_t h22 = getCoeff(coefficients[group][3]);

        std::complex<int32_t> left = leftSamples[index];
        std::complex<int32_t> right = rightSamples[index];

        int32_t l_real = left.real() << 1;
        int32_t l_imag = left.imag() << 1;
        int32_t r_real = right.real() << 1;
        int32_t r_imag = right.imag() << 1;

        // newLeft = 2 * (H11 * left + H21 * right)
        leftSamples[index] = std::complex<int32_t>(
            ((h11 * l_real) + (h21 * r_real)) << 1,
            ((h11 * l_imag) + (h21 * r_imag)) << 1
        );

        // newRight = 2 * (H12 * left + H22 * right)
        rightSamples[index] = std::complex<int32_t>(
            ((h12 * l_real) + (h22 * r_real)) << 1,
            ((h12 * l_imag) + (h22 * r_imag)) << 1
        );
    }

    // Process group 10 (special fixed index 3).
    {
        int index = 3;

        int16_t h11 = getCoeff(coefficients[SECOND_SINGLE_GROUP][0]);
        int16_t h12 = getCoeff(coefficients[SECOND_SINGLE_GROUP][1]);
        int16_t h21 = getCoeff(coefficients[SECOND_SINGLE_GROUP][2]);
        int16_t h22 = getCoeff(coefficients[SECOND_SINGLE_GROUP][3]);

        std::complex<int32_t> left = leftSamples[index];
        std::complex<int32_t> right = rightSamples[index];

        int32_t l_real = left.real() << 1;
        int32_t l_imag = left.imag() << 1;
        int32_t r_real = right.real() << 1;
        int32_t r_imag = right.imag() << 1;

        leftSamples[index] = std::complex<int32_t>(
            ((h11 * l_real) + (h21 * r_real)) << 1,
            ((h11 * l_imag) + (h21 * r_imag)) << 1
        );

        rightSamples[index] = std::complex<int32_t>(
            ((h12 * l_real) + (h22 * r_real)) << 1,
            ((h12 * l_imag) + (h22 * r_imag)) << 1
        );
    }

    // Process groups 11..21: range from groupBorders[group] to
    // min(groupBorders[group+1], maxSubband).
    for (int group = SECOND_SINGLE_GROUP + 1; group < NUM_GROUPS; ++group) {
        int start = groupBorders[group];
        int end = std::min(groupBorders[group + 1], maxSubband);

        int16_t h11 = getCoeff(coefficients[group][0]);
        int16_t h12 = getCoeff(coefficients[group][1]);
        int16_t h21 = getCoeff(coefficients[group][2]);
        int16_t h22 = getCoeff(coefficients[group][3]);

        for (int idx = start; idx < end; ++idx) {
            std::complex<int32_t> left = leftSamples[idx];
            std::complex<int32_t> right = rightSamples[idx];

            int32_t l_real = left.real() << 1;
            int32_t l_imag = left.imag() << 1;
            int32_t r_real = right.real() << 1;
            int32_t r_imag = right.imag() << 1;

            leftSamples[idx] = std::complex<int32_t>(
                ((h11 * l_real) + (h21 * r_real)) << 1,
                ((h11 * l_imag) + (h21 * r_imag)) << 1
            );

            rightSamples[idx] = std::complex<int32_t>(
                ((h12 * l_real) + (h22 * r_real)) << 1,
                ((h12 * l_imag) + (h22 * r_imag)) << 1
            );
        }
    }
}
#include <cassert>
#include <complex>
#include <cstdint>
#include <vector>

// The solution function is assumed to be defined above.
// Test helper: compare two complex vectors for equality.
bool complexVectorsEqual(const std::vector<std::complex<int32_t>>& a,
                         const std::vector<std::complex<int32_t>>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].real() != b[i].real() || a[i].imag() != b[i].imag()) {
            return false;
        }
    }
    return true;
}

int main() {
    // Build coefficients: 22 groups, each with 4 values.
    // For simplicity, use a pattern where h11=1, h12=2, h21=3, h22=4 for all groups.
    // Stored as value << 16 to put the coefficient in the top 16 bits.
    std::vector<std::vector<int32_t>> coeffs(22, std::vector<int32_t>(4));
    for (auto& group : coeffs) {
        group[0] = 1 << 16;  // h11
        group[1] = 2 << 16;  // h12
        group[2] = 3 << 16;  // h21
        group[3] = 4 << 16;  // h22
    }

    // Define group borders for 22 groups (23 entries).
    // Groups 0..9 each have one sample at indices 0..9.
    // Group 10 has one sample at index 3 (overlaps with group 3).
    // Groups 11..21 cover ranges as listed.
    std::vector<int> borders = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  // group 0..9
        3,                               // group 10 (fixed index 3)
        4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20,  // groups 11..21 starts
        24                               // end marker for group 21
    };

    // Test case 1: Basic operation with a few samples.
    {
        std::vector<std::complex<int32_t>> left = {
            {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
            {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}
        };
        std::vector<std::complex<int32_t>> right = {
            {10, 0}, {20, 0}, {30, 0}, {40, 0}, {50, 0},
            {60, 0}, {70, 0}, {80, 0}, {90, 0}, {100, 0}
        };

        // Save copies for manual verification.
        auto left_orig = left;
        auto right_orig = right;

        applyStereoMatrix(left, right, coeffs, borders, 10);

        // For a sample with coefficients (1,2,3,4) and original (l, r):
        // newLeft = 2 * (1*l + 3*r) = 2l + 6r
        // newRight = 2 * (2*l + 4*r) = 4l + 8r
        for (int i = 0; i < 10; ++i) {
            int32_t l = left_orig[i].real();
            int32_t r = right_orig[i].real();
            assert(left[i].real() == 2*l + 6*r);
            assert(right[i].real() == 4*l + 8*r);
        }
    }

    // Test case 2: maxSubband limits the range for groups 11+.
    {
        std::vector<std::complex<int32_t>> left(20, {1, 1});
        std::vector<std::complex<int32_t>> right(20, {2, 2});
        auto left_before = left;
        auto right_before = right;

        // Set maxSubband = 8, so groups 11+ only process up to index 8.
        applyStereoMatrix(left, right, coeffs, borders, 8);

        // Indices 0..9 are processed by groups 0..9 (one each).
        // Index 3 is processed again by group 10.
        // Groups 11+ process indices 4..7 (since group 11 start=4, group 12 start=5,
        // ... group start values less than 8: only indices 4,5,6,7 in ranges that start below 8).
        // Let's verify a few known indices.
        // Index 0: only group 0 (no overlap from groups 11+ because start=4 > 0).
        int32_t l0 = left_before[0].real();
        int32_t r0 = right_before[0].real();
        assert(left[0].real() == 2*l0 + 6*r0);
        assert(right[0].real() == 4*l0 + 8*r0);

        // Index 3: processed by group 3 and group 10. Both use same coeffs (1,2,3,4).
        // After first pass (group 3): L1 = 2l + 6r, R1 = 4l + 8r
        // After second pass (group 10): L2 = 2*L1 + 6*R1 = 2(2l+6r) + 6(4l+8r) = 4l+12r+24l+48r = 28l+60r
        //                               R2 = 4*L1 + 8*R1 = 4(2l+6r) + 8(4l+8r) = 8l+24r+32l+64r = 40l+88r
        int32_t l3 = left_before[3].real();
        int32_t r3 = right_before[3].real();
        int32_t expected_l3 = 28*l3 + 60*r3;
        int32_t expected_r3 = 40*l3 + 88*r3;
        // Note: left[3] and right[3] also had imaginary parts; check both.
        assert(left[3].real() == expected_l3);
        assert(right[3].real() == expected_r3);
        assert(left[3].imag() == expected_l3); // since both parts equal initially
        assert(right[3].imag() == expected_r3);

        // Index 8: processed by group 8 only (group start=8, group 9 start=9, group 11 start=4 but end at min(5,8)=5, so not index 8).
        // So Index 8 gets only one pass.
        int32_t l8 = left_before[8].real();
        int32_t r8 = right_before[8].real();
        assert(left[8].real() == 2*l8 + 6*r8);
        assert(right[8].real() == 4*l8 + 8*r8);

        // Index 10: not processed at all (groups 11+ start at 4 and end at min(start+1,8), so no index 10).
        assert(left[10] == left_before[10]);
        assert(right[10] == right_before[10]);
    }

    // Test case 3: Zero coefficients produce zero output.
    {
        std::vector<std::vector<int32_t>> zero_coeffs(22, std::vector<int32_t>(4, 0));
        std::vector<std::complex<int32_t>> left = {{5, -5}, {3, 0}, {0, 0}, {1, 1}, {2, 2}};
        std::vector<std::complex<int32_t>> right = {{-1, 2}, {4, 4}, {9, 9}, {0, 0}, {3, -3}};
        applyStereoMatrix(left, right, zero_coeffs, borders, 10);
        for (size_t i = 0; i < left.size(); ++i) {
            assert(left[i] == std::complex<int32_t>(0, 0));
            assert(right[i] == std::complex<int32_t>(0, 0));
        }
    }

    // Test case 4: maxSubband = 0 means groups 11+ process nothing, but groups 0..9 and 10 still process.
    {
        std::vector<std::complex<int32_t>> left = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
        std::vector<std::complex<int32_t>> right = {{9, 10}, {11, 12}, {13, 14}, {15, 16}};
        auto left_orig = left;
        auto right_orig = right;

        applyStereoMatrix(left, right, coeffs, borders, 0);

        // Groups 0..3 process indices 0,1,2,3; group 4+ process none because maxSubband=0.
        // Group 10 processes index 3 again.
        for (int i = 0; i <= 3; ++i) {
            int32_t l = left_orig[i].real();
            int32_t r = right_orig[i].real();
            // For i=0,1,2: one pass. For i=3: two passes (group 3 and group 10).
            if (i == 3) {
                int32_t expected_l = 28*l + 60*r;
                int32_t expected_r = 40*l + 88*r;
                assert(left[i].real() == expected_l);
                assert(right[i].real() == expected_r);
            } else {
                assert(left[i].real() == 2*l + 6*r);
                assert(right[i].real() == 4*l + 8*r);
            }
        }
        // Indices 4 and above untouched.
        assert(left[4] == left_orig[4]);
        assert(right[4] == right_orig[4]);
    }

    return 0;
}
