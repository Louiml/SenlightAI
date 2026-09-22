Write a C++ function `long long nthFibonacci(int n)` that, given a non-negative integer `n`, returns the `n`-th Fibonacci number using the iterative dynamic programming approach with an array of size at least `n+1`. The Fibonacci sequence is defined as `F(0) = 0`, `F(1) = 1`, and `F(k) = F(k-1) + F(k-2)` for `k >= 2`. The function must handle `n` up to 90 (where the 90th Fibonacci number is 2880067194370816120, which fits in a 64-bit signed integer). For invalid negative `n`, return 0. Do not use recursion, and avoid unnecessary temporary variables beyond the DP array.

#include <cassert>

int main() {
    assert(nthFibonacci(0) == 0);
    assert(nthFibonacci(1) == 1);
    assert(nthFibonacci(2) == 1);
    assert(nthFibonacci(10) == 55);
    assert(nthFibonacci(50) == 12586269025LL);
    assert(nthFibonacci(90) == 2880067194370816120LL);
    assert(nthFibonacci(-5) == 0);
    assert(nthFibonacci(7) == 13);
    assert(nthFibonacci(20) == 6765);
    assert(nthFibonacci(30) == 832040);
}

#include <vector>

// Return the nth Fibonacci number (0 <= n <= 90); returns 0 for negative n.
long long nthFibonacci(int n) {
    if (n <= 0) {
        return 0;
    }
    std::vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// The solution builds the Fibonacci sequence iteratively from the base cases up to the requested index. Initialize a dynamic programming array `dp` of size `max(n+1, 2)` to safely handle `n=0` and `n=1`. Set `dp[0] = 0` and `dp[1] = 1`. Then for each `i` from 2 to `n`, compute `dp[i] = dp[i-1] + dp[i-2]`. The result is `dp[n]`. Edge cases: if `n <= 0`, return 0 (covers negative and zero). For `n=90`, the largest value fits within `long long` (max ~9.22e18), and no overflow occurs. Time complexity is `O(n)` due to the single loop, and space complexity is `O(n)` for the DP array. The iterative approach avoids the exponential overhead of naive recursion.
