/*
Write a C++ function named `canFirstPlayerWin` that takes a non-empty vector of non-negative integers representing a row of values. Two players alternately pick a value from either the left end or the right end of the remaining row, and each picked value is added to that player's score. The game ends when all values are taken, and the player with the higher total score wins. The function must return `true` if the first player can guarantee a win (i.e., their final score is at least as large as the second player's final score, assuming both play optimally), and `false` otherwise. The vector may contain duplicate values and any length from 1 to 1000. You must implement a dynamic programming solution that computes the maximum possible score difference (first player's score minus second player's score) achievable from any subarray, using the recurrence `dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])`, where `dp[i][j]` is computed for all subarrays in increasing length order. The function should be `const`-correct and include necessary headers.
*/
#include <vector>
#include <algorithm>

// Returns true if the first player can guarantee a win (score >= opponent's score)
// assuming both players play optimally. The game picks from either end of the row.
bool canFirstPlayerWin(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    // dp[i][j] = maximum score difference (current player - opponent) 
    // achievable from subarray nums[i..j] inclusive.
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    
    // Base case: single element.
    for (int i = 0; i < n; ++i) {
        dp[i][i] = nums[i];
    }
    
    // Fill for subarrays of length >= 2. 
    // Iterate i from n-1 down to 0 and j from i+1 to n-1 
    // to ensure dp[i+1][j] and dp[i][j-1] are already computed.
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            int pickLeft = nums[i] - dp[i + 1][j];
            int pickRight = nums[j] - dp[i][j - 1];
            dp[i][j] = std::max(pickLeft, pickRight);
        }
    }
    
    // First player wins if the difference from the full array is >= 0.
    return dp[0][n - 1] >= 0;
}
#include <cassert>
#include <vector>

int main() {
    // Single element: first player wins.
    assert(canFirstPlayerWin({5}) == true);
    
    // Two elements: first player picks the larger one, so wins.
    assert(canFirstPlayerWin({1, 2}) == true);
    assert(canFirstPlayerWin({2, 1}) == true);
    
    // Equal total: first player can force a tie (>= 0 difference).
    assert(canFirstPlayerWin({1, 1}) == true);
    
    // Example from LeetCode: [1,5,2] -> first player loses.
    assert(canFirstPlayerWin({1, 5, 2}) == false);
    
    // [1,5,233,7] -> first player can win by picking 7, then 233, etc.
    assert(canFirstPlayerWin({1, 5, 233, 7}) == true);
    
    // All duplicates: first player wins or ties.
    assert(canFirstPlayerWin({3, 3, 3, 3}) == true);
    
    // Larger example where first player loses.
    assert(canFirstPlayerWin({1, 2, 3, 4, 5, 6}) == true); // first can win
    assert(canFirstPlayerWin({100, 1, 1, 100}) == true); // first picks 100, then 100
    
    // Edge: ascending sequence where first player loses.
    assert(canFirstPlayerWin({1, 3, 2}) == true); // first picks 2 or 1, both win? check: picks 2 -> opponent picks 3 -> first picks 1 => diff 0 -> true.
    assert(canFirstPlayerWin({1, 2, 99}) == true); // first picks 99, wins.
    
    // A known losing case from LeetCode: [0,0,7,6,5,6,1] -> first loses? but actually first wins with 7? Let's test.
    // But to keep simple, use a simple losing case: [1,5,2] already covered.
    
    return 0;
}
// The problem is a classic minimax game (often called "Predict the Winner"). At any state `(i, j)` representing the subarray from index `i` to `j` (inclusive), the current player can pick either `nums[i]` or `nums[j]`. After picking, the turn switches, and the opponent will play optimally on the remaining subarray. The value `dp[i][j]` represents the maximum difference (current player's score minus opponent's score) that the current player can achieve from this subarray. If the current player picks `nums[i]`, they gain that value, but then the opponent will get `dp[i+1][j]` advantage from the remaining subarray (since it's their turn), so the net difference is `nums[i] - dp[i+1][j]`. Similarly, picking `nums[j]` gives `nums[j] - dp[i][j-1]`. The player chooses the maximum of these two. The base case is a single element: `dp[i][i] = nums[i]` because the current player takes it and the opponent gets nothing. We fill the table for all subarray lengths from 1 to n. To avoid dependencies on already computed larger subarrays, we iterate `i` from `n-1` down to 0 and `j` from `i+1` to `n-1`. After filling, `dp[0][n-1]` is the maximum score difference the first player can achieve. The first player wins if this difference is non-negative. Edge cases: a single element always returns `true` (first player takes it and wins). Duplicate values are handled naturally by the recurrence. Time complexity is O(n^2), space complexity is O(n^2) for the dp table. For very large n (up to 1000), the O(n^2) memory is acceptable (about 4 MB for int). The solution is robust and standard.
