/*
Given a vector of integer pairs where each pair represents an interval `[first, second]`, write a C++ function `mergeIntervals` that takes a `const std::vector<std::pair<int, int>>&` and returns a `std::vector<std::pair<int, int>>` containing the merged intervals. The input is guaranteed to be sorted by the start of each interval (non-decreasing `first`). The function should merge overlapping intervals (where `next.first <= current.second`) and return the resulting merged intervals in order. If the input is empty, return an empty vector. You may assume all intervals have `first <= second` (i.e., valid intervals). The function must not modify the input vector.
*/

#include <vector>
#include <algorithm>

// Merge overlapping integer intervals. Input intervals are assumed sorted by start.
// Returns a new vector of merged intervals.
std::vector<std::pair<int, int>> mergeIntervals(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) {
        return {};
    }

    // Copy and sort by first (start) to ensure correctness even if not sorted.
    std::vector<std::pair<int, int>> sorted = intervals;
    std::sort(sorted.begin(), sorted.end());

    std::vector<std::pair<int, int>> result;
    result.push_back(sorted[0]);

    for (size_t i = 1; i < sorted.size(); ++i) {
        // Current interval
        int curStart = sorted[i].first;
        int curEnd = sorted[i].second;

        // Last interval in result
        int lastStart = result.back().first;
        int lastEnd = result.back().second;

        if (lastEnd >= curStart) {
            // Overlapping or touching: merge by updating the end to the max.
            result.back().second = std::max(lastEnd, curEnd);
        } else {
            // Disjoint: add as a new interval.
            result.push_back({curStart, curEnd});
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The mergeIntervals function is assumed to be defined above.

int main() {
    // Basic overlap
    std::vector<std::pair<int, int>> v1 = {{1, 5}, {3, 7}, {6, 8}};
    auto r1 = mergeIntervals(v1);
    assert(r1 == std::vector<std::pair<int, int>>({{1, 8}}));

    // Disjoint intervals
    std::vector<std::pair<int, int>> v2 = {{1, 2}, {3, 4}, {5, 6}};
    auto r2 = mergeIntervals(v2);
    assert(r2 == v2);

    // touching intervals merge (end >= start)
    std::vector<std::pair<int, int>> v3 = {{1, 2}, {2, 3}, {3, 4}};
    auto r3 = mergeIntervals(v3);
    assert(r3 == std::vector<std::pair<int, int>>({{1, 4}}));

    // Already merged input
    std::vector<std::pair<int, int>> v4 = {{1, 10}, {12, 14}};
    auto r4 = mergeIntervals(v4);
    assert(r4 == v4);

    // Single interval
    std::vector<std::pair<int, int>> v5 = {{5, 9}};
    auto r5 = mergeIntervals(v5);
    assert(r5 == v5);

    // Unsorted input (function should handle it)
    std::vector<std::pair<int, int>> v6 = {{5, 10}, {1, 3}, {2, 6}};
    auto r6 = mergeIntervals(v6);
    assert(r6 == std::vector<std::pair<int, int>>({{1, 10}}));

    // Empty input
    std::vector<std::pair<int, int>> v7 = {};
    auto r7 = mergeIntervals(v7);
    assert(r7.empty());

    // Duplicate intervals
    std::vector<std::pair<int, int>> v8 = {{2, 4}, {2, 4}};
    auto r8 = mergeIntervals(v8);
    assert(r8 == std::vector<std::pair<int, int>>({{2, 4}}));

    // Overlap with one containing another
    std::vector<std::pair<int, int>> v9 = {{1, 100}, {2, 50}, {60, 70}};
    auto r9 = mergeIntervals(v9);
    assert(r9 == std::vector<std::pair<int, int>>({{1, 100}}));

    // Many disjoint after merges
    std::vector<std::pair<int, int>> v10 = {{1, 4}, {5, 7}, {6, 10}, {12, 20}, {19, 25}};
    auto r10 = mergeIntervals(v10);
    assert(r10 == std::vector<std::pair<int, int>>({{1, 4}, {5, 10}, {12, 25}}));

    return 0;
}

// The algorithm works by sorting the input intervals by their start if they are not already sorted (though the problem guarantees sorted input, we can still include a sorting step for robustness). Then, we initialize a result vector with the first interval. For each subsequent interval, we compare its start with the end of the last interval in the result. If the current interval's start is less than or equal to the last interval's end, they overlap, so we extend the last interval's end to the maximum of both ends. Otherwise, they are disjoint, and we push the current interval as a new entry into the result. Edge cases: an empty input returns an empty vector; a single interval returns itself; identical intervals merge into one; intervals that exactly touch (e.g., `[1,2]` and `[2,3]`) are considered overlapping (since condition is `>=`). Time complexity is O(n log n) due to sorting (or O(n) if already sorted), and space complexity is O(n) for the result vector.
