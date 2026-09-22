/*
Write a C++ function that takes a non-negative integer `n` (with `1 <= n <= 10^5`) and returns the number of binary strings of length `n` that do not contain consecutive `1`s, modulo `1e9 + 7`. The binary strings are composed only of characters `0` and `1`. For example, for `n = 2`, valid strings are `00`, `01`, `10` (but not `11`), so the result is `3`. The function must handle large `n` efficiently and return the result as a `long long` value.
*/
#include <cstdint>

// Count binary strings of length n with no consecutive 1s, modulo 1e9+7.
// n must be at least 1.
long long countBinaryStringsWithoutConsecutiveOnes(int n) {
    constexpr long long MOD = 1000000007LL;
    
    long long zeroEnd = 1; // strings ending with '0' for length 1
    long long oneEnd = 1;  // strings ending with '1' for length 1
    long long total = 2;   // total valid strings for length 1
    
    if (n == 1) {
        return total;
    }
    
    for (int length = 2; length <= n; ++length) {
        long long newOneEnd = zeroEnd;          // '1' can only follow '0'
        long long newZeroEnd = total;           // '0' can follow anything
        zeroEnd = newZeroEnd % MOD;
        oneEnd = newOneEnd % MOD;
        total = (zeroEnd + oneEnd) % MOD;
    }
    
    return total;
}
#include <cassert>

int main() {
    assert(countBinaryStringsWithoutConsecutiveOnes(1) == 2);
    assert(countBinaryStringsWithoutConsecutiveOnes(2) == 3);
    assert(countBinaryStringsWithoutConsecutiveOnes(3) == 5);
    assert(countBinaryStringsWithoutConsecutiveOnes(4) == 8);
    assert(countBinaryStringsWithoutConsecutiveOnes(5) == 13);
    assert(countBinaryStringsWithoutConsecutiveOnes(10) == 144);
    assert(countBinaryStringsWithoutConsecutiveOnes(20) == 17711);
    // Large n: Fibonacci-like sequence modulo 1e9+7
    assert(countBinaryStringsWithoutConsecutiveOnes(1000000) == 389568357LL);
    return 0;
}
// We solve this using dynamic programming with state tracking for the last character of the string. Define two values as we build strings of increasing length:
// - `zeroEnd`: number of valid strings of current length ending in `0`.
// - `oneEnd`: number of valid strings of current length ending in `1`.
// - `total`: sum of both.
//
// Base case for length `1`: `zeroEnd = 1`, `oneEnd = 1`, `total = 2`.
//
// For each additional character (from length `2` up to `n`), we update:
// - A `0` can follow either a `0` or a `1`, so `newZeroEnd = total`.
// - A `1` can only follow a `0`, so `newOneEnd = zeroEnd`.
// - New `total = newZeroEnd + newOneEnd`.
//
// All operations are taken modulo `1e9 + 7` to avoid overflow. The loop runs `n - 1` times, so time complexity is `O(n)` and space complexity is `O(1)` (only constant number of variables). The edge case `n = 1` returns `2` directly.
