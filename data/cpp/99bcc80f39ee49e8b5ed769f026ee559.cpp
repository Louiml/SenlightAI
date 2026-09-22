/*
Write a C++ function that takes a non-negative integer `n` (as `long long int`) and returns the count of zero digits (0s) in its binary representation. The result should be an `int`. You may assume the input is non-negative, and for the number 0, the binary representation is "0", which contains one zero digit. The function should handle large inputs up to the range of `long long int` and must not use any standard library bit-counting or string-conversion functions; implement the counting manually using arithmetic operations.
*/

#include <cstdint>

// Count the number of zero bits in the binary representation of a non-negative integer.
int countZeroBits(long long int n) {
    int zeroCount = 0;
    while (n > 0) {
        if ((n % 2) == 0) {
            ++zeroCount;
        }
        n /= 2;
    }
    // Special case: n == 0 has binary representation "0" (one zero bit)
    if (n == 0 && zeroCount == 0) {
        // This covers the initial n == 0 case; the loop never runs.
        // For n == 0, we must count the single zero bit.
        // Since the loop above didn't execute, zeroCount is still 0.
        // We need to return 1 for n == 0.
        // But careful: if n was positive, the loop would have executed,
        // and zeroCount would already reflect all bits. So we handle n == 0 here.
        // Actually, after the loop, n is always 0. So we cannot distinguish.
        // Better approach: handle n == 0 before the loop.
    }
    // Simpler: handle n == 0 before loop.
    // The above is flawed; rewrite properly below.
    return zeroCount; // This will be wrong for n == 0.
}
The above implementation is actually incorrect because after the loop `n` is always 0, so we cannot distinguish the initial n==0 case. Below is the corrected solution.

#include <cstdint>

// Count the number of zero bits in the binary representation of a non-negative integer.
int countZeroBits(long long int n) {
    if (n == 0) {
        return 1; // binary "0" has one zero digit
    }
    int zeroCount = 0;
    while (n > 0) {
        if ((n % 2) == 0) {
            ++zeroCount;
        }
        n /= 2;
    }
    return zeroCount;
}

#include <cassert>

int countZeroBits(long long int n); // declaration

int main() {
    assert(countZeroBits(0) == 1);
    assert(countZeroBits(1) == 0);
    assert(countZeroBits(2) == 1);  // 10
    assert(countZeroBits(3) == 0);  // 11
    assert(countZeroBits(4) == 2);  // 100
    assert(countZeroBits(5) == 1);  // 101
    assert(countZeroBits(7) == 0);  // 111
    assert(countZeroBits(8) == 3);  // 1000
    assert(countZeroBits(255) == 0); // 11111111
    assert(countZeroBits(256) == 8); // 100000000
    assert(countZeroBits(9223372036854775807LL) == 0); // all ones
    assert(countZeroBits(9223372036854775806LL) == 1); // one zero at LSB
    return 0;
}

// The solution repeatedly extracts the least significant bit of the number using modulo 2 (`n % 2`) and checks if it is zero. If so, increment a counter. Then divide the number by 2 (`n / 2`) to shift right by one bit. Continue until the remaining number becomes 0. However, the original snippet stops the loop when `number < 2`, and separately handles the case where `number == 0` by incrementing once more (since that final zero bit represents the binary digit for the value 0). To be correct, the loop should continue while `number > 0`, and in each iteration check the current bit. This naturally counts the zero bit for the number 0 (since 0 % 2 == 0, then number becomes 0, loop ends). For positive numbers, the loop runs until the number is fully reduced; the last iteration when number is 1 gives a non-zero bit, so no extra increment is needed. Edge cases: n = 0 → binary "0" → 1 zero. n = 1 → binary "1" → 0 zeros. n = 2 → "10" → 1 zero. Time complexity is O(number of bits) = O(log n). Space complexity is O(1). Use `long long int` to accommodate large inputs, and return an `int` since the maximum zero count is the number of bits (64 for `long long`), which fits in an `int`.
