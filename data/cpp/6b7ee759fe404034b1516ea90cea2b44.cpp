You are given a set of `n` distinct positive integers (each between 1 and 500,000), followed by a non‑negative integer `d` and then the `n` integers themselves. Your task is to simulate a greedy selection process: starting from sum `0`, at each step you may jump to any achievable sum that is at most `d` larger than your current sum, but only if that sum is actually obtainable as a subset sum of the given numbers. You repeat this until no further move is possible (i.e., the next achievable sum is more than `d` above your current sum, or you have reached the maximum achievable sum). Write a C++ function `std::pair<int,int> greedySubsetSum(int n, int d, const std::vector<int>& values)` that returns a pair `{finalSum, stepCount}` where `finalSum` is the largest sum you can reach via this greedy process, and `stepCount` is the number of jumps taken (including the final jump to that sum, but not counting the starting position 0). The original code snippet uses a bitset for subset‑sum DP and a sorted list of achievable sums, then greedily advances.

The solution first computes all possible subset sums of the given numbers using a bitset of size 500,001 (since the maximum possible sum with up to 500,000 total value is 500,000). Set bit 0 to true, then for each number `x`, update the bitset from high to low: if `sum - x` is achievable then `sum` becomes achievable. After processing all numbers, collect all achievable sums into a sorted array (they are naturally in increasing order because the bitset is scanned from 0 upward). Then simulate the greedy process: start with `ans = 0` and `stepCount = 0`. Let `pos` be an index into the sorted array, starting at 1 (since index 0 is sum 0). While `pos` is within the array and the next achievable sum minus `ans` is ≤ `d`, advance `pos` as far as possible while the condition holds, then set `ans` to that achievable sum, increment `stepCount`, and move `pos` to the next index. If the next achievable sum is too far away, break. The final answer is `{ans, stepCount}`. Edge cases: if `n=0`, only sum 0 is achievable, so the loop never runs, returning `{0,0}`. If `d` is large enough to allow a single jump to the maximum sum, the answer is that maximum sum with stepCount 1. Time complexity: O(n * 500000) for the DP (worst‑case), and O(500000) for scanning. Space complexity: O(500000) for the bitset and the sorted array.

#include <vector>
#include <bitset>
#include <utility>

constexpr int MAX_SUM = 500000;

// Given a set of positive integers and a maximum jump distance d,
// return the final sum and number of jumps using a greedy strategy.
std::pair<int,int> greedySubsetSum(int n, int d, const std::vector<int>& values) {
    std::bitset<MAX_SUM + 1> possible;
    possible[0] = true;
    for (int i = 0; i < n; ++i) {
        int x = values[i];
        // Update from high to low to avoid reusing x multiple times.
        for (int s = MAX_SUM; s >= x; --s) {
            if (possible[s - x]) possible[s] = true;
        }
    }
    // Collect achievable sums in increasing order.
    std::vector<int> sums;
    sums.reserve(MAX_SUM + 1);
    for (int s = 0; s <= MAX_SUM; ++s) {
        if (possible[s]) sums.push_back(s);
    }
    int ans = 0;
    int jumps = 0;
    int pos = 1; // index of first sum > 0
    while (pos < static_cast<int>(sums.size())) {
        if (sums[pos] - ans > d) break;
        // Advance as far as possible while the next sum is within d.
        while (pos + 1 < static_cast<int>(sums.size()) && sums[pos + 1] - ans <= d) {
            ++pos;
        }
        ans = sums[pos];
        ++jumps;
        ++pos; // move to the next candidate (which is > ans)
    }
    return {ans, jumps};
}

#include <cassert>
#include <vector>
#include <utility>

// declaration from solution
std::pair<int,int> greedySubsetSum(int n, int d, const std::vector<int>& values);

int main() {
    // Basic case: {1,2,3}, d=3 -> achievable sums: 0,1,2,3,4,5,6 (all)
    // Greedy: 0->3 (jump 3), 3->6 (jump 3) -> final 6, jumps 2
    assert(greedySubsetSum(3, 3, {1,2,3}) == std::make_pair(6, 2));

    // {2,5}, d=4 -> achievable: 0,2,5,7
    // Greedy: 0->2 (2≤4), 2->5 (3≤4), 5->7 (2≤4) -> final 7, jumps 3
    assert(greedySubsetSum(2, 4, {2,5}) == std::make_pair(7, 3));

    // {10}, d=9 -> achievable: 0,10; next jump 10 > 9 -> stop at 0, jumps 0
    assert(greedySubsetSum(1, 9, {10}) == std::make_pair(0, 0));

    // {10}, d=10 -> jump 0->10 -> final 10, jumps 1
    assert(greedySubsetSum(1, 10, {10}) == std::make_pair(10, 1));

    // {1,2,4,8}, d=1 -> achievable: all sums 0..15
    // Greedy: 0->1, 1->2, 2->3, ... up to 15 -> final 15, jumps 15
    assert(greedySubsetSum(4, 1, {1,2,4,8}) == std::make_pair(15, 15));

    // Empty set (n=0) -> only sum 0, no jumps
    assert(greedySubsetSum(0, 5, {}) == std::make_pair(0, 0));

    // {3,5}, d=1 -> achievable: 0,3,5,8; jumps: 0->3 (3>1) stop -> final 0, jumps 0
    assert(greedySubsetSum(2, 1, {3,5}) == std::make_pair(0, 0));

    // {100000}, d=100000 -> jump directly to 100000
    assert(greedySubsetSum(1, 100000, {100000}) == std::make_pair(100000, 1));

    // {500000}, d=500000 -> jump directly to max sum
    assert(greedySubsetSum(1, 500000, {500000}) == std::make_pair(500000, 1));

    // {1, 100000}, d=1 -> achievable: 0,1,100000,100001; greedy: 0->1 (1≤1), then 100000-1=99999>1 stop -> final 1, jumps 1
    assert(greedySubsetSum(2, 1, {1, 100000}) == std::make_pair(1, 1));

    return 0;
}
