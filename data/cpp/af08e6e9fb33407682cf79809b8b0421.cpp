/*
Write a C++ function `bool canReachDivisibility(long long a, long long b)` that determines whether integer `a` can be reduced to 1 by repeatedly dividing it by any prime factor that also appears in the prime factorization of `b`. The function should return `true` if `a` can be reduced to 1 by dividing it by primes shared with `b` (each prime from `b` can be used any number of times, but only by its exact value), and `false` otherwise. The function must handle the base cases: if `a` is already 1, return `true`; if `b` is 1 (and `a` > 1), return `false`. The inputs are positive integers up to \(10^{18}\), and the function should efficiently factorize both numbers by trial division (up to the square root) and compare the sets of prime factors. The main algorithmic insight is that `a` can be reduced to 1 if and only if every prime factor of `a` is also a prime factor of `b` — the exponents don't matter because we can divide repeatedly by the same prime. Edge cases include `a = b`, `a` being a power of `b`, and both numbers containing large primes near \(10^9\).
*/

#include <unordered_set>
#include <cmath>

// Determines if `a` can be reduced to 1 by dividing by prime factors shared with `b`.
// Returns true if every distinct prime factor of `a` is also a prime factor of `b`.
bool canReachDivisibility(long long a, long long b) {
    // Trivial cases
    if (a == 1) {
        return true;
    }
    if (b == 1) {
        return false;
    }

    // Collect all distinct prime factors of b
    std::unordered_set<long long> primeFactorsOfB;
    long long tempB = b;
    for (long long i = 2; i * i <= tempB; ++i) {
        while (tempB % i == 0) {
            primeFactorsOfB.insert(i);
            tempB /= i;
        }
    }
    // If tempB is still > 1, it is a prime factor of b
    if (tempB > 1) {
        primeFactorsOfB.insert(tempB);
    }

    // Check every distinct prime factor of a
    long long tempA = a;
    for (long long i = 2; i * i <= tempA; ++i) {
        while (tempA % i == 0) {
            if (primeFactorsOfB.find(i) == primeFactorsOfB.end()) {
                return false;
            }
            tempA /= i;
        }
    }
    // If tempA is still > 1, it is a prime factor of a
    if (tempA > 1) {
        if (primeFactorsOfB.find(tempA) == primeFactorsOfB.end()) {
            return false;
        }
    }

    return true;
}

#include <cassert>

int main() {
    // Basic cases
    assert(canReachDivisibility(1, 5) == true);   // a is already 1
    assert(canReachDivisibility(6, 1) == false);  // b has no primes
    assert(canReachDivisibility(12, 6) == true);  // 12 = 2^2*3, 6 = 2*3 → all primes shared
    assert(canReachDivisibility(10, 6) == false); // 10 has prime 5 not in 6
    assert(canReachDivisibility(49, 7) == true);  // 49 = 7^2, 7 has prime 7
    assert(canReachDivisibility(100, 10) == true); // 100 = 2^2*5^2, 10 = 2*5

    // Large numbers and prime edge cases
    assert(canReachDivisibility(999999937, 999999937) == true); // same large prime
    assert(canReachDivisibility(999999937, 2) == false);        // large prime not in b
    assert(canReachDivisibility(2 * 3 * 5, 2 * 3) == false);    // 5 missing from b
    assert(canReachDivisibility(2 * 3 * 5, 2 * 3 * 5) == true); // exact match
    assert(canReachDivisibility(4 * 9 * 25, 2 * 3 * 5) == true); // exponents don't matter

    return 0;
}

// The core idea is that reducing `a` to 1 by repeated division by primes from `b` is possible exactly when the set of distinct prime factors of `a` is a subset of the distinct prime factors of `b`. This is because each division removes one occurrence of a prime, and we can divide as many times as needed by the same prime as long as that prime exists in `b` at least once. The algorithm proceeds as follows: first handle the trivial cases — if `a == 1` return `true`, if `b == 1` return `false` (since `a > 1` cannot be divided by primes of `1`). Otherwise, factorize `b` by trial division up to the square root of `b`, collecting all distinct prime factors into an unordered set. Then factorize `a` similarly, and for each distinct prime factor of `a`, check if it exists in the set of `b`'s primes; if any prime factor of `a` is missing from `b`, return `false`. After processing all prime factors of `a`, if no mismatch is found, return `true`. Important edge cases: when `a` and/or `b` are large primes themselves (the loop runs up to the square root, which can be up to ~\(10^9\), but this is acceptable since the problem constraints are modest for a coding challenge); when after trial division the remaining value of `a` or `b` is greater than 1, that remaining part is itself a prime factor and must be accounted for. Time complexity is \(O(\sqrt{a} + \sqrt{b})\) in the worst case due to trial division, and space complexity is \(O(\pi(\sqrt{b}))\) for the set of distinct prime factors of `b`, which is negligible.
