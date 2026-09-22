/*
Write a C++ function named `insertInterval` that takes a vector of non-overlapping intervals sorted by their start times, along with a single new interval, and returns a new vector of intervals after inserting the new interval and merging any overlapping intervals. The input intervals are guaranteed to be sorted by start time and non-overlapping, but the new interval may overlap with zero or more of them. The output must remain sorted by start time and contain only non-overlapping intervals. For example, given intervals `{{1,3},{6,9}}` and newInterval `{2,5}`, the result should be `{{1,5},{6,9}}`. Handle edge cases such as the new interval being completely before all intervals, completely after all intervals, or covering all intervals.
*/
#include <vector>
#include <algorithm>

// Insert a new interval into a sorted, non-overlapping list of intervals,
// merging any overlapping intervals. Returns the updated list sorted by start.
std::vector<std::vector<int>> insertInterval(
    const std::vector<std::vector<int>>& intervals,
    const std::vector<int>& newInterval) {
    
    std::vector<std::vector<int>> result;
    size_t i = 0;
    int newStart = newInterval[0];
    int newEnd = newInterval[1];
    
    // Add all intervals that end before the new interval starts
    while (i < intervals.size() && intervals[i][1] < newStart) {
        result.push_back(intervals[i]);
        ++i;
    }
    
    // Merge all overlapping intervals with the new interval
    while (i < intervals.size() && intervals[i][0] <= newEnd) {
        newStart = std::min(newStart, intervals[i][0]);
        newEnd = std::max(newEnd, intervals[i][1]);
        ++i;
    }
    result.push_back({newStart, newEnd});
    
    // Add all remaining intervals that start after the new interval ends
    while (i < intervals.size()) {
        result.push_back(intervals[i]);
        ++i;
    }
    
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Empty intervals
    assert(insertInterval({}, {5, 7}) == std::vector<std::vector<int>>{{5, 7}});
    
    // New interval before all
    assert(insertInterval({{2, 3}, {5, 6}}, {0, 1}) == std::vector<std::vector<int>>{{0, 1}, {2, 3}, {5, 6}});
    
    // New interval after all
    assert(insertInterval({{1, 2}, {3, 4}}, {5, 6}) == std::vector<std::vector<int>>{{1, 2}, {3, 4}, {5, 6}});
    
    // Simple overlap
    assert(insertInterval({{1, 3}, {6, 9}}, {2, 5}) == std::vector<std::vector<int>>{{1, 5}, {6, 9}});
    
    // Overlap with multiple intervals
    assert(insertInterval({{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}}, {4, 8}) == std::vector<std::vector<int>>{{1, 2}, {3, 10}, {12, 16}});
    
    // New interval covers all
    assert(insertInterval({{1, 2}, {3, 4}}, {0, 10}) == std::vector<std::vector<int>>{{0, 10}});
    
    // Exact match with one interval
    assert(insertInterval({{1, 5}}, {1, 5}) == std::vector<std::vector<int>>{{1, 5}});
    
    // Touch at boundaries (no overlap)
    assert(insertInterval({{1, 2}, {3, 4}}, {2, 3}) == std::vector<std::vector<int>>{{1, 2}, {2, 3}, {3, 4}});
    
    return 0;
}
// The approach is to iterate through the sorted intervals and process them based on how the new interval relates to each current interval. There are three main cases: if the current interval ends before the new interval starts, then the current interval is already valid and can be added directly to the result. If the current interval starts after the new interval ends, then the new interval (after merging) is placed before this interval and the rest of the intervals are copied as-is. Otherwise, the intervals overlap, so we merge them by updating the new interval's start to the minimum of the two starts and the end to the maximum of the two ends. After the loop, if the new interval has not yet been inserted (because it belonged at the end), we append it. An alternative simpler but less efficient method is to push the new interval into the list, sort, and then merge, but that loses the benefit of the input being sorted and runs in O(n log n). The optimal solution runs in O(n) time by a single pass, and uses O(n) space for the result vector (excluding the input). Important edge cases: new interval entirely before the first interval, entirely after the last, overlapping with all intervals, and empty input intervals (in which case the result is just the new interval).
