Write a C++ function `std::vector<long long> primeFactors(long long n)` that takes a single integer `n` (which may be positive, negative, or zero) and returns a vector containing all its positive prime factors in ascending order. For `n = 0` or `n = 1` or `n = -1`, return an empty vector. For negative numbers, use the absolute value. Each prime factor should appear only once (no duplicates), even if it divides the number multiple times. The output should be sorted in increasing order. For example, `primeFactors(60)` returns `{2, 3, 5}`, `primeFactors(-28)` returns `{2, 7}`, and `primeFactors(13)` returns `{13}`.

The solution first handles edge cases: if `n` is 0, 1, or -1, return an empty vector immediately (these have no prime factors). Otherwise, take the absolute value of `n` to work with positive numbers. Then use trial division: starting from `d = 2`, while `d * d <= n`, check if `d` divides `n`. If it does, add `d` to the result and then repeatedly divide `n` by `d` until it’s no longer divisible (this ensures we skip any composite multiples and only collect each prime once). After removing all factors of `d`, increment `d` (or handle `d = 2` then only odd numbers for efficiency). After the loop, if `n` is greater than 1, then `n` itself is a prime factor and should be appended. The algorithm runs in `O(√n)` time in the worst case (when `n` is prime) and uses `O(k)` auxiliary space for the result vector where `k` is the number of distinct prime factors. Edge cases include handling negative input, very large primes, and perfect squares.

#include <vector>
#include <cmath>
#include <cstdlib>

// Returns a sorted vector of distinct prime factors of the absolute value of n.
// Returns an empty vector for n = 0, 1, -1, or any other number with no prime factors.
std::vector<long long> primeFactors(long long n) {
    std::vector<long long> factors;
    if (n == 0 || n == 1 || n == -1) {
        return factors;
    }
    
    long long value = std::llabs(n);
    
    // Handle factor 2 separately
    if (value % 2 == 0) {
        factors.push_back(2);
        while (value % 2 == 0) {
            value /= 2;
        }
    }
    
    // Check odd divisors from 3 upwards
    for (long long d = 3; d * d <= value; d += 2) {
        if (value % d == 0) {
            factors.push_back(d);
            while (value % d == 0) {
                value /= d;
            }
        }
    }
    
    // If value is still greater than 1, it's a prime factor
    if (value > 1) {
        factors.push_back(value);
    }
    
    return factors;
}

#include <cassert>
#include <vector>

int main() {
    // Basic positive cases
    assert((primeFactors(60) == std::vector<long long>{2, 3, 5}));
    assert((primeFactors(13) == std::vector<long long>{13}));
    assert((primeFactors(2) == std::vector<long long>{2}));
    assert((primeFactors(1) == std::vector<long long>{}));
    assert((primeFactors(0) == std::vector<long long>{}));
    
    // Negative numbers use absolute value
    assert((primeFactors(-28) == std::vector<long long>{2, 7}));
    assert((primeFactors(-97) == std::vector<long long>{97}));
    assert((primeFactors(-1) == std::vector<long long>{}));
    
    // Perfect squares and powers
    assert((primeFactors(100) == std::vector<long long>{2, 5}));
    assert((primeFactors(64) == std::vector<long long>{2}));
    assert((primeFactors(81) == std::vector<long long>{3}));
    
    // Large prime
    assert((primeFactors(104729) == std::vector<long long>{104729}));
    
    // Product of distinct primes
    assert((primeFactors(2 * 3 * 5 * 7 * 11) == std::vector<long long>{2, 3, 5, 7, 11}));
    
    return 0;
}
