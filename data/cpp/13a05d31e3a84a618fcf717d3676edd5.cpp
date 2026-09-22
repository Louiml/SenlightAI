/*
Write a C++ function that implements the Sieve of Eratosthenes algorithm to find all prime numbers up to a given positive integer `n`. The function should accept a single parameter `n` (the upper limit, inclusive) and return a `std::vector<int>` containing all prime numbers from 2 up to `n` in ascending order. The function must handle edge cases where `n` is less than 2 (returning an empty vector), use proper memory management without dynamic raw pointers (prefer `std::vector<bool>` or similar), and avoid unnecessary computation by only iterating up to the square root of `n` for marking composites. The solution must be self-contained with appropriate `const` correctness and include only the free function — no `main` function in the solution section.
*/

#include <vector>
#include <cmath>

// Returns all prime numbers in the range [2, n] using the Sieve of Eratosthenes.
std::vector<int> sieveOfEratosthenes(int n) {
    std::vector<int> primes;
    if (n < 2) {
        return primes;
    }

    // Boolean sieve: index i represents whether i is prime.
    std::vector<bool> isPrime(n + 1, true);

    // Mark composites: for each i up to sqrt(n), mark multiples starting from i*i.
    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // Collect all primes.
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

#include <cassert>
#include <vector>

// Function declaration (assuming the solution is in the same file or included).
std::vector<int> sieveOfEratosthenes(int n);

int main() {
    // Edge case: n < 2
    assert(sieveOfEratosthenes(0).empty());
    assert(sieveOfEratosthenes(1).empty());
    assert(sieveOfEratosthenes(-5).empty());

    // Small cases
    assert(sieveOfEratosthenes(2) == std::vector<int>({2}));
    assert(sieveOfEratosthenes(3) == std::vector<int>({2, 3}));
    assert(sieveOfEratosthenes(10) == std::vector<int>({2, 3, 5, 7}));

    // Larger case with known primes
    std::vector<int> expectedUpTo30 = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    assert(sieveOfEratosthenes(30) == expectedUpTo30);

    // n = 100 (contains 25 primes)
    assert(sieveOfEratosthenes(100).size() == 25);
    assert(sieveOfEratosthenes(100).front() == 2);
    assert(sieveOfEratosthenes(100).back() == 97);

    // Perfect square boundary (n = 49, sqrt = 7)
    std::vector<int> expectedUpTo49 = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    assert(sieveOfEratosthenes(49) == expectedUpTo49);

    // Large prime n
    assert(sieveOfEratosthenes(97).back() == 97);
    assert(sieveOfEratosthenes(97).size() == 25);

    // n = 1000 (168 primes)
    assert(sieveOfEratosthenes(1000).size() == 168);
    assert(sieveOfEratosthenes(1000).back() == 997);

    return 0;
}

// The classic Sieve of Eratosthenes works by creating a boolean array (or bit-accessible container like `std::vector<bool>`) of size `n+1`, initially assuming all numbers from 2 to `n` are prime (set to `true`). Then, for each integer `i` starting at 2 up to `sqrt(n)`, if `i` is still marked as prime, all multiples of `i` starting from `i*i` (since smaller multiples have already been marked by smaller primes) are marked as composite (`false`). After this marking pass, iterate from 2 to `n` and collect any index still marked `true` into a result vector. Edge cases: if `n < 2`, return an empty vector immediately to avoid allocating unnecessary memory. For `n == 2`, the loop from `i=2; i<=sqrt(2)` does not execute (since `sqrt(2)` ≈ 1.414 < 2), so `2` remains marked and gets collected correctly. Using `i*i` as the starting point for j avoids redundant marking and slightly improves performance. Time complexity is O(n log log n) for the marking phase, plus O(n) for collection, dominated by O(n log log n). Space complexity is O(n) for the boolean sieve array plus O(π(n)) for the result vector, where π(n) is the number of primes ≤ n.
