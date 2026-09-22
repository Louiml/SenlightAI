// Write a C++ function that takes a positive integer `n` and returns the `n`-th prime number, where the 1st prime is 2, the 2nd is 3, the 3rd is 5, and so on. The function must be named `nthPrime` and accept a single `int` parameter. Assume `n` is at least 1. The function should compute the prime efficiently enough for `n` up to 2000 (i.e., primes up to ~17500), using a primality check that only tests divisors up to the square root of a candidate. Do not include a `main` function in the solution; only provide the function.

// The solution iterates over candidate integers starting from 2, counting how many are prime until reaching `n`. For each candidate, a helper `isPrime` checks divisibility only up to the square root (using `i * i <= candidate`), which avoids unnecessary checks. Edge cases: `n = 1` returns 2 immediately; `n` may be large requiring consideration of many candidates. The main algorithm runs in `O(n * sqrt(k))` time where `k` is the `n`-th prime (approximately `n log n`), but since `n` is at most 2000, this is fast. Space complexity is `O(1)` auxiliary, since we only store a counter and the current candidate. The primality check correctly handles even numbers (except 2) by starting from 3 and stepping by 2 for efficiency, though a simple loop from 2 also works.

#include <cmath>

// Returns true if the non-negative integer x is prime, false otherwise.
bool isPrime(int x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (int i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

// Returns the n-th prime number, where n >= 1.
int nthPrime(int n) {
    int count = 0;
    int candidate = 2;
    while (true) {
        if (isPrime(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}

#include <cassert>

// Declaration of the function under test
int nthPrime(int n);

int main() {
    assert(nthPrime(1) == 2);
    assert(nthPrime(2) == 3);
    assert(nthPrime(3) == 5);
    assert(nthPrime(4) == 7);
    assert(nthPrime(10) == 29);
    assert(nthPrime(25) == 97);
    assert(nthPrime(100) == 541);
    assert(nthPrime(500) == 3571);
    assert(nthPrime(1000) == 7919);
    assert(nthPrime(2000) == 17389);
    return 0;
}
