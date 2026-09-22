Write a C++ function that takes a non-negative integer `n` and returns the number of binary strings of length `n` that contain no two consecutive zeros. For example, for `n = 2`, the valid strings are "01", "10", "11" (and "00" is invalid), so the answer is 3. For `n = 1`, valid strings are "0" and "1", so the answer is 2. The function must handle `n = 0` as a special case, returning 1 (the empty string is trivially valid). The function should be efficient for `n` up to at least 10^6, using only constant auxiliary space, and should return the result as a `long long` to avoid overflow for moderate `n` (for `n` up to 90, it fits in a 64-bit integer; for larger `n` you may assume the result fits in `long long`).
The problem is a classic dynamic programming or Fibonacci-like recurrence. Let `end0[i]` be the number of valid strings of length `i` that end with `0`, and `end1[i]` be the number of valid strings of length `i` that end with `1`. For length 1: `end0[1] = 1` (string "0"), `end1[1] = 1` (string "1"). For each next length `i > 1`:  
- A valid string ending in `0` must have been preceded by a `1`, so `end0[i] = end1[i-1]`.  
- A valid string ending in `1` can be preceded by either `0` or `1`, so `end1[i] = end0[i-1] + end1[i-1]`.  
Thus the total for length `n` is `end0[n] + end1[n]`. This recurrence matches the Fibonacci sequence (shifted): the total counts for n=1,2,3,4,... are 2,3,5,8,13,... which is `F(n+2)` (with F(1)=1, F(2)=1). Edge case: for `n = 0`, the empty string is valid, so return 1. For `n = 1`, return 2 directly by the base cases. The algorithm uses only two variables to track the previous `end0` and `end1`, yielding `O(n)` time and `O(1)` auxiliary space. No overflow occurs for `n ≤ 90` with `long long`; for larger `n` results exceed 64-bit, but the problem guarantees the result fits.
#include <cstdint>

// Counts binary strings of length n with no consecutive zeros.
// Returns the count as a long long. Handles n=0 as special case returning 1.
long long countBinaryStringsNoConsecutiveZeros(int n) {
    if (n < 0) return 0; // Invalid input, but defined defensively
    if (n == 0) return 1; // Empty string is valid

    long long end0 = 1; // strings of length 1 ending in '0'
    long long end1 = 1; // strings of length 1 ending in '1'

    for (int len = 2; len <= n; ++len) {
        long long newEnd0 = end1;          // must be preceded by '1'
        long long newEnd1 = end0 + end1;   // can be preceded by '0' or '1'
        end0 = newEnd0;
        end1 = newEnd1;
    }

    return end0 + end1;
}
#include <cassert>

int main() {
    // Base cases
    assert(countBinaryStringsNoConsecutiveZeros(0) == 1);
    assert(countBinaryStringsNoConsecutiveZeros(1) == 2);
    // Small values
    assert(countBinaryStringsNoConsecutiveZeros(2) == 3);
    assert(countBinaryStringsNoConsecutiveZeros(3) == 5);
    assert(countBinaryStringsNoConsecutiveZeros(4) == 8);
    assert(countBinaryStringsNoConsecutiveZeros(5) == 13);
    // Known Fibonacci-related values (F(n+2))
    assert(countBinaryStringsNoConsecutiveZeros(6) == 21);
    assert(countBinaryStringsNoConsecutiveZeros(7) == 34);
    assert(countBinaryStringsNoConsecutiveZeros(8) == 55);
    // Larger value to test loop correctness
    assert(countBinaryStringsNoConsecutiveZeros(10) == 144);
    // Negative input (defensive)
    assert(countBinaryStringsNoConsecutiveZeros(-1) == 0);
    return 0;
}
