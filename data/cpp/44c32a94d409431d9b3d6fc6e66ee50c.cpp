/*
Write a C++ function named `countUniqueBinarySearchTrees` that takes a non-negative integer `n` as input and returns the number of structurally distinct binary search trees (BSTs) that can be formed using exactly `n` nodes with values from 1 to `n`. The function must handle `n = 0` correctly, returning 1 (the empty tree), and must be efficient for `n` up to at least 19, where the answer fits within a 32-bit `int`. The solution must avoid recursion and use dynamic programming.
*/
#include <vector>

// Returns the number of distinct binary search trees that can be formed with n nodes.
// n is the number of nodes (non-negative integer).
int countUniqueBinarySearchTrees(int n) {
    // Base case: 0 and 1 nodes have exactly 1 BST.
    if (n <= 1) return 1;
    
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1; // empty tree
    dp[1] = 1; // single node
    
    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            // j nodes in left subtree, i-j-1 nodes in right subtree
            dp[i] += dp[j] * dp[i - j - 1];
        }
    }
    
    return dp[n];
}
#include <cassert>

// The function is declared in the solution section, but we include it here for completeness.
int main() {
    // Base cases
    assert(countUniqueBinarySearchTrees(0) == 1); // empty tree
    assert(countUniqueBinarySearchTrees(1) == 1); // single node
    
    // Known Catalan numbers: C_0=1, C_1=1, C_2=2, C_3=5, C_4=14, C_5=42
    assert(countUniqueBinarySearchTrees(2) == 2);
    assert(countUniqueBinarySearchTrees(3) == 5);
    assert(countUniqueBinarySearchTrees(4) == 14);
    assert(countUniqueBinarySearchTrees(5) == 42);
    
    // Larger values within int range
    assert(countUniqueBinarySearchTrees(10) == 16796);
    assert(countUniqueBinarySearchTrees(15) == 9694845);
    assert(countUniqueBinarySearchTrees(19) == 1767263190); // largest within int
    
    return 0;
}
// The problem is the classic Catalan number application. For a given number of nodes `n`, we consider every possible root value `root` from 1 to `n`. The left subtree will contain `root-1` nodes (values smaller than root), and the right subtree will contain `n-root` nodes (values larger than root). The number of distinct BSTs with that root is the product of the number of distinct BSTs for the left and right subtrees. Summing over all possible roots gives the total count.
//
// The recurrence is: `dp[i] = sum_{j=0}^{i-1} dp[j] * dp[i-j-1]`, where `dp[i]` is the number of BSTs with `i` nodes. We initialize `dp[0] = 1` (empty tree) and `dp[1] = 1` (single node). We then compute for `i` from 2 to `n` using nested loops. The result `dp[n]` is the answer.
//
// Edge cases: `n=0` should return 1 (empty tree). Since the answer grows quickly (Catalan numbers), we use `int` as specified; for `n` beyond 19 the result overflows, but the problem constraints keep `n` small.
//
// Time complexity: O(n²) due to the double loop. Space complexity: O(n) for the DP array.
