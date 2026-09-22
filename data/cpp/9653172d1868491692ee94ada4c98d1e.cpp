// Write a C++ function `mergeIntervals` that takes a vector of integer intervals, where each interval is represented as a `vector<int>` of exactly two elements `[start, end]`, and returns a new vector of merged intervals. Two intervals `[a, b]` and `[c, d]` overlap (or touch) if `b >= c`; if they do, they should be combined into `[min(a,c), max(b,d)]`. The input intervals are unsorted, may appear in any order, may contain duplicate intervals, and may include intervals where `start > end` (invalid input — handle gracefully by treating them as empty and skipping them). The output must be a vector of merged intervals sorted by start time, with no overlapping or adjacent intervals. The function must not modify the input vector.
// The solution begins by creating a copy of the input intervals and sorting them by start time (and by end time for ties) using `std::sort`, which places intervals in ascending order of their left endpoints. After sorting, iterate through the sorted list while maintaining a current interval `[curStart, curEnd]`. For each subsequent interval, check if its start is less than or equal to the current end; if so, they overlap and we extend the current end to the maximum of the two ends. If they don’t overlap, the current interval is finalized and pushed to the result, and we move to the next interval. This process continues until all intervals are processed, and the last current interval is pushed after the loop. Edge cases include: empty input (return empty vector); intervals with `start > end` (skip them, as they are invalid); single interval (return it unchanged); intervals that are adjacent (e.g., `[1,2]` and `[3,4]`) — since `end >= start` check uses `>=`, they are merged into `[1,4]`; and duplicate intervals (merged naturally). Time complexity is O(n log n) due to sorting, where n is the number of intervals, and O(n) space for the output and the copied sorted vector. The algorithm is a standard greedy merge.
#include <vector>
#include <algorithm>

// Merge overlapping or adjacent intervals in a vector of [start, end] pairs.
// Invalid intervals (start > end) are ignored. Returns merged, sorted intervals.
std::vector<std::vector<int>> mergeIntervals(const std::vector<std::vector<int>>& intervals) {
    std::vector<std::vector<int>> result;
    if (intervals.empty()) {
        return result;
    }

    // Copy and filter valid intervals
    std::vector<std::vector<int>> valid;
    for (const auto& interval : intervals) {
        if (interval.size() == 2 && interval[0] <= interval[1]) {
            valid.push_back(interval);
        }
    }
    if (valid.empty()) {
        return result;
    }

    // Sort by start, then by end
    std::sort(valid.begin(), valid.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
    });

    int currentStart = valid[0][0];
    int currentEnd = valid[0][1];

    for (size_t i = 1; i < valid.size(); ++i) {
        if (valid[i][0] <= currentEnd) {
            // Overlap or touch: merge
            currentEnd = std::max(currentEnd, valid[i][1]);
        } else {
            // No overlap: finalize current interval
            result.push_back({currentStart, currentEnd});
            currentStart = valid[i][0];
            currentEnd = valid[i][1];
        }
    }
    // Push the last interval
    result.push_back({currentStart, currentEnd});

    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic merge
    std::vector<std::vector<int>> v1 = {{1,3},{2,6},{8,10},{15,18}};
    auto r1 = mergeIntervals(v1);
    assert((r1 == std::vector<std::vector<int>>{{1,6},{8,10},{15,18}}));

    // Unsorted input
    std::vector<std::vector<int>> v2 = {{5,7},{1,3},{2,4}};
    auto r2 = mergeIntervals(v2);
    assert((r2 == std::vector<std::vector<int>>{{1,4},{5,7}}));

    // Adjacent intervals (touch) merge
    std::vector<std::vector<int>> v3 = {{1,2},{2,3}};
    auto r3 = mergeIntervals(v3);
    assert((r3 == std::vector<std::vector<int>>{{1,3}}));

    // Duplicate intervals
    std::vector<std::vector<int>> v4 = {{1,4},{1,4},{2,3}};
    auto r4 = mergeIntervals(v4);
    assert((r4 == std::vector<std::vector<int>>{{1,4}}));

    // Single interval
    std::vector<std::vector<int>> v5 = {{3,3}};
    auto r5 = mergeIntervals(v5);
    assert((r5 == std::vector<std::vector<int>>{{3,3}}));

    // Empty input
    std::vector<std::vector<int>> v6 = {};
    auto r6 = mergeIntervals(v6);
    assert(r6.empty());

    // Invalid intervals skipped
    std::vector<std::vector<int>> v7 = {{5,2},{1,3}};
    auto r7 = mergeIntervals(v7);
    assert((r7 == std::vector<std::vector<int>>{{1,3}}));

    // All invalid
    std::vector<std::vector<int>> v8 = {{4,1},{2,0}};
    auto r8 = mergeIntervals(v8);
    assert(r8.empty());

    // Non-overlapping already sorted
    std::vector<std::vector<int>> v9 = {{1,2},{3,4},{5,6}};
    auto r9 = mergeIntervals(v9);
    assert((r9 == std::vector<std::vector<int>>{{1,2},{3,4},{5,6}}));

    // Overlap that extends end multiple times
    std::vector<std::vector<int>> v10 = {{1,10},{2,3},{4,11},{12,13}};
    auto r10 = mergeIntervals(v10);
    assert((r10 == std::vector<std::vector<int>>{{1,11},{12,13}}));
}
