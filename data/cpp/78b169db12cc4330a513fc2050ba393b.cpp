Write a C++ function named `countAlmostPrimes` that takes an integer `n` (the number of values to examine) and a vector of integers, and returns the count of numbers in the vector that are "almost prime" — meaning the sum of all positive divisors of the number equals the number itself plus 1. This is equivalent to the number being prime, but the function should compute it by directly summing divisors. The function must be robust: `n` will match the vector size, numbers can be as large as 100,000, and the function must correctly handle edge cases like `1` (where the divisor sum is 1, which is not equal to 2, so not counted) and large primes. Do not use any primality tests; only sum all divisors from 1 up to the number itself.

// The core algorithm is straightforward: for each candidate number `x`, iterate `d` from 1 to `x`, checking if `x % d == 0`, and accumulate the sum of such divisors. If the sum equals `x + 1`, increment the counter. The condition `sum == x + 1` is exactly the definition of a prime number (since the only divisors of a prime are 1 and itself), but the solution intentionally uses the divisor-sum approach to match the original snippet's logic. Important edge cases: `x = 1` → divisor sum is 1, but `1 + 1 = 2`, so not counted; `x = 2` → divisors 1+2=3, equals `2+1=3`, counted; very large prime like 99991 requires iterating all the way to 99991 — O(n) per number. Time complexity is O(n * m) where `n` is the count of numbers and `m` is the maximum value (up to 100,000). Space complexity is O(1) beyond the input vector storage. The function must be `const`-correct: take the vector by `const&`, use `int` for sums but be aware that for numbers up to 100,000, the divisor sum won't exceed ~100,000^2 which fits in `long long` to be safe, but the original used `int` — we'll use `long long` internally to avoid overflow for larger ranges, though the problem constraints keep it within `int`. The solution uses a helper `divisorSum` function that takes a `long long` and returns `long long`, but the main counting function is the public interface.

#include <vector>

// Helper: compute sum of all positive divisors of x.
// x is expected to be positive. Returns sum as long long to prevent overflow.
long long divisorSum(long long x) {
    long long sum = 0;
    for (long long d = 1; d <= x; ++d) {
        if (x % d == 0) {
            sum += d;
        }
    }
    return sum;
}

// Count numbers in values whose divisor sum equals value + 1.
// n is the number of elements in values (must match values.size()).
int countAlmostPrimes(int n, const std::vector<int>& values) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        long long x = values[i];
        if (divisorSum(x) == x + 1) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Declare the function (since no main in solution, tests provide it)
long long divisorSum(long long x);
int countAlmostPrimes(int n, const std::vector<int>& values);

int main() {
    // Test basic primes and non-primes
    assert(countAlmostPrimes(3, {2, 3, 4}) == 2); // 2 and 3 are prime-like, 4 is not
    assert(countAlmostPrimes(1, {1}) == 0); // 1 fails
    assert(countAlmostPrimes(1, {2}) == 1); // 2 passes
    assert(countAlmostPrimes(1, {6}) == 0); // 6 sum=12, 6+1=7
    // Larger prime
    assert(countAlmostPrimes(1, {99991}) == 1); // 99991 is prime
    // Mixed with small and large, including 1 and 0 (0 should be safe? x=0 would loop forever, but we won't pass 0)
    assert(countAlmostPrimes(4, {1, 2, 3, 5}) == 3); // 2,3,5 pass
    assert(countAlmostPrimes(3, {7, 8, 9}) == 1); // only 7 passes
    // Edge: n=0
    assert(countAlmostPrimes(0, {}) == 0);
    // Duplicates
    assert(countAlmostPrimes(3, {11, 11, 12}) == 2);
    return 0;
}
