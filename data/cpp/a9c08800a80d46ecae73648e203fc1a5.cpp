// Given a positive integer `n` (1 ≤ n ≤ 10^12) and a positive integer `k`, write a C++ function `long long kthDivisor(long long n, long long k)` that returns the `k`-th smallest positive divisor of `n` (1-indexed). If `k` exceeds the total number of positive divisors of `n`, return `-1`. For example, for `n = 12` and `k = 4`, the divisors in ascending order are `1, 2, 3, 4, 6, 12`, so the 4th divisor is `4`; for `k = 7`, return `-1`.

The straightforward approach of iterating from 1 to `n` is infeasible for up to 10^12. Instead, observe that every divisor `d` of `n` has a complementary divisor `n/d`. Therefore, to enumerate all divisors, we only need to iterate up to `sqrt(n)`. For each integer `i` from 1 to `sqrt(n)`, if `i` divides `n`, we collect both `i` and (if different) `n/i` into a vector. After collecting all candidate divisors, sort the vector in ascending order. The number of divisors is at most about `2 * sqrt(n)`, which is at most ~2×10^6 for `n = 10^12`, so this fits within typical memory/time constraints. Edge cases include `n` being a perfect square (to avoid duplicating the square root divisor), and `k` being larger than the divisor count, in which case return `-1`. Time complexity is `O(sqrt(n) + D log D)` where `D` is the divisor count; space complexity is `O(D)`.

#include <vector>
#include <algorithm>
#include <cmath>

// Return the k-th smallest positive divisor of n (1-indexed), or -1 if k exceeds the number of divisors.
long long kthDivisor(long long n, long long k) {
    std::vector<long long> divisors;
    for (long long i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }
    std::sort(divisors.begin(), divisors.end());
    if (k > static_cast<long long>(divisors.size())) {
        return -1;
    }
    return divisors[static_cast<size_t>(k - 1)];
}

#include <cassert>

int main() {
    // Basic cases
    assert(kthDivisor(12, 1) == 1);
    assert(kthDivisor(12, 2) == 2);
    assert(kthDivisor(12, 3) == 3);
    assert(kthDivisor(12, 4) == 4);
    assert(kthDivisor(12, 5) == 6);
    assert(kthDivisor(12, 6) == 12);
    assert(kthDivisor(12, 7) == -1);

    // Prime number
    assert(kthDivisor(17, 1) == 1);
    assert(kthDivisor(17, 2) == 17);
    assert(kthDivisor(17, 3) == -1);

    // Perfect square
    assert(kthDivisor(16, 1) == 1);
    assert(kthDivisor(16, 3) == 4);
    assert(kthDivisor(16, 5) == 16);
    assert(kthDivisor(16, 6) == -1);

    // Large n, small k
    assert(kthDivisor(1000000000000LL, 1) == 1);
    assert(kthDivisor(1000000000000LL, 2) == 2);
    assert(kthDivisor(1000000000000LL, 3) == 4);
}
