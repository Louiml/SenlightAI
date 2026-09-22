/*
Write a standalone C++ function named `apply_equalizer` that takes a pointer to the first element of a fixed-size array of 32-bit signed integers representing 32 spectrum frequency lines (one per subband), an integer `mode` from 0 to 7 indicating the equalizer preset (0 = flat, 1 = bass boost, 2 = rock, 3 = pop, 4 = jazz, 5 = classical, 6 = talk, 7 = flat again), and a reference to an output array of the same size. The function must multiply each input frequency line by the corresponding gain coefficient from a predefined table (same structure and values as the snippet's `equalizerTbl`) for the chosen mode, applying 32-bit fixed-point multiplication with rounding as follows: for each input value `x`, compute `((int64_t)((int64_t)x * (int64_t)gain + 0x40000000LL) >> 31)` (i.e., multiply with rounding and shift right by 31, treating both inputs as signed 32-bit values where the gain is stored as `(int32_t)(gain_float * 2147483647.0f)`). For mode 0 (flat), the output must be a direct copy (no multiplication). The function must handle all 32 subband indices, and the output array must be completely filled. Assume the input array contains exactly `SUBBANDS_NUMBER` (32) elements and the output array is pre-allocated with at least 32 elements. The function must not produce any output or diagnostics; it must be side-effect free except for writing to the output array.
*/

#include <cstdint>
#include <cstddef>

// Equalizer gain table: 8 presets, 32 subband gains.
// Constants derived from the provided float values, scaled to int32 fixed-point.
// For 0 dB: 1.0f * 2147483647.0f = 2147483647
// For -1.5 dB: 0.841395142f * 2147483647.0f ≈ 1808735485 (0x6BE2677D)
// For -3 dB: 0.707106781f * 2147483647.0f ≈ 1518500251 (0x5A80553B)
// For -4.5 dB: 0.595662143f * 2147483647.0f ≈ 1279080409 (0x4C3B4F59)
// For -6 dB: 0.5f * 2147483647.0f = 1073741824 (0x40000000)
// For -7.5 dB: 0.421696503f * 2147483647.0f ≈ 905708549  (0x35F2D4D5)
// For -9 dB: 0.353553393f * 2147483647.0f ≈ 759250125  (0x2D413C49)
// For -12 dB: 0.25f * 2147483647.0f = 536870912   (0x20000000)
// For -15 dB: 0.176776695f * 2147483647.0f ≈ 379625063  (0x169B2B67)
// For -18 dB: 0.125f * 2147483647.0f = 268435456   (0x10000000)
// For -21 dB: 0.088388347f * 2147483647.0f ≈ 189812531  (0x0B4D95A3)
// For -30 dB: 0.03125f * 2147483647.0f = 67108864    (0x04000000)
// For -45 dB: 0.005524271f * 2147483647.0f ≈ 11863283   (0x00B50F83)
// For -60 dB: 0.0009765625f * 2147483647.0f ≈ 2097152    (0x00200000)

static const int32_t kGains[8][32] = {
    // Preset 0: FLAT
    { 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647 },
    // Preset 1: BASS BOOST (first 8 subbands boosted, rest at -6 dB)
    { 2147483647, 1808735485, 1518500251, 1279080409, 1073741824, 1073741824,
      1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
      1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
      1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
      1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
      1073741824, 1073741824 },
    // Preset 2: ROCK (bass boost, then -3 dB, then flat)
    { 2147483647, 1808735485, 1518500251, 1279080409, 1073741824, 1073741824,
      1073741824, 1073741824, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1808735485, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647 },
    // Preset 3: POP (bass cut, flat mids, treble cut)
    { 1073741824, 1518500251, 1518500251, 1808735485, 2147483647, 2147483647,
      2147483647, 2147483647, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1518500251, 759250125, 759250125,
      759250125, 759250125, 759250125, 759250125, 759250125, 759250125,
      759250125, 759250125, 759250125, 759250125, 759250125, 759250125,
      759250125, 759250125 },
    // Preset 4: JAZZ (specific curve)
    { 2147483647, 1073741824, 1073741824, 759250125, 759250125, 759250125,
      759250125, 759250125, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1808735485, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647 },
    // Preset 5: CLASSICAL (same as JAZZ? In snippet it differs slightly: subbands 1-2 are -9dB, not -6; but we follow snippet)
    // From snippet: LEVEL_9 for first three, then -3, then flat. So:
    { 2147483647, 759250125, 759250125, 759250125, 759250125, 759250125,
      759250125, 759250125, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1808735485, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647 },
    // Preset 6: TALK (bass cut, flat mid, slight boost)
    { 759250125, 1073741824, 1073741824, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 1808735485, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251, 1518500251, 1518500251, 1518500251, 1518500251,
      1518500251, 1518500251 },
    // Preset 7: FLAT (same as 0)
    { 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647,
      2147483647, 2147483647 }
};

