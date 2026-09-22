Write a standalone C++ function `disperse_code` that takes as input: a 16-bit integer `gain_code`, a 16-bit fixed-point gain `gain_pit` (Q14 format), an array `code` of 16-bit signed integers of length 64 (representing an excitation code vector), an integer `mode` (0, 1, or 2), a pointer `disp_mem` to an array of at least 3 16-bit integers (used as static state memory), and an output scratch array `scratch` of at least 128 16-bit integers. The function must implement a simplified phase dispersion post-processing algorithm: based on the past and current pitch and code gains, the function determines a dispersion state (0, 1, or 2), adds the mode offset (clamping if needed), and if the resulting state is 0 or 1, performs a circular convolution between the input code vector and one of two fixed 64-tap impulse response tables (provided within the solution). The output overwrites `code` with the dispersed vector. For state 2 (i.e., when the final state ≥ 2), the code vector remains unchanged. The state memory should store: `prev_state`, `prev_gain_code`, and a history of the last 6 pitch gains (the memory layout is: `disp_mem[0]` = previous state, `disp_mem[1]` = previous gain_code, `disp_mem[2..7]` = previous pitch gain history, with the most recent at index 2). The algorithm to compute the state: start with `state` = 0 if `gain_pit < 14746` (0.6 in Q14), else 1 if `< 9830` (0.9 in Q14), else 2. Shift the pitch history (oldest dropped, newest inserted). If the current code gain is greater than twice the previous code gain (i.e., `gain_code > (prev_gain_code << 1)`), and state is not already 2, increment state (onset). Otherwise, count how many of the last 6 pitch gains (including the current) are less than 14746; if more than 2 are low, set state = 0. Also, if the new state is more than one greater than the previous state (from memory), decrement state. Then update the memory: store `gain_code` as previous code gain, and store `state` as previous state. Finally, add `mode` to state; if state becomes > 2, set it to 2, if it becomes < 0, set it to 0. If the final state is 0, convolve with the low dispersion impulse response (provided below); if 1, convolve with the mid response. The convolution uses 16-bit saturating multiply-add (rounding) and two 64-length buffers within scratch (first 64 for intermediate, next 64 as overflow). The function must be self-contained, using only integer arithmetic and fixed constants, and must not call any dynamic allocation.

#include <cassert>
#include <cstdint>
#include <cstring>

// The solution function is declared here (assume it is above or included)
// For test, include the full implementation in the same file.

