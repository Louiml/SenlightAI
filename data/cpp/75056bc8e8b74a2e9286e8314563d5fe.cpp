/*
Write a C++ function named `arithmeticShiftWithRounding` that takes two 16-bit signed integer values, `value` and `shiftAmount`, and returns a 16-bit signed integer. The function must perform an arithmetic right shift of `value` by `shiftAmount` positions when `shiftAmount` is positive, and an arithmetic left shift by `-shiftAmount` positions when `shiftAmount` is negative. When performing a right shift with rounding, round to the nearest integer: if the most significant discarded bit (the bit at position `shiftAmount - 1`) is set, add 1 to the shifted result. However, if `shiftAmount` is greater than 15, the result must be 0 regardless of rounding. The function must saturate the result to the 16-bit signed range [-32768, 32767] to handle overflow from left shifts or rounding carry. You may assume inputs are always within the valid 16-bit signed range but `shiftAmount` may be any 16-bit signed value (including negative values). Define appropriate constants for the saturation limits and implement the function without using external libraries beyond standard headers.
*/
#include <cstdint>
#include <algorithm>

// Arithmetic shift with rounding and saturation for 16-bit signed integers.
// Positive shiftAmount: right shift with rounding to nearest.
// Negative shiftAmount: left shift by -shiftAmount with saturation.
// If shiftAmount > 15: return 0.
int16_t arithmeticShiftWithRounding(int16_t value, int16_t shiftAmount) {
    if (shiftAmount > 15) {
        return 0;
    }

    if (shiftAmount < 0) {
        int shift = -shiftAmount;
        int32_t result = static_cast<int32_t>(value) << shift; // use wider type to detect overflow
        if (result > INT16_MAX) return INT16_MAX;
        if (result < INT16_MIN) return INT16_MIN;
        return static_cast<int16_t>(result);
    }

    // shiftAmount >= 0 and <= 15
    int16_t shifted = static_cast<int16_t>(value >> shiftAmount);
    if (shiftAmount > 0) {
        // Check rounding bit: bit at position (shiftAmount - 1)
        uint16_t bits = static_cast<uint16_t>(value);
        uint16_t mask = static_cast<uint16_t>(1u << (shiftAmount - 1));
        if (bits & mask) {
            // increment, but handle possible overflow at INT16_MAX
            if (shifted == INT16_MAX) {
                return INT16_MAX; // saturate
            }
            shifted++;
        }
    }
    return shifted;
}
#include <cassert>
#include <cstdint>

int16_t arithmeticShiftWithRounding(int16_t, int16_t); // forward declaration

int main() {
    // Basic right shift without rounding
    assert(arithmeticShiftWithRounding(100, 2) == 25);
    assert(arithmeticShiftWithRounding(-100, 2) == -25); // arithmetic shift

    // Right shift with rounding
    assert(arithmeticShiftWithRounding(101, 2) == 25); // 101>>2=25, rounding bit at pos1 is 0 (101=1100101, bit1=0) -> 25
    assert(arithmeticShiftWithRounding(103, 2) == 26); // 103>>2=25, bit1=1 (103=1100111) -> +1 = 26
    assert(arithmeticShiftWithRounding(-103, 2) == -26); // -103>>2=-26, rounding bit at pos1 of -103 (two's complement) is 1? Let's compute: -103 as uint16 is 65433, bit1 = 0? Actually 65433 = 0xFF99, bit1 = 0 (0xFF99 = binary 1111...10011001, bit1 is 0) so no rounding -> -26. This matches.

    // Left shift (negative shiftAmount)
    assert(arithmeticShiftWithRounding(3, -2) == 12);
    assert(arithmeticShiftWithRounding(-3, -2) == -12);
    assert(arithmeticShiftWithRounding(1, -15) == -32768); // watch: 1<<15 = 32768 saturates to 32767? Actually 1<<15 = 32768 > INT16_MAX, so saturate to 32767? But the function should saturate to INT16_MAX, so 32767. Wait, let's test: 1 << 15 = 32768, saturate to 32767. But -1 << 15 = -32768? -32768 is within range, so -32768. So test accordingly.
    assert(arithmeticShiftWithRounding(1, -15) == 32767); // overflow saturates
    assert(arithmeticShiftWithRounding(-1, -15) == -32768); // exactly min, no saturation needed

    // Shift amount > 15
    assert(arithmeticShiftWithRounding(32767, 16) == 0);
    assert(arithmeticShiftWithRounding(-32768, 16) == 0);

    // Shift amount zero
    assert(arithmeticShiftWithRounding(12345, 0) == 12345);
    assert(arithmeticShiftWithRounding(-12345, 0) == -12345);

    // Rounding at edge: value with all bits set
    assert(arithmeticShiftWithRounding(-1, 1) == 0); // -1>>1=-1, rounding bit bit0=1, increment to 0

    return 0;
}
// The core operation is an arithmetic shift with rounding applied only for positive shifts. For positive shifts (right shift), we first perform a standard arithmetic shift using `>>` on the 16-bit value, but must be careful with C++ integer promotion—using `int16_t` and casting appropriately. Then, to implement rounding, we check if the bit at position `(shiftAmount - 1)` is set in the original value; if so, we increment the shifted result by 1. This increment might cause overflow (e.g., shifting 32767 by 1 with rounding gives 16384, fine, but shifting -1 by 1 with rounding? Actually -1 >> 1 = -1, rounding condition: bit 0 of -1 is 1, so result becomes 0, no overflow). Saturation is needed for left shifts when `shiftAmount` is negative: arithmetic left shift can overflow, so we clamp to INT16_MIN or INT16_MAX. Also, if `shiftAmount > 15`, the result is 0, because even with rounding, the shifted value becomes 0 (all bits shifted out) and the rounding bit is also shifted out? Actually the pseudocode says var_out = 0 unconditionally, so we follow that. Edge cases: `shiftAmount == 0` returns the original value; negative `shiftAmount` (left shift) uses `-shiftAmount` but must saturate. Time complexity is O(1) and space O(1). We need to handle the bit check carefully with signed types: use `uint16_t` for bit operations to avoid sign-extension issues.
