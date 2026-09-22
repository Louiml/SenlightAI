// Write a C++ function named `climbStairs` that takes a non-negative integer `n` representing the number of stairs in a staircase, and returns the number of distinct ways to reach the top if you can climb either 1 or 2 steps at a time. The function must handle all valid inputs from `0` to `45` inclusively (matching the constraints of the original problem). For `n = 0`, there is exactly 1 way to "do nothing" (reach the top without moving), which is conventionally defined as 1. The solution should use dynamic programming with an iterative approach, storing only the previous two values to achieve optimal space usage, and must be `const`-correct (i.e., the function does not modify any input parameters and can be called on `const` objects if it were a member function). The function should be declared with `const` qualifier if placed inside a class, but as a free function, it should simply not modify any external state. Provide the implementation with necessary includes and a descriptive comment.

#include <cassert>

// Forward declaration of the function under test
int climbStairs(const int n);

int main() {
    // Base cases
    assert(climbStairs(0) == 1);
    assert(climbStairs(1) == 1);

    // Small values
    assert(climbStairs(2) == 2);   // 1+1, 2
    assert(climbStairs(3) == 3);   // 1+1+1, 1+2, 2+1
    assert(climbStairs(4) == 5);   // Fibonacci(5) = 5

    // Larger known values (from Fibonacci sequence offset)
    assert(climbStairs(5) == 8);
    assert(climbStairs(10) == 89);
    assert(climbStairs(20) == 10946);

    // Boundary value (maximum for 32-bit int)
    assert(climbStairs(45) == 1836311903);

    // Ensure const correctness (pass a const int)
    const int n = 7;
    assert(climbStairs(n) == 21);

    return 0;
}

#include <cassert>   // for assert (if needed in tests, but here for completeness)
#include <stdexcept> // not needed but kept for clarity

// Returns the number of distinct ways to climb n stairs, taking 1 or 2 steps at a time.
// The result is well-defined for n >= 0. For n = 0, returns 1 by convention.
int climbStairs(const int n) {
    // Handle base cases directly
    if (n == 0) return 1;
    if (n == 1) return 1;

    // Iterative dynamic programming with constant space
    int prevPrev = 1; // ways for n = 0
    int prev = 1;     // ways for n = 1
    int current = 1;
    for (int i = 2; i <= n; ++i) {
        current = prevPrev + prev;
        prevPrev = prev;
        prev = current;
    }
    return current;
}

// This is a classic Fibonacci-like dynamic programming problem. To reach step `i`, you can come from step `i-1` by taking 1 step, or from step `i-2` by taking 2 steps. Therefore, the number of ways to reach step `i` equals the sum of ways to reach steps `i-1` and `i-2`. Define `dp[i]` as the number of ways to reach step `i`. Base cases: `dp[0] = 1` (one way to stay at ground, conceptually), `dp[1] = 1` (only one step). For `i >= 2`, apply the recurrence `dp[i] = dp[i-1] + dp[i-2]`. The result is `dp[n]`. Edge cases: for `n = 0`, return 1; for `n = 1`, return 1. The naive approach would use an array of size `n+1`, but we can optimize space by keeping only the last two computed values, reducing space to O(1). Time complexity is O(n) because we compute each value once, and for `n` up to 45 this is trivial. The numbers grow quickly but fit within a 32-bit integer, as `dp[45] = 1836311903`, which is less than 2^31-1. No additional edge cases beyond `n = 0` need special handling, but we must ensure the function correctly returns 1 for `n = 0` and `n = 1`.
