// Write a C++ function named `countPrimesUpTo` that takes an integer `n` (where `n >= 0`) and returns the number of prime numbers strictly less than `n`. The function must use the Sieve of Eratosthenes algorithm to efficiently count primes, and it must handle edge cases such as `n = 0`, `n = 1`, and `n = 2` correctly (all of which should return 0). The function should be `const`-correct (i.e., it does not modify its parameter, which is passed by value) and should be self-contained, including only necessary headers. The function must return an `int` representing the count.

// The Sieve of Eratosthenes works by first creating a boolean vector `isPrime` of size `n+1` (because we need indices from 0 to `n` to check primality for numbers less than `n`), initialized to `true`. Then we mark `isPrime[0]` and `isPrime[1]` as `false` since 0 and 1 are not prime. We iterate `i` from 2 to `n-1` (since we only care about numbers less than `n`). For each `i` that is still marked as prime, we increment our count, and then mark all multiples of `i` (starting from `2*i`, up to `n-1`) as `false`. This works because any composite number less than `n` has a prime factor less than or equal to its square root, so by the time we reach that prime factor, the composite is marked. Edge cases: if `n <= 1`, the loop doesn't execute and the count remains 0; if `n = 2`, the loop runs `i` from 2 to 1, so no iterations, and count is 0. Time complexity is `O(n log log n)` for the sieve, and space complexity is `O(n)` for the boolean vector.

#include <vector>

// Count the number of prime numbers strictly less than n using Sieve of Eratosthenes.
int countPrimesUpTo(int n) {
    if (n <= 2) {
        return 0;
    }
    std::vector<bool> isPrime(n, true); // indices 0 .. n-1
    isPrime[0] = false;
    isPrime[1] = false;
    int count = 0;
    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            ++count;
            for (int j = 2 * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return count;
}

#include <cassert>

int main() {
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 0);
    assert(countPrimesUpTo(3) == 1); // 2
    assert(countPrimesUpTo(10) == 4); // 2,3,5,7
    assert(countPrimesUpTo(11) == 4); // 2,3,5,7 (11 is not less than 11)
    assert(countPrimesUpTo(20) == 8); // 2,3,5,7,11,13,17,19
    assert(countPrimesUpTo(100) == 25); // known count
    assert(countPrimesUpTo(1000) == 168); // known count
    assert(countPrimesUpTo(10000) == 1229); // known count
    return 0;
}
