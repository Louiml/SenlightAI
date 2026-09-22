Write a C++ function `int minJumpCost(const std::vector<int>& heights, int k)` that, given a non-empty vector of stone heights (at least one stone) and a positive integer `k`, returns the minimum total cost to reach the last stone starting from the first stone. You are standing on the first stone (index 0). From any stone at index `i`, you may jump to any stone at index `i + j` where `1 ≤ j ≤ k` and `i + j < vector size`. The cost of a jump from stone `i` to stone `i + j` is the absolute difference of their heights: `abs(heights[i] - heights[i+j])`. The total cost is the sum of all jump costs. If the vector contains only one stone, the cost is 0. The heights can be negative or large up to 10^9, and `k` can be as large as the number of stones.

This is a classic dynamic programming problem. Let `dp[i]` be the minimum cost to reach stone `i` from stone 0. Base case: `dp[0] = 0` because you start there. For every other stone, we initialize to a large sentinel value (e.g., `INT_MAX`). Then iterate from left to right: for each index `i`, for all possible jump lengths `j` from 1 to `min(k, n-1-i)`, we attempt to update `dp[i+j]` with `dp[i] + abs(heights[i] - heights[i+j])`. Since all costs are non-negative, this forward DP works correctly because we process stones in increasing index order, ensuring that when we update `dp[i+j]`, `dp[i]` is already final. Edge cases: (1) `n == 1` → return 0 immediately; (2) very large heights can cause overflow when using `int`, so use `long long` for `dp` and for absolute difference; (3) if `k` is larger than the remaining distance, we limit the inner loop to `n - i - 1`. Time complexity is `O(n * k)` (or `O(n^2)` in the worst case when `k` is large), and space complexity is `O(n)` for the DP array.

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>

// Returns the minimum total cost to jump from the first to the last stone,
// where from stone i you can jump up to k stones forward.
long long minJumpCost(const std::vector<int>& heights, int k) {
    int n = static_cast<int>(heights.size());
    if (n <= 1) {
        return 0;
    }

    std::vector<long long> dp(n, LLONG_MAX);
    dp[0] = 0;

    for (int i = 0; i < n; ++i) {
        int maxJump = std::min(k, n - 1 - i);
        for (int j = 1; j <= maxJump; ++j) {
            long long cost = std::llabs(static_cast<long long>(heights[i]) - static_cast<long long>(heights[i + j]));
            dp[i + j] = std::min(dp[i + j], dp[i] + cost);
        }
    }

    return dp[n - 1];
}

#include <cassert>
#include <vector>

// Assumes the solution function is defined above.
int main() {
    // Single stone
    assert(minJumpCost({0}, 5) == 0);
    assert(minJumpCost({42}, 1) == 0);

    // Two stones
    assert(minJumpCost({0, 4}, 1) == 4);
    assert(minJumpCost({4, 0}, 3) == 4);
    assert(minJumpCost({-3, -7}, 2) == 4);

    // k=1 forces sequential jumps
    assert(minJumpCost({1, 10, 5}, 1) == 9 + 5); // 1->10 cost 9, 10->5 cost 5, total 14
    assert(minJumpCost({2, 5, 1}, 1) == 3 + 4); // 3 + 4 = 7

    // k=2 gives flexibility
    assert(minJumpCost({1, 10, 5}, 2) == 4); // jump 1->5 directly
    assert(minJumpCost({2, 5, 1}, 2) == 1); // jump 2->1 directly

    // Larger example with negative heights
    std::vector<int> h = {10, -2, 8, 0, 5};
    // For k=1 total = |10+2| + | -2-8| + |8-0| + |0-5| = 12+10+8+5=35
    assert(minJumpCost(h, 1) == 35);
    // For k=2, optimal: 10->8 (cost 2) then 8->5 (cost 3) total 5
    assert(minJumpCost(h, 2) == 5);
    // For k=3, can jump 10->0 (cost 10) then 0->5 (cost 5) total 15, or 10->8 (2) and 8->5 (3)=5 still best
    assert(minJumpCost(h, 3) == 5);

    // k as large as n-1
    assert(minJumpCost({3, -1, 4, -2, 9}, 4) == 6); // 3->(-2) cost 5, then -2->9 cost 11? Better: 3->9 direct cost 6

    return 0;
}