int main() {
    // Test 1: Basic state 0 (low pitch), mode 0, should convolve with low impulse
    {
        int16_t code[64];
        for (int i = 0; i < 64; ++i) code[i] = 0;
        code[0] = 1000; // one nonzero tap
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        int16_t expected[64];
        // Precompute expected: code2[i] = code[0]*ph_imp_low[i] (since only code[0] nonzero)
        // Then out[i] = code2[i] + code2[i+64]; but code2 beyond 63 is from j beyond, but here only i=0, so code2[i] = mult(code0, imp[i]), and code2[i+64] = mult(code0, imp[i+64]) which is zero because imp is only 64 long. So expected = mult(1000, imp[i]).
        // We'll compute manually using the same rounding.
        for (int i = 0; i < 64; ++i) expected[i] = (int16_t)((1000 * 20182 + 0x4000) >> 15);
        disperse_code(1000, 10000, code, 0, disp_mem, scratch);
        // Just check first few match exactly (since we computed first element manually)
        assert(code[0] == (int16_t)((1000 * 20182 + 0x4000) >> 15));
        // Check that all other code values are zero (since impulse is non-zero but only for i=0, no wrap)
        for (int i = 1; i < 64; ++i) {
            assert(code[i] == 0);
        }
    }

    // Test 2: State 2 (high pitch, strong), mode 2 (off) => no change
    {
        int16_t code[64];
        for (int i = 0; i < 64; ++i) code[i] = i; // arbitrary
        int16_t original[64];
        std::memcpy(original, code, sizeof(code));
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        disperse_code(5000, 20000, code, 2, disp_mem, scratch);
        assert(std::memcmp(code, original, sizeof(code)) == 0);
    }

    // Test 3: Onset detection increments state
    {
        int16_t code[64] = {0};
        code[0] = 100;
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        // First call: previous gain_code=0, current gain_code=100 > 2*0 => onset, state from gain_pit low (0) but onset increments to 1
        // mode 0 => state=1, so use mid impulse
        disperse_code(100, 5000, code, 0, disp_mem, scratch);
        // After first call, prev_gain_code = 100, prev_state = 1
        assert(disp_mem[1] == 100);
        assert(disp_mem[0] == 1);
        // Second call with same small code gain: no onset, state from pitch low =0, but history has one low (current), count=1 not >2, and state (0) not > prev_state+1 (1+1=2), so state stays 0
        int16_t code2[64] = {0};
        code2[1] = 50;
        disperse_code(50, 4000, code2, 0, disp_mem, scratch);
        assert(disp_mem[0] == 0);
    }

    // Test 4: History low count resets state to 0
    {
        // Pre-fill memory with 3 lows and prev_state=2
        int16_t disp_mem[8] = {2, 0, 8000, 8000, 8000, 20000, 20000, 20000}; // three lows at start
        int16_t code[64] = {0};
        code[0] = 10;
        int16_t scratch[128];
        // current gain_pit = 8000 (low), low_count will be 4 (since three old lows + new) > 2 => state=0
        disperse_code(10, 8000, code, 0, disp_mem, scratch);
        assert(disp_mem[0] == 0); // state reset to 0
    }

    // Test 5: Clamping mode addition beyond max state
    {
        int16_t code[64] = {0};
        code[0] = 1;
        int16_t disp_mem[8] = {2, 100, 20000, 20000, 20000, 20000, 20000, 20000};
        int16_t scratch[128];
        // gain_pit=20000 => state=2, mode=2 => sum=4 => clamp to 2 => no convolution
        disperse_code(1, 20000, code, 2, disp_mem, scratch);
        assert(code[0] == 1); // unchanged
    }

    // Test 6: Convolution with mid response for state 1
    {
        int16_t code[64] = {0};
        code[0] = 500;
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        disperse_code(500, 12000, code, 1, disp_mem, scratch); // gain_pit between 0.6 and 0.9 => state=1, mode=1 => final 2? Actually state=1+mode1=2 => no convolution
        // So set mode=0 to get state 1
        for (int i = 0; i < 64; ++i) code[i] = 0;
        code[0] = 500;
        disp_mem[0] = 0; disp_mem[1] = 0;
        disperse_code(500, 12000, code, 0, disp_mem, scratch);
        int16_t expected_first = (int16_t)((500 * 24098 + 0x4000) >> 15);
        assert(code[0] == expected_first);
    }

    // Test 7: Zero code remains zero
    {
        int16_t code[64] = {0};
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        disperse_code(0, 5000, code, 0, disp_mem, scratch);
        for (int i = 0; i < 64; ++i) assert(code[i] == 0);
    }

    // Test 8: Negative code values (signed) convolution works
    {
        int16_t code[64] = {0};
        code[0] = -1000;
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        disperse_code(-1000, 5000, code, 0, disp_mem, scratch);
        int16_t expected = (int16_t)((-1000 * 20182 + 0x4000) >> 15);
        assert(code[0] == expected);
    }

    // Test 9: Full convolution with multiple non-zero taps (just verify no crash and memory updated)
    {
        int16_t code[64];
        for (int i = 0; i < 64; ++i) code[i] = (i % 3) * 100 - 50;
        int16_t copy[64];
        std::memcpy(copy, code, sizeof(copy));
        int16_t disp_mem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int16_t scratch[128];
        disperse_code(100, 5000, code, 0, disp_mem, scratch);
        // Just ensure at least one value changed, and memory updated
        bool changed = false;
        for (int i = 0; i < 64; ++i) if (code[i] != copy[i]) { changed = true; break; }
        assert(changed);
        assert(disp_mem[0] == 0 || disp_mem[0] == 1); // state either 0 or 1 due to onset? Actually with mode 0, state might be 1 after onset if gain_code large enough
    }

    // Test 10: Memory shift correctness after many calls
    {
        int16_t disp_mem[8] = {0, 0, 1, 2, 3, 4, 5, 6}; // hist: [1,2,3,4,5,6]
        int16_t code[64] = {0};
        int16_t scratch[128];
        disperse_code(1, 20000, code, 0, disp_mem, scratch); // new gain_pit=20000
        // After shift, hist should be [20000,1,2,3,4,5] (old 6 dropped)
        assert(disp_mem[2] == 20000);
        assert(disp_mem[3] == 1);
        assert(disp_mem[4] == 2);
        assert(disp_mem[5] == 3);
        assert(disp_mem[6] == 4);
        assert(disp_mem[7] == 5);
    }

    return 0;
}

