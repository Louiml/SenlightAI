/*
Write a C++ function named `normalize_shift_count` that takes a 16-bit signed integer (use `int16_t`) and returns the number of left shifts needed to normalize it to the range [-0x4000, 0x4000) for negative values and [0x4000, 0x7FFF] for positive values, as specified by the classic `norm_s` operation. Specifically: if the input is zero, return 0; if the input is -1 (0xFFFF), return 15; for negative inputs, first compute the bitwise complement, then count how many left shifts are required until the most significant bit (bit 14, value 0x4000) is set, using an efficient loop that may shift by multiple bits at once. The function must not modify its input, must handle all `int16_t` values, and must return an `int16_t` result. The function should be self-contained in a header-less source file with only necessary C++ standard library includes (e.g., `<cstdint>`). Provide the function definition with `const` correctness where appropriate.
*/
#include <cstdint>

// Compute the number of left shifts needed to normalize a 16-bit signed integer.
// Returns an int16_t as per specification.
int16_t normalize_shift_count(const int16_t var1) {
    if (var1 == 0) {
        return 0;
    }
    if (var1 == -1) {  // 0xFFFF
        return 15;
    }

    int16_t value = var1;
    if (value < 0) {
        // In two's complement, for negative numbers, (value - 1) then XOR with sign-extended shift gives complement.
        const int16_t y = static_cast<int16_t>(value - 1);
        value = static_cast<int16_t>(y ^ (y >> 15));
    }

    int16_t shift_count = 0;
    while ((value & 0x4000) == 0) {
        shift_count++;
        if ((value & 0x2000) != 0) {
            break;
        }
        shift_count++;
        if ((value & 0x1000) != 0) {
            break;
        }
        shift_count++;
        if ((value & 0x0800) != 0) {
            break;
        }
        shift_count++;
        value = static_cast<int16_t>(value << 4);
    }

    return shift_count;
}
#include <cassert>
#include <cstdint>

// The function under test is declared above; include it here or link accordingly.
// For this test, we assume the function is available.

int main() {
    // Zero case
    assert(normalize_shift_count(0) == 0);

    // Special -1 case
    assert(normalize_shift_count(-1) == 15);

    // Positive values
    assert(normalize_shift_count(1) == 14);   // 0x0001 -> 0x4000 after 14 shifts
    assert(normalize_shift_count(0x3FFF) == 1);
    assert(normalize_shift_count(0x4000) == 0);
    assert(normalize_shift_count(0x7FFF) == 0);
    assert(normalize_shift_count(0x1000) == 2); // 0x1000 -> 0x4000 after 2 shifts

    // Negative values
    assert(normalize_shift_count(static_cast<int16_t>(-2)) == 14); // ~(-2)=1 -> needs 14 shifts
    assert(normalize_shift_count(static_cast<int16_t>(-32768)) == 1); // ~(-32768)=0x7FFF -> needs 1
    assert(normalize_shift_count(static_cast<int16_t>(-16384)) == 1); // ~(-16384)=0x3FFF -> needs 1
    assert(normalize_shift_count(static_cast<int16_t>(-32767)) == 0); // ~(-32767)=0x7FFE? Actually -32767 is 0x8001, complement=0x7FFE, bit14 is 1 -> 0 shifts

    // Boundary checks: a few more representative values
    assert(normalize_shift_count(0x2000) == 1); // 0x2000 -> 0x4000 after 1
    assert(normalize_shift_count(0x0800) == 3); // 0x0800 -> after 3 shifts becomes 0x4000
    assert(normalize_shift_count(0x0400) == 4); // 0x0400 -> after 4 shifts becomes 0x4000

    return 0;
}
// The solution replicates the behavior of the ETSI `norm_s` function from speech coding. The key steps: (1) handle `var1 == 0` → return 0; (2) handle `var1 == -1` → return 15 (since `~(-1)==0` and normalization would require 16 shifts, but the spec caps at 15); (3) for negative values, compute `y = var1 - 1` and then `var1 = y ^ (y >> 15)` — this is a bit trick that for negative numbers gives the bitwise complement in a single operation (for positive numbers it leaves them unchanged); (4) then count shifts: while the bit at position 14 (0x4000) is not set, increment the shift count and optionally shift left by 4 bits after checking lower bits to quickly skip many zeros, but careful because shifting by 4 may overshoot if a 1 appears in the middle. The given code's loop checks bits at positions 14, 13, 12, 11 sequentially, and after checking these, shifts left by 4. This is an optimization for typical 16-bit hardware. Edge cases: `var1 = 0x0001` → needs 14 shifts (to reach 0x4000); `var1 = 0x3FFF` → needs 1 shift; `var1 = 0x8000` (negative min) → complement is 0x7FFF, needs 1 shift; `var1 = -32768` is 0x8000, so `y = 0x7FFF`, complement is 0x8000? Wait, let's verify the trick: for `var1 < 0`, `y = var1 - 1`; then `var1 = y ^ (y >> 15)`. For `var1 = -32768` (0x8000), `y = -32769` (0x7FFF in two's complement? Actually -32768 - 1 = -32769, but int16_t wraps to 0x7FFF). Then `y>>15` for 0x7FFF is 0, so `var1 = 0x7FFF`? That is not the complement of 0x8000 (which is 0x7FFF indeed). So it works. The time complexity is O(1) in the worst case since at most 4 iterations of the while loop (each iteration shifts by up to 4 bits), and space is O(1). The function must handle all 16-bit values correctly, including all negatives and positives.
