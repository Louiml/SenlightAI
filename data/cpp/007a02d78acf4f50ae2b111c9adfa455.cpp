Write a C++ function that takes a non-negative integer `n` and returns the number of prime numbers that are strictly less than `n`. The function must handle the edge cases where `n = 0` or `n = 1` (both return 0) and use an efficient sieve-based approach rather than checking each number individually. The function should be named `countPrimesLessThan` and accept an `int` parameter. The input is guaranteed to be non-negative, but you should handle values up to at least 10^6 efficiently.

#include <cassert>

int main() {
    // Edge cases: n <= 2
    assert(countPrimesLessThan(0) == 0);
    assert(countPrimesLessThan(1) == 0);
    assert(countPrimesLessThan(2) == 0);

    // Small n
    assert(countPrimesLessThan(3) == 1); // {2}
    assert(countPrimesLessThan(4) == 2); // {2,3}
    assert(countPrimesLessThan(5) == 2); // {2,3}
    assert(countPrimesLessThan(10) == 4); // {2,3,5,7}
    assert(countPrimesLessThan(20) == 8); // {2,3,5,7,11,13,17,19}
    assert(countPrimesLessThan(100) == 25);

    // Larger n for efficiency check
    assert(countPrimesLessThan(1000000) == 78498);

    return 0;
}

#include <vector>

// Count the number of prime numbers strictly less than n using Sieve of Eratosthenes.
int countPrimesLessThan(int n) {
    if (n <= 2) return 0; // No primes below 2

    std::vector<bool> isComposite(n, false); // index 0..n-1
    int count = 0;

    for (int i = 2; i < n; ++i) {
        if (!isComposite[i]) {
            ++count; // i is prime
            // Mark multiples of i as composite, starting from i*i for efficiency
            if ((long long)i * i < n) {
                for (int j = i * i; j < n; j += i) {
                    isComposite[j] = true;
                }
            }
        }
    }
    return count;
}

// The classic solution uses the Sieve of Eratosthenes. We maintain a boolean vector `isComposite` of size `n` (since we only care about numbers 0 to n-1), initialized to `false`. We iterate `i` from 2 to `n-1`. If `i` is not marked as composite, it is prime, so we increment the count. Then we mark all multiples of `i` (starting from `i*i` for efficiency) as composite. The inner loop can start at `j = i` (or `i*i` if we are certain `i*i` won't overflow and fits within the vector size) and multiply `j` by `i` each step. Important edge cases: if `n <= 2`, there are no primes less than `n`, so return 0. Also, we must ensure that the vector size is at least `n` (when `n = 0` or `1`, the vector will be empty, and the loop will not run). Time complexity is \(O(n \log \log n)\), and space complexity is \(O(n)\). The solution avoids checking all numbers individually, so it scales well.
