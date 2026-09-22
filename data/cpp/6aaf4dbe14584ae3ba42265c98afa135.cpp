// Write a C++ function `isPermutationPossible` that takes two integer vectors: `permutation` (a 1-indexed permutation of numbers from 1 to n) and `intervals` (a vector of pairs representing inclusive intervals [l, r] over the positions 1..n), and returns a boolean indicating whether it is possible to rearrange the permutation values within each maximal connected group of overlapping intervals so that every value in each group falls inside that group's unioned range. More precisely, the intervals are processed in a sorted order by their left endpoint; overlapping intervals are merged into a single contiguous segment. For each merged segment [L, R], the function must check that for every position j in [L, R], the value `permutation[j-1]` lies within [L, R]. If all merged segments satisfy this, return `true`; otherwise return `false`.

// The core idea is to first sort the intervals by their left endpoint. Then we scan through them, merging overlapping or touching intervals (where the next interval's left is ≤ the previous merged right) by expanding the right boundary to the maximum. When we encounter a gap (next interval's left > current merged right), we finalize the current merged segment [L, R] and check the condition: for each index j from L to R (inclusive, 1-indexed), the value at `permutation[j-1]` must be between L and R. If any value is outside, the result is immediately false. After processing all intervals, we must also check the final merged segment. Edge cases include: empty intervals vector (should return true because no constraint), single interval, intervals that exactly touch (e.g., [1,2] and [3,4] merge because 3 ≤ 2+1? Actually the code uses `interval[i].first > interval[i-1].second` as the breaking condition, meaning if the next left is strictly greater than the previous right, they don't merge. So touching intervals like [1,2] and [3,4] merge because 3 ≤ 2? No, 3 > 2, so they become separate groups. The condition is non-overlapping, not touching. So we treat only overlapping where next left ≤ current right. Also, the permutation values are checked only within each group's union range; values outside that range are allowed elsewhere. Time complexity is O(m log m + n) for sorting and scanning, and space is O(m) for storing intervals.

#include <vector>
#include <algorithm>
#include <utility>

// Returns true if for every merged interval group, all values in that range are within the group's bounds.
bool isPermutationPossible(const std::vector<int>& permutation,
                           std::vector<std::pair<int, int>> intervals) {
    if (intervals.empty()) return true; // no constraints

    std::sort(intervals.begin(), intervals.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    int n = static_cast<int>(permutation.size());
    int begin = 0;
    for (size_t i = 1; i < intervals.size(); ++i) {
        // If current interval does not overlap with previous merged, finalize previous group
        if (intervals[i].first > intervals[i - 1].second) {
            int L = intervals[begin].first;
            int R = intervals[i - 1].second;
            for (int j = L; j <= R; ++j) {
                if (j < 1 || j > n) return false; // out of bounds (should not happen)
                int val = permutation[j - 1];
                if (val < L || val > R) return false;
            }
            begin = static_cast<int>(i);
        } else {
            // Merge: extend the right endpoint of the current interval
            intervals[i].second = std::max(intervals[i].second, intervals[i - 1].second);
        }
    }
    // Process the last group
    int L = intervals[begin].first;
    int R = intervals.back().second;
    for (int j = L; j <= R; ++j) {
        if (j < 1 || j > n) return false;
        int val = permutation[j - 1];
        if (val < L || val > R) return false;
    }
    return true;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared here (or included from the solution)
bool isPermutationPossible(const std::vector<int>&, std::vector<std::pair<int, int>>);

int main() {
    // Example 1: overlapping intervals, permutation values inside each group
    assert(isPermutationPossible({2, 1, 3, 4}, {{1, 2}, {2, 3}}) == true);
    // {1,2} and {2,3} merge into [1,3]; positions 1..3 have values 2,1,3 all in [1,3]

    // Example 2: non-overlapping intervals, values must match each group
    assert(isPermutationPossible({1, 3, 2}, {{1, 1}, {3, 3}}) == true);
    // Two separate groups: [1,1] value 1 ok, [3,3] value 2 (out of [3,3]? 2 is not in [3,3]) → false
    assert(isPermutationPossible({1, 3, 2}, {{1, 1}, {3, 3}}) == false);

    // Example 3: single interval covering entire permutation
    assert(isPermutationPossible({3, 1, 2}, {{1, 3}}) == true);

    // Example 4: value outside its group's range
    assert(isPermutationPossible({1, 2, 4, 3}, {{1, 2}, {3, 4}}) == true); // groups [1,2] and [3,4] have values 1,2 and 4,3 inside

    // Example 5: touching intervals are separate groups
    assert(isPermutationPossible({1, 3, 2}, {{1, 1}, {2, 2}}) == false); // group [2,2] has value 3, not 2

    // Example 6: empty intervals returns true
    assert(isPermutationPossible({2, 1}, {}) == true);

    // Example 7: overlapping intervals with a violation
    assert(isPermutationPossible({1, 3, 2, 4}, {{1, 2}, {2, 4}}) == true); // merged [1,4] all values in range

    // Example 8: large range merged, values all inside
    assert(isPermutationPossible({4, 3, 2, 1}, {{1, 2}, {2, 3}, {3, 4}}) == true); // merged [1,4] all inside

    // Example 9: as above but with a value outside
    assert(isPermutationPossible({4, 1, 2, 3}, {{1, 2}, {3, 4}}) == false); // group [1,2] has 4 (out of range)

    return 0;
}
