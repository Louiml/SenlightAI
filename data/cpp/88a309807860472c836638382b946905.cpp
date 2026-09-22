// Write a C++ function `findNearestPrimes(int n, int& lower, int& upper)` that, given a positive integer `n` greater than 2, finds the largest prime number strictly less than `n` and the smallest prime number strictly greater than `n`. Assign these to the reference parameters `lower` and `upper`. The search for the upper prime should only consider values up to 1,000,000,000. If no such prime exists in that range, set `upper` to `-1`. The function must correctly handle cases where `n` itself is prime (the nearest primes are still strictly less and greater) and where `n` is near very small primes (e.g., `n = 3` has no lower prime, so set `lower` to `-1`). You may assume `n` is between 3 and 1,000,000,000 inclusive.
// The main algorithm is to check numbers downward from `n-1` and upward from `n+1` until a prime is found. A number `x` is prime if it has exactly two distinct positive divisors (1 and itself). A straightforward primality test iterates divisors from 2 up to `sqrt(x)` and returns `false` if any divide `x`; otherwise `true`. This is efficient enough because we only test a small number of candidates near `n` (the gap between consecutive primes is typically small). Edge cases: if `n = 3`, the downward loop starts at 2 and finds nothing, so set `lower` to `-1`. For the upward loop, if we reach 1,000,000,000 without finding a prime (which cannot happen in practice for the given range because there are many primes below 1e9, but for completeness we set `upper = -1` if search exceeds the bound). Time complexity is `O(g * sqrt(n))` where `g` is the gap to the nearest primes, which is very small (less than a few hundred for n up to 1e9). Space complexity is O(1) besides the input integer.
#include <cmath>

// Finds the largest prime < n and smallest prime > n.
// Sets lower/upper to -1 if no such prime exists in the allowed range.
void findNearestPrimes(int n, int& lower, int& upper) {
    // Helper lambda to check primality.
    auto isPrime = [](int x) -> bool {
        if (x < 2) return false;
        if (x % 2 == 0) return x == 2;
        for (int d = 3; d * d <= x; d += 2) {
            if (x % d == 0) return false;
        }
        return true;
    };

    // Find lower prime: scan downward from n-1.
    lower = -1;
    for (int i = n - 1; i >= 2; --i) {
        if (isPrime(i)) {
            lower = i;
            break;
        }
    }

    // Find upper prime: scan upward from n+1 up to 1e9.
    upper = -1;
    const int limit = 1000000000;
    for (int i = n + 1; i <= limit; ++i) {
        if (isPrime(i)) {
            upper = i;
            break;
        }
    }
}
#include <cassert>

int main() {
    int lower, upper;

    // n = 10: primes below: 7, above: 11
    findNearestPrimes(10, lower, upper);
    assert(lower == 7 && upper == 11);

    // n = 3: no lower prime, above: 5
    findNearestPrimes(3, lower, upper);
    assert(lower == -1 && upper == 5);

    // n = 4: below: 3, above: 5
    findNearestPrimes(4, lower, upper);
    assert(lower == 3 && upper == 5);

    // n = 5 is prime: below: 3, above: 7
    findNearestPrimes(5, lower, upper);
    assert(lower == 3 && upper == 7);

    // n = 2 is not allowed by spec, but skip.

    // n = 100: below 97, above 101
    findNearestPrimes(100, lower, upper);
    assert(lower == 97 && upper == 101);

    // n = 1000000000: below 999999937, above 1000000007 (which is > limit, but our scan stops at limit,
    // and 1000000007 is > limit, so upper = -1). However, within the limit, there is a prime? 1000000007 > limit,
    // but 999999937 is prime, below. For above, the next prime after 1e9 is 1000000007, which exceeds limit, so upper = -1.
    findNearestPrimes(1000000000, lower, upper);
    assert(lower == 999999937 && upper == -1);

    // n = 1000001: below 999983, above 1000033 (both within limit)
    findNearestPrimes(1000001, lower, upper);
    assert(lower == 999983 && upper == 1000033);
}
