/*
Write a C++ function that takes a positive integer `n` and returns the minimum number of primes needed to represent `n` as a sum of primes. The function must assume Goldbach’s conjecture is true (every even integer greater than 2 is the sum of two primes) and therefore the answer is always 1 (if `n` itself is prime), 2 (if `n` can be written as a sum of two primes, which includes all even numbers > 2 and some odd numbers), or 3 (otherwise, since every odd composite can be written as a sum of three primes by the weak Goldbach conjecture). The function must be efficient for `n` up to 10^6 and must not use trial division beyond a precomputed sieve up to 10^6. The return type is an integer. If `n` is 1, 2, or 3, handle these small cases explicitly: for `n = 1`, return 0 (cannot be expressed), for `n = 2` and `n = 3` return 1 (since they are prime). For `n` greater than 3, apply the logic described. You must implement a helper function to check primality using the sieve. The main solution function must be named `minimalPrimeSumCount` and must take a single integer argument `n`, returning an `int`.
*/

#include <vector>
#include <algorithm>

// Returns the minimum number of primes whose sum equals n.
// Assumes Goldbach's conjecture and weak Goldbach's conjecture.
// n must be a positive integer (n >= 1).
// For n = 1, returns 0 (not expressible as a sum of any primes).
int minimalPrimeSumCount(int n) {
    if (n <= 0) return -1; // invalid input
    if (n == 1) return 0;
    if (n == 2 || n == 3) return 1;

    // Sieve of Eratosthenes up to n
    std::vector<bool> isPrime(n + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // If n itself is prime, only one prime is needed.
    if (isPrime[n]) return 1;

    // For even n > 2, Goldbach's conjecture guarantees two primes.
    if (n % 2 == 0) return 2;

    // For odd n, check if n = p + q with both p and q prime.
    // We only need to iterate p from 2 to n-2.
    for (int p = 2; p <= n - 2; ++p) {
        if (isPrime[p]) {
            int q = n - p;
            if (isPrime[q]) {
                return 2;
            }
        }
    }

    // By weak Goldbach conjecture, every odd composite > 5 is sum of 3 primes.
    return 3;
}

#include <cassert>

int main() {
    // Small edge cases
    assert(minimalPrimeSumCount(1) == 0);
    assert(minimalPrimeSumCount(2) == 1);
    assert(minimalPrimeSumCount(3) == 1);

    // Prime numbers
    assert(minimalPrimeSumCount(11) == 1);
    assert(minimalPrimeSumCount(97) == 1);

    // Even numbers (always 2 except 2 itself)
    assert(minimalPrimeSumCount(4) == 2);
    assert(minimalPrimeSumCount(10) == 2);
    assert(minimalPrimeSumCount(100) == 2);
    assert(minimalPrimeSumCount(1000) == 2);

    // Odd numbers that are sum of two primes
    assert(minimalPrimeSumCount(5) == 2); // 2+3
    assert(minimalPrimeSumCount(9) == 2); // 2+7
    assert(minimalPrimeSumCount(15) == 2); // 2+13
    assert(minimalPrimeSumCount(21) == 2); // 2+19

    // Odd numbers that require three primes
    assert(minimalPrimeSumCount(27) == 3); // e.g., 3+5+19
    assert(minimalPrimeSumCount(35) == 3); // e.g., 3+3+29
    assert(minimalPrimeSumCount(51) == 3); // e.g., 3+5+43
    assert(minimalPrimeSumCount(105) == 3); // e.g., 3+3+99? Actually 3+5+97

    // Larger random odd composite not expressible as sum of two primes (must be 3)
    assert(minimalPrimeSumCount(999) == 3);

    // Large even number
    assert(minimalPrimeSumCount(1000000) == 2);

    // Large prime near upper bound
    assert(minimalPrimeSumCount(999983) == 1); // 999983 is prime

    return 0;
}

// The approach uses a bool vector `isPrime` of size `n+1` (or up to a safe bound) computed by the Sieve of Eratosthenes. Then we check if `n` is prime: if yes, return 1. Otherwise, we need to check if `n` can be expressed as a sum of two primes. According to Goldbach’s conjecture (which is verified for all even numbers up to 4×10^18 and for odd numbers one prime plus an even Goldbach pair), every even number > 2 is a sum of two primes, so for even `n` return 2. For odd `n`, we try subtracting each prime `p` from 2 to `n-2` (using the sieve) and check if `n - p` is prime. If any such pair exists, return 2. If not, return 3 (by weak Goldbach, every odd composite > 5 is a sum of three primes). Edge cases: `n = 1` (return 0), `n = 2` and `n = 3` (return 1), `n = 4` (even, return 2), `n = 5` (odd, try 2+3, return 2), `n = 7` (prime, return 1), `n = 9` (odd composite, try 2+7 (7 prime) so return 2), `n = 27` (odd, try 2+25 no, 3+24 no, 5+22 no, 7+20 no, 11+16 no, 13+14 no, 17+10 no, 19+8 no, 23+4 no → no pair, so return 3). Time complexity is O(n log log n) for sieving and O(number of primes up to n) for the two-prime check, so O(n log log n + n / log n) which is essentially O(n log log n). Space complexity is O(n) for the sieve array. The function is deterministic and does not rely on randomness.
