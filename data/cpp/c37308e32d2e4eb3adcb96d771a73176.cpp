// Write a standalone C++ function named `fibonacciNumber` that takes a non-negative integer `n` as input and returns the `n`-th Fibonacci number using a bottom-up dynamic programming approach with a `std::vector<int>` as the memoization table. The function must handle the base cases `n = 0` and `n = 1` correctly, and it must be robust for values of `n` up to 30 (where the result fits into a 32-bit `int`). The solution must not use recursion or any external libraries beyond the standard C++ headers, and it must apply appropriate `const` correctness. The function signature should be `int fibonacciNumber(int n);`.
#include <cassert>

int main() {
    // Base cases
    assert(fibonacciNumber(0) == 0);
    assert(fibonacciNumber(1) == 1);
    
    // Small values
    assert(fibonacciNumber(2) == 1);
    assert(fibonacciNumber(3) == 2);
    assert(fibonacciNumber(4) == 3);
    assert(fibonacciNumber(5) == 5);
    assert(fibonacciNumber(6) == 8);
    
    // Larger value within int range
    assert(fibonacciNumber(10) == 55);
    
    // Edge case: invalid input
    assert(fibonacciNumber(-1) == -1);
    
    // Verify a known larger Fibonacci number (F(20) = 6765)
    assert(fibonacciNumber(20) == 6765);
    
    // Check F(30) = 832040 (still fits in 32-bit int)
    assert(fibonacciNumber(30) == 832040);
    
    return 0;
}
#include <vector>

// Compute the n-th Fibonacci number using bottom-up dynamic programming.
// n must be non-negative; returns -1 for invalid input.
int fibonacciNumber(int n) {
    if (n < 0) return -1;
    
    std::vector<int> dp;
    dp.push_back(0);
    dp.push_back(1);
    
    for (int i = 2; i <= n; ++i) {
        dp.push_back(dp[i - 1] + dp[i - 2]);
    }
    
    return dp[n];
}
// The bottom-up dynamic programming approach builds the Fibonacci sequence iteratively from the base cases upward. We start by creating a `std::vector<int> dp` initialized with the first two Fibonacci numbers: `dp[0] = 0` and `dp[1] = 1`. Then, for each index `i` from 2 to `n` inclusive, we compute `dp[i]` as the sum of the previous two entries: `dp[i-1] + dp[i-2]`, and push this value onto the vector. Finally, we return `dp[n]`. Edge cases: when `n == 0`, the loop does not run, and we return `dp[0]` which is `0`; when `n == 1`, we return `dp[1]` which is `1`. Negative input is not expected, but we can guard by returning `-1` if `n < 0` to avoid undefined behavior (though the main algorithm is defined for non-negative integers). For time complexity, the loop runs exactly `n-1` times, each doing constant work, so it is `O(n)`. Space complexity is `O(n)` because we store all `n+1` Fibonacci numbers in the vector. This matches the given snippet's approach but improves naming and const correctness.
