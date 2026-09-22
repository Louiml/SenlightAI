// Write a C++ function named `decode_pitch_gain` that takes an integer `index` and an integer `mode` as parameters. The function must return an integer representing the decoded pitch gain in Q14 fixed-point format. The gain is looked up from a constant lookup table `qua_gain_pitch`, which contains 10 pre-defined values (e.g., {0, 130, 260, 390, 520, 650, 780, 910, 1040, 1170}). If `mode` equals a special constant `MR122` (defined as 122), the function must clear the two least significant bits (LSBs) of the looked-up gain value before returning it. For all other modes, the gain is returned unchanged. The function should validate that `index` is within the valid range [0, 9]; if it is out of range, the function should return 0. The implementation must be self-contained, using only standard C++ headers and no external dependencies.

#include <cassert>

// Include the solution function here (or link against it)

int main() {
    // Test normal modes (not MR122) - gain should be unchanged
    assert(decode_pitch_gain(0, 0) == 0);
    assert(decode_pitch_gain(1, 5) == 130);
    assert(decode_pitch_gain(5, 100) == 650);
    assert(decode_pitch_gain(9, 42) == 1170);
    
    // Test MR122 mode - last two bits cleared
    // For value 130 (binary 10000010), clearing last two bits gives 128 (10000000)
    assert(decode_pitch_gain(1, MR122) == 128);
    // For 390 (110000110), clearing last two bits gives 388 (110000100)
    assert(decode_pitch_gain(3, MR122) == 388);
    // For 1170 (10010010010), clearing last two bits gives 1168 (10010010000)
    assert(decode_pitch_gain(9, MR122) == 1168);
    // Value 0 stays 0
    assert(decode_pitch_gain(0, MR122) == 0);
    
    // Test out-of-range indices
    assert(decode_pitch_gain(-1, 0) == 0);
    assert(decode_pitch_gain(10, MR122) == 0);
    assert(decode_pitch_gain(100, 42) == 0);
    
    return 0;
}

#include <array>
#include <cstddef>

// Lookup table for pitch gain values in Q14 format
constexpr std::array<int, 10> qua_gain_pitch = {0, 130, 260, 390, 520, 650, 780, 910, 1040, 1170};

// Special mode constant for MR122
constexpr int MR122 = 122;

// Decodes the pitch gain from an index and mode.
// Returns the gain in Q14 format or 0 if index is out of range.
int decode_pitch_gain(int index, int mode) {
    // Validate index range
    if (index < 0 || index >= static_cast<int>(qua_gain_pitch.size())) {
        return 0;
    }
    
    int gain = qua_gain_pitch[static_cast<std::size_t>(index)];
    
    // For MR122 mode, clear the two least significant bits
    if (mode == MR122) {
        gain &= 0xFFFC;  // Clears bits 0 and 1
    }
    
    return gain;
}

// The solution involves a straightforward table lookup: given an index, retrieve the corresponding value from the constant array `qua_gain_pitch`. The main logic branches on the `mode` parameter: if `mode == MR122`, we perform a bitwise AND with `0xFFFC` to zero out the lowest two bits of the gain. This is equivalent to clearing bits 0 and 1. Edge cases include an out-of-range index: we must guard against negative indices or indices ≥ 10 to avoid undefined behavior, returning 0 in such cases. The mode check should be done after the index validation but before returning. The function has O(1) time complexity (constant lookup) and O(1) space complexity (only the lookup table uses fixed memory). No special error handling is needed beyond the index bound check, and the table is declared as `const` to prevent modification.
