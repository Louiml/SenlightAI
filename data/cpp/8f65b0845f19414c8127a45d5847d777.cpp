// Implement a C++ function that, given a vector of non-negative integers, returns a new vector containing only the prime numbers from the input, in ascending order, with each prime appearing exactly once (duplicates removed). The function must handle empty inputs and inputs with no primes by returning an empty vector. The solution should not use any external libraries beyond the standard template library and must be efficient for vectors with up to 100,000 elements where each element is at most 1,000,000.
#include <cassert>
#include <vector>

// The solution function is declared above; test it here.
int main() {
    // Basic case with mixed numbers.
    std::vector<int> test1 = {10, 7, 2, 7, 5, 10};
    assert((uniquePrimes(test1) == std::vector<int>{2, 5, 7}));

    // Empty input.
    std::vector<int> test2;
    assert(uniquePrimes(test2).empty());

    // No primes.
    std::vector<int> test3 = {4, 6, 8, 9, 0, 1};
    assert(uniquePrimes(test3).empty());

    // All primes, already sorted but with duplicates.
    std::vector<int> test4 = {3, 3, 2, 2, 5};
    assert((uniquePrimes(test4) == std::vector<int>{2, 3, 5}));

    // Single prime.
    std::vector<int> test5 = {11};
    assert((uniquePrimes(test5) == std::vector<int>{11}));

    // Large numbers at the boundary.
    std::vector<int> test6 = {1000000, 999983, 999983, 1000000};
    assert((uniquePrimes(test6) == std::vector<int>{999983}));

    // Already unique and sorted ascending.
    std::vector<int> test7 = {2, 3, 5, 7};
    assert((uniquePrimes(test7) == std::vector<int>{2, 3, 5, 7}));

    // Descending input with duplicates of primes and composites.
    std::vector<int> test8 = {15, 13, 13, 12, 11, 10};
    assert((uniquePrimes(test8) == std::vector<int>{11, 13}));
}
#include <vector>
#include <algorithm>
#include <cmath>

// Return a sorted vector of unique prime numbers from the input.
std::vector<int> uniquePrimes(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return {};
    }

    // Find the maximum value to bound the sieve.
    int maxValue = *std::max_element(numbers.begin(), numbers.end());
    if (maxValue < 2) {
        return {};
    }

    // Sieve of Eratosthenes up to maxValue.
    std::vector<bool> isPrime(maxValue + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= maxValue; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= maxValue; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // Sort a copy to deduplicate and ensure ascending order.
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());

    std::vector<int> result;
    for (int i = 0; i < static_cast<int>(sorted.size()); ++i) {
        // Skip duplicates; only consider the first occurrence of each value.
        if (i > 0 && sorted[i] == sorted[i - 1]) {
            continue;
        }
        if (sorted[i] >= 2 && isPrime[sorted[i]]) {
            result.push_back(sorted[i]);
        }
    }
    return result;
}
// First, determine the maximum value in the input vector to bound the sieve. If the vector is empty, return an empty result immediately. Use the Sieve of Eratosthenes to generate a boolean array `isPrime` up to the maximum value. Initialize all entries as true, then mark 0 and 1 as false, and for each number from 2 to sqrt(max), if it is prime, mark all its multiples as non-prime. After building the sieve, sort a copy of the input vector and iterate through it, adding each unique prime number to the result using the sieve to test primality. This avoids duplicate entries because we skip repeated values during the iteration. Edge cases include inputs with 0, 1, or numbers up to the maximum. Time complexity is O(M log log M + N log N) where M is the maximum input value and N is the number of elements, dominated by the sieve and sorting. Space complexity is O(M) for the sieve plus O(N) for the sorted copy and result.
