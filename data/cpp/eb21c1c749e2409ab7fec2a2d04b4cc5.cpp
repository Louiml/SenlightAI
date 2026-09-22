Given a vector of integer intervals `[start, end]` where each interval is represented as a vector of two integers and the vector is non-empty, write a C++ function `std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals)` that merges all overlapping intervals and returns a new vector containing the merged intervals, sorted by their start values. Two intervals overlap if they share any common point (i.e., `intervalA[1] >= intervalB[0]` assuming `intervalA` starts at or before `intervalB`). The function must handle intervals with equal start/end values (e.g., `[1,4]` and `[4,5]` overlap), intervals that are completely contained within others, and an input containing only one interval. The returned intervals must maintain the relative order of their starts as they appeared after the given input is not necessarily sorted. The function must not modify the input vector (pass by value or take a copy) and must work correctly for intervals with non-negative integer bounds. The algorithm should be efficient for up to 10^5 intervals.

#include <cassert>
#include <vector>

// Function under test is assumed to be declared above.
int main() {
    // Test 1: Basic overlapping intervals.
    std::vector<std::vector<int>> intervals1 = {{1,3}, {2,6}, {8,10}, {15,18}};
    std::vector<std::vector<int>> result1 = mergeIntervals(intervals1);
    assert((result1 == std::vector<std::vector<int>>{{1,6}, {8,10}, {15,18}}));

    // Test 2: Touch at endpoints (overlap).
    std::vector<std::vector<int>> intervals2 = {{1,4}, {4,5}};
    std::vector<std::vector<int>> result2 = mergeIntervals(intervals2);
    assert((result2 == std::vector<std::vector<int>>{{1,5}}));

    // Test 3: One interval completely contained in another.
    std::vector<std::vector<int>> intervals3 = {{1,10}, {2,3}, {4,5}, {6,7}};
    std::vector<std::vector<int>> result3 = mergeIntervals(intervals3);
    assert((result3 == std::vector<std::vector<int>>{{1,10}}));

    // Test 4: Single interval.
    std::vector<std::vector<int>> intervals4 = {{5,7}};
    std::vector<std::vector<int>> result4 = mergeIntervals(intervals4);
    assert((result4 == std::vector<std::vector<int>>{{5,7}}));

    // Test 5: Already sorted and non-overlapping.
    std::vector<std::vector<int>> intervals5 = {{1,2}, {3,4}, {5,6}};
    std::vector<std::vector<int>> result5 = mergeIntervals(intervals5);
    assert((result5 == std::vector<std::vector<int>>{{1,2}, {3,4}, {5,6}}));

    // Test 6: Unsorted input requiring sorting.
    std::vector<std::vector<int>> intervals6 = {{4,5}, {1,2}, {2,4}};
    std::vector<std::vector<int>> result6 = mergeIntervals(intervals6);
    assert((result6 == std::vector<std::vector<int>>{{1,5}}));

    // Test 7: Large interval merging into many small ones.
    std::vector<std::vector<int>> intervals7 = {{0,0}, {1,1}, {0,10}};
    std::vector<std::vector<int>> result7 = mergeIntervals(intervals7);
    assert((result7 == std::vector<std::vector<int>>{{0,10}}));

    // Test 8: Intervals with equal start.
    std::vector<std::vector<int>> intervals8 = {{1,5}, {1,3}, {1,4}};
    std::vector<std::vector<int>> result8 = mergeIntervals(intervals8);
    assert((result8 == std::vector<std::vector<int>>{{1,5}}));

    // Test 9: Duplicate identical intervals.
    std::vector<std::vector<int>> intervals9 = {{2,3}, {2,3}, {2,3}};
    std::vector<std::vector<int>> result9 = mergeIntervals(intervals9);
    assert((result9 == std::vector<std::vector<int>>{{2,3}}));

    // Test 10: Disjoint intervals after sorting.
    std::vector<std::vector<int>> intervals10 = {{3,4}, {1,2}};
    std::vector<std::vector<int>> result10 = mergeIntervals(intervals10);
    assert((result10 == std::vector<std::vector<int>>{{1,2}, {3,4}}));

    return 0;
}

#include <vector>
#include <algorithm>

// Merges all overlapping intervals in the input vector.
// The input vector is passed by value to avoid modifying the caller's data.
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals) {
    if (intervals.size() <= 1) {
        return intervals;
    }

    // Sort by start value (and implicitly by end value for equal starts,
    // which is fine because we handle end updates when merging).
    std::sort(intervals.begin(), intervals.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[0] < b[0];
              });

    std::vector<std::vector<int>> merged;
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i) {
        // Reference to the last merged interval's end.
        int& lastEnd = merged.back()[1];

        if (intervals[i][0] <= lastEnd) {
            // Overlap: extend the end if needed.
            lastEnd = std::max(lastEnd, intervals[i][1]);
        } else {
            // No overlap: finalize the current and start a new one.
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}

// The core idea is to sort the intervals primarily by their start value and secondarily by their end value (though sorting by start alone is sufficient because we handle end values during merging). After sorting, we iterate through the sorted list and merge adjacent intervals whenever the current interval's start is less than or equal to the previous merged interval's end. Specifically, we maintain a "current merged interval" initialized from the first interval. For each subsequent interval, if its start is less than or equal to the current merged interval's end, we extend the current merged interval's end to the maximum of its current end and the new interval's end. If the start is greater than the current merged interval's end, then the current merged interval is finalized and pushed into the result, and we start a new current merged interval from the current interval. Edge cases: (1) A single interval – the result is the input itself; (2) intervals that fully contain another – handled by the max update; (3) intervals that touch at endpoints (e.g., `[1,2]` and `[2,3]`) overlap because the condition is `<=`. Sorting takes `O(n log n)` time, and the linear scan is `O(n)`; overall time is `O(n log n)`. Auxiliary space is `O(n)` in the worst case for the result vector (actually the result can reuse the input vector if allowed, but we create a new vector; the sorting uses `O(log n)` stack space for quicksort typical, or `O(n)` if using mergesort; but `std::sort` uses `O(log n)` extra space on average). We avoid modifying the caller's data by taking the vector by value.
