/*
Write a standalone C++ function that extracts and returns the i-th bit (0-indexed from the least significant bit) of a given non-negative integer `n`, where `i` is a non-negative integer. The function should return 1 if the bit at that position is set (1), and 0 otherwise. Assume input values are valid (`n >= 0`, `i >= 0`), and handle cases where `i` may exceed the number of bits in `n` (in which case the bit is 0). The function should be const-correct and not modify inputs.
*/

#include <limits>

// Return the i-th bit (0-indexed from LSB) of non-negative integer n.
int getBit(int n, int i) {
    // For non-negative n, bits beyond the highest representable bit are zero.
    if (i >= std::numeric_limits<int>::digits) {
        return 0;
    }
    int mask = (1 << i);
    return (mask & n) ? 1 : 0;
}

int main() {
    // Basic tests with n = 5 (binary 101)
    assert(getBit(5, 0) == 1); // LSB is 1
    assert(getBit(5, 1) == 0); // second bit is 0
    assert(getBit(5, 2) == 1); // third bit is 1
    // Zero input
    assert(getBit(0, 0) == 0);
    assert(getBit(0, 5) == 0);
    // Large i (beyond typical bit width)
    assert(getBit(5, 31) == 0); // 32-bit safe
    assert(getBit(5, 30) == 0);
    // Power of two
    assert(getBit(8, 3) == 1); // 8 = 1000
    assert(getBit(8, 2) == 0);
    // Max int (all ones except sign bit in 32-bit)
    assert(getBit(INT_MAX, 30) == 1);
    assert(getBit(INT_MAX, 31) == 0); // sign bit is 0 for non-negative
    // Edge: i = 0
    assert(getBit(1, 0) == 1);
    assert(getBit(2, 0) == 0);
    // Large i beyond 32 bits (if int is 32-bit, 33 is safe because we check digits = 31? Actually digits = 31 for 32-bit int, so i=31 returns 0 already. Use i=32 as safe)
    assert(getBit(5, 32) == 0);
    return 0;
}

// The core algorithm uses bitwise masking: create a mask with a single 1 at position `i` using `(1 << i)`, then AND it with `n`. If the result is non-zero, the bit is 1; otherwise 0. For `i` greater than or equal to the bit width (e.g., `i >= 31` for 32-bit int), shifting by `i` could cause undefined behavior. To avoid this, we can check if `i >= 31` (or use `std::numeric_limits<int>::digits`) and directly return 0, because all leading bits are zero for non-negative integers. Important edge cases: `i = 0` returns the least significant bit; `n = 0` always returns 0; large `i` returns 0 safely. Time complexity is O(1); space complexity is O(1).
