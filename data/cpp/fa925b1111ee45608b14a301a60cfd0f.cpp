Write a C++ function `countBinaryStringsWithoutConsecutiveOnes(int n, int mod)` that returns the number of binary strings of length `n` (where each character is either '0' or '1') that **do not** contain the substring `"11"` (i.e., no two consecutive '1's). The result must be computed modulo `mod`. The function should handle `n` from 1 to 10^6 efficiently. Note: The original snippet computes something related, but you must produce a correct, self-contained solution. Edge cases: `n=1` should return 2 (strings: "0","1"), `n=2` should return 3 ("00","01","10"). Ensure the function works for any positive `n`.

This is a classic dynamic programming problem. Let `end0` be the number of valid strings of current length that end with '0', and `end1` be the number that end with '1'. For length 1, `end0=1`, `end1=1`. For each next character, you can append '0' to any string (both `end0` and `end1` become `end0+end1` for new `end0`), and you can append '1' only to strings ending with '0' (so new `end1` equals old `end0`). The total is `end0+end1`. This recurrence is equivalent to Fibonacci numbers: total for length n is F(n+2) where F(1)=1,F(2)=1. However, iterative DP is straightforward. The modulo is applied at each step to avoid overflow. The algorithm runs in O(n) time and O(1) space. For `n=1` and `n=2`, the loop naturally handles them if you start from length 1. Edge case: if `mod` is 1, the result is 0 for all n>0; handle by applying modulo correctly. Also ensure `n` is positive; if not, return 0 (or handle per specification).

#include <cstdint>

// Returns the number of binary strings of length n with no consecutive '1's, modulo mod.
// n must be >= 1. mod is any positive integer.
std::int64_t countBinaryStringsWithoutConsecutiveOnes(int n, int mod) {
    if (n <= 0 || mod <= 0) return 0;

    // end0: valid strings of current length ending with '0'
    // end1: valid strings of current length ending with '1'
    std::int64_t end0 = 1 % mod;
    std::int64_t end1 = 1 % mod;

    for (int i = 2; i <= n; ++i) {
        std::int64_t newEnd0 = (end0 + end1) % mod;
        std::int64_t newEnd1 = end0;
        end0 = newEnd0;
        end1 = newEnd1;
    }

    return (end0 + end1) % mod;
}

#include <cassert>
#include <cstdint>

// Declaration (assume the function is defined above)
std::int64_t countBinaryStringsWithoutConsecutiveOnes(int n, int mod);

int main() {
    // Basic cases
    assert(countBinaryStringsWithoutConsecutiveOnes(1, 1000000007) == 2);
    assert(countBinaryStringsWithoutConsecutiveOnes(2, 1000000007) == 3);
    assert(countBinaryStringsWithoutConsecutiveOnes(3, 1000000007) == 5);
    assert(countBinaryStringsWithoutConsecutiveOnes(4, 1000000007) == 8);

    // Modulo behavior
    assert(countBinaryStringsWithoutConsecutiveOnes(5, 10) == 3); // 13 % 10 = 3
    assert(countBinaryStringsWithoutConsecutiveOnes(10, 7) == (144 % 7)); // 144 % 7 = 4

    // n=1 with mod=1 -> all results 0
    assert(countBinaryStringsWithoutConsecutiveOnes(1, 1) == 0);
    assert(countBinaryStringsWithoutConsecutiveOnes(5, 1) == 0);

    // Larger n
    assert(countBinaryStringsWithoutConsecutiveOnes(20, 1000000007) == 17711);

    return 0;
}
