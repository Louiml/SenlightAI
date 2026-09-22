/*
Write a standalone C++ function `bitWiseScale` that takes an integer `value`, an integer `shift` (which may be negative, zero, or positive), and a boolean `multiply` (when `true`, perform left shift / multiplication by 2^shift; when `false`, perform right shift / division by 2^shift). The function should return the scaled integer result. If `shift` is positive and `multiply` is `true`, the result is `value << shift`; if `shift` is positive and `multiply` is `false`, the result is `value >> shift`. If `shift` is negative, treat the absolute value as the opposite operation (i.e., negative shift with `multiply=true` means shift right, and negative shift with `multiply=false` means shift left). If `shift` is zero, return `value` unchanged. Assume the input `value` is within the range of a signed 32-bit integer, and that all operations, including shifts for negative numbers and potential overflow/underflow, are performed using standard two's-complement arithmetic without additional error checking. The function should be `const`-correct and use only bitwise and arithmetic operators.
*/

#include <cstdint>

// Scale an integer by a power of two using bitwise shifts.
// When multiply is true, left shift by shift (positive) or right shift by |shift| (negative).
// When multiply is false, right shift by shift (positive) or left shift by |shift| (negative).
int bitWiseScale(int value, int shift, bool multiply) {
    int effectiveShift = multiply ? shift : -shift;
    if (effectiveShift > 0) {
        return value << effectiveShift;
    } else if (effectiveShift < 0) {
        return value >> (-effectiveShift);
    }
    return value;
}

#include <cassert>

int bitWiseScale(int, int, bool); // declaration for testing

int main() {
    assert(bitWiseScale(35, 2, true) == 140);    // 35 << 2 = 35 * 4
    assert(bitWiseScale(35, 2, false) == 8);     // 35 >> 2 = 35 / 4 = 8 (integer division)
    assert(bitWiseScale(-35, 2, true) == -140);  // negative left shift
    assert(bitWiseScale(-35, 2, false) == -9);   // -35 >> 2 = -9 (arithmetic shift)
    assert(bitWiseScale(35, -2, true) == 8);     // negative shift with multiply -> right shift
    assert(bitWiseScale(35, -2, false) == 140);  // negative shift with !multiply -> left shift
    assert(bitWiseScale(35, 0, true) == 35);     // zero shift
    assert(bitWiseScale(35, 0, false) == 35);    // zero shift
    assert(bitWiseScale(1, 31, true) == INT_MIN); // overflow but defined for bit shift
    assert(bitWiseScale(-1, 1, true) == -2);      // negative value left shift
}

// The solution approach is straightforward: convert the given `shift` and `multiply` flag into a single effective shift direction and magnitude. The key insight is that a negative shift magnitude means the opposite operation: if `multiply` is true (left shift), then a negative shift actually means a right shift by the absolute value, and vice versa. So we compute the effective shift amount as `shift` if `multiply` is true, otherwise as `-shift` (because shifting right by k is the same as shifting left by -k conceptually, but we need to handle the sign explicitly). More concretely: let `effectiveShift = multiply ? shift : -shift`. Then if `effectiveShift > 0`, result = `value << effectiveShift`; if `effectiveShift < 0`, result = `value >> (-effectiveShift)`; if zero, result = `value`. This handles all cases uniformly. Edge cases: `shift` can be large, but standard bit shift behavior is defined for shifts less than the width of the type (typically 32 for int). If `shift` magnitude exceeds 31, the behavior is undefined in C++, but the problem states we assume standard arithmetic without additional checks, so we assume valid inputs. Time complexity is O(1), space complexity O(1). No loops or recursion. We must be careful to handle negative `shift` correctly and avoid sign conversion issues, but using `int` and straightforward arithmetic is fine for the given constraints.
