Write a C++ function named `longestIncreasingSubsequence` that takes a `std::vector<int>` and returns the length of the longest strictly increasing subsequence (LIS). A subsequence is obtained by deleting some (or none) elements without changing the order of the remaining elements. The function must handle empty vectors (returning 0), vectors with all equal elements (returning 1), negative numbers, and large inputs up to 10^4 elements efficiently. Implement a recursive dynamic programming solution with memoization using a 2D table where one dimension tracks the current index and the other tracks the previous selected index (using an offset to handle -1). The function should be `const`-correct where possible and include necessary headers, but no `main` function.
// The problem is a classic LIS problem. The provided snippet uses a recursive top-down approach: `solve(i, prev)` returns the length of the LIS starting from index `i` given that the last chosen element is at index `prev` (or -1 if none chosen yet). At each step, we have two choices: either skip the current element, or take it if `nums[prev] < nums[i]`. The base case is when `i >= n`, returning 0. Memoization is stored in `dp[i][prev]`, but since `prev` can be -1, we offset it by 1 (using `prev+1` as the column index). The state space is O(n^2) because `i` ranges from 0..n and `prev` from -1..n-1 (offset to 0..n). Each state does constant work, so time complexity is O(n^2) and space is O(n^2). Edge cases: empty array returns 0, all elements equal yields length 1 (since strictly increasing is required), and negative numbers are handled naturally by comparing values. The memoization avoids recomputation, making the solution suitable for n up to ~2500, but for larger n a O(n log n) approach would be better; however, the task is to implement the given DP pattern.
#include <vector>
#include <cstring>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;

    // dp[i][prev+1] stores the result for solve(i, prev)
    // where prev is the index of last chosen element, or -1 if none.
    // We allocate n+1 columns because prev+1 ranges from 0 to n.
    std::vector<std::vector<int>> dp(n, std::vector<int>(n + 1, -1));

    // Helper lambda for recursion.
    // Since C++14, we can use a std::function or a recursive lambda with explicit std::function.
    std::function<int(int, int)> solve = [&](int i, int prev) -> int {
        if (i >= n) return 0;
        if (prev != -1 && dp[i][prev + 1] != -1) {
            return dp[i][prev + 1];
        }

        int take = 0;
        if (prev == -1 || nums[prev] < nums[i]) {
            take = 1 + solve(i + 1, i);
        }
        int skip = solve(i + 1, prev);

        int result = std::max(take, skip);
        if (prev != -1) {
            dp[i][prev + 1] = result;
        }
        return result;
    };

    return solve(0, -1);
}
#include <cassert>
#include <vector>
#include <functional>

// Forward declaration of the solution (assume it's included above)
int longestIncreasingSubsequence(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // [2,3,7,101]
    assert(longestIncreasingSubsequence({0, 1, 0, 3, 2, 3}) == 4);           // [0,1,2,3]
    assert(longestIncreasingSubsequence({7, 7, 7, 7}) == 1);                 // all equal
    assert(longestIncreasingSubsequence({5}) == 1);                           // single element
    assert(longestIncreasingSubsequence({}) == 0);                           // empty vector

    // Strictly increasing requirement
    assert(longestIncreasingSubsequence({1, 2, 2, 3}) == 3);                 // [1,2,3] not [1,2,2,3]
    assert(longestIncreasingSubsequence({3, 2, 1}) == 1);                    // decreasing

    // Negative numbers
    assert(longestIncreasingSubsequence({-5, -1, -3, 0}) == 3);              // [-5,-3,0] or [-5,-1,0]

    // Larger random test: strictly increasing array of length 10
    std::vector<int> inc(10);
    for (int i = 0; i < 10; ++i) inc[i] = i * 2;
    assert(longestIncreasingSubsequence(inc) == 10);

    // Duplicates with a break
    assert(longestIncreasingSubsequence({2, 2, 3, 1, 4}) == 3);              // [2,3,4] or [1,4]

    return 0;
}
