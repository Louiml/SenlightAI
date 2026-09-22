Write a C++ function named `isEvenPowerOfTwo` that takes a single integer argument and returns a boolean value indicating whether the input is a power of 2 *and* the exponent is even (i.e., the number is \( 4^k \) for some non-negative integer \( k \)). The function must handle negative numbers, zero, and the full range of 32-bit signed integers. Assume the input is always a valid `int`. Your solution should be self-contained, use bitwise operations for efficiency, and be correct for edge cases such as `1` (which is \( 4^0 \)) and `INT_MIN`. The function must be declared with `const` correctness where appropriate, but since it takes a value parameter, it should be a free function with a descriptive name.
The main idea is to first verify that the number is a pure power of two using the classic bitwise check: for positive `n`, `(n & (n - 1)) == 0` ensures exactly one bit is set. However, this alone is insufficient because the task also requires the exponent to be even (i.e., the number is a perfect square of a power of two, specifically \( 4^k \)). For an even exponent, the single set bit must be at an even bit position (bit 0, 2, 4, ...). A convenient way to test this is to mask the number with a pattern of alternating bits for odd positions: `0xAAAAAAAA` (binary `1010...1010`). If the number has any of those odd-position bits set, the bitwise AND with this mask will be nonzero, indicating an odd exponent. Therefore, the condition becomes: `n > 0` and `(n & (n - 1)) == 0` and `(n & 0xAAAAAAAA) == 0`. Edge cases: `0` is not a power of two, so false. Negative numbers: For negative `n`, `n - 1` and bitwise operations behave with two's complement, but since no negative number is a power of two (powers of two are positive), we can short-circuit with `n <= 0` returning false. `1` is `2^0`, exponent 0 is even, and `1 & 0xAAAAAAAA == 0`, so true. `4` (`2^2`) true, `8` (`2^3`) false, `INT_MIN` is negative, false immediately. Time complexity is O(1) constant operations, space O(1) auxiliary.
#include <cstdint>

// Returns true if n is a power of 2 and the exponent is even (i.e., n = 4^k for k >= 0).
bool isEvenPowerOfTwo(int n) {
    // Powers of two are positive; zero and negatives are not.
    if (n <= 0) {
        return false;
    }
    // Check single-bit property: power of two.
    bool singleBit = (n & (n - 1)) == 0;
    // For even exponent, the set bit must be at an even position (bit 0, 2, 4, ...).
    // Odd-position mask: 0xAAAAAAAA (bits 1, 3, 5, ...).
    bool noOddBit = (n & 0xAAAAAAAA) == 0;
    return singleBit && noOddBit;
}
#include <cassert>
#include <climits>

// Forward declaration or include the solution header.
bool isEvenPowerOfTwo(int n);

int main() {
    assert(isEvenPowerOfTwo(1) == true);   // 2^0, exponent 0 even
    assert(isEvenPowerOfTwo(4) == true);   // 2^2
    assert(isEvenPowerOfTwo(16) == true);  // 2^4
    assert(isEvenPowerOfTwo(64) == true);  // 2^6

    assert(isEvenPowerOfTwo(2) == false);  // 2^1, odd exponent
    assert(isEvenPowerOfTwo(8) == false);  // 2^3
    assert(isEvenPowerOfTwo(32) == false); // 2^5

    assert(isEvenPowerOfTwo(0) == false);
    assert(isEvenPowerOfTwo(-4) == false);
    assert(isEvenPowerOfTwo(INT_MIN) == false);
    assert(isEvenPowerOfTwo(-1) == false);
    assert(isEvenPowerOfTwo(3) == false);
    assert(isEvenPowerOfTwo(6) == false);

    // Large power with odd exponent
    assert(isEvenPowerOfTwo(1 << 31) == false); // 2^31, odd (since 31 is odd)
    // Large power with even exponent (2^30 = 1073741824)
    assert(isEvenPowerOfTwo(1 << 30) == true);
}