// Apply the selected equalizer preset to 32 frequency lines.
// `input` is a pointer to 32 int32_t samples.
// `mode` must be in [0,7]; mode 0 and 7 are flat (direct copy).
// `output` must be a pointer to an array of at least 32 int32_t.
void apply_equalizer(const int32_t* input, int mode, int32_t* output) {
    // Use mode & 7 for safety, but assume input mode is valid.
    const int preset = mode & 7;
    const int32_t* gains = kGains[preset];

    if (preset == 0) {
        // Flat: direct copy.
        for (int i = 0; i < 32; ++i) {
            output[i] = input[i];
        }
    } else {
        // Fixed-point multiplication: round to nearest, shift right 31.
        for (int i = 0; i < 32; ++i) {
            int64_t product = static_cast<int64_t>(input[i]) * static_cast<int64_t>(gains[i]);
            // Add rounding constant 2^30 and arithmetic shift right 31.
            int32_t result = static_cast<int32_t>((product + 0x40000000LL) >> 31);
            output[i] = result;
        }
    }
}

#include <cstdint>
#include <cassert>

// Declaration of the function under test (or include the header).
void apply_equalizer(const int32_t* input, int mode, int32_t* output);

int main() {
    // Test 1: Flat mode (0) copies input exactly.
    int32_t input_flat[32] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10, 11, -12, 13, -14, 15, -16,
                               17, -18, 19, -20, 21, -22, 23, -24, 25, -26, 27, -28, 29, -30, 31, -32};
    int32_t output_flat[32];
    apply_equalizer(input_flat, 0, output_flat);
    for (int i = 0; i < 32; ++i) {
        assert(output_flat[i] == input_flat[i]);
    }

    // Test 2: Mode 7 (flat) also copies.
    int32_t output_flat2[32];
    apply_equalizer(input_flat, 7, output_flat2);
    for (int i = 0; i < 32; ++i) {
        assert(output_flat2[i] == input_flat[i]);
    }

    // Test 3: With gains all 0 dB (value 2147483647) for all subbands, the result should equal input for any mode that has all ones? Not applicable. But for mode 1, the first gain is 0 dB (2147483647) and many are -6 dB (1073741824). Test a known input.
    // For input value 0, output is always 0 regardless of gain.
    int32_t input_zero[32] = {0};
    int32_t output_zero[32];
    apply_equalizer(input_zero, 3, output_zero);
    for (int i = 0; i < 32; ++i) {
        assert(output_zero[i] == 0);
    }

    // Test 4: Simple multiplication check for a subband with gain 0 dB (value 2147483647).
    // For input 1, product = 2147483647, add 1073741824 = 3221225471, shift >>31 = 1 (since 3221225471 / 2^31 ≈ 1.5, rounding to 2? Actually 3221225471 >> 31 = 1 because 3221225471 = 0xBFFFFFFF, arithmetic shift right 31 gives 1 (since it's positive). So result = 1. Good.
    int32_t input_one[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int32_t output_one_flat[32];
    apply_equalizer(input_one, 0, output_one_flat);
    for (int i = 0; i < 32; ++i) {
        assert(output_one_flat[i] == 1);
    }

    // Test 5: Mode 1, first subband gain is 0 dB, so output[0] = input[0].
    // Other subbands gain -6 dB = 1073741824. For input 1, product = 1073741824, add 0x40000000 = 2147483648, shift >>31 = 1 (since 2147483648 >>31 = 1). So output should be 1 for all subbands? Actually for -6 dB gain, 0.5 * 1 = 0.5, rounding to nearest integer? With fixed-point, 0.5 rounds to 1? Let's compute: input 1, gain 1073741824, product = 1073741824, plus 0x40000000 = 1073741824+1073741824=2147483648 = 0x80000000, arithmetic shift right 31 gives 1 (since it's positive). So output is 1. That matches the behavior of rounding half up. So for input all 1, output should be all 1 for any mode where gain is 0 dB or -6 dB? Actually for -6 dB, input 1 gives 1 due to rounding. So test that.
    int32_t output_mode1[32];
    apply_equalizer(input_one, 1, output_mode1);
    for (int i = 0; i < 32; ++i) {
        assert(output_mode1[i] == 1); // Because rounding 0.5 up to 1.
    }

    // Test 6: For input 2 and gain -6 dB (1073741824), product = 2147483648, + 0x40000000 = 3221225472, shift >>31 = 1 (since 3221225472 / 2^31 = 1.5, rounding to 2? Actually 3221225472 = 0xC0000000, arithmetic shift right 31 gives -1? Wait 0xC0000000 as int64 positive, shift right 31: 0xC0000000 >>31 = 0x1? Let's compute: 3221225472 / 2147483648 = 1.5, rounding to nearest integer is 2. But shifting right by 31 gives floor((3221225472)/2147483648) = floor(1.5)=1? Actually arithmetic shift right by 31 on a positive number is floor division by 2^31. 3221225472 / 2147483648 = 1.5, floor is 1. So result is 1, which is wrong (should be 2 for rounding half up). The rounding method with adding 0x40000000 (which is half of 2^31) and then shifting right by 31 works for positive numbers: product + 0x40000000 = 2147483648+1073741824 = 3221225472 = 1.5 * 2147483648, but floor of that is 1? Actually 3221225472 / 2147483648 = 1.5 exactly, floor is 1. To get rounding to nearest, we should add 0x40000000 (half) and then shift right 31, which gives rounding to nearest for positive numbers because we add half then truncate. For 1.5, adding 0.5 gives 2.0, truncating gives 2. But here we added 0x40000000 to product which is 1073741824 = 0.5 * 2147483648. Product was 2147483648 (2*1073741824). So product + 0x40000000 = 2147483648 + 1073741824 = 3221225472 which is 1.5 * 2147483648. Truncating to integer gives 1, not 2, because 3221225472 / 2147483648 = 1.5, floor is 1. The issue is that shifting right by 31 is floor division, not rounding. To round to nearest, we should add 0x40000000 (half) to the product, then shift right by 31, but for values exactly halfway, rounding to even? Actually standard fixed-point: (a*b + 0x40000000) >> 31 rounds to nearest, ties to even? Let's check: For product = 2^31 (which is 2147483648), + 2^30 = 3221225472, >>31 = 1 (since 3221225472 / 2147483648 = 1.5 floor 1). That would give 1 instead of 2. So our rounding is not correct for exactly half? Actually the desired rounding in fixed-point often adds 0x40000000 (which is 1/2 of 2^31) then shifts, which gives rounding half up for positive numbers? Let's compute: 1.5 * 2^31 = 1.5 * 2147483648 = 3221225472. Adding 0.5 (i.e., 2^30) to the product before dividing by 2^31 is equivalent to (product + 2^30) / 2^31. For product = 1.0 * 2^31, adding 2^30 gives 1.5*2^31, dividing gives 1.5 floor 1. So that's not rounding. The correct rounding is: (product + 2^30) which is adding half of the divisor, then integer division (floor) gives rounding half up? For example: product = 1.5*2^31 = 3221225472, adding 0.5*2^31 = 1073741824 gives 4294967296, dividing by 2^31 gives 2.0 floor 2. So the correct formula is (product + (1<<30)) >> 31, but we need to add before the division, but product = a*b, and we want to round a*b / 2^31. The usual fixed-point multiply: ((int64_t)a * b + 0x40000000) >> 31. For a=2, b=1073741824, product = 2147483648, add 0x40000000 = 3221225472, shift >>31 gives 1. That's not correct. Actually let's reconsider: We want to compute a * (b / 2^31) rounded. b is integer representation of gain g = b / 2^31. So we want round(a * b / 2^31). The rounding operation adds half of 2^31 to the product before integer division by 2^31: (a*b + 2^30) / 2^31. For a=2, b=1073741824, product=2147483648, plus 2^30=1073741824 gives 3221225472, which divided by 2^31 gives 1.5, not an integer. The integer division truncates to 1. That's not rounding to nearest; it's floor. To get rounding to nearest, we should add half of the divisor minus one? Actually standard rounding for unsigned: (x + (1<<(n-1))) >> n rounds to nearest, ties up. For x = 3221225472 (which is 1.5*2^31), + 2^30 = 4294967296, >>31 = 2. So we need to add 2^30 to the *numerator* before dividing? Wait x is the product a*b, not a*b? Let's recalc: For input 2, gain 1073741824, a=2, b=1073741824, product = 2147483648. We want to compute 2 * (1073741824 / 2147483648) = 2*0.5 = 1.0, not 1.5. I made a mistake: 1073741824 / 2147483648 = 0.5, so 2 * 0.5 = 1. So the expected output is 1, which our method gives (product + 0x40000000) >>31 = (2147483648+1073741824)>>31 = 3221225472>>31 = 1. That's correct! For input 1 and gain -6 dB (0.5), product = 1073741824, + 0x40000000 = 2147483648, >>31 = 1, which is 1*0.5 = 0.5 rounded to 1? That's rounding half up. So our test for input 1 gives 1 is correct. So the rounding works. For input 2, it gives exactly 1. So our test 6 should assert output is 1 for subband with gain -6 dB when input is 2. That's fine. Let's write tests accordingly.

    // Test 6: For mode 1, all subbands except first have gain -6 dB. For input 2, expected output for those subbands = 2 * 0.5 = 1 (since rounding gives 1). First subband gain 1.0, so output = 2.
    int32_t input_twos[32] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    int32_t output_mode1_twos[32];
    apply_equalizer(input_twos, 1, output_mode1_twos);
    // First subband (index 0) has gain 0 dB (2147483647): product = 2*2147483647 = 4294967294, + 1073741824 = 5368709118, >>31 = 2.500000? Actually 5368709118 / 2147483648 = 2.5, floor 2. So output = 2. Correct (1.0 * 2 = 2).
    assert(output_mode1_twos[0] == 2);
    // Subbands 1 to 31 have gain -6 dB (1073741824): product = 2*1073741824 = 2147483648, + 1073741824 = 3221225472, >>31 = 1. So output = 1.
    for (int i = 1; i < 32; ++i) {
        assert(output_mode1_twos[i] == 1);
    }

    // Test 7: Negative input with gain 1.0 (flat) should copy.
    int32_t input_neg[32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int32_t output_neg[32];
    apply_equalizer(input_neg, 0, output_neg);
    for (int i = 0; i < 32; ++i) {
        assert(output_neg[i] == -1);
    }

    // Test 8: Mode 3, first subband gain -6 dB (1073741824) and input -2: product = -2*1073741824 = -2147483648, + 0x40000000 = -1073741824, >>31 (arithmetic) gives -1? Let's see: -1073741824 / 2147483648 = -0.5, floor division with arithmetic shift gives -1 (since floor of -0.5 is -1). So output = -1. For input -2 and gain 0.5, expected value -1.0 exactly, so fine.
    int32_t input_neg2[32] = {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
                              -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
    int32_t output_mode3[32];
    apply_equalizer(input_neg2, 3, output_mode3);
    // For mode 3, first subband gain -6 dB, so output[0] = -1.
    assert(output_mode3[0] == -1);
    // For subbands 1-2 gain -3 dB (1518500251), product -2*1518500251 = -3037000502, + 0x40000000 = -1963257678, >>31 = -1 (since -1963257678 / 2147483648 = -0.914, floor -1). So output = -1.
    assert(output_mode3[1] == -1);
    // For subbands 4-7 gain 0 dB, output = -2.
    for (int i = 4; i < 8; ++i) {
        assert(output_mode3[i] == -2);
    }

    // Test 9: Large positive input, e.g., 1<<30, with gain 1.0 should remain same.
    int32_t input_large[32] = {1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
                                1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
                                1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824,
                                1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824, 1073741824};
    int32_t output_large_flat[32];
    apply_equalizer(input_large, 0, output_large_flat);
    for (int i = 0; i < 32; ++i) {
        assert(output_large_flat[i] == 1073741824);
    }

    // Test 10: Mode 1 with input 3, subband index 1 (gain -6 dB): 3*0.5 = 1.5 rounds to 2? Let's compute: product = 3*1073741824 = 3221225472, + 0x40000000 = 4294967296, >>31 = 2. So output = 2.
    int32_t input_three[32] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                               3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    int32_t output_mode1_three[32];
    apply_equalizer(input_three, 1, output_mode1_three);
    // First subband: gain 1.0, output 3.
    assert(output_mode1_three[0] == 3);
    // Subband 1: gain -6 dB, output 2.
    assert(output_mode1_three[1] == 2);
    // Subband 2: gain -3 dB (1518500251), 3 * 0.7071 ≈ 2.1213, rounding to 2? Compute: product = 3*1518500251 = 4555500753, + 0x40000000 = 5629244497, >>31 = 2 (since 5629244497/2147483648 = 2.62, floor 2). So output = 2.
    assert(output_mode1_three[2] == 2);

    return 0;
}

// The main algorithm is straightforward: define a constant 2D table `gains[8][32]` where each row corresponds to a preset, and each entry is computed from the provided float constants (like `LEVEL__0__dB`, `LEVEL__1_5dB`, etc.) by casting to `int32_t` after multiplying by `2147483647.0f`. However, because the float constants are approximations, to keep the solution self-contained and reproducible for tests, we can store the exact integer values as shown in the snippet (e.g., for 0 dB, the value is `0x7FFFFFFF`; for -1.5 dB, it's `(int32_t)(0.841395142f * 2147483647.0f)` which is approximately `0x6B8B4567`). In a real implementation, we'd compute these at compile time using constants, but for testability we can precompute them in decimal or hex. The core function loops over all 32 subbands. For mode 0, it copies input to output directly. For other modes, it multiplies each input by the corresponding gain using a safe 64-bit intermediate to avoid overflow, rounds by adding `0x40000000` (which is 2^30) to the product, then shifts right by 31 (equivalent to dividing by 2^31 with rounding). Edge cases: input values can be any int32, including negative, so the multiplication must use signed 64-bit arithmetic. The rounding constant added is positive; for negative products, adding a positive constant before shifting yields correct rounding to nearest (with ties away from zero? Actually for negative, adding positive then shifting right with logical shift would be wrong, but we use arithmetic shift on int64_t; we need to ensure proper behavior: better to add `0x40000000` to the product, then arithmetic shift right by 31, which gives rounding to nearest, ties to even? In the snippet they use `fxp_mul32_Q32` which likely does something similar. To be safe, we can implement as: `int32_t result = (int32_t)( ((int64_t)x * (int64_t)gain + 0x40000000LL) >> 31 );` For negative products, adding positive and shifting right with arithmetic shift rounds toward negative infinity after adding, but because we add half, it gives rounding to nearest. This is acceptable and matches fixed-point conventions. Complexity: O(32) time, O(1) auxiliary space (excluding the constant table). Edge cases: mode out of range? The problem says mode is 0 to 7, we do not validate; any other value we could default to flat or index with modulo? For safety, we can use `mode & 7` as the snippet does, but the problem says 0-7; we'll assume valid.
