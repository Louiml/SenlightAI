Write a C++ function `int countUniqueBST(int n)` that returns the number of structurally unique binary search trees (BSTs) that can be formed using exactly `n` distinct integer keys (1 through `n`). The function must use dynamic programming (either top-down with memoization or bottom-up) and must handle the base case `n = 0` (empty tree) and `n = 1` (single node) correctly. Assume `n` is non-negative and fits within a standard `int`. The result is guaranteed to fit in a 32-bit signed integer for the test cases used. Do not use any external libraries beyond the standard C++ headers.
The problem is a classic application of Catalan numbers. If we choose the root as the `i`-th smallest key (1-based index), then the left subtree contains keys `1..i-1` and the right subtree contains keys `i+1..n`. The number of unique BSTs with that root is the product of the number of unique BSTs for the left and right subtrees. Summing over all possible roots gives the total count. The recurrence relation is:
`dp[n] = sum_{i=1 to n} dp[i-1] * dp[n-i]`, with `dp[0] = 1` (empty tree) and `dp[1] = 1`.  
The bottom-up approach iterates from `n=2` to the target, computing each value using previously computed smaller values. This avoids recursion overhead and stack overflow risks.  
Edge cases include `n=0` returning 1, `n=1` returning 1, and handling large `n` where the intermediate products might overflow if not using `long long` during multiplication; but the final result fits in `int`, so we use `long long` internally to be safe.  
Time complexity is `O(n^2)` due to the nested loop, and space complexity is `O(n)` for the DP array.
#include <vector>

// Compute the number of unique BSTs with n nodes using bottom-up DP.
int countUniqueBST(int n) {
    if (n < 0) return 0; // invalid input, but assume non-negative
    std::vector<long long> dp(n + 1, 0);
    dp[0] = 1; // empty tree
    if (n >= 1) dp[1] = 1; // single node

    for (int i = 2; i <= n; ++i) {
        long long total = 0;
        for (int j = 1; j <= i; ++j) {
            total += dp[j - 1] * dp[i - j];
        }
        dp[i] = total;
    }
    return static_cast<int>(dp[n]);
}
#include <cassert>

int main() {
    assert(countUniqueBST(0) == 1);
    assert(countUniqueBST(1) == 1);
    assert(countUniqueBST(2) == 2);
    assert(countUniqueBST(3) == 5);
    assert(countUniqueBST(4) == 14);
    assert(countUniqueBST(5) == 42);
    assert(countUniqueBST(6) == 132);
    assert(countUniqueBST(7) == 429);
    assert(countUniqueBST(8) == 1430);
    assert(countUniqueBST(10) == 16796);
    return 0;
}
