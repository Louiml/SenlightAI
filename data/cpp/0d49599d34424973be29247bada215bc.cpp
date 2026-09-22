Write a C++ function that, given a positive integer `n` (where `n >= 2`), returns a `std::vector<int>` containing all distinct prime factors of `n` in ascending order. Each prime factor should appear exactly once, regardless of its multiplicity in the prime factorization. For example, for `n = 12` (which factors as `2^2 * 3`), the function should return `{2, 3}`; for `n = 97` (a prime), it should return `{97}`. The function must be efficient for values up to `10^9` and must not include `1` in the result. The output vector must be sorted ascending.
// The core algorithm follows a trial-division approach using the fact that any composite number has at least one prime factor ≤ sqrt(n). We iterate `i` from 2 up to `sqrt(n)`. For each `i`, if `n % i == 0`, then `i` is a prime factor, so we add it to the result and then repeatedly divide `n` by `i` to remove all occurrences of this factor. After this inner loop, `n` is no longer divisible by `i`. We continue to the next `i`. After the loop, if the remaining `n` is greater than 1, it must be a prime factor (since any composite factor would have been caught earlier), so we append it. Edge cases: when `n` is prime, the loop finds no divisors and we append `n` itself; when `n` is a perfect square of a prime (e.g., 49), the loop catches `7` and reduces `n` to 1, so nothing extra is appended. The loop's upper bound `i <= sqrt(n)` is safe because we keep dividing `n` down as we find factors, so `n` shrinks. Time complexity is O(sqrt(n)) in the worst case (e.g., when `n` is prime), but typically much faster because `n` decreases; space complexity is O(k) where `k` is the number of distinct prime factors (bounded by about 9 for 10^9).
#include <vector>
#include <cmath>

// Returns a sorted vector of all distinct prime factors of n (n >= 2).
std::vector<int> distinctPrimeFactors(int n) {
    std::vector<int> factors;
    int limit = static_cast<int>(std::sqrt(n));
    for (int i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            factors.push_back(i);
            // Remove all occurrences of this factor.
            while (n % i == 0) {
                n /= i;
            }
            // Update limit because n has become smaller.
            limit = static_cast<int>(std::sqrt(n));
        }
    }
    // If n is still > 1, it is a prime factor.
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}
#include <cassert>
#include <vector>

int main() {
    assert(distinctPrimeFactors(2) == std::vector<int>({2}));
    assert(distinctPrimeFactors(4) == std::vector<int>({2}));
    assert(distinctPrimeFactors(12) == std::vector<int>({2, 3}));
    assert(distinctPrimeFactors(97) == std::vector<int>({97}));
    assert(distinctPrimeFactors(100) == std::vector<int>({2, 5}));
    assert(distinctPrimeFactors(360) == std::vector<int>({2, 3, 5}));
    assert(distinctPrimeFactors(49) == std::vector<int>({7}));
    assert(distinctPrimeFactors(1'000'000'000) == std::vector<int>({2, 5}));
    assert(distinctPrimeFactors(999'983) == std::vector<int>({999'983}));
    assert(distinctPrimeFactors(8) == std::vector<int>({2}));
}
