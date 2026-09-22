// Write a C++ function `mergeIntervals` that takes a non-empty vector of integer intervals, where each interval is a vector of two integers `[start, end]` with `start <= end`. The intervals may be unsorted, may overlap, may be adjacent (e.g., `[1,2]` and `[3,4]`), and may contain duplicate intervals. The function must return a new vector of merged intervals covering all input intervals exactly, sorted in ascending order of start time, with no overlapping or adjacent intervals. For example, input `{{1,3},{2,6},{8,10},{15,18}}` returns `{{1,6},{8,10},{15,18}}`, and input `{{1,4},{4,5}}` returns `{{1,5}}` because adjacency merges. The function must not modify the input vector of intervals, must work with vectors of any size ≥1, and must be efficient.

Sort the input intervals by their first element (start time) using the default lexicographic sort, which orders by start then end; since merging only depends on start order, this is sufficient. Initialize `currentStart` and `currentEnd` from the first interval. Iterate through the remaining intervals: if the next interval's start is greater than the current end, then no overlap or adjacency exists, so push the current merged interval into the result and reset `currentStart`/`currentEnd` to the next interval. Otherwise, the intervals overlap or are adjacent (since the next start ≤ currentEnd), so extend the current end to the maximum of the current end and the next interval's end. After the loop, push the final accumulated interval. Edge cases: exactly one interval returns itself; all intervals overlapping yields one merged interval; intervals like `[1,2]` and `[2,3]` merge because `ed < s` is false when `s == ed`. Time complexity is O(n log n) due to sorting, where n is the number of intervals; space is O(1) auxiliary plus O(n) for the output (excluding input copy). The function takes the input by const reference and returns a new vector.

#include <vector>
#include <algorithm>

// Merge all overlapping or adjacent intervals in the given vector.
// The input intervals are non-empty, each [start, end] with start <= end.
// Returns a sorted vector of disjoint merged intervals.
std::vector<std::vector<int>> mergeIntervals(const std::vector<std::vector<int>>& intervals) {
    if (intervals.empty()) {
        return {};
    }
    
    // Copy and sort by start time (then by end, but irrelevant for merging).
    std::vector<std::vector<int>> sorted = intervals;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<std::vector<int>> result;
    int currentStart = sorted[0][0];
    int currentEnd = sorted[0][1];
    
    for (size_t i = 1; i < sorted.size(); ++i) {
        int nextStart = sorted[i][0];
        int nextEnd = sorted[i][1];
        
        // No overlap or adjacency: finalize current and start new.
        if (currentEnd < nextStart) {
            result.push_back({currentStart, currentEnd});
            currentStart = nextStart;
            currentEnd = nextEnd;
        } else {
            // Overlap or adjacency: extend current end if needed.
            currentEnd = std::max(currentEnd, nextEnd);
        }
    }
    // Push the last accumulated interval.
    result.push_back({currentStart, currentEnd});
    
    return result;
}

#include <cassert>
#include <vector>

// The mergeIntervals function is assumed to be defined above in the same translation unit.
// For standalone testing, the solution code is included here.

int main() {
    // Basic overlapping intervals.
    std::vector<std::vector<int>> input1 = {{1,3},{2,6},{8,10},{15,18}};
    std::vector<std::vector<int>> expected1 = {{1,6},{8,10},{15,18}};
    assert(mergeIntervals(input1) == expected1);

    // Adjacent intervals merge.
    std::vector<std::vector<int>> input2 = {{1,4},{4,5}};
    std::vector<std::vector<int>> expected2 = {{1,5}};
    assert(mergeIntervals(input2) == expected2);

    // Single interval remains unchanged.
    std::vector<std::vector<int>> input3 = {{3,7}};
    std::vector<std::vector<int>> expected3 = {{3,7}};
    assert(mergeIntervals(input3) == expected3);

    // Unsorted input.
    std::vector<std::vector<int>> input4 = {{5,6},{1,2},{2,3},{0,1}};
    std::vector<std::vector<int>> expected4 = {{0,3},{5,6}};
    assert(mergeIntervals(input4) == expected4);

    // All intervals overlap into one.
    std::vector<std::vector<int>> input5 = {{2,3},{1,5},{4,6},{0,4}};
    std::vector<std::vector<int>> expected5 = {{0,6}};
    assert(mergeIntervals(input5) == expected5);

    // Duplicate intervals.
    std::vector<std::vector<int>> input6 = {{1,2},{1,2},{1,2}};
    std::vector<std::vector<int>> expected6 = {{1,2}};
    assert(mergeIntervals(input6) == expected6);

    // Intervals that are disjoint and already sorted.
    std::vector<std::vector<int>> input7 = {{1,2},{3,4},{5,6}};
    std::vector<std::vector<int>> expected7 = {{1,2},{3,4},{5,6}};
    assert(mergeIntervals(input7) == expected7);

    // Zero-length intervals.
    std::vector<std::vector<int>> input8 = {{1,1},{1,2}};
    std::vector<std::vector<int>> expected8 = {{1,2}};
    assert(mergeIntervals(input8) == expected8);

    // Negative values.
    std::vector<std::vector<int>> input9 = {{-3,-1},{-2,2},{3,5}};
    std::vector<std::vector<int>> expected9 = {{-3,2},{3,5}};
    assert(mergeIntervals(input9) == expected9);
}
