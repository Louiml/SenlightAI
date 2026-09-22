// Write a C++ function `long long countUniqueBSTs(int n)` that computes the number of structurally unique binary search trees (BSTs) that can be formed with exactly `n` distinct nodes (with values 1 through n). The function must handle `n` up to 19 (since the result fits in a 64-bit signed integer; for `n=19` the result is 1767263190, which is within the 64-bit range but overflow could occur for larger `n`). Your implementation must use dynamic programming (bottom-up tabulation) to avoid exponential time, and must be efficient for all inputs in the valid range. Do not use recursion or memoization; implement the tabulation approach. Return the result as a `long long` to prevent overflow, even though intermediate multiplications should be performed carefully (they fit within `long long` for `n <= 19`). Assume `n >= 0` (for `n = 0`, return 1, as an empty tree is considered a valid unique BST).
// The problem is the classic Catalan number sequence: the number of unique BSTs with `n` nodes is the nth Catalan number. The key recurrence is: for a given number of nodes `n`, we pick any node `k` from 1 to `n` as the root. The left subtree will contain `k-1` nodes and the right subtree `n-k` nodes. The total number of trees for that root is the product of the counts for the left and right subtrees. Summing over all possible roots gives `dp[n] = sum_{k=1}^{n} dp[k-1] * dp[n-k]`, with base cases `dp[0] = dp[1] = 1` (an empty tree or a single-node tree). 
// The tabulation approach fills the dp array from `0` up to `n` in increasing order. For each `i` from 2 to `n`, we compute `dp[i]` by iterating over possible roots `j` from 1 to `i`. Since `dp[j-1]` and `dp[i-j]` are already computed (they are smaller than `i`), we can safely compute the sum. 
// Edge cases: `n = 0` returns 1 (empty tree). `n = 1` returns 1. `n` up to 19 is safe; for `n` larger than 19, the result would overflow a 64-bit signed integer, so we restrict the input to `n <= 19`. 
// Time complexity: O(n^2) because we have two nested loops over up to `n` iterations each. Space complexity: O(n) for the dp array.
#include <vector>

// Count the number of structurally unique BSTs with n nodes.
// Uses bottom-up dynamic programming (tabulation).
long long countUniqueBSTs(int n) {
    // Base cases: 0 or 1 node gives exactly 1 tree.
    if (n <= 1) return 1LL;

    // dp[i] = number of unique BSTs with i nodes.
    std::vector<long long> dp(n + 1, 0LL);
    dp[0] = 1LL;
    dp[1] = 1LL;

    // Compute for all node counts from 2 up to n.
    for (int i = 2; i <= n; ++i) {
        // Try each node as the root.
        for (int root = 1; root <= i; ++root) {
            // left subtree size = root - 1, right subtree size = i - root.
            dp[i] += dp[root - 1] * dp[i - root];
        }
    }

    return dp[n];
}
#include <cassert>

int main() {
    // Basic correctness tests
    assert(countUniqueBSTs(0) == 1LL);   // Empty tree
    assert(countUniqueBSTs(1) == 1LL);   // Single node
    assert(countUniqueBSTs(2) == 2LL);   // Two nodes: root 1 or root 2
    assert(countUniqueBSTs(3) == 5LL);   // Classic example
    assert(countUniqueBSTs(4) == 14LL);  // Known Catalan number
    assert(countUniqueBSTs(5) == 42LL);
    assert(countUniqueBSTs(6) == 132LL);

    // Larger values within safe range
    assert(countUniqueBSTs(10) == 16796LL);
    assert(countUniqueBSTs(15) == 9694845LL);
    assert(countUniqueBSTs(19) == 1767263190LL);

    // Ensure function is const-correct usage (not tested directly, but call is fine)
    const int n = 3;
    assert(countUniqueBSTs(n) == 5LL);

    return 0;
}
