// Write a C++ function that solves the classic "herbal medicine" knapsack problem: given a total available time `T` (1 ≤ T ≤ 1000) and a number of herbs `M` (1 ≤ M ≤ 100), where each herb i has a picking time `time_i` and value `value_i` (both between 1 and 100), return the maximum total value that can be collected without exceeding time `T`. Each herb can be picked at most once. The function should take `T`, `M`, and two vectors (or arrays) of times and values, and return an integer representing the maximum total value. Ensure the function is efficient for the given constraints and handles edge cases like `T` smaller than the smallest picking time.
#include <cassert>
#include <vector>

// Function declaration
int maxHerbValue(int T, int M, const std::vector<int>& times, const std::vector<int>& values);

int main() {
    // Test 1: Basic example
    std::vector<int> times1 = {71, 69, 1};
    std::vector<int> values1 = {100, 1, 2};
    assert(maxHerbValue(70, 3, times1, values1) == 3); // pick 69+1, total value 1+2=3

    // Test 2: All herbs too long
    std::vector<int> times2 = {10, 20};
    std::vector<int> values2 = {5, 6};
    assert(maxHerbValue(5, 2, times2, values2) == 0); // no herb can be picked

    // Test 3: All fit exactly
    std::vector<int> times3 = {1, 2, 3};
    std::vector<int> values3 = {3, 4, 5};
    assert(maxHerbValue(6, 3, times3, values3) == 12); // all picked

    // Test 4: Duplicate times, choose best combination
    std::vector<int> times4 = {2, 2, 3, 3};
    std::vector<int> values4 = {10, 20, 5, 15};
    assert(maxHerbValue(5, 4, times4, values4) == 35); // 2+3, best values 20+15

    // Test 5: T = 0 (though constraints say >=1, but still test)
    std::vector<int> times5 = {1};
    std::vector<int> values5 = {100};
    assert(maxHerbValue(0, 1, times5, values5) == 0); // no time

    // Test 6: One herb exactly fits
    std::vector<int> times6 = {10};
    std::vector<int> values6 = {42};
    assert(maxHerbValue(10, 1, times6, values6) == 42);

    // Test 7: Larger case, simple
    std::vector<int> times7 = {5, 5, 5, 5};
    std::vector<int> values7 = {1, 2, 3, 4};
    assert(maxHerbValue(15, 4, times7, values7) == 9); // pick any three, best values 2+3+4

    // Test 8: Mixed, skip low value item to fit high value
    std::vector<int> times8 = {3, 4, 5};
    std::vector<int> values8 = {5, 6, 10};
    assert(maxHerbValue(8, 3, times8, values8) == 16); // 3+5 => 5+10

    return 0;
}
#include <vector>
#include <algorithm>

// Solve 0/1 knapsack: maximize total value given time limit T.
// Each herb can be used at most once.
int maxHerbValue(int T, int M, const std::vector<int>& times, const std::vector<int>& values) {
    // dp[t] = max value achievable with total time <= t (or exactly t, since we initialize all to 0)
    std::vector<int> dp(T + 1, 0);
    for (int i = 0; i < M; ++i) {
        int t = times[i];
        int v = values[i];
        if (t > T) continue; // cannot pick this herb
        // Traverse backwards to avoid reuse of same herb
        for (int capacity = T; capacity >= t; --capacity) {
            dp[capacity] = std::max(dp[capacity], dp[capacity - t] + v);
        }
    }
    return dp[T];
}
// This is the 0/1 knapsack problem. We use dynamic programming with a 1D array `dp[t]` representing the maximum value achievable using exactly (or at most) time `t`. Iterate over each herb, and for each herb, update `dp` from `T` down to `time` (backwards to avoid reusing the same herb multiple times). The recurrence is `dp[t] = max(dp[t], dp[t - time] + value)`. After processing all herbs, the answer is `dp[T]`. 
// Edge cases: if `time > T`, the herb cannot be picked and is ignored. The initial state `dp[0] = 0`, all other entries 0 (since we want max value, and not using time is fine). Time complexity is O(M * T) (≤ 100 * 1000 = 100,000 operations) which is very fast. Space complexity is O(T) for the dp array. The algorithm is correct for 0/1 knapsack because the backward loop ensures each item is used at most once.
