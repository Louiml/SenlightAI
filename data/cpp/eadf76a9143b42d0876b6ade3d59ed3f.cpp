// Write a C++ function `countTwinPrimes(int n)` that, given a positive integer `n` (where `n >= 2`), returns the number of pairs of consecutive prime numbers less than or equal to `n` that differ by exactly 2. Such pairs are called "twin primes" (e.g., (3,5), (5,7), (11,13)). The function must determine primality efficiently for each number from 2 to `n`, collect the primes in order, and then count the adjacent pairs with a difference of 2. The function should be self-contained, use appropriate helper functions, and handle edge cases like small `n` where fewer than two primes exist (returning 0).

#include <cassert>

int main() {
    // n = 2 -> only one prime (2), no pairs
    assert(countTwinPrimes(2) == 0);
    // n = 3 -> primes {2,3}, difference 1, no pairs
    assert(countTwinPrimes(3) == 0);
    // n = 5 -> primes {2,3,5}, pairs: (3,5) => 1
    assert(countTwinPrimes(5) == 1);
    // n = 10 -> primes {2,3,5,7}, pairs: (3,5) and (5,7) => 2
    assert(countTwinPrimes(10) == 2);
    // n = 20 -> primes {2,3,5,7,11,13,17,19}, pairs: (3,5),(5,7),(11,13),(17,19) => 4
    assert(countTwinPrimes(20) == 4);
    // n = 100 -> known count is 8 twin prime pairs up to 100
    assert(countTwinPrimes(100) == 8);
    // n = 1000 -> known count of twin primes up to 1000 is 35
    assert(countTwinPrimes(1000) == 35);
}

#include <vector>
#include <cmath>

// Helper function to check if a number is prime
bool isPrimeNumber(int x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (int d = 3; d <= std::sqrt(x); d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

// Count pairs of consecutive primes <= n that differ by 2
int countTwinPrimes(int n) {
    std::vector<int> primes;
    for (int num = 2; num <= n; ++num) {
        if (isPrimeNumber(num)) {
            primes.push_back(num);
        }
    }

    int pairCount = 0;
    for (std::size_t i = 1; i < primes.size(); ++i) {
        if (primes[i] - primes[i - 1] == 2) {
            ++pairCount;
        }
    }
    return pairCount;
}

// The main approach is a straightforward sieve-of-divisibility check for primality. First, write a helper `bool isPrime(int x)` that naively tests divisibility from 2 up to `sqrt(x)`, returning `false` immediately if any divisor is found, and `true` otherwise (with 2 as the smallest prime). Then, iterate `i` from 2 to `n`, pushing every prime into a `vector<int> primes`. After collecting all primes, iterate over indices `1` to `primes.size()-1`, incrementing a counter `count` whenever `primes[i] - primes[i-1] == 2`. This is the only condition since the primes are sorted in ascending order by construction. Edge cases: if `n` is 2 or 3, the vector contains fewer than two primes, so the loop over pairs does nothing and the result is 0. The time complexity is O(n * sqrt(n)) for primality checks plus O(number of primes) for the pair scan, which is O(n√n) overall in the worst case. Space complexity is O(n) for storing the primes (in practice, the number of primes up to `n` is about `n / log n`). No special handling for overflow is needed since `n` is expected to be small enough for a competition-style problem, but a reasonable upper bound is `n <= 10^5` for performance.
