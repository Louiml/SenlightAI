Write a standalone C++ function `mult_int16_r(int16_t var1, int16_t var2)` that multiplies two 16-bit signed integers in Q15 fixed-point format and returns a rounded 16-bit signed result. The function must multiply `var1` and `var2` as 32-bit integers, add a rounding constant of `0x4000` (16384), shift the sum right by 15 bits, saturate the result to the 16-bit signed range `[-32768, 32767]`, and return the final value. The saturation must handle the case where the shifted value exceeds the representable range (e.g., `-32768 * -32768` overflows and should clamp to `32767`). The function should be self-contained, include necessary headers, and be declared with proper `const` correctness.

The solution approach is straightforward: multiply the two 16-bit inputs as 32-bit integers to avoid any intermediate overflow, then add the rounding constant `0x4000` (which is half of `0x8000`, i.e., half of the divisor `2^15`), and right-shift by 15 bits. The shift effectively divides by 32768 with rounding to the nearest integer. After shifting, the result may still exceed the 16-bit signed range if the product is at the extreme negative end (i.e., `-32768 * -32768 = 1073741824`; after rounding and shifting, we get approximately `32768`, which is out of range). Saturation is implemented by checking whether the shifted result's upper bits indicate overflow: if the value is not representable in 16 bits, clamp to `MAX_16` (32767) for positive overflow or to `MIN_16` (-32768) for negative overflow. Edge cases include the multiplication of `-32768` and `-32768`, where the result after rounding becomes exactly `32768`, which must be clamped to `32767`. Also, any product that after rounding and shifting exceeds the 16-bit range should be saturated. The time complexity is O(1) with constant space.

#include <cstdint>
#include <algorithm>

/**
 * @brief Multiply two 16-bit signed integers in Q15 format with rounding and saturation.
 * 
 * @param var1 First 16-bit signed operand.
 * @param var2 Second 16-bit signed operand.
 * @return int16_t Rounded and saturated product in Q15 format.
 */
int16_t mult_int16_r(int16_t var1, int16_t var2) {
    // Perform 32-bit multiplication to avoid overflow.
    int32_t product = static_cast<int32_t>(var1) * static_cast<int32_t>(var2);
    
    // Add rounding constant (0x4000 = 16384) for rounding to nearest.
    product += static_cast<int32_t>(0x4000);
    
    // Shift right by 15 bits to bring product back to Q15 range.
    product >>= 15;
    
    // Saturate to 16-bit signed range.
    if (product > INT16_MAX) {
        product = INT16_MAX;
    } else if (product < INT16_MIN) {
        product = INT16_MIN;
    }
    
    return static_cast<int16_t>(product);
}

#include <cassert>
#include <cstdint>
#include <climits>

// Declaration of the function under test (assuming it is defined elsewhere).
int16_t mult_int16_r(int16_t var1, int16_t var2);

int main() {
    // Basic positive multiplication: 0.5 * 0.5 = 0.25 (in Q15: 16384 * 16384 = 268435456, round+shift -> 8192)
    assert(mult_int16_r(16384, 16384) == 8192);
    
    // Negative multiplication: -0.5 * 0.5 = -0.25 -> -8192
    assert(mult_int16_r(-16384, 16384) == -8192);
    
    // Multiplication by zero: any * 0 = 0
    assert(mult_int16_r(0, 12345) == 0);
    assert(mult_int16_r(-32768, 0) == 0);
    
    // Multiplication by one: 1.0 in Q15 is 32767 (max positive), but 1.0 * 0.5 = 0.5 -> 16384
    assert(mult_int16_r(32767, 16384) == 16384);
    
    // Saturation case: (-1.0) * (-1.0) = 1.0, but max Q15 is 0.9999695, so should clamp to 32767
    assert(mult_int16_r(-32768, -32768) == 32767);
    
    // Multiplication resulting in exactly 1.0? Not representable, so check round-up behavior:
    // 0.9999695 * 0.9999695 ~ 0.999939, should round to 32767? Let's test a near-max product.
    assert(mult_int16_r(32767, 32767) == 32767); // 32767*32767=1073676289, +16384 -> shift -> 32768, saturate to 32767
    
    // Another rounding: 0.5 * 0.25 = 0.125 -> 4096 in Q15 (16384 * 8192 = 134217728, +16384 -> shift 15 -> 4096)
    assert(mult_int16_r(16384, 8192) == 4096);
    
    // Rounding up case: 0.5 * 0.5000305 ~ 0.250015, should round to 8193? Example: 16385 * 16384 = 268451840, +16384 -> 268468224 >> 15 = 8193
    assert(mult_int16_r(16385, 16384) == 8193);
    
    return 0;
}
