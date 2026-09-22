// Write a C++ function that takes a positive integer `n` and returns the sum of the first `n` numbers of the form (10^i - 1), where i starts from 1. For example, for n=5, the numbers are 9, 99, 999, 9999, 99999, and their sum is 111105. Your function should handle any positive `n` (including n=0, returning 0) and must not rely on floating-point math; instead, build each term iteratively (e.g., start with term=9 and multiply by 10 then add 9 for each subsequent term). The function must be named `sumRepunits` (or a descriptive name) and must be `const`-correct where applicable. The solution must be self-contained with only necessary headers.

The core idea is to generate each term of the sequence: t1 = 10^1 - 1 = 9, t2 = 10^2 - 1 = 99, t3 = 10^3 - 1 = 999, and so on. Instead of using `pow` (which introduces floating-point issues and potential precision loss for large n), we can compute each term iteratively: start with `term = 9`, and for each subsequent i, `term = term * 10 + 9`. This exactly produces 9, 99, 999, ... because multiplying by 10 shifts digits left and adding 9 appends another 9. For each term we add it to a running sum. Edge case: if n=0, the loop does not run and the sum is 0. If n is very large (e.g., n=10^6), the terms grow exponentially (10^i), so the sum may overflow an `int`; we should use `long long` for both the term and the sum to handle large values safely. Time complexity is O(n) because we iterate exactly n times, each doing constant work. Space complexity is O(1) since we only keep a fixed number of variables.

#include <cstdint>

// Compute the sum of the first n numbers of the form (10^i - 1) for i=1..n.
// Each term is a repunit-like number: 9, 99, 999, ...
// Returns 0 when n <= 0.
long long sumRepunits(int n) {
    if (n <= 0) return 0LL;

    long long term = 9;      // First term: 10^1 - 1
    long long total = 0;

    for (int i = 0; i < n; ++i) {
        total += term;
        term = term * 10 + 9; // Next term: 10^(i+1) - 1
    }

    return total;
}

#include <cassert>

int main() {
    // Basic cases
    assert(sumRepunits(0) == 0);
    assert(sumRepunits(1) == 9);
    assert(sumRepunits(2) == 9 + 99);          // 108
    assert(sumRepunits(3) == 9 + 99 + 999);    // 1107
    assert(sumRepunits(5) == 111105);          // 9+99+999+9999+99999

    // Larger n, checking it still works with long long
    assert(sumRepunits(10) == 1111111110LL);   // sum of first 10 terms

    // Negative input treated as 0
    assert(sumRepunits(-5) == 0);

    // Very large n (but still within long long range)
    // For n=18, sum = 111111111111111111 (18 ones) which fits in long long
    assert(sumRepunits(18) == 111111111111111111LL);

    return 0;
}
