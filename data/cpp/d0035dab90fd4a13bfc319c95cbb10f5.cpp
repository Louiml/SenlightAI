Write a C++ function that takes a positive integer `n` and returns a vector of integers containing all positive divisors of `n` in ascending order. The function should handle edge cases including `n = 1` (divisor is just 1) and large values up to `int` maximum. The returned vector must be sorted in non-decreasing order. Do not include the number 0 or any negative numbers; only positive divisors.
#include <cassert>
#include <vector>

// Function declaration (for testing; normally would be in a header)
std::vector<int> getDivisors(int n);

int main() {
    assert(getDivisors(1) == std::vector<int>({1}));
    assert(getDivisors(2) == std::vector<int>({1, 2}));
    assert(getDivisors(6) == std::vector<int>({1, 2, 3, 6}));
    assert(getDivisors(12) == std::vector<int>({1, 2, 3, 4, 6, 12}));
    assert(getDivisors(16) == std::vector<int>({1, 2, 4, 8, 16}));
    assert(getDivisors(17) == std::vector<int>({1, 17}));
    assert(getDivisors(100) == std::vector<int>({1, 2, 4, 5, 10, 20, 25, 50, 100}));
    // Edge case: very large prime (need to be careful with overflow, but 1000003 is int-safe)
    assert(getDivisors(1000003) == std::vector<int>({1, 1000003}));
    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

// Returns all positive divisors of n in ascending order.
std::vector<int> getDivisors(int n) {
    std::vector<int> divisors;
    if (n <= 0) return divisors; // handle invalid input gracefully

    // Use sqrt optimization
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }
    std::sort(divisors.begin(), divisors.end());
    return divisors;
}
// The solution iterates from 1 up to `n` and checks if `i` divides `n` exactly (i.e., `n % i == 0`). If so, append `i` to the result vector. This brute-force approach is straightforward and correct, but for large `n` the loop runs `n` times, giving O(n) time. A more efficient approach loops only up to `sqrt(n)`, adding both `i` and `n/i` when `i` is a divisor, then sorting the result. However, the simpler O(n) version is acceptable for moderate inputs and easiest to verify. Edge cases: `n = 1` should return `{1}`; if `n` is prime, only `1` and `n` are divisors; if `n` is a perfect square, the square root divisor should appear only once. For the reference solution, I'll use the sqrt optimization: for each `i` from 1 to `sqrt(n)`, if `i` divides `n`, push `i` and (if different) `n/i`; then sort the vector. Time complexity is O(√n + k log k) where k is the number of divisors, and space is O(k).
