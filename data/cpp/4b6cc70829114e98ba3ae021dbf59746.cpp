// Write a C++ function named `countTrailingZeroes` that takes a non-negative integer `n` as input and returns the number of trailing zeroes in `n!` (the factorial of `n`). The function must handle values of `n` up to 2,147,483,647 (the maximum `int` value) without causing integer overflow during computation. You are not allowed to compute the factorial directly; instead, use the mathematical property that trailing zeroes are determined by the number of times 5 is a factor in the multiplication sequence from 1 to `n`. The function should be `const`-correct (i.e., take its parameter by value and not modify any external state), and it should return an `int` representing the count of trailing zeroes.
// The number of trailing zeros in `n!` equals the number of times 10 is a factor, which is the minimum of the exponent of 2 and the exponent of 5 in the prime factorization of `n!`. Since factors of 2 are far more abundant than factors of 5 in any factorial, the count of trailing zeros is exactly the number of factors of 5 in `n!`. For each integer `k` from 1 to `n`, a multiple of 5 contributes one factor of 5, a multiple of 25 contributes an extra factor (two total), a multiple of 125 contributes an extra again, and so on. Therefore, the total is the sum of `floor(n / 5)`, `floor(n / 25)`, `floor(n / 125)`, ..., until the divisor exceeds `n`. The standard loop multiplies the divisor by 5 each iteration, checking `n / divisor >= 1`. Edge cases: `n = 0` returns 0 (since 0! = 1, no trailing zeros). For large `n` up to `INT_MAX`, the maximum divisor needed is 5^13 (since 5^13 ≈ 1.22e9, and 5^14 > INT_MAX), so the loop runs at most 13 iterations, and the multiplication `divisor * 5` stays below `INT_MAX` most of the time but may overflow if not carefully checked—however, by checking `n / divisor >= 1` before multiplying, we can use a `long long` for the divisor to avoid any overflow. Time complexity: O(log₅(n)), which is at most O(log n). Space complexity: O(1).
#include <cstdint>

// Returns the number of trailing zeroes in n! (n factorial) for n >= 0.
// Uses the count of factors of 5 in n!.
int countTrailingZeroes(int n) {
    int count = 0;
    // Use int64_t to avoid overflow when multiplying the divisor by 5.
    for (int64_t divisor = 5; n / divisor >= 1; divisor *= 5) {
        count += static_cast<int>(n / divisor);
    }
    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countTrailingZeroes(0) == 0);    // 0! = 1
    assert(countTrailingZeroes(1) == 0);    // 1! = 1
    assert(countTrailingZeroes(4) == 0);    // 4! = 24
    assert(countTrailingZeroes(5) == 1);    // 5! = 120
    assert(countTrailingZeroes(10) == 2);   // 10! = 3628800
    assert(countTrailingZeroes(25) == 6);   // includes 25, 5, 10, 15, 20
    assert(countTrailingZeroes(100) == 24); // known value
    // Large value to ensure no overflow
    assert(countTrailingZeroes(2147483647) == 536870902); // mathematical result
    // Another large value
    assert(countTrailingZeroes(1000000000) == 249999998); // known pattern
    return 0;
}
