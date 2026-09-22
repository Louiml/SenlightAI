/*
Write a C++ function `int countTrailingZeros(int N)` that takes a non-negative integer `N` and returns the number of trailing zeros in the decimal representation of `N!` (N factorial). The function must handle the edge case where `N` is 0 (where `0! = 1` has zero trailing zeros) and must correctly compute the result for large values of `N` up to at least 2,000,000,000 without using floating-point arithmetic or direct factorial computation. The result can be determined by counting how many times factors of 5 appear in the prime factorization of `N!`.
*/

#include <cstdint>

// Returns the number of trailing zeros in the decimal representation of N!.
// For N < 5, the result is 0. Uses the classic Legendre's formula.
int countTrailingZeros(int N) {
    if (N < 0) {
        return 0; // invalid input; treat as 0
    }
    int count = 0;
    // Use long long to avoid overflow when computing powers of 5.
    for (long long divisor = 5; divisor <= N; divisor *= 5) {
        count += static_cast<int>(N / divisor);
    }
    return count;
}

#include <cassert>

// The function is defined elsewhere; this is for testing.
int countTrailingZeros(int N);

int main() {
    assert(countTrailingZeros(0) == 0);
    assert(countTrailingZeros(1) == 0);
    assert(countTrailingZeros(4) == 0);
    assert(countTrailingZeros(5) == 1);    // 120 has one zero
    assert(countTrailingZeros(10) == 2);   // 3628800 has two zeros
    assert(countTrailingZeros(25) == 6);   // 25! has 6 zeros
    assert(countTrailingZeros(100) == 24); // known value
    assert(countTrailingZeros(125) == 31); // 125/5=25 + 125/25=5 + 125/125=1 =31
    assert(countTrailingZeros(2000000000) == 499999997); // large input
    return 0;
}

// The number of trailing zeros in `N!` is determined by the number of times 10 divides into it. Since 10 = 2 × 5, and there are always more factors of 2 than factors of 5 in `N!`, the count is exactly the number of factors of 5 in the prime factorization of `N!`. This count is given by the sum: `⌊N/5⌋ + ⌊N/25⌋ + ⌊N/125⌋ + ...` until the division result becomes zero. For example, `N = 25` gives `25/5 = 5` and `25/25 = 1`, total `6`. Edge cases: `N = 0` or `N < 5` yields `0` trailing zeros because `0! = 1` and small factorials contain no factor of 5. For very large `N` (up to 2 billion), only about 10 iterations are needed because `5^20` exceeds 2 billion. The algorithm runs in `O(log_5 N)` time and uses `O(1)` space. Be careful with integer overflow: `i` in the loop must be of a type that can hold up to at least `5^20` (about 9.5e13), so use `long long` for the multiplier, but the result fits in `int` because the maximum trailing zeros for `N = 2e9` is roughly `2e9/4 = 500 million`, which fits in a 32-bit signed int.
