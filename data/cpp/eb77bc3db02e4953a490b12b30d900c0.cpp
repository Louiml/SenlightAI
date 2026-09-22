/*
Write a C++ function `maxValueIncludingRepeatedItems` that takes an integer `N` (number of item types), an integer `W` (maximum knapsack capacity), an array `values` of size `N`, and an array `weights` of size `N`, and returns the maximum total value that can be obtained by selecting items, where each item type can be used any number of times (unbounded). Each item type has a given weight and value, and the total weight of selected items must not exceed `W`. The function must handle cases where no item can be placed (return 0), and must work for `N ≥ 1`, `W ≥ 0`, and positive integer weights and values. The solution must use a top-down memoized recursive approach with a 2D DP table sized `N × (W+1)`, initialized to -1. You are not allowed to use bottom-up tabulation or greedy techniques.
*/
#include <vector>
#include <algorithm>
#include <climits>

// Unbounded knapsack: maximize total value with unlimited copies of each item.
// N: number of item types, W: capacity, values: array of values, weights: array of weights.
int maxValueIncludingRepeatedItems(int N, int W, const int values[], const int weights[]) {
    // DP table: dp[ind][w] = max value using items 0..ind with capacity w; -1 means not computed.
    std::vector<std::vector<int>> dp(N, std::vector<int>(W + 1, -1));

    // Recursive helper using lambda with captures.
    // Using std::function for clarity; could use a separate function but this is self-contained.
    std::function<int(int, int)> solve = [&](int ind, int w) -> int {
        // Base case: only item type 0 available.
        if (ind == 0) {
            return (w / weights[0]) * values[0];
        }
        // Return memoized result if computed.
        if (dp[ind][w] != -1) return dp[ind][w];

        // Option 1: do not take any copy of item 'ind'.
        int notTake = solve(ind - 1, w);

        // Option 2: take one copy of item 'ind' (if fits) and stay at same index.
        int take = INT_MIN;
        if (w >= weights[ind]) {
            take = values[ind] + solve(ind, w - weights[ind]);
        }

        return dp[ind][w] = std::max(notTake, take);
    };

    // If capacity or items are zero (edge case) return 0.
    if (N == 0 || W == 0) return 0;

    int result = solve(N - 1, W);
    // Guard against INT_MIN (should not happen with positive values and weights).
    return result == INT_MIN ? 0 : result;
}
#include <cassert>
#include <vector>

int main() {
    // Test case 1: Basic unbounded knapsack from the problem statement.
    int val1[] = {1, 4, 5, 7};
    int wt1[] = {1, 3, 4, 5};
    assert(maxValueIncludingRepeatedItems(4, 8, val1, wt1) == 11); // Example: take two of weight 3 (value 4 each) => 8 weight, 8 value; or take one weight 5 + one weight 3 => 7+4=11
    
    // Test case 2: Capacity zero -> no value.
    int val2[] = {10, 20};
    int wt2[] = {2, 3};
    assert(maxValueIncludingRepeatedItems(2, 0, val2, wt2) == 0);

    // Test case 3: All items heavier than capacity -> value 0.
    int val3[] = {100};
    int wt3[] = {10};
    assert(maxValueIncludingRepeatedItems(1, 5, val3, wt3) == 0);

    // Test case 4: Single item that fits multiple times.
    int val4[] = {3};
    int wt4[] = {2};
    assert(maxValueIncludingRepeatedItems(1, 6, val4, wt4) == 9); // 3 copies of 2 weight => 3*3=9

    // Test case 5: Capacity exactly one item weight.
    int val5[] = {5, 6};
    int wt5[] = {2, 3};
    assert(maxValueIncludingRepeatedItems(2, 3, val5, wt5) == 6);

    // Test case 6: Multiple items with same weight but different values - choose best value.
    int val6[] = {2, 5};
    int wt6[] = {2, 2};
    assert(maxValueIncludingRepeatedItems(2, 4, val6, wt6) == 10); // Use two of the better item (value 5 each)

    // Test case 7: Large capacity with mixed items.
    int val7[] = {6, 10, 12};
    int wt7[] = {1, 2, 3};
    assert(maxValueIncludingRepeatedItems(3, 5, val7, wt7) == 30); // Five of weight 1 => 5*6=30

    // Test case 8: Edge with N=1, W=1.
    int val8[] = {7};
    int wt8[] = {1};
    assert(maxValueIncludingRepeatedItems(1, 1, val8, wt8) == 7);

    // Test case 9: Value array with large numbers (check no overflow in simple case).
    int val9[] = {1000000};
    int wt9[] = {1};
    assert(maxValueIncludingRepeatedItems(1, 10, val9, wt9) == 10000000);

    // Test case 10: Case where not picking a light high-value item is better.
    int val10[] = {1, 3};
    int wt10[] = {1, 3};
    assert(maxValueIncludingRepeatedItems(2, 3, val10, wt10) == 3); // One item of weight 3 value 3; three of weight 1 would give 3, equal

    return 0;
}
// The problem is the classic “Unbounded Knapsack” (or “Rod Cutting” / “Coin Change” with values). The approach uses recursion with memoization. For each item index `ind` (from 0 to N-1) and remaining capacity `w`, we compute the maximum value achievable using items from types `0..ind`. The base case occurs when `ind == 0`: we can only use item type 0, so we take as many copies as fit: `(w / wt[0]) * val[0]`. For other indices, we have two choices: either do not take any copy of item `ind` (move to `ind-1` with same capacity), or take one copy of item `ind` (if `w >= wt[ind]`) and then recurse on the same index `ind` with reduced capacity `w - wt[ind]` (since we may take more copies). The answer for state `(ind, w)` is the maximum of these two choices, stored in `dp[ind][w]`. We memoize results to avoid recomputation. The final answer is `dp[N-1][W]`. Edge cases: if `W == 0` or `N == 0` (though N ≥ 1 per spec), return 0. Also, if an item’s weight is 0? But problem states positive weights. If all weights exceed W, the recursion will eventually return 0 correctly (since base case `w/wt[0]` is 0 for all items). The recursion depth is O(N + W) because each step either decreases `ind` or decreases `w`. Time complexity is O(N * W) since each state is computed once. Space complexity is O(N * W) for the DP table plus recursion stack depth up to O(N + W) in the worst case (though actually O(N) because you can take many copies of the same item, but since W is the bound, depth can be up to W if you keep picking item 0, so O(N + W) is a safe bound).
