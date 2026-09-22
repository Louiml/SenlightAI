// Write a C++ function `mincostTickets` that, given a sorted vector of integers `days` representing travel days (inclusive, strictly increasing, each between 1 and 365) and a vector `costs` of three positive integers representing the prices of a 1-day, 7-day, and 30-day pass respectively, returns the minimum total cost required to cover all travel days. You may purchase passes on any day, and a pass of duration `d` covers travel days from its purchase day through day `purchase_day + d - 1`. The function must be efficient and handle up to 365 days.

The problem is a classic dynamic programming on indices. Let `dp(idx)` be the minimum cost to cover all travel days from index `idx` to the end. The base case is `dp(n) = 0` when `idx` reaches the end. For a given `idx`, we have three choices: buy a 1-day pass (cost `costs[0]`) and move to the first day strictly greater than `days[idx]` (i.e., the next index after `days[idx]`), buy a 7-day pass (cost `costs[1]`) and move to the first day strictly greater than `days[idx] + 6`, or buy a 30-day pass (cost `costs[2]`) and move to the first day strictly greater than `days[idx] + 29`. The recurrence is `dp(idx) = min( costs[i] + dp(next_i) )` for `i=0,1,2`, where `next_i` is found by advancing a pointer while the current day is less than the coverage end. Since `days` is sorted, we can use a simple while loop inside recursion for each state, but doing so naively leads to O(n^2) worst-case because for each state we may scan up to n days. However, because `days` values are at most 365 and at most 365 entries, a simple memoization with the while loop is acceptable; the worst-case time is O(n * 3 * 365) but practically O(n^2) if n=365, which is acceptable. To make it efficient, we can precompute the next indices for each possible `idx` and each pass type using two-pointer technique, but the given constraints are small enough that the direct while loop is fine. For clarity, we'll use a while loop inside recursion with memoization. Space complexity: O(n) for memoization plus O(n) recursion stack, so O(n) auxiliary space. Edge cases: if `days` is empty (though problem says non-empty), handle gracefully; if a pass covers beyond the last day, the next index becomes `n` and recursion returns 0. The solution uses an unordered_map for memoization (or vector<int>). We'll use a vector of size `n` initialized to -1 for efficiency.

#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    // Returns the minimum total cost to cover all travel days.
    int mincostTickets(const vector<int>& days, const vector<int>& costs) {
        int n = static_cast<int>(days.size());
        memo.assign(n, -1);
        return dp(days, costs, 0);
    }

private:
    vector<int> memo;

    int dp(const vector<int>& days, const vector<int>& costs, int idx) {
        if (idx >= days.size()) {
            return 0;
        }
        if (memo[idx] != -1) {
            return memo[idx];
        }

        int res = INT_MAX;

        for (int i = 0; i < 3; ++i) {
            // Duration of the pass: 1, 7, or 30 days.
            int duration = (i == 0) ? 1 : (i == 1 ? 7 : 30);
            int next_idx = idx;
            // Advance to the first day not covered by this pass.
            while (next_idx < days.size() && days[next_idx] < days[idx] + duration) {
                ++next_idx;
            }
            res = min(res, costs[i] + dp(days, costs, next_idx));
        }

        memo[idx] = res;
        return res;
    }
};

#include <cassert>
#include <vector>
#include "Solution.h" // assuming the solution is in a header

int main() {
    Solution sol;

    // Basic test: only one day
    assert(sol.mincostTickets({1}, {2, 7, 15}) == 2);
    // Two days close together, 1-day pass cheaper than 7-day
    assert(sol.mincostTickets({1, 2}, {2, 7, 15}) == 4);
    // Many days within a week, use 7-day pass
    assert(sol.mincostTickets({1, 2, 3, 4, 5, 6, 7}, {2, 7, 15}) == 7);
    // Days spanning over 30 days, use 30-day pass
    assert(sol.mincostTickets({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}, {2, 7, 15}) == 15);

    // Classic LeetCode example 1
    assert(sol.mincostTickets({1, 4, 6, 7, 8, 20}, {2, 7, 15}) == 11);
    // Classic LeetCode example 2
    assert(sol.mincostTickets({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 30, 31}, {2, 7, 15}) == 17);

    // Sparse days, individual passes best
    assert(sol.mincostTickets({1, 365}, {3, 10, 40}) == 6);
    // Heavy travel over 30 days, 30-day pass best
    assert(sol.mincostTickets({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}, {5, 20, 30}) == 30);

    // Edge case: days are consecutive but 1-day passes expensive, 7-day pass cheap
    assert(sol.mincostTickets({1, 2, 3, 4, 5, 6, 7}, {5, 6, 10}) == 6);

    return 0;
}
