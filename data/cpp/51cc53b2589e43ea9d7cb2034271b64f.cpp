/*
Write a standalone C++ function that takes a vector of integer intervals, where each interval is represented as a pair of integers `{low, high}` with `low <= high`, and returns a new vector of merged intervals. The input intervals are unsorted and may overlap, be adjacent, or be disjoint. The output should be a vector of merged intervals sorted by their starting point, where any two intervals that overlap or touch (i.e., `next.low <= current.high + 1`) are combined into a single interval covering the union of their ranges. The function must handle empty input (returning an empty vector), intervals with equal bounds, and intervals that are already non-overlapping. The solution should not modify the input vector and should be implemented with appropriate `const` correctness.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Merge overlapping or adjacent intervals, sorted by start.
// Each interval is {start, end} with start <= end.
std::vector<std::pair<int, int>> mergeIntervals(
    const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) {
        return {};
    }

    // Copy and sort by start (first element)
    std::vector<std::pair<int, int>> sorted = intervals;
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    std::vector<std::pair<int, int>> result;
    // Initialize with the first interval
    int currentStart = sorted[0].first;
    int currentEnd = sorted[0].second;

    for (size_t i = 1; i < sorted.size(); ++i) {
        int nextStart = sorted[i].first;
        int nextEnd = sorted[i].second;

        // Check if overlapping or touching (nextStart <= currentEnd + 1)
        if (nextStart <= currentEnd + 1) {
            // Merge: extend the end if needed
            currentEnd = std::max(currentEnd, nextEnd);
        } else {
            // Disjoint: push current, start new
            result.emplace_back(currentStart, currentEnd);
            currentStart = nextStart;
            currentEnd = nextEnd;
        }
    }

    // Push the last merged interval
    result.emplace_back(currentStart, currentEnd);
    return result;
}
#include <cassert>
#include <iostream>
#include <vector>
#include <utility>

// The solution function is included above or externally linked.

int main() {
    // Empty input
    assert(mergeIntervals({}).empty());

    // No overlapping or touching intervals
    std::vector<std::pair<int,int>> disjoint = {{1,2}, {4,5}, {7,8}};
    auto r1 = mergeIntervals(disjoint);
    assert(r1 == std::vector<std::pair<int,int>>({{1,2}, {4,5}, {7,8}}));

    // Overlapping and touching intervals, unsorted
    std::vector<std::pair<int,int>> overlap = {{5,6}, {1,3}, {3,4}, {6,9}, {12,14}};
    auto r2 = mergeIntervals(overlap);
    assert(r2 == std::vector<std::pair<int,int>>({{1,4}, {5,9}, {12,14}}));

    // One interval fully contained in another
    std::vector<std::pair<int,int>> contained = {{1,10}, {2,3}, {4,8}};
    auto r3 = mergeIntervals(contained);
    assert(r3 == std::vector<std::pair<int,int>>({{1,10}}));

    // Adjacent intervals (touch at boundary, e.g., [1,2] and [3,4] -> [1,4])
    std::vector<std::pair<int,int>> adjacent = {{1,2}, {3,4}, {5,6}};
    auto r4 = mergeIntervals(adjacent);
    assert(r4 == std::vector<std::pair<int,int>>({{1,6}}));

    // Identical intervals
    std::vector<std::pair<int,int>> identical = {{2,2}, {2,2}, {2,2}};
    auto r5 = mergeIntervals(identical);
    assert(r5 == std::vector<std::pair<int,int>>({{2,2}}));

    // Single interval
    auto r6 = mergeIntervals({{42, 100}});
    assert(r6 == std::vector<std::pair<int,int>>({{42, 100}}));

    // Negative values, largest possible gap
    std::vector<std::pair<int,int>> negative = {{-10,-5}, {-4,-1}, {0,0}, {5,10}};
    auto r7 = mergeIntervals(negative);
    assert(r7 == std::vector<std::pair<int,int>>({{-10,-1}, {0,0}, {5,10}}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The core algorithm is a classic interval merging approach: first sort the intervals by their start (low) value to bring overlapping or adjacent intervals into sequential order. Then iterate through the sorted list, maintaining a "current merged interval". For each interval `{l, h}`, if it is the first interval, or if its start `l` is greater than the current interval's end `current.high + 1` (allowing adjacency), then the current interval is finalized and pushed to the result, and the new interval becomes the current one. Otherwise, the interval overlaps or touches the current one, so we update the current interval's end to be the maximum of its existing end and `h` (since the current start is already smaller or equal due to sorting). This greedy approach works because once an interval is finalized, no later interval can start before its end due to sorting, and adjacency handling via `+1` merges touching intervals. Edge cases include empty input, intervals with identical ranges, and intervals where one is fully contained within another—all handled correctly by the maximum update. Time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the output vector (plus stack space for sorting).
