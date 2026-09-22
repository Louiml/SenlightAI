Write a C++ function named `countPrimesUpTo` that takes a single integer `n` (where `0 <= n <= 5,000,000`) and returns the number of prime numbers strictly less than `n`. A prime number is a positive integer greater than 1 that has no positive divisors other than 1 and itself. The function must handle edge cases where `n` is 0, 1, or 2 (returning 0 in all these cases). Optimize using the Sieve of Eratosthenes; do not use trial division for each number. The function should be `const`-correct, take the input by value, and return an `int`. You may use any standard library containers, but the solution must avoid unnecessary allocations beyond the sieve array.

// The problem is a classic prime-counting task. The Sieve of Eratosthenes works by creating a boolean vector `isPrime` of size `n` (since we only care about numbers `< n`, indices `0` to `n-1`), initialized to `true`. We then mark `0` and `1` as not prime (if `n > 1`). For each integer `i` starting from 2 up to `n-1`, if `isPrime[i]` is still true, we increment the count and mark all multiples of `i` (starting from `i*i` or `2*i` — both work, but `i*i` is a minor optimization, though for simplicity we can use `2*i`) as false. This works because any composite number has a prime factor smaller than itself. Edge cases: if `n <= 2`, the answer is 0 (since primes must be ≥2 and strictly less than `n`). Time complexity is `O(n log log n)` for the sieve, and we also have an `O(n)` loop for counting, so overall `O(n log log n)`. Space complexity is `O(n)` for the boolean vector (which can be optimized to `bitset` but not required). For the given constraint `n <= 5,000,000`, the memory usage is acceptable (~5 MB for a `vector<bool>` which is bit-packed). Ensure we use `long long` for intermediate multiplications if using `i*i` to avoid overflow — but with `n ≤ 5,000,000`, `i` is at most about 2,236 when `i*i` exceeds `n`, which fits in `int`, but it's safer to use `long long` for the loop condition. In the solution, we'll use `i*i` to optimize and cast to `long long` for safety.

#include <vector>

// Returns the number of prime numbers strictly less than n.
// Uses the Sieve of Eratosthenes algorithm.
int countPrimesUpTo(int n) {
    if (n <= 2) {
        return 0;
    }

    std::vector<bool> isPrime(n, true); // indices 0..n-1
    isPrime[0] = false;
    isPrime[1] = false;

    int count = 0;
    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            ++count;
            // Mark multiples of i starting from i*i as non-prime
            for (long long j = static_cast<long long>(i) * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Edge cases
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 0);

    // Small values
    assert(countPrimesUpTo(3) == 1); // prime: 2
    assert(countPrimesUpTo(4) == 2); // primes: 2,3
    assert(countPrimesUpTo(5) == 2); // primes: 2,3
    assert(countPrimesUpTo(10) == 4); // primes: 2,3,5,7
    assert(countPrimesUpTo(20) == 8); // primes: 2,3,5,7,11,13,17,19

    // Known value: primes less than 100 = 25
    assert(countPrimesUpTo(100) == 25);

    // Known value: primes less than 1000 = 168
    assert(countPrimesUpTo(1000) == 168);

    // Larger known value: primes less than 5000000 = 348513 (approximate known count)
    assert(countPrimesUpTo(5000000) == 348513);

    return 0;
}
