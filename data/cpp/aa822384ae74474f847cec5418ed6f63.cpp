/*
Write a C++ function named `primeFactorization` that takes a positive integer `n` and returns a `std::vector<int>` containing all prime factors of `n` in non-decreasing order. If `n` is 1, return an empty vector. The function must handle `n` up to `2^31 - 1` (i.e., within the range of `int`), and must correctly handle large prime numbers, repeated prime factors, and the special case `n = 2`. The input is guaranteed to be positive, but you should still apply `const` correctness in the parameter list where applicable. Do not include any I/O inside the function; only return the result vector.
*/

#include <vector>
#include <cmath>

// Returns a vector containing all prime factors of n in non-decreasing order.
// For n = 1, returns an empty vector.
std::vector<int> primeFactorization(const int n) {
    std::vector<int> factors;
    int value = n; // mutable copy
    
    // Extract all factors of 2
    while (value % 2 == 0) {
        factors.push_back(2);
        value /= 2;
    }
    
    // Check odd factors from 3 up to sqrt(value)
    if (value > 1) {
        int limit = static_cast<int>(std::sqrt(value));
        for (int i = 3; i <= limit; i += 2) {
            while (value % i == 0) {
                factors.push_back(i);
                value /= i;
                // Update limit when value changes to avoid unnecessary iterations
                limit = static_cast<int>(std::sqrt(value));
            }
        }
        // If value is still > 1, it's a prime factor
        if (value > 1) {
            factors.push_back(value);
        }
    }
    
    return factors;
}

#include <cassert>
#include <vector>

// Include the solution here or link appropriately
// (The function definition from the solution section is assumed to be present.)

int main() {
    // Basic cases
    assert(primeFactorization(1).empty());
    assert(primeFactorization(2) == std::vector<int>({2}));
    assert(primeFactorization(3) == std::vector<int>({3}));
    
    // Repeated factors and mixed
    assert(primeFactorization(12) == std::vector<int>({2, 2, 3}));
    assert(primeFactorization(36) == std::vector<int>({2, 2, 3, 3}));
    
    // Product of two primes
    assert(primeFactorization(15) == std::vector<int>({3, 5}));
    assert(primeFactorization(77) == std::vector<int>({7, 11}));
    
    // Large prime
    assert(primeFactorization(2147483647) == std::vector<int>({2147483647}));
    
    // Power of 2
    assert(primeFactorization(1024) == std::vector<int>({2, 2, 2, 2, 2, 2, 2, 2, 2, 2}));
    
    // Odd composite
    assert(primeFactorization(945) == std::vector<int>({3, 3, 3, 5, 7}));
    
    return 0;
}

// The algorithm is based on trial division. First, repeatedly divide `n` by 2 to extract all factors of 2, appending each `2` to the result. After that, loop from `i = 3` up to `sqrt(n)` (checking only odd numbers to reduce iterations by half). For each `i`, while `n` is divisible by `i`, append `i` and divide `n` by `i`. This ensures that after the loop, if `n` is still greater than 2, it must be a prime factor (because any composite leftover would have a factor ≤ its square root, which would have been found earlier). Edge cases: `n = 1` returns empty; `n = 2` returns `{2}`; `n` itself is a large prime (e.g., `2147483647`) – the loop will run up to `sqrt(n)`≈46340, which is fine, and the final `n` will be appended. Time complexity is O(sqrt(n)) in the worst case (when `n` is prime), but typically much faster for composites with small factors. Space complexity is O(number of prime factors), which is at most O(log n). The use of `sqrt` from `<cmath>` should be done once and stored in a local variable to avoid recomputing.
