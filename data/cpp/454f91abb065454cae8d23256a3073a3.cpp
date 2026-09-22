Write a C++ function `minimumMaxDigits(long long n)` that, given a positive integer `n`, finds the smallest possible value of the larger digit-count among all pairs of positive integers `(a, b)` such that `a * b = n`. In other words, for each divisor pair `(a, b)` of `n`, compute the number of digits in `a` and the number of digits in `b`, take the maximum of those two counts, and return the minimum such maximum over all divisor pairs. The function should return this minimum value as a `long long`. The input `n` is guaranteed to be a positive integer up to \(10^{12}\). The digit count of a positive integer is the number of decimal digits it has (e.g., 9 has 1 digit, 10 has 2 digits, 123 has 3 digits).
// We need to consider all factor pairs `(a, b)` with `a <= b` and `a * b = n`. For each pair, the maximum digit count is `max(digits(a), digits(b))`. Since `a <= b`, the maximum is always `digits(b)` (because `b >= a` implies `b` has at least as many digits as `a`, but careful: if `a` and `b` have same digit count, still max is that count; if `b` has more, max is digits(b)). So essentially we want the smallest possible digit count of the larger factor among all factor pairs.
//
// We can iterate `i` from 1 to `sqrt(n)`. For each `i` that divides `n`, we have a pair `(i, n/i)`. The maximum digits is `max(digits(i), digits(n/i))`. We take the minimum over all such. The smallest possible maximum will be achieved when the two factors are as close as possible, i.e., the divisor near `sqrt(n)`. Iterating up to `sqrt(n)` is sufficient because all factor pairs are covered. Edge case: `n = 1` — only pair is `(1,1)`, digit count 1. Also for perfect squares, `i = n/i` and we count it once. The function `calc` counts digits by repeatedly dividing by 10.
//
// Time complexity: `O(sqrt(n))` because we loop up to `sqrt(n)`. For `n` up to `10^12`, `sqrt(n)` is `10^6`, which is fine. Space complexity: `O(1)`.
#include <bits/stdc++.h>

// Count the number of decimal digits in a positive integer x.
long long countDigits(long long x) {
    long long digits = 0;
    do {
        ++digits;
        x /= 10;
    } while (x > 0);
    return digits;
}

// Return the minimum possible maximum digit count among all factor pairs of n.
long long minimumMaxDigits(long long n) {
    long long best = 1000000; // larger than any possible digit count (max ~13 for 10^12)
    for (long long i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            long long j = n / i;
            long long current = std::max(countDigits(i), countDigits(j));
            best = std::min(best, current);
        }
    }
    return best;
}
#include <cassert>

int main() {
    // Known results:
    // n = 1 -> pair (1,1) -> digits 1,1 -> max=1
    assert(minimumMaxDigits(1) == 1);
    // n = 2 -> pairs (1,2) -> max digits = 1
    assert(minimumMaxDigits(2) == 1);
    // n = 10 -> pairs (1,10)->max=2, (2,5)->max=1 -> answer 1
    assert(minimumMaxDigits(10) == 1);
    // n = 100 -> pairs: (1,100)->3, (2,50)->2, (4,25)->2, (5,20)->2, (10,10)->2 -> answer 2
    assert(minimumMaxDigits(100) == 2);
    // n = 999 -> pairs near sqrt(999) ~31.6, factor 27*37 -> digits 2 and 2 -> answer 2
    assert(minimumMaxDigits(999) == 2);
    // n = 1000 -> pairs (1,1000)->4, (2,500)->3, (4,250)->3, (5,200)->3, (8,125)->3, (10,100)->3, (20,50)->2, (25,40)->2 -> answer 2
    assert(minimumMaxDigits(1000) == 2);
    // n = 123456789 -> well-known factors? Not needed, just check it runs and returns reasonable value
    long long result = minimumMaxDigits(123456789);
    assert(result >= 1 && result <= 9);
    // n = 1000000000000 (10^12) -> sqrt=10^6, pair (10^6,10^6) -> digits 7 and 7 -> answer 7
    assert(minimumMaxDigits(1000000000000LL) == 7);
    // n = 999999999999 (just below 10^12) -> near sqrt ~999999.5, pair around (999999, 1000001) but not exact; just check it works
    result = minimumMaxDigits(999999999999LL);
    assert(result >= 1 && result <= 12);
}
