// Given a vector of intervals, where each interval is represented as a vector of two integers `[start, end]` (with `start <= end`), write a C++ function `int minRemovedIntervals(vector<vector<int>>& intervals)` that returns the minimum number of intervals that must be removed so that the remaining intervals are non-overlapping. Two intervals are considered overlapping if they share any common point (e.g., `[1,2]` and `[2,3]` do overlap at point `2`). The input vector may be empty (in which case return 0), may contain duplicate intervals, and may contain intervals with equal start and end times. The function should not modify the original input vector (i.e., it must handle `const` input or copy/sort internally without altering the caller's data), and it should be efficient for large inputs.

#include <cassert>
#include <vector>

// The solution function is declared above (or included from elsewhere).
// Provide a main function with assertions.

int main() {
    // Basic case: two overlapping intervals, remove one.
    std::vector<std::vector<int>> intervals1 = {{1,2}, {2,3}};
    assert(minRemovedIntervals(intervals1) == 1);
    
    // Non-overlapping intervals: remove none.
    std::vector<std::vector<int>> intervals2 = {{1,2}, {3,4}};
    assert(minRemovedIntervals(intervals2) == 0);
    
    // Empty input.
    std::vector<std::vector<int>> intervals3;
    assert(minRemovedIntervals(intervals3) == 0);
    
    // Single interval.
    std::vector<std::vector<int>> intervals4 = {{5,5}};
    assert(minRemovedIntervals(intervals4) == 0);
    
    // Multiple overlapping: need to remove 2 to keep only [1,2] and [3,4]? Actually check.
    // Intervals: [1,3], [2,4], [3,5] -> all pairwise overlap? [1,3] and [3,5] share 3, so overlap. So only one can be kept, remove 2.
    std::vector<std::vector<int>> intervals5 = {{1,3}, {2,4}, {3,5}};
    assert(minRemovedIntervals(intervals5) == 2);
    
    // Duplicate intervals: [1,2] twice → keep one, remove one.
    std::vector<std::vector<int>> intervals6 = {{1,2}, {1,2}};
    assert(minRemovedIntervals(intervals6) == 1);
    
    // Case where choosing earliest end is crucial:
    // [0,10], [1,2], [3,4] → sorted by end: [1,2], [3,4], [0,10]. Keep first two, remove last → 1.
    std::vector<std::vector<int>> intervals7 = {{0,10}, {1,2}, {3,4}};
    assert(minRemovedIntervals(intervals7) == 1);
    
    // All intervals overlap heavily: remove all but one.
    std::vector<std::vector<int>> intervals8 = {{1,5}, {2,6}, {3,7}, {4,8}};
    assert(minRemovedIntervals(intervals8) == 3);
    
    // Including zero-length intervals: [2,2] and [2,2] overlap, but [1,2] and [2,2] overlap too.
    std::vector<std::vector<int>> intervals9 = {{2,2}, {1,2}, {2,3}};
    // Sorted by end: [2,2] (end 2), [1,2] (end 2), [2,3] (end 3). Keep first [2,2], then check [1,2] (start 1 >= 2? no), skip; [2,3] (start 2 >= 2? yes) keep. So kept=2, remove=1.
    assert(minRemovedIntervals(intervals9) == 1);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the minimum number of intervals to remove so that remaining intervals are non-overlapping.
// Intervals are given as [start, end] vectors. Overlap means sharing any point (including endpoints).
int minRemovedIntervals(const std::vector<std::vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    
    // Copy to avoid modifying the original input (const correctness).
    std::vector<std::vector<int>> sortedIntervals = intervals;
    
    // Sort by end time ascending. For equal ends, order doesn't matter.
    std::sort(sortedIntervals.begin(), sortedIntervals.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[1] < b[1];
              });
    
    int keptCount = 1;               // Always keep the first interval after sorting.
    int currentEnd = sortedIntervals[0][1];
    
    for (size_t i = 1; i < sortedIntervals.size(); ++i) {
        if (sortedIntervals[i][0] >= currentEnd) {
            // Non-overlapping (endpoint equality means overlap, so >= is correct)
            ++keptCount;
            currentEnd = sortedIntervals[i][1];
        }
    }
    
    return static_cast<int>(sortedIntervals.size()) - keptCount;
}

// The classic greedy solution sorts intervals by their ending time. The key insight is that to maximize the number of non-overlapping intervals, we should always pick the interval that ends earliest, because that leaves the most room for subsequent intervals. After sorting by `end` ascending, we iterate through intervals, keeping track of the end time of the last selected interval. We count how many intervals we can keep: an interval `[start, end]` can be kept if `start >= current_end` (non-overlapping; equality is allowed since sharing a point means overlap, so `start >= end` of previous is needed). If it can be kept, we increment the count and update `current_end` to its end. Otherwise, we skip it (it overlaps with the last kept interval). At the end, the minimum number to remove equals `total_intervals - kept_count`. Edge cases: empty input returns 0; single interval always keeps 1, so remove 0; intervals like `[1,1]` and `[1,1]` overlap, so only one can be kept; sorting must be stable or at least consistent, but any order for equal ends is fine. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (ignoring the copy if we need to preserve input, we can sort a copy or sort in place if allowed; for const correctness, we'll copy the vector then sort the copy, which is O(n) extra space).
