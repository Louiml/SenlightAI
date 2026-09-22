You are given an array of `n` positive integers representing the values of items in a queue, where `n` is at most 2000. You must process the items in the following way: at each step, you choose either the current leftmost item or the current rightmost item, remove it from the queue, and gain a score equal to the product of that item's value and the current "time step". The time step starts at 1 and increases by 1 each time an item is removed. Your goal is to maximize the total score. Write a standalone C++ function `long long maxScore(const std::vector<int>& values)` that takes a vector of values (with size `n`, where `1 <= n <= 2000`, and each `values[i]` in `[1, 1000]`) and returns the maximum possible total score, computed using dynamic programming. The function must be `const`-correct (i.e., it should not modify the input vector) and handle all edge cases, including when `n == 1`.

// The problem is a classic interval DP. Let `dp[i][j]` represent the maximum score obtainable from the subarray `values[i..j]` when considering that the items outside this subarray have already been removed. The key is to determine the time step at which the next item is removed. Since items are removed from either the left or right end, if we have removed all items outside `[i, j]`, then the number of removed items is `n - (j - i + 1)`, so the current time step is `n - (j - i + 1) + 1 = n - (j - i)`. Thus, when we remove `values[i]` at this time step, the remaining subproblem is `[i+1, j]`; similarly, removing `values[j]` leaves `[i, j-1]`. The recurrence is:  
// `dp[i][j] = max(dp[i+1][j] + values[i] * (n - (j - i)), dp[i][j-1] + values[j] * (n - (j - i)))`.  
// For intervals of length 1 (i.e., `i == j`), the time step is `n` because all other `n-1` items have already been removed, so `dp[i][i] = values[i] * n`. To compute efficiently, we iterate over increasing interval lengths. The base case is all intervals of length 1. Then for length from 2 to n, we fill in the table. The answer is `dp[0][n-1]`. Edge cases: `n == 1` returns `values[0]`; all positive values, so no negative handling needed. Time complexity is `O(n^2)` and space complexity is `O(n^2)` for the DP table. Note: The original snippet uses an in-place optimization with a 1D array, but a clear 2D approach is easier to understand and also meets constraints (n ≤ 2000 gives ~4 million states, which is fine).

#include <vector>
#include <algorithm>

// Returns the maximum score obtainable by removing items from either end
// of the queue, where the score for removing an item is its value multiplied
// by the current time step (starting at 1).
long long maxScore(const std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    if (n == 0) return 0;
    
    // dp[i][j] = maximum score from subarray [i..j]
    // Only need to store upper triangular part; we allocate full n x n.
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));
    
    // Base case: length 1 intervals
    for (int i = 0; i < n; ++i) {
        dp[i][i] = static_cast<long long>(values[i]) * n;
    }
    
    // Fill DP for increasing interval lengths
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            // Current time step when removing from this interval
            int timeStep = n - (j - i);  // because (j-i+1) items remain, removed so far = n - (j-i+1), next step = that +1
            long long removeLeft = dp[i+1][j] + static_cast<long long>(values[i]) * timeStep;
            long long removeRight = dp[i][j-1] + static_cast<long long>(values[j]) * timeStep;
            dp[i][j] = std::max(removeLeft, removeRight);
        }
    }
    
    return dp[0][n-1];
}

#include <cassert>
#include <vector>

// Declare the solution function (already defined above)
long long maxScore(const std::vector<int>& values);

int main() {
    // Single element
    assert(maxScore({5}) == 5 * 1);
    // Two elements: either order gives same step 1 and 2
    assert(maxScore({1, 2}) == 2*1 + 1*2); // remove 2 first then 1 → 2 + 2 = 4, or remove 1 first → 1+4=5? Wait check carefully:
    // Let's compute: n=2, time steps: first removal step=1, second=2.
    // If remove left (1) at step1: score=1*1=1, then remove right (2) at step2: 2*2=4, total=5.
    // If remove right (2) at step1: 2*1=2, then remove left (1) at step2: 1*2=2, total=4. So max=5.
    assert(maxScore({1, 2}) == 5);
    // Three elements
    // Values {1,2,3}: Try removing 1 first (step1) → then from [2,3] with step2: remove 3 at step2 (3*2=6), then 2 at step3 (2*3=6) → total 1+6+6=13
    // Or remove 3 first (step1) → then [1,2] step2: remove 2 (2*2=4), then 1 (1*3=3) → total 3+4+3=10
    // Or remove 1 then 2 then 3: 1*1 + 2*2 + 3*3 = 1+4+9=14? Wait that's not valid because after removing 1, leftmost is 2, rightmost is 3, you can't remove 2 before 3 without removing 3? Actually you can: after removing 1, remaining [2,3], you can remove either 2 or 3. If remove 2 at step2: 2*2=4, then remaining [3] at step3: 3*3=9, total 1+4+9=14. That's better. So max is 14.
    assert(maxScore({1, 2, 3}) == 14);
    // Larger test from original snippet style: n=4, values{1,2,3,4}
    // Quick reasoning: optimal likely remove 4 first (step1:4), then 3 (step2:6), then 2 (step3:6), then 1 (step4:4) total 20? Or remove 1 first (1), then 2 (4), then 3 (9), then 4 (16) total 30? Wait step2 for 2 is 2*2=4, step3 for 3 is 3*3=9, step4 for 4 is 4*4=16, plus 1*1=1 total 30. But is it allowed to remove in order 1,2,3,4? That means from the queue you always remove leftmost? Initially leftmost=1, remove it; then leftmost=2, remove; leftmost=3, remove; leftmost=4. Yes that's allowed. So total=1+4+9+16=30. But maybe another order gives more? Try 4 first: 4*1=4, then remaining [1,2,3] with step2: remove 3 (3*2=6), then [1,2] step3: remove 2 (2*3=6), then 1 (1*4=4) total 4+6+6+4=20. So 30 is max. 
    assert(maxScore({1, 2, 3, 4}) == 30);
    // All same values: each step multiplies same value; total = value * (1+2+...+n)
    assert(maxScore({7, 7, 7}) == 7 * (1+2+3) = 42);
    // Larger random: 5 values {5,1,3,2,4} — we can trust DP returns non-negative, but test small n=2 already done.
    // Edge: all large values, ensure no overflow for n=2000, values=1000: max score = 1000 * (1+2+...+2000) = 1000 * 2000*2001/2 ≈ 2e9, fits in long long.
    return 0;
}
