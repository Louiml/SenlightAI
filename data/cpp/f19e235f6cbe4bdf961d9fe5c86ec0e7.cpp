Write a C++ function named `countSetPartitions` that takes two non-negative integers `n` and `k` and returns the number of ways to partition a set of `n` distinct elements into exactly `k` non-empty, unlabeled subsets (i.e., the Stirling number of the second kind). The function must handle `n = 0` or `k = 0` gracefully by returning 0. The solution must use dynamic programming with a 2D table, where the value `dp[i][j]` represents the number of partitions of `i` elements into `j` subsets. The recurrence is `dp[i][j] = j * dp[i-1][j] + dp[i-1][j-1]` for `i > 0` and `j > 0`, with base cases `dp[i][1] = 1` and `dp[i][i] = 1` (for `i >= 1`). The function must be self-contained, avoid global variables, and be efficient for moderate inputs (e.g., `n` up to 1000).

// The problem asks for the Stirling number of the second kind, commonly denoted `S(n, k)`, which counts the number of partitions of an `n`-element set into `k` non-empty subsets. The dynamic programming approach builds a table `dp` of size `(n+1) x (k+1)`. The base cases are: if `k == 0`, then there are 0 partitions for any `n` (except `n=0, k=0` which is conventionally 1, but here we return 0 for simplicity given the problem constraints that both can be zero). Also, if `n == 0` and `k > 0`, there are 0 partitions. For `i >= 1`, `dp[i][1] = 1` (all elements in one subset) and `dp[i][i] = 1` (each element in its own subset). For general `j` where `1 < j < i`, we apply the recurrence: either the `i`-th element is placed into an existing subset among the `j` subsets (there are `j` choices, giving `j * dp[i-1][j]`), or it forms a new subset (giving `dp[i-1][j-1]`). We fill the table row by row from `i=1` to `n`, and for each row only `j` from `1` to `min(i, k)` since `j` cannot exceed `i`. Important edge cases: if `k > n`, the answer is 0 because you cannot have more non-empty subsets than elements. If `n == 0` and `k == 0`, mathematically `S(0,0)=1`, but for simplicity and consistency with typical programming assignments, we treat it as 0 (or you may choose to return 1; the task statement says return 0 for any `n=0` or `k=0`). The time complexity is `O(n * k)` because we fill a table of that size. The auxiliary space is also `O(n * k)` for the table. For large `n` and `k`, this is acceptable but could be optimized to `O(k)` space by using a 1D array, but the task explicitly asks for a 2D table for clarity.

#include <vector>

// Count the number of ways to partition a set of n elements into exactly k non-empty subsets.
// Returns 0 if n == 0 or k == 0.
int countSetPartitions(int n, int k) {
    // No partitions possible when either n or k is zero.
    if (n == 0 || k == 0) return 0;
    // Cannot have more subsets than elements.
    if (k > n) return 0;

    // dp[i][j] = number of partitions of i elements into j subsets.
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(k + 1, 0));

    // Base cases.
    for (int i = 1; i <= n; ++i) {
        dp[i][1] = 1; // All elements in one subset.
        if (i <= k) {
            dp[i][i] = 1; // Each element in its own subset.
        }
    }

    // Fill the table bottom-up.
    for (int i = 2; i <= n; ++i) {
        // j cannot exceed i or k.
        int maxJ = (i < k) ? i : k;
        for (int j = 2; j <= maxJ; ++j) {
            // Either element i joins one of j existing subsets (j choices),
            // or it forms a new subset by itself.
            dp[i][j] = j * dp[i - 1][j] + dp[i - 1][j - 1];
        }
    }

    return dp[n][k];
}

#include <cassert>

int main() {
    // Basic cases
    assert(countSetPartitions(3, 2) == 3); // {1,2}|{3}, {1,3}|{2}, {2,3}|{1}
    assert(countSetPartitions(4, 2) == 7);
    assert(countSetPartitions(4, 3) == 6);
    assert(countSetPartitions(5, 3) == 25);
    assert(countSetPartitions(5, 4) == 10);

    // Edge cases
    assert(countSetPartitions(0, 0) == 0);
    assert(countSetPartitions(0, 5) == 0);
    assert(countSetPartitions(5, 0) == 0);
    assert(countSetPartitions(5, 6) == 0); // k > n
    assert(countSetPartitions(1, 1) == 1);
    assert(countSetPartitions(1, 2) == 0);

    // Larger consistent with known Stirling numbers
    assert(countSetPartitions(6, 3) == 90);
    assert(countSetPartitions(7, 4) == 350);
    return 0;
}
