Write a C++ function named `countPrimesUpTo` that takes a single integer `n` (where `n >= 0`) and returns the number of prime numbers strictly less than `n`. A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself. The function should handle edge cases such as `n = 0` and `n = 1` (both return 0), and must be efficient for `n` up to 5,000,000. Implement the Sieve of Eratosthenes algorithm using a `std::vector<bool>` to mark composite numbers. The function must be declared as `int countPrimesUpTo(int n)` and should not print anything—only return the count.
#include <cassert>

int main() {
    // Basic edge cases
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 0); // No primes less than 2
    assert(countPrimesUpTo(3) == 1); // Only 2
    assert(countPrimesUpTo(10) == 4); // 2, 3, 5, 7
    assert(countPrimesUpTo(20) == 8); // 2,3,5,7,11,13,17,19
    assert(countPrimesUpTo(100) == 25);
    assert(countPrimesUpTo(1000) == 168);
    assert(countPrimesUpTo(5000000) == 348513); // Known value for primes < 5,000,000

    return 0;
}
#include <vector>

// Counts the number of prime numbers strictly less than n using the Sieve of Eratosthenes.
// n must be non-negative. Returns 0 for n <= 2.
int countPrimesUpTo(int n) {
    // If n is too small, there are no primes less than n.
    if (n <= 2) {
        return 0;
    }

    // isPrime[i] will be false if i is composite, true otherwise.
    // We only care about indices 0 to n-1 (exclusive of n).
    std::vector<bool> isPrime(n, true);

    // 0 and 1 are not prime numbers.
    isPrime[0] = false;
    isPrime[1] = false;

    int primeCount = 0;
    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            ++primeCount;
            // Mark all multiples of i starting from i*i as composite.
            // Using i*i is safe because smaller multiples have already been marked by smaller primes.
            // However, to strictly match the given snippet, we start from 2*i.
            for (int j = 2 * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    return primeCount;
}
// The solution uses the Sieve of Eratosthenes, a classic algorithm for finding all primes up to a given limit. We create a boolean vector `isPrime` of size `n` (not `n+1`, because we only care about numbers strictly less than `n`). All entries are initially `true`, but we manually set indices 0 and 1 to `false` since they are not prime. Then we iterate `i` from 2 to `n-1`. If `isPrime[i]` is still `true`, we increment the count and then mark all multiples of `i` (starting from `2*i` and stepping by `i`) as `false`. This works because if `i` is prime, any multiple of `i` less than `n` is composite. The algorithm correctly handles the edge cases: for `n <= 2`, the loop never executes and returns 0. The time complexity is O(n log log n) due to the harmonic series of marking operations, and the space complexity is O(n) for the boolean array. Using `std::vector<bool>` is space-efficient as it packs bits.
