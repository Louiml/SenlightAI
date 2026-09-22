/*
Write a C++ function `countWaysToClimb(int n)` that returns the number of distinct ways to climb a staircase with `n` steps, where at each step you can climb either 1 or 2 steps. The function must handle small positive integers (1 ≤ n ≤ 90) without overflow (use `long long` for the return type), and must be efficient for large `n`. The staircase has exactly `n` steps; for `n = 0`, return 1 (the empty way to reach the top), and for `n = 1`, return 1. The solution should not use recursion (to avoid exponential time) and must compute the result iteratively, storing only the last two computed values (not a full array) to achieve O(1) auxiliary space. Provide a clean, self-contained free function with appropriate `const` correctness, and include necessary headers.
*/

#include <cstdint>   // for int64_t
#include <limits>    // for numeric_limits

// Return the number of distinct ways to climb n steps, taking 1 or 2 steps at a time.
// For n=0, returns 1 (empty way). For n>=1, returns Fibonacci(n+1) (with F(1)=1, F(2)=1).
// n is assumed to be in the range [0, 90] to avoid 64-bit overflow.
long long countWaysToClimb(int n) {
    if (n <= 1) {
        // base cases: 0 steps -> 1 way, 1 step -> 1 way
        return 1;
    }
    long long prev = 1;  // ways for n = 0
    long long curr = 1;  // ways for n = 1
    for (int step = 2; step <= n; ++step) {
        long long next = prev + curr;  // sum of two previous ways
        prev = curr;
        curr = next;
    }
    return curr;
}

#include <cassert>

int main() {
    // Base cases
    assert(countWaysToClimb(0) == 1);
    assert(countWaysToClimb(1) == 1);
    // Small known values: Fibonacci sequence shifted: n=2 -> 2, n=3 -> 3, n=4 -> 5
    assert(countWaysToClimb(2) == 2);
    assert(countWaysToClimb(3) == 3);
    assert(countWaysToClimb(4) == 5);
    // Another small value: n=5 -> 8
    assert(countWaysToClimb(5) == 8);
    // Larger than the given snippet example: n=10 -> 89 (Fibonacci(11))
    assert(countWaysToClimb(10) == 89);
    // Test n=90 (largest safe value) – should be 2880067194370816120 (Fibonacci(91))
    assert(countWaysToClimb(90) == 2880067194370816120LL);
    // Consistency check: ways(n) = ways(n-1) + ways(n-2) for n>=2
    for (int n = 2; n <= 20; ++n) {
        assert(countWaysToClimb(n) == countWaysToClimb(n-1) + countWaysToClimb(n-2));
    }
    return 0;
}

// This problem is equivalent to computing the Fibonacci sequence shifted by one, because the number of ways to reach step `i` is the sum of ways to reach step `i-1` (by taking a 1-step from there) and step `i-2` (by taking a 2-step from there). The base cases are `ways(0)=1` (one way to stand at the ground) and `ways(1)=1` (only one way: one 1-step). For `n ≥ 2`, `ways(n) = ways(n-1) + ways(n-2)`. To avoid overflow, we use `long long` (64-bit), which can hold Fibonacci numbers up to `F(93)` ≈ 1.2e19, but we restrict to `n ≤ 90` to be safe (F(90) ≈ 2.88e18). Edge cases: `n=0` and `n=1` both return 1. Since we only need the last two values, we can iterate from `2` to `n`, updating two variables, giving O(n) time and O(1) extra space. The iterative approach avoids the exponential blowup of naive recursion.
