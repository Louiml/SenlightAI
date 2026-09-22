/*
Write a C++ function `int countDivisors(int x)` that takes a single positive integer `x` and returns the total number of positive divisors of `x`. For example, `6` has divisors `1, 2, 3, 6` so it should return `4`. The input `x` is guaranteed to be at least `1`. The function must count divisors efficiently by iterating only up to the square root of `x`, and must handle perfect squares (like `9`, divisors `1, 3, 9` → return `3`) without double-counting the square root as two distinct divisors. The function should not read from standard input or print anything; it only computes and returns the count.
*/
#include <cmath>

// Count the number of positive divisors of a positive integer x.
// Uses trial division up to sqrt(x) and accounts for perfect squares.
int countDivisors(int x) {
    int count = 0;
    const int limit = static_cast<int>(std::sqrt(x));

    for (int i = 1; i <= limit; ++i) {
        if (x % i == 0) {
            // i and x/i are both divisors
            count += 2;
        }
    }

    // If x is a perfect square, the divisor i == x/i was counted twice.
    if (limit * limit == x) {
        --count;
    }

    return count;
}
#include <cassert>

int countDivisors(int x); // declaration from solution

int main() {
    assert(countDivisors(1) == 1);
    assert(countDivisors(2) == 2);
    assert(countDivisors(4) == 3);   // 1, 2, 4
    assert(countDivisors(6) == 4);   // 1, 2, 3, 6
    assert(countDivisors(9) == 3);   // 1, 3, 9
    assert(countDivisors(12) == 6);  // 1, 2, 3, 4, 6, 12
    assert(countDivisors(16) == 5);  // 1, 2, 4, 8, 16
    assert(countDivisors(100) == 9); // 1,2,4,5,10,20,25,50,100
    assert(countDivisors(997) == 2); // prime
    return 0;
}
// The key idea is to iterate `i` from `1` up to `sqrt(x)`. For each `i` where `x % i == 0`, we know two divisors exist: `i` and `x/i`. However, if `i * i == x` (i.e., `x` is a perfect square), then `i` and `x/i` are the same number, so we must count it only once. We can count all divisor pairs by incrementing the count by 2 for every `i` that divides `x`, and then after the loop, if `i * i == x` (or equivalently `(int)sqrt(x) * (int)sqrt(x) == x`), subtract 1 from the total to remove the double count of the square root. Alternatively, we can track the square root case during the loop as done in the original snippet, but a cleaner approach is to first count all pairs as `count += 2`, then after the loop check if `i * i == x` and subtract 1. Edge cases: `x = 1` — loop runs only for `i = 1`, `1 % 1 == 0`, we add 2 → count = 2, then check `1*1 == 1`, subtract 1 → returns 1, correct. Large values like `1000000007` (prime) — loop runs to about 31623 iterations, each O(1), so O(sqrt(x)) time and O(1) auxiliary space, independent of x's magnitude except for sqrt.