#include <cstdint>
#include <cstring>

namespace {
constexpr int16_t pitch_0_9 = 14746;  // 0.9 in Q14
constexpr int16_t pitch_0_6 = 9830;   // 0.6 in Q14
constexpr int L_SUBFR = 64;

// Impulse response for high frequency (2.0-6.4 kHz) dispersion
const int16_t ph_imp_low[L_SUBFR] = {
    20182,  9693,  3270, -3437, 2864, -5240,  1589, -1357,
     600,  3893, -1497,  -698, 1203, -5249,  1199,  5371,
   -1488,  -705, -2887,  1976,  898,   721, -3876,  4227,
   -5112,  6400, -1032, -4725, 4093, -4352,  3205,  2130,
   -1996, -1835,  2648, -1786, -406,   573,  2484, -3608,
    3139, -1363, -2566,  3808, -639, -2051,  -541,  2376,
    3932, -6262,  1432, -3601, 4889,   370,   567, -1163,
   -2854,  1914,    39, -2418, 3454,  2975, -4021,  3431
};

// Impulse response for middle frequency (3.2-6.4 kHz) dispersion
const int16_t ph_imp_mid[L_SUBFR] = {
    24098, 10460, -5263,  -763,  2048,  -927,  1753, -3323,
    2212,   652, -2146,  2487, -3539,  4109, -2107,  -374,
    -626,  4270, -5485,  2235,  1858, -2769,   744,  1140,
    -763, -1615,  4060, -4574,  2982, -1163,   731, -1098,
     803,   167,  -714,   606,  -560,   639,    43, -1766,
    3228, -2782,   665,   763,   233, -2002,  1291,  1871,
   -3470,  1032,  2710, -4040,  3624, -4214,  5292, -4270,
    1563,   108,  -580,  1642, -2458,   957,   544,  2540
};

// Saturating 16-bit addition
inline int16_t add_sat(int16_t a, int16_t b) {
    int32_t sum = static_cast<int32_t>(a) + static_cast<int32_t>(b);
    if (sum > 32767) return 32767;
    if (sum < -32768) return -32768;
    return static_cast<int16_t>(sum);
}

// Saturating 16-bit multiplication with rounding: (a*b + 0x4000) >> 15
inline int16_t mult_sat_round(int16_t a, int16_t b) {
    int32_t prod = static_cast<int32_t>(a) * static_cast<int32_t>(b);
    int32_t rounded = (prod + 0x4000) >> 15;
    if (rounded > 32767) return 32767;
    if (rounded < -32768) return -32768;
    return static_cast<int16_t>(rounded);
}
} // unnamed namespace

