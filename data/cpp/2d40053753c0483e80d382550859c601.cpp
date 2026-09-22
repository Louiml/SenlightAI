// Write a C++ function that takes two integers `a` and `b` (with `a ≤ b`) and returns the sum of all odd integers in the inclusive range `[a, b]`. The function must handle negative numbers correctly (e.g., odd negative numbers like `-3` should be included), and it must work efficiently for very large ranges where `b` may be up to `10^9` and `a` may be negative. The function should return an `long long` to avoid overflow when summing many large odd numbers. The task does not require input/output handling—only the core summation logic in a free function.
#include <cassert>

int main() {
    assert(sumOddRange(1, 5) == 9);          // 1+3+5=9
    assert(sumOddRange(2, 4) == 3);          // 3
    assert(sumOddRange(-3, -1) == -4);       // -3 + -1 = -4
    assert(sumOddRange(-2, 2) == -1 + 1);    // -1+1=0
    assert(sumOddRange(-5, -3) == -8);       // -5 + -3 = -8
    assert(sumOddRange(0, 0) == 0);          // no odds
    assert(sumOddRange(5, 5) == 5);          // single odd
    assert(sumOddRange(6, 6) == 0);          // single even
    assert(sumOddRange(10, 20) == 75);       // 11+13+15+17+19 = 75
    assert(sumOddRange(-4, 4) == 0);         // -3-1+1+3 = 0
}
#include <cstdint>

// Helper: sum of odd numbers from 1 to n (inclusive) for n >= 1, otherwise 0.
// Works for n > 0; for n <= 0 returns 0.
long long sumOddUpToPositive(long long n) {
    if (n <= 0) return 0;
    long long k = (n + 1) / 2;  // number of odd numbers from 1 to n
    return k * k;
}

// Returns the sum of all odd integers in the inclusive range [a, b].
// Handles negative numbers, zero, and positive numbers. Assumes a <= b.
long long sumOddRange(long long a, long long b) {
    if (a > b) return 0;

    // If both are positive or a is positive
    if (a >= 0) {
        return sumOddUpToPositive(b) - sumOddUpToPositive(a - 1);
    }

    // If both are negative
    if (b <= 0) {
        // Symmetric: sum of odds from a to b = negative of sum of odds from -b to -a
        return -(sumOddUpToPositive(-a) - sumOddUpToPositive(-b - 1));
    }

    // Mixed: a <= 0 < b
    // Sum of odds from a to -1 (negative side) + sum of odds from 1 to b
    long long negativePart = -(sumOddUpToPositive(-a)); // odds from a to -1 (negative values)
    long long positivePart = sumOddUpToPositive(b);
    return negativePart + positivePart;
}
// The straightforward approach is to iterate from `a` to `b` and add each odd number, but this is `O(b - a + 1)` which is too slow for ranges up to `10^9`. Instead, we can derive a mathematical formula. For any integer `n`, the sum of all odd numbers from `1` to `n` (if `n` is odd) or from `1` to `n-1` (if `n` is even) is given by `((n + 1) / 2)^2` when `n` is positive. To handle negative numbers, we can shift the range: sum of odds from `a` to `b` equals `sumOfOddsUpTo(b) - sumOfOddsUpTo(a-1)`. Define a helper `sumOddUpTo(x)` that returns the sum of all positive odd integers ≤ `x` if `x` is positive, and `0` for `x ≤ 0` (since negative odds are symmetric and we can transform). For negative ranges, we can use symmetry: sum of odds from `a` to `b` where `a` is negative equals negative of sum of odds from `-b` to `-a`. We can convert any range so that `a` is positive by splitting: if `a ≤ 0 ≤ b`, we sum odds from `a` to `-1` (which is negative of odds from `1` to `-a`) plus odds from `1` to `b`. If both `a` and `b` are negative, the sum is negative of odds from `-b` to `-a`. Edge cases: `a > b` should return 0 (though the contract says `a ≤ b`), and ranges with no odd numbers (e.g., `2` to `2`) return 0. The formula for positive `n`: if `n` is odd, number of odd numbers from 1 to n is `(n+1)/2`, sum = `((n+1)/2)^2`. If `n` is even, it's the same as for `n-1`. So `sumOddUpToPositive(n) = ((n+1)/2)^2` where integer division truncates. Time complexity is `O(1)`, space `O(1)`.
