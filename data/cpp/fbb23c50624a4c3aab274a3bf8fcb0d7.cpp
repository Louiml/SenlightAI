Write a standalone C++ function named `extractPrimesFromVector` that accepts a constant reference to a `std::vector<int>` and returns a new `std::vector<int>` containing only the prime numbers from the input, preserving their original relative order. The input vector may contain duplicate values, negative numbers, zero, and numbers up to any `int` range. Prime numbers are defined as positive integers greater than 1 that have no positive divisors other than 1 and themselves. The function must correctly handle edge cases: an empty input vector returns an empty output; a vector with no primes returns an empty output; values 0, 1, and all negative numbers are not considered prime; large prime numbers must be tested efficiently. You must not modify the input vector, and the returned vector must be built in the exact order the primes appear in the input. The function should use a simple trial-division primality test with an optimization: check only divisors up to the square root of the candidate, and first eliminate even numbers and multiples of small primes where appropriate. The solution must be self-contained, include all necessary headers, and be implemented with proper `const` correctness and clear comments.
The core algorithm processes each integer in the input vector in order. For each value, we first check if it is less than 2; if so, it is not prime and we skip it. For 2 and 3, we directly mark them as prime. For any value greater than 3, we first check if it is even (divisible by 2) or divisible by 3; if so, it is composite. Otherwise, we perform trial division with a loop starting from 5, incrementing by 6 (i.e., checking 5, 7, 11, 13, ...) because all primes greater than 3 are of the form 6k±1. The loop continues while the divisor squared is less than or equal to the candidate, which is a safe alternative to computing the square root. If any divisor divides the candidate evenly, it is composite; otherwise, it is prime. This handles all edge cases: values 0, 1, negatives return false immediately; duplicates are processed independently; large primes require roughly O(sqrt(n)) iterations per number. For an input of size `n`, with each value up to `m`, the time complexity is O(n * sqrt(m)) in the worst case, but on average much faster due to early exits for even numbers and small factors. Space complexity is O(p) where `p` is the number of primes found, plus O(1) auxiliary space for the loop variables.
#include <vector>
#include <cmath>

// Check if a single integer is prime.
// Returns true only for positive integers greater than 1 with no divisors other than 1 and itself.
bool isPrime(int value) {
    if (value < 2) return false;
    if (value == 2 || value == 3) return true;
    if (value % 2 == 0 || value % 3 == 0) return false;

    // All primes > 3 are of the form 6k ± 1.
    for (int divisor = 5; divisor * divisor <= value; divisor += 6) {
        if (value % divisor == 0 || value % (divisor + 2) == 0) {
            return false;
        }
    }
    return true;
}

// Return a vector containing only the prime numbers from the input, preserving original order.
std::vector<int> extractPrimesFromVector(const std::vector<int>& input) {
    std::vector<int> primes;
    for (int value : input) {
        if (isPrime(value)) {
            primes.push_back(value);
        }
    }
    return primes;
}
#include <cassert>
#include <vector>

// Forward declaration of the function to be tested (if not already included above).
// In a real test, this would be included from the solution header.
std::vector<int> extractPrimesFromVector(const std::vector<int>& input);

int main() {
    // Empty input
    assert(extractPrimesFromVector({}) == std::vector<int>{});

    // No primes (negatives, zero, one, composites)
    assert(extractPrimesFromVector({-7, 0, 1, 4, 6, 8, 9, 10}) == std::vector<int>{});

    // Only primes
    assert(extractPrimesFromVector({2, 3, 5, 7, 11, 13}) == std::vector<int>({2, 3, 5, 7, 11, 13}));

    // Mixed with duplicates and negatives
    assert(extractPrimesFromVector({4, 2, 3, -5, 2, 1, 7, 7, 0, 11}) == std::vector<int>({2, 3, 2, 7, 7, 11}));

    // Large prime and large composite
    assert(extractPrimesFromVector({2147483647, 2147483646}) == std::vector<int>({2147483647}));

    // Single prime and single non-prime
    assert(extractPrimesFromVector({17}) == std::vector<int>({17}));
    assert(extractPrimesFromVector({18}) == std::vector<int>{});

    // Input with very large non-prime (even)
    assert(extractPrimesFromVector({2000000000, 1999999999}) == std::vector<int>({1999999999}));

    return 0;
}