// Perform phase dispersion on the code vector.
// gain_code: Q0 code gain, gain_pit: Q14 pitch gain, mode: 0=hi, 1=lo, 2=off
// code: in-place 64-element vector, disp_mem: state (must have 8 elements, but only first 3 used here)
// scratch: at least 128 elements for convolution
void disperse_code(int16_t gain_code, int16_t gain_pit, int16_t code[], int mode,
                   int16_t disp_mem[], int16_t scratch[]) {
    int16_t* prev_state = &disp_mem[0];
    int16_t* prev_gain_code = &disp_mem[1];
    int16_t* prev_gain_hist = &disp_mem[2]; // history[0] = most recent

    std::memset(scratch, 0, 2 * L_SUBFR * sizeof(int16_t));

    // Determine initial state from current pitch gain
    int state;
    if (gain_pit < pitch_0_6) {
        state = 0;
    } else if (gain_pit < pitch_0_9) {
        state = 1;
    } else {
        state = 2;
    }

    // Shift pitch history and insert new value
    for (int i = 5; i > 0; --i) {
        prev_gain_hist[i] = prev_gain_hist[i - 1];
    }
    prev_gain_hist[0] = gain_pit;

    // Onset detection: if current code gain > 2 * previous code gain
    int16_t doubled_prev = static_cast<int16_t>((*prev_gain_code) << 1); // saturation handled by caller? assume small
    if (gain_code > doubled_prev) {
        if (state < 2) {
            ++state;
        }
    } else {
        // Count how many of the last 6 pitch gains are below 0.6
        int low_count = 0;
        for (int i = 0; i < 6; ++i) {
            if (prev_gain_hist[i] < pitch_0_6) {
                ++low_count;
            }
        }
        if (low_count > 2) {
            state = 0;
        }
        // Prevent jumping more than one level above previous state
        if (state > static_cast<int>(*prev_state) + 1) {
            --state;
        }
    }

    // Update memory
    *prev_gain_code = gain_code;
    *prev_state = static_cast<int16_t>(state);

    // Add mode to state and clamp to [0,2]
    state += mode;
    if (state > 2) state = 2;
    if (state < 0) state = 0;

    // If state is 0 or 1, perform circular convolution
    if (state == 0 || state == 1) {
        const int16_t* impulse = (state == 0) ? ph_imp_low : ph_imp_mid;
        for (int i = 0; i < L_SUBFR; ++i) {
            if (code[i] != 0) {
                int16_t code_val = code[i];
                for (int j = 0; j < L_SUBFR; ++j) {
                    int16_t product = mult_sat_round(code_val, impulse[j]);
                    scratch[i + j] = add_sat(scratch[i + j], product);
                }
            }
        }
        // Combine first half and second half
        for (int i = 0; i < L_SUBFR; ++i) {
            code[i] = add_sat(scratch[i], scratch[i + L_SUBFR]);
        }
    }
}

// The main challenge is correctly implementing state determination based on past gain history and the circular convolution with fixed impulse responses. The state machine uses three thresholds: pitch gain below 0.6 sets state=0, between 0.6 and 0.9 sets state=1, above 0.9 sets state=2. The history shift moves all previous pitch gains one slot forward (index 6 dropped, index 2 receives new). The onset detection compares `gain_code` to `2 * prev_gain_code`; since gains are 16-bit, use saturating shift. The fallback logic counts low pitch gains in the 6-element history; if more than two are low, reset state to 0. Additionally, prevent the state from jumping upward more than one step from the previous stored state. After computing state, add mode and clamp to [0,2]. If final state is 0 or 1, perform the convolution: for each nonzero input `code[i]`, multiply by the impulse response coefficients and accumulate into a 128-length scratch buffer (to handle the wrap-around). Then combine the first half and second half (i.e., `out[i] = scratch[i] + scratch[i+64]`, using saturating add). The provided impulse response tables are 64 values each and must be defined as static const arrays. Edge cases: when gain_code is negative (perhaps representing signed values), the convolution still works; but the onset comparison uses `gain_code > (prev_gain_code << 1)` as signed comparison. The scratch array must be zero-initialized before each call. The function should be `void` and modify `code` in place. Time complexity is O(64^2) worst-case = 4096 multiplications, plus O(6) for history, so O(1) effectively. Space complexity is O(1) besides the provided arrays.
