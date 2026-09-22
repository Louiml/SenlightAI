/*
Write a C++ function named `mergeIntervals` that takes a reference to a vector of integer intervals, where each interval is represented as a vector of exactly two integers `[start, end]` (with `start <= end`), and returns a new vector of merged intervals such that all overlapping or adjacent intervals (where one interval's `end` is greater than or equal to the next interval's `start`) are combined into a single interval that covers their union. The input can be unsorted and may contain duplicate intervals. The output intervals must be sorted in ascending order of their start values. For example, given `{{1,3},{2,6},{8,10},{15,18}}`, the function should return `{{1,6},{8,10},{15,18}}`; given `{{1,4},{4,5}}`, it should return `{{1,5}}`. The function must not modify the input vector unless it is explicitly passed as a non-const reference (you may choose to work on a sorted copy). Handle edge cases such as a single interval, intervals that fully contain another, and intervals with equal bounds.
*/
#include <vector>
#include <algorithm>

// Merge overlapping or adjacent intervals. Each interval is a vector<int> of size 2: {start, end}.
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals) {
    if (intervals.empty()) {
        return {};
    }
    // Sort by start value (and then by end value) using default lexicographical order.
    std::sort(intervals.begin(), intervals.end());

    std::vector<std::vector<int>> result;
    // Start with the first interval as the current candidate.
    std::vector<int> current = intervals[0];

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (current[1] >= intervals[i][0]) {
            // Overlap or adjacency: merge by extending the end if needed.
            current[1] = std::max(current[1], intervals[i][1]);
        } else {
            // No overlap: finalize current and move to the next interval.
            result.push_back(current);
            current = intervals[i];
        }
    }
    // Add the last interval.
    result.push_back(current);
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic overlapping intervals
    std::vector<std::vector<int>> input1 = {{1,3},{2,6},{8,10},{15,18}};
    std::vector<std::vector<int>> expected1 = {{1,6},{8,10},{15,18}};
    assert(mergeIntervals(input1) == expected1);

    // Adjacent intervals should merge
    std::vector<std::vector<int>> input2 = {{1,4},{4,5}};
    std::vector<std::vector<int>> expected2 = {{1,5}};
    assert(mergeIntervals(input2) == expected2);

    // Unsorted input
    std::vector<std::vector<int>> input3 = {{2,3},{5,7},{1,2},{6,9}};
    std::vector<std::vector<int>> expected3 = {{1,3},{5,9}};
    assert(mergeIntervals(input3) == expected3);

    // Fully contained intervals
    std::vector<std::vector<int>> input4 = {{1,10},{2,5},{3,7}};
    std::vector<std::vector<int>> expected4 = {{1,10}};
    assert(mergeIntervals(input4) == expected4);

    // Single interval
    std::vector<std::vector<int>> input5 = {{4,7}};
    std::vector<std::vector<int>> expected5 = {{4,7}};
    assert(mergeIntervals(input5) == expected5);

    // No overlap
    std::vector<std::vector<int>> input6 = {{1,2},{3,4},{5,6}};
    std::vector<std::vector<int>> expected6 = {{1,2},{3,4},{5,6}};
    assert(mergeIntervals(input6) == expected6);

    // Duplicate intervals
    std::vector<std::vector<int>> input7 = {{1,3},{1,3},{2,4}};
    std::vector<std::vector<int>> expected7 = {{1,4}};
    assert(mergeIntervals(input7) == expected7);

    // Empty input (edge case)
    std::vector<std::vector<int>> input8 = {};
    std::vector<std::vector<int>> expected8 = {};
    assert(mergeIntervals(input8) == expected8);

    // Equal start, different ends
    std::vector<std::vector<int>> input9 = {{2,3},{2,5},{2,4}};
    std::vector<std::vector<int>> expected9 = {{2,5}};
    assert(mergeIntervals(input9) == expected9);

    return 0;
}
// The solution follows a greedy merging strategy after sorting. First, sort the intervals by their start value (and if starts are equal, by end, which is the default for `std::sort` on vectors of vectors). This ensures that any interval that could overlap with a previous one appears immediately after it. Then, maintain a "current" interval initialized to the first interval. Iterate through the rest: if the current interval's `end` is greater than or equal to the next interval's `start`, they overlap or touch, so we merge by updating the current `end` to the maximum of the two ends (since the start is already ≤). Otherwise, the current interval is complete: push it into the answer and set the current interval to the next one. After the loop, push the last current interval. Edge cases include: an empty input (but the task implies at least one interval—still, handle gracefully by returning empty), a single interval (correctly returned as is), and intervals that are exactly adjacent (e.g., [1,2] and [2,3]) which should merge because the condition uses `>=`. Time complexity is O(n log n) due to sorting, plus O(n) for the single pass. Space complexity is O(n) for the output vector (or O(1) auxiliary if we ignore the output), plus O(n) for a copy if we choose not to modify the original.
