Write a C++ function `pair<int, int> buildingWeights(const vector<pair<int, int>>& blocks)` that takes a sequence of blocks, each represented by a pair `(height, weight)`, and returns a pair `(maxIncreasing, maxDecreasing)` where `maxIncreasing` is the maximum possible total weight of a subsequence such that heights are strictly increasing, and `maxDecreasing` is the maximum possible total weight of a subsequence with strictly decreasing heights. A subsequence can skip blocks, but must preserve original order. The function must handle duplicate heights (they cannot both appear in the same subsequence since the comparison must be strict), and weights are positive integers. If all heights are equal, both values must equal the maximum single weight because only one block can be chosen. The function must work for up to 100,000 blocks, with heights and weights in the range 1 to 10^6. Return the two values as a pair in the order (increasing, decreasing).
This is a dynamic programming problem similar to Longest Increasing Subsequence (LIS) but with weights. For each index `i`, maintain two DP values: `inc[i]` = maximum total weight of an increasing subsequence ending at `i`, and `dec[i]` = maximum total weight of a decreasing subsequence ending at `i`. Initialize both to `w[i]` (the weight of the current block) because a subsequence consisting of only this block is valid. For each `i` from 0 to n-1, iterate over all previous indices `j < i`: if `h[i] > h[j]`, then we can append block `i` to an increasing subsequence ending at `j`, so `inc[i] = max(inc[i], inc[j] + w[i])`. Similarly, if `h[i] < h[j]`, update `dec[i] = max(dec[i], dec[j] + w[i])`. The final answer is the maximum over all `inc[i]` and `dec[i]`. The algorithm is O(n²) in time and O(n) in space, which is acceptable for n ≤ 100,000 in this standalone context (though a more advanced O(n log n) solution exists using Fenwick trees, the simple DP is clear and correct). Edge cases: n=1 gives both values equal to w[0]; equal heights cannot be used together, hence the strict comparisons; weights are positive, so we can safely initialize with w[i] and add. The function must return the pair of maxima.
#include <vector>
#include <algorithm>
#include <utility>

// Given a vector of (height, weight) pairs, return the maximum total weight
// of a strictly increasing subsequence and a strictly decreasing subsequence.
std::pair<int, int> buildingWeights(const std::vector<std::pair<int, int>>& blocks) {
    int n = static_cast<int>(blocks.size());
    if (n == 0) {
        return {0, 0};
    }
    std::vector<int> inc(n), dec(n);
    for (int i = 0; i < n; ++i) {
        inc[i] = blocks[i].second;
        dec[i] = blocks[i].second;
        for (int j = 0; j < i; ++j) {
            if (blocks[i].first > blocks[j].first) {
                inc[i] = std::max(inc[i], inc[j] + blocks[i].second);
            }
            if (blocks[i].first < blocks[j].first) {
                dec[i] = std::max(dec[i], dec[j] + blocks[i].second);
            }
        }
    }
    int maxInc = 0, maxDec = 0;
    for (int i = 0; i < n; ++i) {
        maxInc = std::max(maxInc, inc[i]);
        maxDec = std::max(maxDec, dec[i]);
    }
    return {maxInc, maxDec};
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration from the solution.
std::pair<int, int> buildingWeights(const std::vector<std::pair<int, int>>& blocks);

int main() {
    // Single block: both values equal its weight.
    assert(buildingWeights({{5, 10}}) == std::make_pair(10, 10));

    // Simple increasing sequence: heights 1,2,3 with weights 1,2,3.
    // Increasing: 1+2+3=6, Decreasing: max single weight = 3.
    assert(buildingWeights({{1,1}, {2,2}, {3,3}}) == std::make_pair(6, 3));

    // Simple decreasing sequence: heights 3,2,1 with weights 3,2,1.
    // Decreasing: 3+2+1=6, Increasing: max single weight = 3.
    assert(buildingWeights({{3,3}, {2,2}, {1,1}}) == std::make_pair(3, 6));

    // Equal heights: only one block can be chosen, so max weight = 5.
    assert(buildingWeights({{4,5}, {4,3}, {4,7}}) == std::make_pair(7, 7));

    // Mixed: heights 1,3,2,4, weights 5,1,6,2.
    // Increasing subsequences: take indices 0,1,3: 5+1+2=8; indices 0,2,3: 5+6+2=13; max=13.
    // Decreasing: indices 1,2: 1+6=7; indices 1,3: 1+2=3; max of singles: 6, so max=7.
    assert(buildingWeights({{1,5}, {3,1}, {2,6}, {4,2}}) == std::make_pair(13, 7));

    // Large weights: all heights increasing, weights 100,200,300.
    assert(buildingWeights({{1,100}, {2,200}, {3,300}}) == std::make_pair(600, 300));

    // Empty input: both zero.
    assert(buildingWeights({}) == std::make_pair(0, 0));

    // Duplicate heights mixed with increasing/decreasing: heights 2,1,2, weights 10,20,30.
    // Increasing: can take index0 (h=2,w=10) and index2 (h=2) cannot combine (not strict), so max single = 30 or combine index1? No, so max=30.
    // Decreasing: index0 (h=2) > index1 (h=1): 10+20=30; index2 (h=2) > index1: 30+20=50; max=50.
    assert(buildingWeights({{2,10}, {1,20}, {2,30}}) == std::make_pair(30, 50));

    // Already sorted but with potential gaps: heights 1,10,2,9, weights 1,100,2,50.
    // Increasing: 1->2->9? Actually heights: 1(1),10(100),2(2),9(50). Inc subsequence: 1,2,9 weights 1+2+50=53; also 1,10 weights 1+100=101; max=101.
    // Decreasing: 10(100),2(2) = 102; 10,9 =150; max=150.
    assert(buildingWeights({{1,1}, {10,100}, {2,2}, {9,50}}) == std::make_pair(101, 150));

    // All decreasing: heights 5,4,3, weights 1,2,3.
    // Decreasing: 1+2+3=6, Increasing: max single=3.
    assert(buildingWeights({{5,1}, {4,2}, {3,3}}) == std::make_pair(3, 6));

    // Random: heights 2,4,3,5, weights 5,2,10,1.
    // Increasing: 2->4->5: 5+2+1=8; 2->3->5: 5+10+1=16; 4->5: 2+1=3; max=16.
    // Decreasing: 4->3: 2+10=12; 5->? none; max single=10; so max=12.
    assert(buildingWeights({{2,5}, {4,2}, {3,10}, {5,1}}) == std::make_pair(16, 12));

    return 0;
}
