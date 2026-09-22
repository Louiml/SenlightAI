/*
Write a C++ function that takes an integer `n` and a positive integer `total`, followed by an array of `n` positive integers, and determines whether any subset of the array sums exactly to `total`. The function should return a `bool` indicating whether such a subset exists. The input array uses 1-based indexing (elements are provided as `a[1]` through `a[n]`), and the function must handle cases where `total` is 0 (always solvable with an empty subset) and where `n` is 0 (only solvable if `total` is also 0). The solution must use dynamic programming with a boolean 2D table of size `(n+1) x (total+1)`.
*/

#include <vector>

// Returns true if a subset of the array (1-indexed from a[1] to a[n]) sums exactly to total.
bool subsetSumExists(int n, int total, const std::vector<int>& a) {
    // Ensure a is at least size n+1 (indices 0..n), but only a[1..n] are used.
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(total + 1, false));

    // Base case: empty subset sums to 0.
    dp[0][0] = true;
    // For j > 0, dp[0][j] is already false by initialization.

    for (int i = 1; i <= n; ++i) {
        int current = a[i]; // Note: a[0] is ignored; assume a.size() > n
        for (int j = 0; j <= total; ++j) {
            if (j < current) {
                // Cannot include current element because it exceeds the sum j.
                dp[i][j] = dp[i - 1][j];
            } else {
                // Exclude current or include current.
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - current];
            }
        }
    }

    return dp[n][total];
}

#include <cassert>
#include <vector>

// The solution function is declared above (or included via header).

int main() {
    // Basic cases
    std::vector<int> a1 = {0, 3, 4, 5}; // a[1]=3, a[2]=4, a[3]=5
    assert(subsetSumExists(3, 8, a1) == true);  // 3+5=8
    assert(subsetSumExists(3, 9, a1) == true);  // 4+5=9
    assert(subsetSumExists(3, 2, a1) == false); // no subset sums to 2

    // Total = 0 is always true
    assert(subsetSumExists(3, 0, a1) == true);
    assert(subsetSumExists(0, 0, std::vector<int>(1, 0)) == true); // n=0, total=0

    // n = 0, total > 0 is false
    assert(subsetSumExists(0, 5, std::vector<int>(1, 0)) == false);

    // Single element equal to total
    std::vector<int> a2 = {0, 7};
    assert(subsetSumExists(1, 7, a2) == true);
    assert(subsetSumExists(1, 6, a2) == false);

    // Large total with only one small element
    assert(subsetSumExists(1, 100, a2) == false);

    // Duplicate values
    std::vector<int> a3 = {0, 2, 2, 2};
    assert(subsetSumExists(3, 4, a3) == true);  // 2+2=4
    assert(subsetSumExists(3, 6, a3) == true);  // 2+2+2=6
    assert(subsetSumExists(3, 5, a3) == false); // cannot make odd sum with only 2s

    // All elements larger than total
    std::vector<int> a4 = {0, 10, 20};
    assert(subsetSumExists(2, 5, a4) == false);

    // Edge: total exactly the sum of all elements
    assert(subsetSumExists(2, 30, a4) == true); // 10+20=30

    return 0;
}

// The problem is the classic subset-sum dynamic programming problem. We define `dp[i][j]` as `true` if a subset of the first `i` elements (using 1-based indexing for the array) can sum to `j`. The base case is `dp[0][0] = true` (empty subset sums to 0) and `dp[0][j] = false` for all `j > 0`. For each element `a[i]` (where `i` ranges from 1 to `n`) and each sum `j` from 0 to `total`, we decide: if the current element is larger than the sum `j`, we cannot include it, so `dp[i][j] = dp[i-1][j]` (copy the previous row). Otherwise, we can either exclude the element (giving `dp[i-1][j]`) or include it (giving `dp[i-1][j - a[i]]`); the result is the OR of these two. The answer is `dp[n][total]`. Edge cases include `total == 0` (always true), `n == 0` and `total > 0` (false), and elements larger than `total` (they simply copy the previous state). Time complexity is `O(n * total)` and space complexity is `O(n * total)` for the DP table; there is no need to print the table, so avoid output inside the function.
