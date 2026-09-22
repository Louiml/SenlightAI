// Write a C++ function `std::vector<int> firstNPrimes(int n)` that returns a vector containing the first `n` prime numbers in ascending order. The function must handle invalid input (`n < 1`) by returning an empty vector. For valid `n`, the vector must contain exactly `n` primes, starting from 2, 3, 5, 7, and so on. The function should be efficient enough to handle `n` up to 10,000 without excessive runtime, and must use only the standard library. The output order must be strictly increasing, and the function must not print anything (it returns the data). The implementation must be self-contained, with no global variables, and must use a free function.
// The solution uses a simple trial-division primality test. Starting from 2, we check each integer `m` for primality by testing divisibility up to `sqrt(m)`. If no divisor is found, `m` is prime, so we append it to the result vector and increment a counter. We continue until the counter reaches `n`. For invalid `n < 1`, we return an empty vector immediately. Edge cases: `n = 0` returns empty vector; `n = 1` returns `{2}`. The primality test checks only odd numbers after handling 2 separately to halve the work. For each `m`, the loop runs `O(sqrt(m))` iterations in the worst case. The overall time complexity is `O(n * sqrt(p_n))` where `p_n` is the nth prime (approximately `n log n`), so it is roughly `O(n^1.5 sqrt(log n))`. Space complexity is `O(n)` for storing the result vector. To optimize, we skip even numbers after 2 and only test odd candidates.
#include <vector>
#include <cmath>

// Returns a vector containing the first n prime numbers in ascending order.
// If n < 1, returns an empty vector.
std::vector<int> firstNPrimes(int n) {
    std::vector<int> primes;
    if (n < 1) {
        return primes;
    }
    primes.reserve(n);
    primes.push_back(2); // The only even prime
    int candidate = 3;
    while (static_cast<int>(primes.size()) < n) {
        bool isPrime = true;
        int limit = static_cast<int>(std::sqrt(candidate));
        for (int d = 3; d <= limit; d += 2) {
            if (candidate % d == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(candidate);
        }
        candidate += 2; // Only check odd numbers
    }
    return primes;
}
#include <cassert>
#include <vector>

int main() {
    // Invalid input returns empty vector
    assert(firstNPrimes(0).empty());
    assert(firstNPrimes(-5).empty());

    // Small valid cases
    assert(firstNPrimes(1) == std::vector<int>({2}));
    assert(firstNPrimes(2) == std::vector<int>({2, 3}));
    assert(firstNPrimes(3) == std::vector<int>({2, 3, 5}));

    // A larger case: first 10 primes
    std::vector<int> expected10 = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    assert(firstNPrimes(10) == expected10);

    // Ensure the result is in ascending order and has no duplicates
    auto result = firstNPrimes(100);
    assert(result.size() == 100);
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] > result[i-1]);
    }

    // Check the 100th prime is 541 (the 100th prime number)
    assert(firstNPrimes(100).back() == 541);

    return 0;
}
