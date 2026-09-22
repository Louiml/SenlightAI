/*
Given a non-empty vector of positive integers representing the weights of stones, write a C++ function `int lastStoneWeightMin(const std::vector<int>& stones)` that returns the smallest possible weight of the last remaining stone after repeatedly crushing stones according to the following rule: when two stones of weights `x` and `y` are crushed together, if `x == y`, both vanish; otherwise, the heavier stone is replaced by a stone of weight `|x - y|`. The order of crushing can be chosen arbitrarily. The function should accept the vector by const reference and return the minimal possible final stone weight (which could be 0 if all stones can be eliminated).
*/
#include <vector>
#include <algorithm>

// Returns the minimum possible weight of the last remaining stone.
// stones: non-empty vector of positive integers.
int lastStoneWeightMin(const std::vector<int>& stones) {
    if (stones.empty()) return 0;

    int total = 0;
    for (int w : stones) total += w;

    int target = total / 2;
    // dp[j] = maximum subset sum ≤ j using processed stones.
    std::vector<int> dp(target + 1, 0);

    for (int w : stones) {
        // Process backwards to avoid reusing the same stone.
        for (int j = target; j >= w; --j) {
            dp[j] = std::max(dp[j], dp[j - w] + w);
        }
    }

    int best = dp[target];
    return total - 2 * best;
}
#include <cassert>
#include <vector>

int main() {
    // Basic examples from the original problem.
    assert(lastStoneWeightMin({2,7,4,1,8,1}) == 1);
    assert(lastStoneWeightMin({31,26,33,21,40}) == 5);

    // Single stone.
    assert(lastStoneWeightMin({5}) == 5);

    // Two equal stones.
    assert(lastStoneWeightMin({6,6}) == 0);

    // Two different stones.
    assert(lastStoneWeightMin({3,7}) == 4);

    // Three stones where perfect split is possible.
    assert(lastStoneWeightMin({2,2,2}) == 2); // 2 vs (2+2) => difference 2

    // All stones same, even count.
    assert(lastStoneWeightMin({4,4,4,4}) == 0);

    // Larger example.
    assert(lastStoneWeightMin({1,2,3,4,5,6,7}) == 0);

    // Mixed weights with clear best partition.
    assert(lastStoneWeightMin({10,20,15,5}) == 0); // 10+15=25 and 20+5=25

    // Edge case: weights summing to an odd number.
    assert(lastStoneWeightMin({1,2,3,4,5}) == 1); // e.g., (1+2+3)=6 vs (4+5)=9 => diff 3, but optimal is 1).

    return 0;
}
// The problem reduces to partitioning the stones into two groups whose total weights are as close as possible, because the final stone weight is the absolute difference between the sums of the two partitions. This is the classic "partition problem" and can be solved with 0/1 knapsack DP where the knapsack capacity is `target = total_sum / 2`. For each stone, we update a DP array `dp[j]` that represents the maximum achievable total weight of a subset not exceeding `j`. We process stones in reverse order over `j` (from target down to stone weight) to ensure each stone is used at most once. At the end, `dp[target]` gives the largest subset sum ≤ half the total. The minimal last stone weight is `total_sum - 2 * dp[target]`. Edge cases: if there is only one stone, the result is its weight; if `stones` is empty (although spec says non-empty, we can return 0 defensively), result is 0. Total sum may be large, but since each stone weight is positive and ≤ 100 (as in the original problem constraints), the DP array size can be set to 3001 (or `total_sum/2 + 1` dynamically). Time complexity is O(n * target) and space O(target) where n is the number of stones.
